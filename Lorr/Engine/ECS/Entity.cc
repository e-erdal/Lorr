#include "Entity.hh"

#include "Components/BaseComponent.hh"

namespace Lorr
{
    Entity::Entity(const Identifier &ident, EntityRegistry *pRegistry) : m_Registry(pRegistry)
    {
        ZoneScoped;

        m_Handle = pRegistry->create();
        AddComponent<Component::Base>(ident);
    }

    Entity::Entity(EntityRegistry *pRegistry, const EntityHandle &other) : m_Registry(pRegistry), m_Handle(other)
    {
        ZoneScoped;
    }

}  // namespace Lorr