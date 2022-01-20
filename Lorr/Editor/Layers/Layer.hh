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

#define rgb(r, g, b) ImVec4((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f)
static const ImVec4 kDefaultGray = rgb(32, 32, 32);
static const ImVec4 kDefaultGrayLower = rgb(22, 22, 22);
static const ImVec4 kDefaultBlack = rgb(15, 15, 15);

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