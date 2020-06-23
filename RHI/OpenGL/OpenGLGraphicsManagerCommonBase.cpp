#include "OpenGLGraphicsManagerCommonBase.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>

#include "OpenGLPipelineStateManagerCommonBase.hpp"

#if defined(OS_ANDROID) || defined(OS_WEBASSEMBLY)
#include <GLES3/gl32.h>
#define GLAD_GL_ARB_compute_shader 0
#else
#include "glad/glad.h"
#endif

using namespace std;
using namespace My;

void OpenGLGraphicsManagerCommonBase::Present() { glFlush(); }

bool OpenGLGraphicsManagerCommonBase::setShaderParameter(
    const char* paramName, const Matrix4X4f& param) {
    unsigned int location;

    location = glGetUniformLocation(m_CurrentShader, paramName);
    if (location == -1) {
        return false;
    }
    glUniformMatrix4fv(location, 1, false, param);

    return true;
}

bool OpenGLGraphicsManagerCommonBase::setShaderParameter(
    const char* paramName, const Matrix4X4f* param, const int32_t count) {
    bool result = true;
    char uniformName[256];

    for (int32_t i = 0; i < count; i++) {
        sprintf(uniformName, "%s[%d]", paramName, i);
        result &= setShaderParameter(uniformName, *(param + i));
    }

    return result;
}

bool OpenGLGraphicsManagerCommonBase::setShaderParameter(
    const char* paramName, const Vector2f& param) {
    unsigned int location;

    location = glGetUniformLocation(m_CurrentShader, paramName);
    if (location == -1) {
        return false;
    }
    glUniform2fv(location, 1, param);

    return true;
}

bool OpenGLGraphicsManagerCommonBase::setShaderParameter(
    const char* paramName, const Vector3f& param) {
    unsigned int location;

    location = glGetUniformLocation(m_CurrentShader, paramName);
    if (location == -1) {
        return false;
    }
    glUniform3fv(location, 1, param);

    return true;
}

bool OpenGLGraphicsManagerCommonBase::setShaderParameter(
    const char* paramName, const Vector4f& param) {
    unsigned int location;

    location = glGetUniformLocation(m_CurrentShader, paramName);
    if (location == -1) {
        return false;
    }
    glUniform4fv(location, 1, param);

    return true;
}

bool OpenGLGraphicsManagerCommonBase::setShaderParameter(const char* paramName,
                                                         const float param) {
    unsigned int location;

    location = glGetUniformLocation(m_CurrentShader, paramName);
    if (location == -1) {
        return false;
    }
    glUniform1f(location, param);

    return true;
}

bool OpenGLGraphicsManagerCommonBase::setShaderParameter(const char* paramName,
                                                         const int32_t param) {
    unsigned int location;

    location = glGetUniformLocation(m_CurrentShader, paramName);
    if (location == -1) {
        return false;
    }
    glUniform1i(location, param);

    return true;
}

bool OpenGLGraphicsManagerCommonBase::setShaderParameter(const char* paramName,
                                                         const uint32_t param) {
    unsigned int location;

    location = glGetUniformLocation(m_CurrentShader, paramName);
    if (location == -1) {
        return false;
    }
    glUniform1ui(location, param);

    return true;
}

bool OpenGLGraphicsManagerCommonBase::setShaderParameter(const char* paramName,
                                                         const bool param) {
    unsigned int location;

    location = glGetUniformLocation(m_CurrentShader, paramName);
    if (location == -1) {
        return false;
    }
    glUniform1f(location, param);

    return true;
}

