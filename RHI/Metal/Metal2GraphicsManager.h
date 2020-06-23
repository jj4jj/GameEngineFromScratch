#pragma once
#include "GraphicsManager.hpp"
#include "IPipelineStateManager.hpp"
#include "portable.hpp"

OBJC_CLASS(Metal2Renderer);

namespace My {
class Metal2GraphicsManager : public GraphicsManager {
   public:
    int Initialize() final;
    void Finalize() final;

    void Draw() final;
    void Present() final;

    void SetPipelineState(const std::shared_ptr<PipelineState>& pipelineState,
                          const Frame& frame) final;

    void DrawBatch(const Frame& frame) final;

    texture_id GenerateCubeShadowMapArray(const uint32_t width,
                                       const uint32_t height,
                                       const uint32_t count) final;
    texture_id GenerateShadowMapArray(const uint32_t width, const uint32_t height,
                                   const uint32_t count) final;
    void BeginShadowMap(const int32_t light_index, const texture_id& shadowmap,
                        const Frame& frame) final;
    void EndShadowMap(const texture_id& shadowmap) final;
    void SetShadowMaps(const Frame& frame) final;
    void ReleaseTexture(texture_id& texture) final;

    // skybox
    void DrawSkyBox(const Frame& frame) final;

    // compute shader tasks
    void GenerateTextureForWrite(const char* id, const uint32_t width,
                                    const uint32_t height) final;
    void BindTextureForWrite(const char* id, const uint32_t slot_index) final;
    void Dispatch(const uint32_t width, const uint32_t height,
                  const uint32_t depth) final;

    void SetRenderer(Metal2Renderer* renderer) { m_pRenderer = renderer; }

   private:
    void BeginScene(const Scene& scene) final;
    void EndScene() final;

    void BeginFrame(const Frame& frame) final;
    void EndFrame(const Frame& frame) final;

    void BeginPass(const Frame& frame) final;
    void EndPass(const Frame& frame) final;

    void BeginCompute() final;
    void EndCompute() final;

    void initializeGeometries(const Scene& scene) override;
    void initializeSkyBox(const Scene& scene) override;

   private:
    Metal2Renderer* m_pRenderer;
};
}  // namespace My
