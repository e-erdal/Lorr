#include "Scene.hh"

#include "Components/TransformComponent.hh"

#include "Systems/TransformSystem.hh"
#include "Systems/RendererSystem.hh"

namespace Lorr
{
    Scene::~Scene()
    {
        for (auto &&system : m_Systems) delete system;
    }

    void Scene::Init(const Identifier &ident)
    {
        m_Identifier = ident;

        RegisterSystem<System::TransformSystem>();
        RegisterSystem<System::RendererSystem>();
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

    void Scene::SortAllByDepth()
    {
        m_Registry.sort<Component::Transform>([](const Component::Transform &lhs, const Component::Transform &rhs) { return lhs.Position.z < rhs.Position.z; });
    }

}  // namespace Lorr