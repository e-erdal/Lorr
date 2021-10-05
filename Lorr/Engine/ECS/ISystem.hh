//
// Created on Sunday 3rd October 2021 by e-erdal
//

#pragma once

#include "Entity.hh"

namespace Lorr
{
    class ISystem
    {
    public:
        ISystem(EntityRegistry *pRegistry) : m_pRegistry(pRegistry){};
        virtual ~ISystem() = default;
        virtual void Tick(float deltaTime) = 0;
        virtual void Draw() = 0;

    protected:
        EntityRegistry *m_pRegistry = 0;
    };

}  // namespace Lorr