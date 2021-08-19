//
// Created on July 18th 2021 by e-erdal.
//

#pragma once

#include "Core/Graphics/Shader.hh"

#include "ResourceWindow.hh"

#include "Layers/Layer.hh"

#include "Model/Model.hh"
#include "WorldLayerWindow.hh"

class MainLayer: Layer
{
public:
    void Init() override;
    void Delete() override;

    void Update() override;

private:
    ResourceWindow m_ResourceWindow;
    WorldLayerWindow m_WorldLayerWindow;

    void BuildDock(ImGuiID DockID);

    Lorr::Shader *m_pShader = new Lorr::Shader;
    ID3D11Buffer *m_pVConstantBuffer = 0;
    ID3D11Buffer *m_pPConstantBuffer = 0;

    Lorr::Model m_Model;
};