void OpenGLGraphicsManagerCommonBase::initializeGeometries(const Scene& scene) {
    uint32_t batch_index = 0;

    // Geometries
    for (const auto& _it : scene.GeometryNodes) {
        const auto& pGeometryNode = _it.second.lock();
        if (pGeometryNode && pGeometryNode->Visible()) {
            const auto& pGeometry =
                scene.GetGeometry(pGeometryNode->GetSceneObjectRef());
            assert(pGeometry);
            const auto& pMesh = pGeometry->GetMesh().lock();
            if (!pMesh) continue;

            // Set the number of vertex properties.
            const auto vertexPropertiesCount =
                pMesh->GetVertexPropertiesCount();

            // Allocate an OpenGL vertex array object.
            uint32_t vao;
            glGenVertexArrays(1, &vao);

            // Bind the vertex array object to store all the buffers and vertex
            // attributes we create here.
            glBindVertexArray(vao);

            uint32_t buffer_id;

            for (uint32_t i = 0; i < vertexPropertiesCount; i++) {
                const SceneObjectVertexArray& v_property_array =
                    pMesh->GetVertexPropertyArray(i);
                const auto v_property_array_data_size =
                    v_property_array.GetDataSize();
                const auto v_property_array_data = v_property_array.GetData();

                // Generate an ID for the vertex buffer.
                glGenBuffers(1, &buffer_id);

                // Bind the vertex buffer and load the vertex (position and
                // color) data into the vertex buffer.
                glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
                glBufferData(GL_ARRAY_BUFFER, v_property_array_data_size,
                             v_property_array_data, GL_STATIC_DRAW);

                glEnableVertexAttribArray(i);

                switch (v_property_array.GetDataType()) {
                    case VertexDataType::kVertexDataTypeFloat1:
                        glVertexAttribPointer(i, 1, GL_FLOAT, false, 0,
                                              nullptr);
                        break;
                    case VertexDataType::kVertexDataTypeFloat2:
                        glVertexAttribPointer(i, 2, GL_FLOAT, false, 0,
                                              nullptr);
                        break;
                    case VertexDataType::kVertexDataTypeFloat3:
                        glVertexAttribPointer(i, 3, GL_FLOAT, false, 0,
                                              nullptr);
                        break;
                    case VertexDataType::kVertexDataTypeFloat4:
                        glVertexAttribPointer(i, 4, GL_FLOAT, false, 0,
                                              nullptr);
                        break;
#if !defined(OS_ANDROID) && !defined(OS_WEBASSEMBLY)
                    case VertexDataType::kVertexDataTypeDouble1:
                        glVertexAttribPointer(i, 1, GL_DOUBLE, false, 0,
                                              nullptr);
                        break;
                    case VertexDataType::kVertexDataTypeDouble2:
                        glVertexAttribPointer(i, 2, GL_DOUBLE, false, 0,
                                              nullptr);
                        break;
                    case VertexDataType::kVertexDataTypeDouble3:
                        glVertexAttribPointer(i, 3, GL_DOUBLE, false, 0,
                                              nullptr);
                        break;
                    case VertexDataType::kVertexDataTypeDouble4:
                        glVertexAttribPointer(i, 4, GL_DOUBLE, false, 0,
                                              nullptr);
                        break;
#endif
                    default:
                        assert(0);
                }

                m_Buffers.push_back(buffer_id);
            }

            const auto indexGroupCount = pMesh->GetIndexGroupCount();

            uint32_t mode;
            switch (pMesh->GetPrimitiveType()) {
                case PrimitiveType::kPrimitiveTypePointList:
                    mode = GL_POINTS;
                    break;
                case PrimitiveType::kPrimitiveTypeLineList:
                    mode = GL_LINES;
                    break;
                case PrimitiveType::kPrimitiveTypeLineStrip:
                    mode = GL_LINE_STRIP;
                    break;
                case PrimitiveType::kPrimitiveTypeTriList:
                    mode = GL_TRIANGLES;
                    break;
                case PrimitiveType::kPrimitiveTypeTriStrip:
                    mode = GL_TRIANGLE_STRIP;
                    break;
                case PrimitiveType::kPrimitiveTypeTriFan:
                    mode = GL_TRIANGLE_FAN;
                    break;
                default:
                    // ignore
                    continue;
            }

            for (uint32_t i = 0; i < indexGroupCount; i++) {
                // Generate an ID for the index buffer.
                glGenBuffers(1, &buffer_id);

                const SceneObjectIndexArray& index_array =
                    pMesh->GetIndexArray(i);
                const auto index_array_size = index_array.GetDataSize();
                const auto index_array_data = index_array.GetData();

                // Bind the index buffer and load the index data into it.
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_array_size,
                             index_array_data, GL_STATIC_DRAW);

                // Set the number of indices in the index array.
                auto indexCount =
                    static_cast<int32_t>(index_array.GetIndexCount());
                uint32_t type;
                switch (index_array.GetIndexType()) {
                    case IndexDataType::kIndexDataTypeInt8:
                        type = GL_UNSIGNED_BYTE;
                        break;
                    case IndexDataType::kIndexDataTypeInt16:
                        type = GL_UNSIGNED_SHORT;
                        break;
                    case IndexDataType::kIndexDataTypeInt32:
                        type = GL_UNSIGNED_INT;
                        break;
                    default:
                        // not supported by OpenGL
                        cerr << "Error: Unsupported Index Type " << index_array
                             << endl;
                        cerr << "Mesh: " << *pMesh << endl;
                        cerr << "Geometry: " << *pGeometry << endl;
                        continue;
                }

                m_Buffers.push_back(buffer_id);

                auto dbc = make_shared<OpenGLDrawBatchContext>();

                const auto material_index = index_array.GetMaterialIndex();
                const auto& material_key =
                    pGeometryNode->GetMaterialRef(material_index);
                const auto material = scene.GetMaterial(material_key);
                if (material) {
                    function<uint32_t(const string, const shared_ptr<Image>&)>
                        upload_texture = [this](
                                             const string& texture_key,
                                             const shared_ptr<Image>& texture) {
                            uint32_t texture_id;
                            auto it = m_Textures.find(texture_key);
                            if (it == m_Textures.end()) {
                                glGenTextures(1, &texture_id);
                                glBindTexture(GL_TEXTURE_2D, texture_id);
                                uint32_t format, internal_format, type;
                                getOpenGLTextureFormat(*texture, format,
                                                       internal_format, type);
                                if (texture->compressed) {
                                    glCompressedTexImage2D(
                                        GL_TEXTURE_2D, 0, internal_format,
                                        texture->Width, texture->Height, 0,
                                        static_cast<int32_t>(
                                            texture->data_size),
                                        texture->data);
                                } else {
                                    glTexImage2D(
                                        GL_TEXTURE_2D, 0, internal_format,
                                        texture->Width, texture->Height, 0,
                                        format, type, texture->data);
                                }

                                glTexParameteri(GL_TEXTURE_2D,
                                                GL_TEXTURE_WRAP_S, GL_REPEAT);
                                glTexParameteri(GL_TEXTURE_2D,
                                                GL_TEXTURE_WRAP_T, GL_REPEAT);
                                glTexParameteri(GL_TEXTURE_2D,
                                                GL_TEXTURE_MAG_FILTER,
                                                GL_LINEAR);
                                glTexParameteri(GL_TEXTURE_2D,
                                                GL_TEXTURE_MIN_FILTER,
                                                GL_LINEAR_MIPMAP_LINEAR);
                                glGenerateMipmap(GL_TEXTURE_2D);

                                glBindTexture(GL_TEXTURE_2D, 0);

                                m_Textures[texture_key].texture = texture_id;
                                m_Textures[texture_key].width = texture->Width;
                                m_Textures[texture_key].height = texture->Height;
                                m_Textures[texture_key].depth = 1;
                            } else {
                                texture_id = static_cast<uint32_t>(it->second.texture);
                            }

                            return texture_id;
                        };

                    // base color / albedo
                    const auto& color = material->GetBaseColor();
                    if (color.ValueMap) {
                        const auto& texture_key = color.ValueMap->GetName();
                        const auto& texture = color.ValueMap->GetTextureImage();
                        dbc->material.diffuseMap.texture = upload_texture(texture_key, texture);
                        dbc->material.diffuseMap.width = texture->Width;
                        dbc->material.diffuseMap.height = texture->Height;
                        dbc->material.diffuseMap.depth = 1;
                        dbc->material.diffuseMap.index = 0;
                    }

                    // normal
                    const auto& normal = material->GetNormal();
                    if (normal.ValueMap) {
                        const auto& texture_key = normal.ValueMap->GetName();
                        const auto& texture =
                            normal.ValueMap->GetTextureImage();
                        dbc->material.normalMap.texture = upload_texture(texture_key, texture);
                        dbc->material.normalMap.width = texture->Width;
                        dbc->material.normalMap.height = texture->Height;
                        dbc->material.normalMap.depth = 1;
                        dbc->material.normalMap.index = 0;
                    }

                    // metallic
                    const auto& metallic = material->GetMetallic();
                    if (metallic.ValueMap) {
                        const auto& texture_key = metallic.ValueMap->GetName();
                        const auto& texture =
                            metallic.ValueMap->GetTextureImage();
                        dbc->material.metallicMap.texture = upload_texture(texture_key, texture);
                        dbc->material.metallicMap.width = texture->Width;
                        dbc->material.metallicMap.height = texture->Height;
                        dbc->material.metallicMap.depth = 1;
                        dbc->material.metallicMap.index = 0;
                    }

                    // roughness
                    const auto& roughness = material->GetRoughness();
                    if (roughness.ValueMap) {
                        const auto& texture_key = roughness.ValueMap->GetName();
                        const auto& texture =
                            roughness.ValueMap->GetTextureImage();
                        dbc->material.roughnessMap.texture = upload_texture(texture_key, texture);
                        dbc->material.roughnessMap.width = texture->Width;
                        dbc->material.roughnessMap.height = texture->Height;
                        dbc->material.roughnessMap.depth = 1;
                        dbc->material.roughnessMap.index = 0;
                    }

                    // ao
                    const auto& ao = material->GetAO();
                    if (ao.ValueMap) {
                        const auto& texture_key = ao.ValueMap->GetName();
                        const auto& texture = ao.ValueMap->GetTextureImage();
                        dbc->material.aoMap.texture = upload_texture(texture_key, texture);
                        dbc->material.aoMap.width = texture->Width;
                        dbc->material.aoMap.height = texture->Height;
                        dbc->material.aoMap.depth = 1;
                        dbc->material.aoMap.index = 0;
                    }

                    // height map
                    const auto& heightmap = material->GetHeight();
                    if (heightmap.ValueMap) {
                        const auto& texture_key = heightmap.ValueMap->GetName();
                        const auto& texture =
                            heightmap.ValueMap->GetTextureImage();
                        dbc->material.aoMap.texture = upload_texture(texture_key, texture);
                        dbc->material.aoMap.width = texture->Width;
                        dbc->material.aoMap.height = texture->Height;
                        dbc->material.aoMap.depth = 1;
                        dbc->material.aoMap.index = 0;
                    }
                }

                glBindVertexArray(0);

                dbc->batchIndex = batch_index++;
                dbc->vao = vao;
                dbc->mode = mode;
                dbc->type = type;
                dbc->count = indexCount;
                dbc->node = pGeometryNode;

                for (int32_t n = 0;
                     n < GfxConfiguration::kMaxInFlightFrameCount; n++) {
                    m_Frames[n].batchContexts.push_back(dbc);
                }
            }
        }
    }
}

