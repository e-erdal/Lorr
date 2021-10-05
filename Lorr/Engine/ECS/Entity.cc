#include "Entity.hh"

#include "Components/RenderableComponent.hh"

namespace Lorr
{
    Entity::Entity(const Identifier &ident, EntityRegistry *pRegistry) : m_Identifier(ident), m_Registry(pRegistry)
    {
        m_Handle = pRegistry->create();
    }

    Entity::Entity(const Identifier &ident, EntityRegistry *pRegistry, const EntityHandle &other) : m_Identifier(ident), m_Registry(pRegistry), m_Handle(other)
    {
    }

}  // namespace Lorr