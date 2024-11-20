#define _CRT_SECURE_NO_WARNINGS
#include "../../libs/imgui/imgui.h"
#include "../../libs/imgui/imgui_impl_win32.h"
#include "../../libs/imgui/imgui_impl_dx11.h"

#include "../Include/GUI.hpp"

#include <stdlib.h>


#define STB_IMAGE_IMPLEMENTATION
#include "../../libs/stb_image.h"

// Simple helper function to load an image into a DX11 texture with common settings
bool LoadTextureFromMemory(Window* w,const void* data, size_t data_size, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
    // Load from disk into a raw RGBA buffer
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load_from_memory((const unsigned char*)data, (int)data_size, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create texture
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = image_width;
    desc.Height = image_height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;

    ID3D11Texture2D* pTexture = NULL;
    D3D11_SUBRESOURCE_DATA subResource;
    subResource.pSysMem = image_data;
    subResource.SysMemPitch = desc.Width * 4;
    subResource.SysMemSlicePitch = 0;
    w->g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

    // Create texture view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    w->g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
    pTexture->Release();

    *out_width = image_width;
    *out_height = image_height;
    stbi_image_free(image_data);

    D3D11_SAMPLER_DESC ImageSamplerDesc = {};

    ImageSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    ImageSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    ImageSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    ImageSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    ImageSamplerDesc.MipLODBias = 0.0f;
    ImageSamplerDesc.MaxAnisotropy = 1;
    ImageSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    ImageSamplerDesc.BorderColor[0] = 1.0f;
    ImageSamplerDesc.BorderColor[1] = 1.0f;
    ImageSamplerDesc.BorderColor[2] = 1.0f;
    ImageSamplerDesc.BorderColor[3] = 1.0f;
    ImageSamplerDesc.MinLOD = -FLT_MAX;
    ImageSamplerDesc.MaxLOD = FLT_MAX;

    return true;
}

// Open and read a file, then forward to LoadTextureFromMemory()
bool LoadTextureFromFile(Window* w, const char* file_name, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
    FILE* f = fopen(file_name, "rb");
    if (f == NULL)
        return false;
    fseek(f, 0, SEEK_END);
    size_t file_size = (size_t)ftell(f);
    if (file_size == -1)
        return false;
    fseek(f, 0, SEEK_SET);
    void* file_data = IM_ALLOC(file_size);
    fread(file_data, 1, file_size, f);
    bool ret = LoadTextureFromMemory(w, file_data, file_size, out_srv, out_width, out_height);
    IM_FREE(file_data);
    return ret;
}

GUI::GUI(Window* w)
    :
    w(w)
{
    bool ret = LoadTextureFromFile(w, "assets/test.jpg", &my_texture, &my_image_width, &my_image_height);
    IM_ASSERT(ret);
}

/* Public */

void GUI::render()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{

			if (ImGui::MenuItem("Exit"))
			{
				exit(0);
			}

			ImGui::EndMenu();
		}

		if (ImGui::Button("X"))
		{
			exit(0);
		};

		ImGui::EndMainMenuBar();
	}

    ImGui::Begin("DirectX11 Texture Test");
    ImGui::Image((ImTextureID)(intptr_t)my_texture, ImVec2(my_image_width*0.5f, my_image_height*0.5f));
    ImGui::End();

	ImGui::ShowDemoWindow();
}

/* Private */

void GUI::set_style()
{

}