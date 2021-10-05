//
// Created on August 8th 2021 by e-erdal.
//

#pragma once

#include "Engine/App/BaseApp.hh"

#include "Engine/ECS/Scene.hh"

class GameApp : public Lorr::BaseApp
{
public:
    void Init() override;
    void Tick(float fDelta) override;
    void Draw() override;

private:
    Lorr::Scene *m_pCurrentScene = new Lorr::Scene;
};