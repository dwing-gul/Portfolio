#pragma once
#include "Entity.h"
#include "Ptr.h"

class Res :
    public Entity
{
private :
    wstring        m_key;
    wstring        m_relativePath;
    UINT           m_refCount;
    RESOURCE_TYPE  m_type;
    bool           m_bDefault;

public :
    void SetKey(const wstring& _key) { m_key = _key; }
    void SetRelativePath(const wstring& _relativePath) { m_relativePath = _relativePath; }

    const wstring& GetKey() { return m_key; }
    const wstring& GetRelativePath() { return m_relativePath; }
    RESOURCE_TYPE GetType() { return m_type; }
    bool IsDefault() { return m_bDefault; }

    virtual void Save(const wstring& _relativePath) {}

private :
    virtual int Load(const wstring& _filePath) = 0;
    
    void AddRef() { ++m_refCount; }
    void SubRef() { --m_refCount; }
    
public :
    Res(RESOURCE_TYPE _type);
    Res(RESOURCE_TYPE _type, bool _bDefault);
    virtual ~Res();

    friend class ResourceManager;

    template <typename T>
    friend class Ptr;
};

