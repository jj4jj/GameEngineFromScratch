#import <MetalKit/MetalKit.h>
#import "GraphicsManager.hpp"
#import "MetalPipelineState.h"
#import "MetalPipelineStateManager.h"

namespace My {
struct MtlDrawBatchContext : public DrawBatchContext {
    uint32_t index_count;
    uint32_t index_offset;
    MTLPrimitiveType index_mode;
    MTLIndexType index_type;
    uint32_t property_count;
    uint32_t property_offset;
};
}

@interface Metal2Renderer : NSObject

- (void)initialize;

- (void)finalize;

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView
                                      device:(nonnull id<MTLDevice>)device;

- (void)setPerFrameConstants:(const DrawFrameContext &)context frameIndex:(const int32_t)index;

- (void)setLightInfo:(const LightInfo &)lightInfo frameIndex:(const int32_t)index;

- (void)drawSkyBox:(const Frame &)frame;

- (void)drawBatch:(const Frame &)frame;

- (void)updateDrawableSize:(CGSize)size;

- (void)createVertexBuffer:(const My::SceneObjectVertexArray &)v_property_array;

- (void)createIndexBuffer:(const My::SceneObjectIndexArray &)index_array;

- (texture_id)createTexture:(const My::Image &)image;

- (texture_id)createSkyBox:(const std::vector<const std::shared_ptr<My::Image>> &)images;

- (void)beginFrame:(const My::Frame &)frame;

- (void)endFrame:(const My::Frame &)frame;

- (void)beginPass:(const My::Frame &)frame;

- (void)endPass:(const My::Frame &)frame;

- (void)beginCompute;

- (void)endCompute;

- (void)setPipelineState:(const MetalPipelineState &)pipelineState
            frameContext:(const Frame &)frame;

- (texture_id)generateCubeShadowMapArray:(const uint32_t)width
                                height:(const uint32_t)height
                                 count:(const uint32_t)count;

- (texture_id)generateShadowMapArray:(const uint32_t)width
                            height:(const uint32_t)height
                             count:(const uint32_t)count;

- (void)beginShadowMap:(const int32_t)light_index
             shadowmap:(const texture_id&)shadowmap
                 frame:(const Frame &)frame;

- (void)endShadowMap:(const texture_id&)shadowmap;

- (void)setShadowMaps:(const Frame &)frame;

- (void)releaseTexture:(texture_id&)texture;

- (texture_id)generateTextureForWrite:(const uint32_t)width height:(const uint32_t)height;

- (void)bindTextureForWrite:(const texture_id&)texture atIndex:(const uint32_t)atIndex;

- (void)dispatch:(const uint32_t)width height:(const uint32_t)height depth:(const uint32_t)depth;

@property(nonnull, readonly, nonatomic) id<MTLDevice> device;

@end
