//
// Created on Sunday 3rd October 2021 by e-erdal
// TODO: Rewrite this with DOD approach
//

#pragma once

#include "Engine/Graphics/Font.hh"

#include <entt.hpp>
#include <tinyutf8/tinyutf8.h>

namespace lr
{
    using EntityRegistry = entt::registry;
    using EntityHandle = entt::entity;
    constexpr EntityHandle kInvalidEntity = entt::null;

    struct Entity
    {
    public:
        Entity() = default;
        Entity(const Identifier &ident, EntityRegistry *pRegistry);
        Entity(EntityRegistry *pRegistry, const EntityHandle &other);

        template<typename T>
        bool HasComponent()
        {
            ZoneScoped;

            return m_Registry->has<T>(m_Handle);
        }

        template<typename T>
        T &GetComponent()
        {
            ZoneScoped;

            if (!this->HasComponent<T>())
            {
                assert("Component not found in registry. Stop using it!");
            }

            return m_Registry->get<T>(m_Handle);
        }

        template<typename T, typename... Args>
        T &AddComponent(Args &&...tArgs)
        {
            ZoneScoped;

            if (!this->HasComponent<T>())
            {
                assert("Entity already has that component.");
            }

            return m_Registry->emplace<T>(m_Handle, std::forward<Args>(tArgs)...);
        }

        template<typename T>
        void RemoveComponent()
        {
            ZoneScoped;

            m_Registry->remove_if_exists<T>(m_Handle);
        }

    public:
        /// Chain methods to attach components to handle
        Entity &AttachDynamicBox(float destiny, float friction);
        Entity &AttachStaticBox(float destiny = 0.f);
        Entity &AttachText(const Identifier &fontIdent, TextAlignment alignment, const tiny_utf8::string &text, const glm::vec3 &pos, u32 maxWidth = 0);
        Entity &AttachModel(const std::string &path);
        Entity &AttachCamera3D(const glm::vec3 &pos, const glm::vec2 &size, const glm::vec3 &direction, const glm::vec3 &up, float fov, float zNear, float zFar);
        Entity &AttachCamera2D(const glm::vec2 &pos, const glm::vec2 &size);

        /// Chain methods to get component data from handle
        glm::mat4 GetCameraMatrix();

    private:
        EntityRegistry *m_Registry = 0;
        EntityHandle m_Handle = kInvalidEntity;
    };

}  // namespace lr
