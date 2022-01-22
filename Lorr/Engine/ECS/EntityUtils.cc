#include "Entity.hh"

#include "Engine/App/Engine.hh"

#include "Components/BaseComponent.hh"
#include "Components/CameraComponent.hh"
#include "Components/RenderableComponent.hh"
#include "Components/TransformComponent.hh"
#include "Components/PhysicsComponent.hh"
#include "Components/TextComponent.hh"

#include "Engine/Model/Model.hh"
#include "Engine/Utils/Random.hh"

namespace lr
{
    Entity &Entity::AttachDynamicBox(float destiny, float friction)
    {
        ZoneScoped;

        if (!HasComponent<Component::Transform>())
        {
            LOG_ERROR("Dynamic box component requires Transform component! Attach it first.");
            return *this;
        }

        // auto &c = AddComponent<Component::Physics>();
        auto &transform = GetComponent<Component::Transform>();

        // //* Init body definitions
        // b2BodyDef bodyDef;
        // bodyDef.type = b2_dynamicBody;
        // bodyDef.position.Set(transform.Position.x / 32, transform.Position.y / 32);
        // bodyDef.angle = transform.Rotation;

        // //* Init body shape
        // b2CircleShape dynamicBox;
        // dynamicBox.m_radius = 5.f;

        // //* Init fixture definiton
        // b2FixtureDef fixtureDef;
        // fixtureDef.shape = &dynamicBox;
        // fixtureDef.density = destiny;
        // fixtureDef.friction = friction;

        // //* Register definitions
        // c.pBody = GetEngine()->GetPhyWorld()->CreateBody(&bodyDef);
        // c.pBody->CreateFixture(&fixtureDef);
        // transform.SetOriginCenter();

        return *this;
    }

    Entity &Entity::AttachStaticBox(float destiny)
    {
        ZoneScoped;
        
        if (!HasComponent<Component::Transform>())
        {
            LOG_ERROR("Static box component requires Transform component! Attach it first.");
            return *this;
        }

        // auto &c = AddComponent<Component::Physics>();
        auto &transform = GetComponent<Component::Transform>();

        //* Init body definitions
        // b2BodyDef bodyDef;
        // bodyDef.type = b2_dynamicBody;
        // bodyDef.position.Set(transform.Position.x, transform.Position.y);
        // bodyDef.angle = transform.Rotation;

        // //* Init box shape
        // b2PolygonShape staticBox;
        // staticBox.SetAsBox(transform.Size.x, transform.Size.y);

        // c.pBody = GetEngine()->GetPhyWorld()->CreateBody(&bodyDef);
        // c.pBody->CreateFixture(&staticBox, destiny);
        // transform.SetOriginCenter();

        return *this;
    }

    Entity &Entity::AttachText(const Identifier &fontIdent, TextAlignment alignment, const tiny_utf8::string &text, const glm::vec3 &pos, u32 maxWidth)
    {
        ZoneScoped;
        
        if (!HasComponent<Component::Transform>()) AddComponent<Component::Transform>(pos);
        AddComponent<Component::Text>(GetEngine()->GetResourceMan()->GetResource<Font>(fontIdent), alignment, text, maxWidth);

        return *this;
    }

    Entity &Entity::AttachModel(const eastl::string &path)
    {
        ZoneScoped;

        Model &model = this->AddComponent<Model>();
        model.AddSphere(Random::Float(0.5, 5.0), 8, nullptr);
        
        return *this;
    }

    Entity &Entity::AttachCamera3D(const glm::vec3 &pos, const glm::vec2 &size, const glm::vec3 &direction, const glm::vec3 &up, float fov, float zNear, float zFar)
    {
        ZoneScoped;
        
        AddComponent<Component::Camera3DController>(pos, size, direction, up, fov, zNear, zFar);
        return *this;
    }

    Entity &Entity::AttachCamera2D(const glm::vec2 &pos, const glm::vec2 &size)
    {
        ZoneScoped;
        
        AddComponent<Component::Camera2DController>(pos, size);
        return *this;
    }

    glm::mat4 Entity::GetCameraMatrix()
    {
        ZoneScoped;
        
        if (HasComponent<Component::Camera3DController>())
        {
            auto &component = GetComponent<Component::Camera3DController>();
            return glm::transpose(component.m_Handle.GetProjection() * component.m_Handle.GetView());
        }
        else if (HasComponent<Component::Camera2DController>())
        {
            auto &component = GetComponent<Component::Camera2DController>();
            return component.m_Handle.GetMatrix();
        }

        /// Should we assert?
        return {};
    }

}  // namespace lr