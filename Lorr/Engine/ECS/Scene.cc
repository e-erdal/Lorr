#include "Scene.hh"

#include "Components/BaseComponent.hh"
#include "Components/TransformComponent.hh"

#include "Systems/CameraControllerSystem.hh"
#include "Systems/TransformSystem.hh"
#include "Systems/PhysicsSystem.hh"

#include "Systems/Renderer/ModelRendererSystem.hh"
#include "Systems/Renderer/Renderer2DSystem.hh"

namespace lr
{
    Scene::~Scene()
    {
        for (auto &&system : m_Systems) delete system;
    }

    void Scene::Init(const Identifier &ident)
    {
        m_Identifier = ident;

        RegisterSystem<System::PhysicsSystem>();
        RegisterSystem<System::TransformSystem>();
        RegisterSystem<System::CameraControllerSystem>();
        RegisterSystem<System::ModelRendererSystem>();
        RegisterSystem<System::Renderer2DSystem>();
    }

    void Scene::Tick(float deltaTime)
    {
        for (auto system : m_Systems) system->Tick(deltaTime);
    }

    void Scene::Draw()
    {
        for (auto system : m_Systems) system->Draw();
    }

    Entity Scene::CreateEntity(const Identifier &ident)
    {
        return Entity(ident, GetRegistry());
    }

    Entity Scene::GetEntity(const Identifier &ident)
    {
        auto view = m_Registry.view<Component::Base>();
        for (auto entity : view)
        {
            auto &component = view.get<Component::Base>(entity);
            if (component.m_Identifier == ident) return Entity(&m_Registry, entity);
        }

        LOG_ERROR("Trying to get non-existing entity. What we trying to get is {}.", ident);
        return Entity(&m_Registry, kInvalidEntity);
    }

    void Scene::SortAllByDepth()
    {
        m_Registry.sort<Component::Transform>([](const Component::Transform &lhs, const Component::Transform &rhs) {
            return lhs.Position.z < rhs.Position.z;
        });
    }

}  // namespace lr