#pragma once
#include "BaseDrawPass.hpp"
#include "GeometrySubPass.hpp"
#include "GuiSubPass.hpp"
#include "SkyBoxSubPass.hpp"

namespace My {
class ForwardGeometryPass : public BaseDrawPass {
   public:
    ForwardGeometryPass() {
        m_DrawSubPasses.push_back(std::make_shared<GeometrySubPass>());
        m_DrawSubPasses.push_back(std::make_shared<SkyBoxSubPass>());
        m_DrawSubPasses.push_back(std::make_shared<GuiSubPass>());
    }
};
}  // namespace My
