//
// Created on July 18th 2021 by e-erdal.
//

#pragma once

#include <vector>

#include <imgui.h>
#include <imgui_internal.h>

constexpr ImGuiWindowFlags g_DockWindowStyle = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
                                               | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus
                                               | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

constexpr ImGuiWindowFlags g_WindowStyle = 0;

class LayerWindow
{
public:
    virtual void Init() = 0;
    virtual void Delete() = 0;

    virtual void Update() = 0;
};

class Layer
{
public:
    virtual void Init() = 0;
    virtual void Delete() = 0;

    virtual void Update() = 0;
};