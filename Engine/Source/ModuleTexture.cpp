#include "DirectXTex.h"
#include "ModuleTexture.h"
#include <GL/glew.h>

using namespace DirectX;

bool ModuleTexture::Init()
{
    glGenTextures(1, textures);
    image = new ScratchImage();
    glBindTexture(GL_TEXTURE_2D, *textures);
}

update_status ModuleTexture::Update() {
    return UPDATE_CONTINUE;
}

HRESULT ModuleTexture::LoadTextureFromFile(LPCWSTR tex_filename) //, ID3D11ShaderResourceView** srv)
{

    HRESULT hr;
    
    hr = LoadFromDDSFile(tex_filename, DDS_FLAGS_NONE, &imageMetadata, *image);
    if (hr != S_OK) {
        hr = LoadFromTGAFile(tex_filename, &imageMetadata, *image);
    }
    if (hr != S_OK) {
        hr = LoadFromWICFile(tex_filename, WIC_FLAGS_NONE, &imageMetadata, *image);
    }

    //hr = CreateShaderResourceView(mD3DSystem->GetDevice11(), image->GetImages(), image->GetImageCount(), imageMetadata, srv);
 
    return hr;
}

bool ModuleTexture::CleanUp() {
    glDeleteTextures(1, textures);
    delete image;
}
