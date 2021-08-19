//
// Created on August 8th 2021 by e-erdal.
//

#pragma once

#include "Engine.hh"

class GameApp: public Lorr::BaseApp
{
public:
    void Init() override;
    void Tick(float fDelta) override;
    void Draw() override;
};