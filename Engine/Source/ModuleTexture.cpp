#pragma once
#include "ModuleTexture.h"
#include <DirectXTex.h>
#include <GL/glew.h>
#include <string>

using namespace DirectX;


unsigned ModuleTexture::LoadTextureFromFile(const wchar_t* tex_filename) //, ID3D11ShaderResourceView** srv)
{
    GLuint textures;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenTextures(1, &textures);
    HRESULT hr;
    TexMetadata imageMetadata;
    ScratchImage* image = new ScratchImage();
    hr = LoadFromDDSFile(tex_filename, DDS_FLAGS_NONE, &imageMetadata, *image);
    if (hr != S_OK) {
        hr = LoadFromTGAFile(tex_filename, &imageMetadata, *image);
    }
    if (hr != S_OK) {
        hr = LoadFromWICFile(tex_filename, WIC_FLAGS_NONE, &imageMetadata, *image);
    }
    if (hr == S_OK) {
        
        //hr = CreateShaderResourceView(mD3DSystem->GetDevice11(), image->GetImages(), image->GetImageCount(), imageMetadata, srv); 

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
            case DXGI_FORMAT_BC1_UNORM:
                internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
                format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
                type = GL_UNSIGNED_BYTE;
                break;
            default:
                assert(false && "Unsupported format");
        }
    
        glBindTexture(GL_TEXTURE_2D, textures);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, imageMetadata.width, imageMetadata.height, 0, format, type, image->GetPixels());
        if (imageMetadata.mipLevels <= 1) glGenerateMipmap(GL_TEXTURE_2D);
        std::wstring ws = tex_filename;
        std::string s(ws.begin(), ws.end());
        savedTextures.push_back({textures, s, imageMetadata.width, imageMetadata.height, imageMetadata.mipLevels});
        delete image;

        return textures;
    }
    else {
        LOG("The texture could not be loaded.");
        return 0;
    }
}

const Metadata& ModuleTexture::GetMetadata(unsigned tex) const {
    for (const Metadata& st : savedTextures) {
        if (st.id == tex) return st;
    }
    assert(false && "Looking fo non existent texture metadata");
}