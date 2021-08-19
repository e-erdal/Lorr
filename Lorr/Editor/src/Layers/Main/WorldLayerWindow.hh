//
// Created on July 18th 2021 by e-erdal.
//

#pragma once

#include "Layers/Layer.hh"

class WorldLayerWindow: public LayerWindow
{
public:
    void Init() override;
    void Delete() override;

    void Update() override;

    const char *m_szTitle = "World Layers";
    bool m_bIsOpen = true;
};