void OpenGLGraphicsManagerCommonBase::initializeSkyBox(const Scene& scene) {
    // load skybox, irradiance map and radiance map
    uint32_t texture_id;
    const size_t kMaxMipLevels = 10;
    glGenTextures(1, &texture_id);
    GLenum target;
#if defined(OS_WEBASSEMBLY)
    target = GL_TEXTURE_2D_ARRAY;
#else
    target = GL_TEXTURE_CUBE_MAP_ARRAY;
#endif
    glBindTexture(target, texture_id);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, kMaxMipLevels);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    assert(scene.SkyBox);

    // skybox, irradiance map
    for (uint32_t i = 0; i < 12; i++) {
        auto& texture = scene.SkyBox->GetTexture(i);
        const auto& pImage = texture.GetTextureImage();
        uint32_t format, internal_format, type;
        getOpenGLTextureFormat(*pImage, format, internal_format, type);

        if (i == 0)  // do this only once
        {
            const uint32_t faces = 6;
            const uint32_t indexies = 2;
            constexpr int32_t depth = faces * indexies;
            glTexStorage3D(target, kMaxMipLevels, internal_format,
                           pImage->Width, pImage->Height, depth);
        }

        int32_t level = i / 6;
        int32_t zoffset = i % 6;
        if (pImage->compressed) {
            glCompressedTexSubImage3D(
                target, level, 0, 0, zoffset, pImage->Width, pImage->Height, 1,
                internal_format,
                static_cast<int32_t>(pImage->mipmaps[0].data_size),
                pImage->data);
        } else {
            glTexSubImage3D(target, level, 0, 0, zoffset, pImage->Width,
                            pImage->Height, 1, format, type, pImage->data);
        }
    }

    // radiance map
    for (uint32_t i = 12; i < 18; i++) {
        auto& texture = scene.SkyBox->GetTexture(i);
        const auto& pImage = texture.GetTextureImage();
        uint32_t format, internal_format, type;
        getOpenGLTextureFormat(*pImage, format, internal_format, type);

        int32_t zoffset = (i % 6) + 6;
        for (decltype(pImage->mipmaps.size()) level = 0;
             level < min(pImage->mipmaps.size(), kMaxMipLevels); level++) {
            if (pImage->compressed) {
                glCompressedTexSubImage3D(
                    target, level, 0, 0, zoffset, pImage->mipmaps[level].Width,
                    pImage->mipmaps[level].Height, 1, internal_format,
                    static_cast<int32_t>(pImage->mipmaps[level].data_size),
                    pImage->data + pImage->mipmaps[level].offset);
            } else {
                glTexSubImage3D(target, level, 0, 0, zoffset,
                                pImage->mipmaps[level].Width,
                                pImage->mipmaps[level].Height, 1, format, type,
                                pImage->data + pImage->mipmaps[level].offset);
            }
        }
    }

    m_Textures["SkyBox"].texture = texture_id;

    glBindTexture(target, 0);

    // skybox VAO
    uint32_t skyboxVAO, skyboxVBO[2];
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(2, skyboxVBO);
    glBindVertexArray(skyboxVAO);
    // vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), skyboxIndices,
                 GL_STATIC_DRAW);

    glBindVertexArray(0);

    m_Buffers.push_back(skyboxVBO[0]);
    m_Buffers.push_back(skyboxVBO[1]);

    m_SkyBoxDrawBatchContext.vao = skyboxVAO;
    m_SkyBoxDrawBatchContext.mode = GL_TRIANGLES;
    m_SkyBoxDrawBatchContext.type = GL_UNSIGNED_BYTE;
    m_SkyBoxDrawBatchContext.count =
        sizeof(skyboxIndices) / sizeof(skyboxIndices[0]);
}

