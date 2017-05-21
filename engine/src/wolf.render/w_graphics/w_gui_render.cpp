#include "w_render_pch.h"
#include "w_gui_render.h"
#include <w_framework/w_quad.h>

namespace wolf
{
    namespace graphics
    {
        class w_gui_render_pimp
        {
        public:
            w_gui_render_pimp() :
                _name("w_gui_render"),
                _texture(nullptr),
                _shader(nullptr)
            {
            }
            
            ~w_gui_render_pimp()
            {
                release();
            }
            
            HRESULT load(_In_ const std::shared_ptr<w_graphics_device>& pGDevice,
                _In_ const w_render_pass* pRenderPass,
                _In_ const std::string& pPipelineCacheName,
                _In_ const std::vector<std::vector<wolf::gui::w_gui_vertex_2d>>& pWidgetsVertices)
            {
                if (pRenderPass == nullptr)
                {
                    V(S_FALSE, "Render Pass can not be nullptr.", this->_name, 3);
                    return S_FALSE;
                }
                auto _hr = w_texture::load_to_shared_textures(pGDevice, content_path + L"textures/gui.png", &this->_texture);
                if (_hr == S_FALSE || !this->_texture)
                {
                    logger.error("Could not load texture for w_gui_render");
                }

                w_shader_binding_param _shader_param;
                _shader_param.index = 0;
                _shader_param.sampler_info = this->_texture->get_descriptor_info();
                _shader_param.stage = w_shader_stage::FRAGMENT_SHADER;
                _shader_param.type = w_shader_binding_type::SAMPLER;

                _hr = w_shader::load_to_shared_shaders(pGDevice,
                    "gui",
                    content_path + L"shaders/gui.vert.spv",
                    content_path + L"shaders/gui.frag.spv",
                    { _shader_param },
                    &this->_shader);
                if (_hr == S_FALSE || !this->_shader)
                {
                    V(S_FALSE, "loading shader", this->_name, 3);
                    return S_FALSE;
                }

                //load mesh
                for (auto _iter : pWidgetsVertices)
                {
                    auto _mesh = new (std::nothrow) w_mesh();
                    if (!_mesh)
                    {
                        V(S_FALSE, "allocating mesh", this->_name, 3);
                        return S_FALSE;
                    }
                    _mesh->set_vertex_declaration_struct(w_mesh::w_vertex_declaration::VERTEX_GUI_2D);

                    const std::vector<UINT> _indices = { 0, 1, 2, 0, 3, 1 };
                    std::vector<float> _vertices;
                    for (auto _vert : _iter)
                    {
                        _vertices.push_back(_vert.position[0]);
                        _vertices.push_back(_vert.position[1]);
                        _vertices.push_back(_vert.color[0]);
                        _vertices.push_back(_vert.color[1]);
                        _vertices.push_back(_vert.color[2]);
                        _vertices.push_back(_vert.color[3]);
                        _vertices.push_back(1 - _vert.uv[0]);
                        _vertices.push_back(_vert.uv[1]);
                    }
                    _hr = _mesh->load(pGDevice,
                        _vertices.data(),
                        static_cast<UINT>(_vertices.size()),
                        static_cast<UINT>(_vertices.size() * sizeof(float)),
                        _indices.data(),
                        static_cast<UINT>(_indices.size()),
                        this->_shader,
                        pRenderPass,
                        pPipelineCacheName,
                        false);
                    if (_hr == S_FALSE)
                    {
                        V(S_FALSE, "loading mesh", this->_name, 3);
                        return S_FALSE;
                    }
                    this->_meshes.push_back(_mesh);
                }

                return S_OK;
            }
            
            void render(_In_ const VkCommandBuffer& pCommandBuffer)
            {
                if (!this->_meshes.size()) return;

                std::for_each(this->_meshes.begin(), this->_meshes.end(), [pCommandBuffer](w_mesh* pIter)
                {
                    if (pIter)
                    {
                        auto _instance_handle = nullptr;// this->_instances_buffer ? this->_instances_buffer->get_handle() : nullptr;
                        uint32_t _instances_count = 0;
                        pIter->render(pCommandBuffer, _instance_handle, _instances_count);
                    }
                });
            }

            ULONG release()
            {
                //release sampler
                this->_shader = nullptr;
                this->_texture = nullptr;
                for (auto _mesh : this->_meshes)
                {
                    SAFE_RELEASE(_mesh);
                }

                return 1;
            }            
            
#pragma region Getters
            
         
            
#pragma endregion
       
#pragma region Setters
            
        
            
#pragma endregion
            
        private:
            std::string                            _name;
            wolf::graphics::w_texture*             _texture;
            wolf::graphics::w_shader*              _shader;
            std::vector<wolf::graphics::w_mesh*>   _meshes;
        };
    }
}

using namespace wolf::graphics;

w_gui_render::w_gui_render() :
    _pimp(new w_gui_render_pimp())
{
	_super::set_class_name("w_gui_render");
}

w_gui_render::~w_gui_render()
{
	release();
}

HRESULT w_gui_render::load(_In_ const std::shared_ptr<w_graphics_device>& pGDevice,
    _In_ const w_render_pass* pRenderPass,
    _In_ const std::string& pPipelineCacheName,
    _In_ std::vector<std::vector<wolf::gui::w_gui_vertex_2d>>& pWidgetsVertices)
{
    return this->_pimp ? this->_pimp->load(pGDevice, pRenderPass, pPipelineCacheName, pWidgetsVertices) : S_FALSE;
}

void w_gui_render::render(_In_ const VkCommandBuffer& pCommandBuffer)
{
    if (!this->_pimp) return;
    this->_pimp->render(pCommandBuffer);
}

ULONG w_gui_render::release()
{
    if (_super::get_is_released()) return 0;

    SAFE_RELEASE(this->_pimp);

    return _super::release();
}

#pragma region Getters


#pragma endregion

#pragma region Setters

#pragma endregion
