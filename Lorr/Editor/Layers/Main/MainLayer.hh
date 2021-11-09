//
// Created on July 18th 2021 by e-erdal.
//

#pragma once

#include "Layers/Layer.hh"

#include "ResourceWindow.hh"
#include "EntityWindow.hh"
#include "PropertiesWindow.hh"

class MainLayer : Layer
{
public:
    void Init() override;
    void Delete() override;

    void Update() override;

private:
    ResourceWindow m_ResourceWindow;
    EntityWindow m_EntityWindow;
    PropertiesWindow m_PropertiesWindow;

    void BuildDock(ImGuiID DockID);
};