void OpenGLGraphicsManagerCommonBase::EndScene() {
    for (int i = 0; i < m_Frames.size(); i++) {
        auto& batchContexts = m_Frames[i].batchContexts;

        for (auto& dbc : batchContexts) {
            glDeleteVertexArrays(
                1, &dynamic_pointer_cast<OpenGLDrawBatchContext>(dbc)->vao);
        }

        batchContexts.clear();

        if (m_uboDrawFrameConstant[i]) {
            glDeleteBuffers(1, &m_uboDrawFrameConstant[i]);
            m_uboDrawFrameConstant[i] = 0;
        }

        if (m_uboDrawBatchConstant[i]) {
            glDeleteBuffers(1, &m_uboDrawBatchConstant[i]);
            m_uboDrawBatchConstant[i] = 0;
        }

        if (m_uboLightInfo[i]) {
            glDeleteBuffers(1, &m_uboLightInfo[i]);
            m_uboLightInfo[i] = 0;
        }

        if (m_uboShadowMatricesConstant[i]) {
            glDeleteBuffers(1, &m_uboShadowMatricesConstant[i]);
            m_uboShadowMatricesConstant[i] = 0;
        }

#if DEBUG
        if (m_uboDebugConstant[i]) {
            glDeleteBuffers(1, &m_uboDebugConstant[i]);
            m_uboDebugConstant[i] = 0;
        }
#endif
    }

    if (m_SkyBoxDrawBatchContext.vao) {
        glDeleteVertexArrays(1, &m_SkyBoxDrawBatchContext.vao);
        m_SkyBoxDrawBatchContext.vao = 0;
    }

#if DEBUG
    m_DebugDrawBatchContext.clear();

    for (auto& buf : m_DebugBuffers) {
        glDeleteBuffers(1, &buf);
    }
#endif

    for (auto& buf : m_Buffers) {
        glDeleteBuffers(1, &buf);
    }

    for (auto& it : m_Textures) {
        GLuint texture_id = static_cast<GLuint>(it.second.texture);
        glDeleteTextures(1, &texture_id);
    }

    m_Buffers.clear();
    m_Textures.clear();

    GraphicsManager::EndScene();
}

void OpenGLGraphicsManagerCommonBase::BeginFrame(const Frame& frame) {
    // Set the color to clear the screen to.
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
    // Clear the screen and depth buffer.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SetPerFrameConstants(frame.frameContext);
    SetLightInfo(frame.lightInfo);
}

void OpenGLGraphicsManagerCommonBase::EndFrame(const Frame& frame) {
    GraphicsManager::EndFrame(frame);
}

