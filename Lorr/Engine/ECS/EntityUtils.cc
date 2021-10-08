#include "Entity.hh"

#include "Engine/App/Engine.hh"

#include "Components/RenderableComponent.hh"
#include "Components/TransformComponent.hh"
#include "Components/PhysicsComponent.hh"
#include "Components/TextComponent.hh"

namespace Lorr
{
    Entity &ECSUtils::AttachDynamicBox(Entity &target, float destiny, float friction)
    {
        if (!target.HasComponent<Component::Transform>())
        {
            LOG_ERROR("Dynamic box component requires Transform component! Attach it first.");
            return target;
        }

        auto &c = target.AddComponent<Component::Physics>();
        auto &transform = target.GetComponent<Component::Transform>();

        //* Init body definitions
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(transform.Position.x / 32, transform.Position.y / 32);
        bodyDef.angle = transform.Rotation;

        //* Init body shape
        b2CircleShape dynamicBox;
        dynamicBox.m_radius = 5.f;

        //* Init fixture definiton
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = destiny;
        fixtureDef.friction = friction;

        //* Register definitions
        c.pBody = GetEngine()->GetPhyWorld()->CreateBody(&bodyDef);
        c.pBody->CreateFixture(&fixtureDef);
        transform.SetOriginCenter();

        return target;
    }

    Entity &ECSUtils::AttachStaticBox(Entity &target, float destiny)
    {
        if (!target.HasComponent<Component::Transform>())
        {
            LOG_ERROR("Static box component requires Transform component! Attach it first.");
            return target;
        }

        auto &c = target.AddComponent<Component::Physics>();
        auto &transform = target.GetComponent<Component::Transform>();

        //* Init body definitions
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(transform.Position.x, transform.Position.y);
        bodyDef.angle = transform.Rotation;

        //* Init box shape
        b2PolygonShape staticBox;
        staticBox.SetAsBox(transform.Size.x, transform.Size.y);

        c.pBody = GetEngine()->GetPhyWorld()->CreateBody(&bodyDef);
        c.pBody->CreateFixture(&staticBox, destiny);
        transform.SetOriginCenter();

        return target;
    }

}  // namespace Lorr