//
// Created on July 18th 2021 by e-erdal.
//

#pragma once

#include "ResourceWindow.hh"

#include "Layers/Layer.hh"

#include "WorldLayerWindow.hh"

class MainLayer : Layer
{
public:
    void Init() override;
    void Delete() override;

    void Update() override;

private:
    ResourceWindow m_ResourceWindow;
    WorldLayerWindow m_WorldLayerWindow;

    void BuildDock( ImGuiID DockID );
};