void OpenGLGraphicsManagerCommonBase::SetPipelineState(
    const std::shared_ptr<PipelineState>& pipelineState, const Frame& frame) {
    const std::shared_ptr<const OpenGLPipelineState> pPipelineState =
        dynamic_pointer_cast<const OpenGLPipelineState>(pipelineState);
    m_CurrentShader = pPipelineState->shaderProgram;

    // Set the color shader as the current shader program and set the matrices
    // that it will use for rendering.
    glUseProgram(m_CurrentShader);

    switch (pipelineState->depthTestMode) {
        case DEPTH_TEST_MODE::NONE:
            glDisable(GL_DEPTH_TEST);
            break;
        case DEPTH_TEST_MODE::LARGE:
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_GREATER);
            break;
        case DEPTH_TEST_MODE::LARGE_EQUAL:
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_GEQUAL);
            break;
        case DEPTH_TEST_MODE::LESS:
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            break;
        case DEPTH_TEST_MODE::LESS_EQUAL:
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
            break;
        case DEPTH_TEST_MODE::EQUAL:
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_EQUAL);
            break;
        case DEPTH_TEST_MODE::NOT_EQUAL:
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_NOTEQUAL);
            break;
        case DEPTH_TEST_MODE::NEVER:
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_NEVER);
            break;
        case DEPTH_TEST_MODE::ALWAYS:
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_ALWAYS);
            break;
        default:
            assert(0);
    }

    if (pipelineState->bDepthWrite) {
        glDepthMask(GL_TRUE);
    } else {
        glDepthMask(GL_FALSE);
    }

    switch (pipelineState->cullFaceMode) {
        case CULL_FACE_MODE::NONE:
            glDisable(GL_CULL_FACE);
            break;
        case CULL_FACE_MODE::FRONT:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            break;
        case CULL_FACE_MODE::BACK:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            break;
        default:
            assert(0);
    }

    // Prepare & Bind per frame constant buffer
    uint32_t blockIndex =
        glGetUniformBlockIndex(m_CurrentShader, "PerFrameConstants");

    if (blockIndex != GL_INVALID_INDEX) {
        int32_t blockSize;

        glGetActiveUniformBlockiv(m_CurrentShader, blockIndex,
                                  GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

        assert(blockSize >= sizeof(PerFrameConstants));

        glUniformBlockBinding(m_CurrentShader, blockIndex, 10);
        glBindBufferBase(GL_UNIFORM_BUFFER, 10,
                         m_uboDrawFrameConstant[frame.frameIndex]);
    }

    // Prepare per batch constant buffer binding point
    blockIndex = glGetUniformBlockIndex(m_CurrentShader, "PerBatchConstants");

    if (blockIndex != GL_INVALID_INDEX) {
        int32_t blockSize;

        glGetActiveUniformBlockiv(m_CurrentShader, blockIndex,
                                  GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

        assert(blockSize >= sizeof(PerBatchConstants));

        glUniformBlockBinding(m_CurrentShader, blockIndex, 11);
        glBindBufferBase(GL_UNIFORM_BUFFER, 11,
                         m_uboDrawBatchConstant[frame.frameIndex]);
    }

    // Prepare & Bind light info
    blockIndex = glGetUniformBlockIndex(m_CurrentShader, "LightInfo");

    if (blockIndex != GL_INVALID_INDEX) {
        int32_t blockSize;

        glGetActiveUniformBlockiv(m_CurrentShader, blockIndex,
                                  GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

        assert(blockSize >= sizeof(LightInfo));

        glUniformBlockBinding(m_CurrentShader, blockIndex, 12);
        glBindBufferBase(GL_UNIFORM_BUFFER, 12,
                         m_uboLightInfo[frame.frameIndex]);
    }

    if (pPipelineState->flag == PIPELINE_FLAG::SHADOW) {
        uint32_t blockIndex =
            glGetUniformBlockIndex(m_CurrentShader, "ShadowMapConstants");
        assert(blockIndex != GL_INVALID_INDEX);

        int32_t blockSize;

        glGetActiveUniformBlockiv(m_CurrentShader, blockIndex,
                                  GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

        assert(blockSize >= sizeof(ShadowMapConstants));

        glUniformBlockBinding(m_CurrentShader, blockIndex, 13);
        glBindBufferBase(GL_UNIFORM_BUFFER, 13,
                         m_uboShadowMatricesConstant[frame.frameIndex]);
    }

    // Set common textures
    // Bind LUT table
    auto texture_id = frame.brdfLUT;
    setShaderParameter("SPIRV_Cross_CombinedbrdfLUTsamp0", 6);
    glActiveTexture(GL_TEXTURE6);
    if (texture_id.texture > 0) {
        glBindTexture(GL_TEXTURE_2D, texture_id.texture);
    } else {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Set Sky Box
    setShaderParameter("SPIRV_Cross_Combinedskyboxsamp0", 10);
    glActiveTexture(GL_TEXTURE10);
    GLenum target;
#if defined(OS_WEBASSEMBLY)
    target = GL_TEXTURE_2D_ARRAY;
#else
    target = GL_TEXTURE_CUBE_MAP_ARRAY;
#endif
    texture_id = frame.skybox;
    if (texture_id.texture >= 0) {
        glBindTexture(target, (GLuint)texture_id.texture);
    }
}

void OpenGLGraphicsManagerCommonBase::SetPerFrameConstants(
    const DrawFrameContext& context) {
    if (!m_uboDrawFrameConstant[m_nFrameIndex]) {
        glGenBuffers(1, &m_uboDrawFrameConstant[m_nFrameIndex]);
    }

    glBindBuffer(GL_UNIFORM_BUFFER, m_uboDrawFrameConstant[m_nFrameIndex]);

    auto constants = static_cast<PerFrameConstants>(context);

    glBufferData(GL_UNIFORM_BUFFER, kSizePerFrameConstantBuffer, &constants,
                 GL_DYNAMIC_DRAW);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OpenGLGraphicsManagerCommonBase::SetLightInfo(const LightInfo& lightInfo) {
    if (!m_uboLightInfo[m_nFrameIndex]) {
        glGenBuffers(1, &m_uboLightInfo[m_nFrameIndex]);
    }

    glBindBuffer(GL_UNIFORM_BUFFER, m_uboLightInfo[m_nFrameIndex]);

    glBufferData(GL_UNIFORM_BUFFER, kSizeLightInfo, &lightInfo,
                 GL_DYNAMIC_DRAW);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OpenGLGraphicsManagerCommonBase::SetPerBatchConstants(
    const DrawBatchContext& context) {
    if (!m_uboDrawBatchConstant[m_nFrameIndex]) {
        glGenBuffers(1, &m_uboDrawBatchConstant[m_nFrameIndex]);
    }

    glBindBuffer(GL_UNIFORM_BUFFER, m_uboDrawBatchConstant[m_nFrameIndex]);

    const auto& constant = static_cast<const PerBatchConstants&>(context);

    glBufferData(GL_UNIFORM_BUFFER, kSizePerBatchConstantBuffer, &constant,
                 GL_DYNAMIC_DRAW);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OpenGLGraphicsManagerCommonBase::DrawBatch(const Frame& frame) {
    for (auto& pDbc : frame.batchContexts) {
        SetPerBatchConstants(*pDbc);

        const auto& dbc = dynamic_cast<const OpenGLDrawBatchContext&>(*pDbc);

        // Bind textures
        setShaderParameter("SPIRV_Cross_CombineddiffuseMapsamp0", 0);
        glActiveTexture(GL_TEXTURE0);
        if (dbc.material.diffuseMap.texture > 0) {
            glBindTexture(GL_TEXTURE_2D, dbc.material.diffuseMap.texture);
        } else {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        setShaderParameter("SPIRV_Cross_CombinednormalMapsamp0", 1);
        glActiveTexture(GL_TEXTURE1);
        if (dbc.material.normalMap.texture > 0) {
            glBindTexture(GL_TEXTURE_2D, dbc.material.normalMap.texture);
        } else {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        setShaderParameter("SPIRV_Cross_CombinedmetallicMapsamp0", 2);
        glActiveTexture(GL_TEXTURE2);
        if (dbc.material.metallicMap.texture > 0) {
            glBindTexture(GL_TEXTURE_2D, dbc.material.metallicMap.texture);
        } else {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        setShaderParameter("SPIRV_Cross_CombinedroughnessMapsamp0", 3);
        glActiveTexture(GL_TEXTURE3);
        if (dbc.material.roughnessMap.texture > 0) {
            glBindTexture(GL_TEXTURE_2D, dbc.material.roughnessMap.texture);
        } else {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        setShaderParameter("SPIRV_Cross_CombinedaoMapsamp0", 4);
        glActiveTexture(GL_TEXTURE4);
        if (dbc.material.aoMap.texture > 0) {
            glBindTexture(GL_TEXTURE_2D, dbc.material.aoMap.texture);
        } else {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        setShaderParameter("SPIRV_Cross_CombinedheightMapsamp0", 5);
        glActiveTexture(GL_TEXTURE5);
        if (dbc.material.heightMap.texture > 0) {
            glBindTexture(GL_TEXTURE_2D, dbc.material.heightMap.texture);
        } else {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        glBindVertexArray(dbc.vao);

        glDrawElements(dbc.mode, dbc.count, dbc.type, nullptr);
    }

    glBindVertexArray(0);
}

texture_id OpenGLGraphicsManagerCommonBase::GenerateCubeShadowMapArray(
    const uint32_t width, const uint32_t height, const uint32_t count) {
    texture_id result;

    uint32_t shadowMap;

    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, shadowMap);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T,
                    GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R,
                    GL_CLAMP_TO_EDGE);
    glTexStorage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 1, GL_DEPTH_COMPONENT24, width,
                   height, count * 6);

    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, 0);

    // register the shadow map
    result.texture = static_cast<intptr_t>(shadowMap);
    result.width = width;
    result.height = height;
    result.depth = count;
    result.index = 0;

    return result;
}

texture_id OpenGLGraphicsManagerCommonBase::GenerateShadowMapArray(
    const uint32_t width, const uint32_t height, const uint32_t count) {

    texture_id result;

    uint32_t shadowMap;

    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_2D_ARRAY, shadowMap);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_DEPTH_COMPONENT24, width, height,
                   count);

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    // register the shadow map
    result.texture = static_cast<intptr_t>(shadowMap);
    result.width = width;
    result.height = height;
    result.depth = count;
    result.index = 0;

    return result;
}

void OpenGLGraphicsManagerCommonBase::BeginShadowMap(
    const int32_t light_index, const texture_id& shadowmap, const Frame& frame) {
    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth
    // buffer.
    glGenFramebuffers(1, &m_ShadowMapFramebufferName);

    glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowMapFramebufferName);

    if (frame.lightInfo.lights[light_index].lightType == LightType::Omni) {
#if defined(OS_WEBASSEMBLY)
        glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                  (uint32_t)shadowmap.texture, 0, shadowmap.index);
#else
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                             (uint32_t)shadowmap.texture, 0);
#endif
    } else {
        // we only bind the single layer to FBO
        glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                  (uint32_t)shadowmap.texture, 0, shadowmap.index);
    }

    // Always check that our framebuffer is ok
    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        assert(0);
    }

    glViewport(0, 0, shadowmap.width, shadowmap.height);

    glDrawBuffers(0, nullptr);  // No color buffer is drawn to.
    // make sure omni light shadowmap arrays get cleared only
    // once, because glClear will clear all cubemaps in the array
    if (frame.lightInfo.lights[light_index].lightType != LightType::Omni ||
        shadowmap.index == 0) {
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    float nearClipDistance = 1.0f;
    float farClipDistance = 100.0f;
    ShadowMapConstants constants;

    constants.light_index = light_index;
    constants.shadowmap_layer_index = static_cast<float>(shadowmap.index);
    constants.near_plane = nearClipDistance;
    constants.far_plane = farClipDistance;

    if (!m_uboShadowMatricesConstant[frame.frameIndex]) {
        glGenBuffers(1, &m_uboShadowMatricesConstant[frame.frameIndex]);
    }

    glBindBuffer(GL_UNIFORM_BUFFER,
                 m_uboShadowMatricesConstant[frame.frameIndex]);

    glBufferData(GL_UNIFORM_BUFFER, sizeof(constants), &constants,
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OpenGLGraphicsManagerCommonBase::EndShadowMap([[maybe_unused]] const texture_id& shadowmap) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDeleteFramebuffers(1, &m_ShadowMapFramebufferName);

    const GfxConfiguration& conf = g_pApp->GetConfiguration();
    glViewport(0, 0, conf.screenWidth, conf.screenHeight);
}

void OpenGLGraphicsManagerCommonBase::SetShadowMaps(const Frame& frame) {
    const float color[] = {1.0f, 1.0f, 1.0f, 1.0f};
    setShaderParameter("SPIRV_Cross_CombinedshadowMapsamp0", 7);
    glActiveTexture(GL_TEXTURE7);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, color);
    auto texture_id = frame.frameContext.shadowMap;
    if (texture_id.texture >= 0) {
        glBindTexture(GL_TEXTURE_2D_ARRAY, (GLuint)texture_id.texture);
    }

    setShaderParameter("SPIRV_Cross_CombinedglobalShadowMapsamp0", 8);
    glActiveTexture(GL_TEXTURE8);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, color);
    texture_id = frame.frameContext.globalShadowMap;
    if (texture_id.texture >= 0) {
        glBindTexture(GL_TEXTURE_2D_ARRAY, (GLuint)texture_id.texture);
    }

    setShaderParameter("SPIRV_Cross_CombinedcubeShadowMapsamp0", 9);
    glActiveTexture(GL_TEXTURE9);
    GLenum target;
#if defined(OS_WEBASSEMBLY)
    target = GL_TEXTURE_2D_ARRAY;
#else
    target = GL_TEXTURE_CUBE_MAP_ARRAY;
#endif
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture_id = frame.frameContext.cubeShadowMap;
    if (texture_id.texture >= 0) {
        glBindTexture(target, (GLuint)texture_id.texture);
    }
}

void OpenGLGraphicsManagerCommonBase::ReleaseTexture(texture_id& texture) {
    auto id = (uint32_t)texture.texture;
    glDeleteTextures(1, &id);
    texture.texture = -1;
    texture.width = 0;
    texture.height = 0;
    texture.depth = 0;
    texture.index = 0;
}

void OpenGLGraphicsManagerCommonBase::DrawSkyBox([[maybe_unused]] const Frame& frame) {
    glBindVertexArray(m_SkyBoxDrawBatchContext.vao);

    glDrawElements(m_SkyBoxDrawBatchContext.mode,
                   m_SkyBoxDrawBatchContext.count,
                   m_SkyBoxDrawBatchContext.type, nullptr);

    glBindVertexArray(0);
}

void OpenGLGraphicsManagerCommonBase::GenerateTexture(
    const char* id, const uint32_t width, const uint32_t height) {
    // Depth texture. Slower than a depth buffer, but you can sample it later in
    // your shader
    uint32_t texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RG16F, width, height);

    m_Textures[id].texture = texture;
    m_Textures[id].width = width;
    m_Textures[id].height = height;
    m_Textures[id].depth = 0;
}

void OpenGLGraphicsManagerCommonBase::BeginRenderToTexture(
    int32_t& context, const int32_t texture, const uint32_t width,
    const uint32_t height) {
    uint32_t framebuffer;
    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth
    // buffer.
    glGenFramebuffers(1, &framebuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

#if defined(OS_WEBASSEMBLY)
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           (uint32_t)texture, 0);
#else
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                         (uint32_t)texture, 0);
#endif

    // Always check that our framebuffer is ok
    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        assert(0);
    }

    context = (int32_t)framebuffer;

    uint32_t buf[] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, buf);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, width, height);
}

