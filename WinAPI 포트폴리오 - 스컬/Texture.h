#pragma once
#include "Res.h"
class Texture :
    public Res
{
private :
    HBITMAP m_bit;
    HDC m_dc;
    BITMAP m_bitmap; 

    virtual int Load(const wchar_t* _filePath);
    void Create(UINT _width, UINT _height);

public :
    HDC GetTextureDC() { return m_dc; }
    UINT GetWidth() { return m_bitmap.bmWidth; }
    UINT GetHeight() { return m_bitmap.bmHeight; }

public :
    Texture();
    virtual ~Texture();

    friend class ResourceManager;
    friend class ImageUI;
};

