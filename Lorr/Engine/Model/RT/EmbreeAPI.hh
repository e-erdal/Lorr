//
// Created on Friday 21st January 2022 by e-erdal
//

#pragma once

namespace lr::RT
{
    void Init();
    
    void NewGeometry(glm::vec3 *&pPositionsOut, u32 vertexSize, u32 *&pIndicesOut, u32 indexSize);
    void CommitGeometry();
    bool ShootRay(const glm::vec3 &origin, const glm::vec3 &direction, float &hitFarOut);

}  // namespace lr::RT