void OpenGLGraphicsManagerCommonBase::EndRenderToTexture(int32_t& context) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    auto framebuffer = (uint32_t)context;
    glDeleteFramebuffers(1, &framebuffer);
    context = 0;

    const GfxConfiguration& conf = g_pApp->GetConfiguration();
    glViewport(0, 0, conf.screenWidth, conf.screenHeight);
}

void OpenGLGraphicsManagerCommonBase::GenerateTextureForWrite(
    const char* id, const uint32_t width, const uint32_t height) {
    uint32_t tex_output;
    glGenTextures(1, &tex_output);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_output);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, width, height, 0, GL_RG, GL_FLOAT,
                 nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    m_Textures[id].texture = tex_output;
    m_Textures[id].width = width;
    m_Textures[id].height = height;
    m_Textures[id].depth = 1;
    m_Textures[id].index = 0;
}

void OpenGLGraphicsManagerCommonBase::BindTextureForWrite(
    const char* id, const uint32_t slot_index) {
#if !defined(OS_WEBASSEMBLY)
    // Bind it as Write-only Texture
    if (GLAD_GL_ARB_compute_shader) {
        glBindImageTexture(0, m_Textures[id].texture, 0, GL_FALSE, 0, GL_WRITE_ONLY,
                           GL_RG32F);
    }
#endif
}

