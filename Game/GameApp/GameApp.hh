//
// Created on August 8th 2021 by e-erdal.
//

#pragma once

#include "Engine/App/BaseApp.hh"

#include "Engine/ECS/Scene.hh"

class GameApp : public lr::BaseApp
{
public:
    void Init() override;
    void Tick(float fDelta) override;
    void Draw() override;

private:
    void LoadResources();
    void OnResolutionChanged(u32 width, u32 height);
};