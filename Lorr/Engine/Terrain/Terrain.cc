#include "Terrain.hh"

#include "Chunk/DetailedChunkManager.hh"

#include "Engine/App/Engine.hh"
#include "Engine/ECS/Components/CameraComponent.hh"
#include "Engine/ECS/Components/TransformComponent.hh"

#include "Engine/Model/RT/EmbreeAPI.hh"

namespace lr
{
    void Terrain::Init(TerrainType type, u32 width, u32 depth)
    {
        switch (type)
        {
            case TerrainType::Detailed: m_pChunkManager = new DetailedChunkManager; break;
            default: break;
        }

        m_pChunkManager->Init(width, depth);
    }

    void Terrain::Update(float deltaTime)
    {
        m_pChunkManager->Update(deltaTime);
    }

    void Terrain::Render()
    {
        m_pChunkManager->Render();
    }

    void Terrain::PickMouse(float x, float y)
    {
        BaseWindow *pWindow = GetEngine()->GetWindow();
        Scene *pActiveScene = GetApp()->GetActiveScene();
        Entity camera3D = pActiveScene->GetEntity("entity://camera3d");
        Camera3D &cameraHandle = camera3D.GetComponent<Component::Camera3DController>().m_Handle;

        /// [-1, -1] to [1, 1]
        glm::vec4 screenPos = glm::vec4((2.0 * x) / pWindow->GetWidth() - 1.0, (1.0 - (2.0 * y) / pWindow->GetHeight()), 1.0f, 1.0f);
        glm::vec4 worldPos = glm::inverse(cameraHandle.GetProjection() * cameraHandle.GetView()) * screenPos;

        glm::vec3 direction = glm::normalize(glm::vec3(worldPos));
        glm::vec3 startPos = cameraHandle.GetPosition();

        float intersectFar = 0.0;
        if (RT::ShootRay(startPos, direction, intersectFar))
        {
            Entity a = pActiveScene->CreateEntity("");
            auto &transformComponent = a.AddComponent<Component::Transform>();
            transformComponent.Position = startPos + direction * intersectFar;
            a.AttachModel("");

            LOG_TRACE("Created ray: (%f, %f, %f) -> (%f, %f, %f)", startPos.x, startPos.y, startPos.z, transformComponent.Position.x,
                      transformComponent.Position.y, transformComponent.Position.z);
        }
    }

}  // namespace lr