void OpenGLGraphicsManagerCommonBase::Dispatch(const uint32_t width,
                                               const uint32_t height,
                                               const uint32_t depth) {
#if !defined(OS_WEBASSEMBLY)
    if (GLAD_GL_ARB_compute_shader) {
        glDispatchCompute((GLuint)width, (GLuint)height, (GLuint)depth);
        // make sure writing to image has finished before read
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    }

    glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RG32F);
#endif
}

void OpenGLGraphicsManagerCommonBase::ResizeCanvas(int32_t width,
                                                   int32_t height) {
    // Reset View
    glViewport(0, 0, (GLint)width, (GLint)height);
}
#ifdef DEBUG

void OpenGLGraphicsManagerCommonBase::DrawPoint(const Point& point,
                                                const Vector3f& color) {
    uint32_t vao;
    glGenVertexArrays(1, &vao);

    // Bind the vertex array object to store all the buffers and vertex
    // attributes we create here.
    glBindVertexArray(vao);

    uint32_t buffer_id;

    // Generate an ID for the vertex buffer.
    glGenBuffers(1, &buffer_id);

    // Bind the vertex buffer and load the vertex (position and color) data into
    // the vertex buffer.
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Point), point, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);

    m_DebugBuffers.push_back(buffer_id);

    DebugDrawBatchContext& dbc = *(new DebugDrawBatchContext);
    dbc.vao = vao;
    dbc.mode = GL_POINTS;
    dbc.count = 1;
    dbc.color = color;
    BuildIdentityMatrix(dbc.trans);

    m_DebugDrawBatchContext.push_back(std::move(dbc));
}

void OpenGLGraphicsManagerCommonBase::drawPoints(const Point* buffer,
                                                 const size_t count,
                                                 const Matrix4X4f& trans,
                                                 const Vector3f& color) {
    uint32_t vao;
    glGenVertexArrays(1, &vao);

    // Bind the vertex array object to store all the buffers and vertex
    // attributes we create here.
    glBindVertexArray(vao);

    uint32_t buffer_id;

    // Generate an ID for the vertex buffer.
    glGenBuffers(1, &buffer_id);

    // Bind the vertex buffer and load the vertex (position and color) data into
    // the vertex buffer.
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * count, buffer,
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);

    m_DebugBuffers.push_back(buffer_id);

    DebugDrawBatchContext& dbc = *(new DebugDrawBatchContext);
    dbc.vao = vao;
    dbc.mode = GL_POINTS;
    dbc.count = static_cast<int32_t>(count);
    dbc.color = color;
    dbc.trans = trans;

    m_DebugDrawBatchContext.push_back(std::move(dbc));
}

void OpenGLGraphicsManagerCommonBase::DrawPointSet(const PointSet& point_set,
                                                   const Vector3f& color) {
    Matrix4X4f trans;
    BuildIdentityMatrix(trans);

    DrawPointSet(point_set, trans, color);
}

void OpenGLGraphicsManagerCommonBase::DrawPointSet(const PointSet& point_set,
                                                   const Matrix4X4f& trans,
                                                   const Vector3f& color) {
    const auto count = point_set.size();
    auto* buffer = new Point[count];
    int i = 0;
    for (const auto& point_ptr : point_set) {
        buffer[i++] = *point_ptr;
    }

    drawPoints(buffer, count, trans, color);

    delete[] buffer;
}

void OpenGLGraphicsManagerCommonBase::DrawLine(const PointList& vertices,
                                               const Matrix4X4f& trans,
                                               const Vector3f& color) {
    const auto count = vertices.size();
    auto* _vertices = new GLfloat[3 * count];

    for (auto i = 0; i < count; i++) {
        _vertices[3 * i] = vertices[i]->data[0];
        _vertices[3 * i + 1] = vertices[i]->data[1];
        _vertices[3 * i + 2] = vertices[i]->data[2];
    }

    uint32_t vao;
    glGenVertexArrays(1, &vao);

    // Bind the vertex array object to store all the buffers and vertex
    // attributes we create here.
    glBindVertexArray(vao);

    uint32_t buffer_id;

    // Generate an ID for the vertex buffer.
    glGenBuffers(1, &buffer_id);

    // Bind the vertex buffer and load the vertex (position and color) data into
    // the vertex buffer.
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * count, _vertices,
                 GL_STATIC_DRAW);

    delete[] _vertices;

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);

    m_DebugBuffers.push_back(buffer_id);

    DebugDrawBatchContext& dbc = *(new DebugDrawBatchContext);
    dbc.vao = vao;
    dbc.mode = GL_LINES;
    dbc.count = static_cast<int32_t>(count);
    dbc.color = color;
    dbc.trans = trans;

    m_DebugDrawBatchContext.push_back(std::move(dbc));
}

