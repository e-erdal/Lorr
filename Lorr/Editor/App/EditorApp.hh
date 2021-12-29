//
// Created on Sunday 26th December 2021 by e-erdal
//

#pragma once

#include "Engine/App/Engine.hh"

#include "Layers/Main/MainLayer.hh"

class EditorApp : public lr::BaseApp
{
public:
    void Init() override;

    void Tick(float fDelta) override;
    void Draw() override;

    void OnResolutionChanged(u32 width, u32 height);

private:
    MainLayer *m_MainLayer = new MainLayer;
};