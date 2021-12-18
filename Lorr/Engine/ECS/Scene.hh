//
// Created on Sunday 3rd October 2021 by e-erdal
//

#pragma once

#include "Entity.hh"
#include "ISystem.hh"

namespace lr
{
    class Scene
    {
    public:
        Scene() = default;
        ~Scene();

        void Init(const Identifier &ident);
        void Tick(float deltaTime);
        void Draw();

        Entity CreateEntity(const Identifier &ident);
        Entity GetEntity(const Identifier &ident);

        void SortAllByDepth();

        template<typename T = ISystem>
        void RegisterSystem()
        {
            m_Systems.push_back(new T(&m_Registry));
        }

    public:
        EntityRegistry *GetRegistry()
        {
            return &m_Registry;
        }

    private:
        Identifier m_Identifier = kEmptyIdentifier;

        EntityRegistry m_Registry;

        std::vector<ISystem *> m_Systems;
    };

}  // namespace lr