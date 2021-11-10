//
// Created on Sunday 3rd October 2021 by e-erdal
// TODO: Rewrite this with DOD approach
//

#pragma once

#include <entt.hpp>
#include <tinyutf8/tinyutf8.h>

namespace Lorr
{
    using EntityRegistry = entt::registry;
    using EntityHandle = entt::entity;
    constexpr EntityHandle kInvalidEntity = entt::null;

    struct Entity
    {
    public:
        Entity() = default;
        Entity(const Identifier &ident, EntityRegistry *pRegistry);
        Entity(const Identifier &ident, EntityRegistry *pRegistry, const EntityHandle &other);

        template<typename T>
        bool HasComponent()
        {
            return m_Registry->has<T>(m_Handle);
        }

        template<typename T>
        T &GetComponent()
        {
            if (!this->HasComponent<T>())
            {
                assert("Component not found in registry. Stop using it!");
            }

            return m_Registry->get<T>(m_Handle);
        }

        template<typename T, typename... Args>
        T &AddComponent(Args &&...tArgs)
        {
            if (!this->HasComponent<T>())
            {
                assert("Entity already has that component.");
            }

            return m_Registry->emplace<T>(m_Handle, std::forward<Args>(tArgs)...);
        }

        template<typename T>
        void RemoveComponent()
        {
            m_Registry->remove_if_exists<T>(m_Handle);
        }

    public:
        Entity &AttachDynamicBox(float destiny, float friction);
        Entity &AttachStaticBox(float destiny = 0.f);
        Entity &AttachText(const Identifier &fontIdent, const tiny_utf8::string &text, const glm::vec3 &pos, u32 maxWidth = 0);

    private:
        Identifier m_Identifier = kEmptyIdentifier;

        EntityRegistry *m_Registry = 0;
        EntityHandle m_Handle = kInvalidEntity;
    };

}  // namespace Lorr
