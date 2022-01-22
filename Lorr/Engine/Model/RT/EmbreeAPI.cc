#include "EmbreeAPI.hh"

#include <embree3/rtcore_device.h>
#include <embree3/rtcore.h>

namespace lr
{
    void EmbreeErrorHandler(void *userPtr, enum RTCError code, const char *str)
    {
        LOG_ERROR("Embree Error: %s", str);
    }

    struct EmbreeDevice
    {
        EmbreeDevice()
        {
            m_Handle = rtcNewDevice(nullptr);
            rtcSetDeviceErrorFunction(m_Handle, EmbreeErrorHandler, nullptr);

            m_Scene = rtcNewScene(m_Handle);
        }

        ~EmbreeDevice()
        {
            rtcReleaseDevice(m_Handle);
        }

        RTCDevice m_Handle;
        RTCScene m_Scene;
        RTCGeometry m_CurrentGeo = nullptr;  // Current geometry being processed
    };

    static EmbreeDevice *m_pContext = nullptr;

    void RT::Init()
    {
        m_pContext = new EmbreeDevice;
    }

    void RT::NewGeometry(glm::vec3 *&pPositionsOut, u32 vertexSize, u32 *&pIndicesOut, u32 indexSize)
    {
        assert(!m_pContext->m_CurrentGeo);

        m_pContext->m_CurrentGeo = rtcNewGeometry(m_pContext->m_Handle, RTC_GEOMETRY_TYPE_TRIANGLE);

        // clang-format off
        pPositionsOut = (glm::vec3 *)rtcSetNewGeometryBuffer(
            m_pContext->m_CurrentGeo, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, sizeof(glm::vec3), vertexSize);
        pIndicesOut = (u32 *)rtcSetNewGeometryBuffer(
            m_pContext->m_CurrentGeo, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, 3 * sizeof(u32), indexSize);
        // clang-format on
    }

    void RT::CommitGeometry()
    {
        rtcCommitGeometry(m_pContext->m_CurrentGeo);
        rtcAttachGeometry(m_pContext->m_Scene, m_pContext->m_CurrentGeo);
        rtcReleaseGeometry(m_pContext->m_CurrentGeo);
        rtcCommitScene(m_pContext->m_Scene);

        m_pContext->m_CurrentGeo = nullptr;
    }

    bool RT::ShootRay(const glm::vec3 &origin, const glm::vec3 &direction, float &hitFarOut)
    {
        RTCIntersectContext intersectCtx;
        rtcInitIntersectContext(&intersectCtx);

        RTCRayHit rayHit;
        rayHit.ray.org_x = origin.x;
        rayHit.ray.org_y = origin.y;
        rayHit.ray.org_z = origin.z;

        rayHit.ray.dir_x = direction.x;
        rayHit.ray.dir_y = direction.y;
        rayHit.ray.dir_z = direction.z;

        rayHit.ray.tnear = 0.f;
        rayHit.ray.tfar = std::numeric_limits<float>::infinity();
        rayHit.hit.geomID = RTC_INVALID_GEOMETRY_ID;

        rtcIntersect1(m_pContext->m_Scene, &intersectCtx, &rayHit);

        hitFarOut = rayHit.ray.tfar;

        return rayHit.hit.geomID != RTC_INVALID_GEOMETRY_ID;
    }

}  // namespace lr