void OpenGLGraphicsManagerCommonBase::DrawLine(const PointList& vertices,
                                               const Vector3f& color) {
    Matrix4X4f trans;
    BuildIdentityMatrix(trans);

    DrawLine(vertices, trans, color);
}

void OpenGLGraphicsManagerCommonBase::DrawLine(const Point& from,
                                               const Point& to,
                                               const Vector3f& color) {
    PointList point_list;
    point_list.push_back(make_shared<Point>(from));
    point_list.push_back(make_shared<Point>(to));

    DrawLine(point_list, color);
}

void OpenGLGraphicsManagerCommonBase::DrawTriangle(const PointList& vertices,
                                                   const Vector3f& color) {
    Matrix4X4f trans;
    BuildIdentityMatrix(trans);

    DrawTriangle(vertices, trans, color);
}

void OpenGLGraphicsManagerCommonBase::DrawTriangle(const PointList& vertices,
                                                   const Matrix4X4f& trans,
                                                   const Vector3f& color) {
    const auto count = vertices.size();
    assert(count >= 3);

    uint32_t vao;
    glGenVertexArrays(1, &vao);

    // Bind the vertex array object to store all the buffers and vertex
    // attributes we create here.
    glBindVertexArray(vao);

    uint32_t buffer_id;

    // Generate an ID for the vertex buffer.
    glGenBuffers(1, &buffer_id);

    // Bind the vertex buffer and load the vertex (position and color) data into
    // the vertex buffer.
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    auto* data = new Vector3f[count];
    for (auto i = 0; i < count; i++) {
        data[i] = *vertices[i];
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * count, data,
                 GL_STATIC_DRAW);
    delete[] data;

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);

    m_DebugBuffers.push_back(buffer_id);

    DebugDrawBatchContext& dbc = *(new DebugDrawBatchContext);
    dbc.vao = vao;
    dbc.mode = GL_TRIANGLES;
    dbc.count = static_cast<int32_t>(vertices.size());
    dbc.color = color;
    dbc.trans = trans;

    m_DebugDrawBatchContext.push_back(std::move(dbc));
}

void OpenGLGraphicsManagerCommonBase::DrawTriangleStrip(
    const PointList& vertices, const Vector3f& color) {
    const auto count = vertices.size();
    assert(count >= 3);

    uint32_t vao;
    glGenVertexArrays(1, &vao);

    // Bind the vertex array object to store all the buffers and vertex
    // attributes we create here.
    glBindVertexArray(vao);

    uint32_t buffer_id;

    // Generate an ID for the vertex buffer.
    glGenBuffers(1, &buffer_id);

    // Bind the vertex buffer and load the vertex (position and color) data into
    // the vertex buffer.
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    auto* data = new Vector3f[count];
    for (auto i = 0; i < count; i++) {
        data[i] = *vertices[i];
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * count, data,
                 GL_STATIC_DRAW);
    delete[] data;

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);

    m_DebugBuffers.push_back(buffer_id);

    DebugDrawBatchContext& dbc = *(new DebugDrawBatchContext);
    dbc.vao = vao;
    dbc.mode = GL_TRIANGLE_STRIP;
    dbc.count = static_cast<int32_t>(vertices.size());
    dbc.color = color * 0.5f;

    m_DebugDrawBatchContext.push_back(std::move(dbc));
}

void OpenGLGraphicsManagerCommonBase::ClearDebugBuffers() {
    for (auto& dbc : m_DebugDrawBatchContext) {
        glDeleteVertexArrays(1, &dbc.vao);
    }

    m_DebugDrawBatchContext.clear();

    for (auto& buf : m_DebugBuffers) {
        glDeleteBuffers(1, &buf);
    }

    m_DebugBuffers.clear();
}

void OpenGLGraphicsManagerCommonBase::RenderDebugBuffers() {
    const auto pipelineState =
        g_pPipelineStateManager->GetPipelineState("Debug Drawing");

    // Set the color shader as the current shader program and set the matrices
    // that it will use for rendering.
    SetPipelineState(pipelineState, m_Frames[m_nFrameIndex]);

    if (!m_uboDebugConstant[m_nFrameIndex]) {
        glGenBuffers(1, &m_uboDebugConstant[m_nFrameIndex]);

        uint32_t blockIndex =
            glGetUniformBlockIndex(m_CurrentShader, "DebugConstants");

        assert(blockIndex != GL_INVALID_INDEX);

        int32_t blockSize;

        glGetActiveUniformBlockiv(m_CurrentShader, blockIndex,
                                  GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
        assert(blockSize == sizeof(DebugConstants));

        glUniformBlockBinding(m_CurrentShader, blockIndex, 13);
    }

    glBindBuffer(GL_UNIFORM_BUFFER, m_uboDebugConstant[m_nFrameIndex]);
    glBindBufferBase(GL_UNIFORM_BUFFER, 13, m_uboDebugConstant[m_nFrameIndex]);

    DebugConstants constants;
    for (const auto& dbc : m_DebugDrawBatchContext) {
        constants.front_color = dbc.color;

        glBufferData(GL_UNIFORM_BUFFER, sizeof(constants), &constants,
                     GL_DYNAMIC_DRAW);

        glBindVertexArray(dbc.vao);
        glDrawArrays(dbc.mode, 0x00, dbc.count);
    }
}

#endif

void OpenGLGraphicsManagerCommonBase::DrawFullScreenQuad() {
    GLfloat vertices[] = {-1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f,
                          1.0f,  1.0f, 0.0f, 1.0f,  -1.0f, 0.0f};

    GLfloat uv[] = {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f};

    uint32_t vao;
    glGenVertexArrays(1, &vao);

    // Bind the vertex array object to store all the buffers and vertex
    // attributes we create here.
    glBindVertexArray(vao);

    uint32_t buffer_id[2];

    // Generate an ID for the vertex buffer.
    glGenBuffers(2, buffer_id);

    // Bind the vertex buffer and load the vertex (position) data into the
    // vertex buffer.
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);

    // Bind the vertex buffer and load the vertex (uv) data into the vertex
    // buffer.
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);

    glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, nullptr);

    glDrawArrays(GL_TRIANGLE_STRIP, 0x00, 4);

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(2, buffer_id);
}
