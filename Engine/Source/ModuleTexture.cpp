//#include "DirectXTex.h"
#include "ModuleTexture.h"
#include <GL/glew.h>

using namespace DirectX;

bool ModuleTexture::Init()
{
    image = new ScratchImage();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenTextures(1, &textures);

    return 1;
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

HRESULT ModuleTexture::LoadTextureGPU() {
    int internalFormat; int format; int type;
    switch (imageMetadata.format)
    {
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
        case DXGI_FORMAT_R8G8B8A8_UNORM:
            internalFormat = GL_RGBA8;
            format = GL_RGBA;
            type = GL_UNSIGNED_BYTE;
            break;
        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
        case DXGI_FORMAT_B8G8R8A8_UNORM:
            internalFormat = GL_RGBA8;
            format = GL_BGRA;
            type = GL_UNSIGNED_BYTE;
            break;
        case DXGI_FORMAT_B5G6R5_UNORM:
            internalFormat = GL_RGB8;
            format = GL_BGR;
            type = GL_UNSIGNED_BYTE;
            break;
        default:
            assert(false && "Unsupported format");
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, imageMetadata.width, imageMetadata.height, 0, format, type, image->GetPixels());
    if (imageMetadata.mipLevels <= 1) glGenerateMipmap(GL_TEXTURE_2D);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures);
    //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

    return S_OK;
}

void ModuleTexture::RenderTex() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}

bool ModuleTexture::CleanUp() {
    glDeleteTextures(1, &textures);
    delete image;
    return true;
}
