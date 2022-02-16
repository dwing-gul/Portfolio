#include "pch.h"
#include "ResourceManager.h"
#include "ParticleUpdateShader.h"

ResourceManager::ResourceManager() :
    m_bEvent(false)
{
}

ResourceManager::~ResourceManager()
{
    Safe_Delete_Vector(m_vectorCloneMaterial);

    for (UINT i = 0; i < (UINT)RESOURCE_TYPE::END; ++i)
    {
        Safe_Delete_Map(m_mapRes[i]);
    }
}

void ResourceManager::init()
{
    CreateDefaultMesh();
    CreateDefaultTexture();
    CreateDefaultShader();
    CreateDefaultMaterial();
    CreateComputeShader();
    InitSound();
}

void ResourceManager::CreateDefaultMesh()
{
    Mesh* mesh = nullptr;

    vector<tVertex> vectorVertex;
    vector<UINT> vectorIndex;

    tVertex vertex;

    vertex.pos = Vector3(0.f, 0.f, 0.f);
    vertex.uv = Vector2(0.f, 0.f);
    vectorVertex.push_back(vertex);
    vectorIndex.push_back(0);

    mesh = new Mesh;
    mesh->Create(vectorVertex.data(), UINT(sizeof(tVertex) * vectorVertex.size()), vectorIndex.data(), UINT(sizeof(UINT) * vectorIndex.size()));

    AddRes(L"PointMesh", mesh);

    vectorVertex.clear();
    vectorIndex.clear();

    vertex.pos = Vector3(-0.5f, 0.5f, 0.5f);
    vertex.uv = Vector2(0.f, 0.f);
    vectorVertex.push_back(vertex);

    vertex.pos = Vector3(0.5f, 0.5f, 0.5f);
    vertex.uv = Vector2(1.f, 0.f);
    vectorVertex.push_back(vertex);

    vertex.pos = Vector3(0.5f, -0.5f, 0.5f);
    vertex.uv = Vector2(1.f, 1.f);
    vectorVertex.push_back(vertex);

    vertex.pos = Vector3(-0.5f, -0.5f, 0.5f);
    vertex.uv = Vector2(0.f, 1.f);
    vectorVertex.push_back(vertex);

    vectorIndex.push_back(0); vectorIndex.push_back(1); vectorIndex.push_back(2);
    vectorIndex.push_back(0); vectorIndex.push_back(2); vectorIndex.push_back(3);

    mesh = new Mesh;
    mesh->Create(vectorVertex.data(), UINT(sizeof(tVertex) * vectorVertex.size()), vectorIndex.data(), UINT(sizeof(UINT) * vectorIndex.size()));

    AddRes(L"RectMesh", mesh);

    vectorIndex.clear();
    vectorIndex.push_back(0); vectorIndex.push_back(1); vectorIndex.push_back(2); vectorIndex.push_back(3); vectorIndex.push_back(0);

    mesh = new Mesh;
    mesh->Create(vectorVertex.data(), UINT(sizeof(tVertex) * vectorVertex.size()), vectorIndex.data(), UINT(sizeof(UINT) * vectorIndex.size()));

    AddRes(L"RectLineMesh", mesh);

    vectorVertex.clear();
    vectorIndex.clear();

    float radius = 0.5f;
    int divide = 40;
    float theta = XM_2PI / (float)divide;

    vertex.pos = Vector3(0.f, 0.f, 0.f);
    vertex.uv = Vector2(0.5f, 0.5f);
    vectorVertex.push_back(vertex);

    for (int i = 0; i < divide + 1; ++i)
    {
        vertex.pos = Vector3(radius * cosf(theta * (float)i), radius * sinf(theta * (float)i), 0.f);
        vertex.uv = Vector2((vertex.pos.x + radius) / (2 * radius), (-vertex.pos.y + radius) / (2 * radius));
        vectorVertex.push_back(vertex);
    }

    for (int i = 0; i < divide; ++i)
    {
        vectorIndex.push_back(0);
        vectorIndex.push_back(i + 2);
        vectorIndex.push_back(i + 1);
    }

    mesh = new Mesh;
    mesh->Create(vectorVertex.data(), UINT(sizeof(tVertex) * vectorVertex.size()), vectorIndex.data(), UINT(sizeof(UINT) * vectorIndex.size()));
    AddRes(L"CircleMesh", mesh);

    vectorIndex.clear();
    for (int i = 0; i < divide + 1; ++i)
    {
        vectorIndex.push_back(i + 1);
    }

    mesh = new Mesh;
    mesh->Create(vectorVertex.data(), UINT(sizeof(tVertex) * vectorVertex.size()), vectorIndex.data(), UINT(sizeof(UINT) * vectorIndex.size()));
    AddRes(L"CircleLineMesh", mesh);

    vectorVertex.clear();
    vectorIndex.clear();

    tVertex arrCube[24] = {};
    // À­¸é
    arrCube[0].pos = Vector3(-0.5f, 0.5f, 0.5f);
    arrCube[0].uv = Vector2(0.f, 0.f);
    arrCube[0].normal = Vector3(0.f, 1.f, 0.f);

    arrCube[1].pos = Vector3(0.5f, 0.5f, 0.5f);
    arrCube[1].uv = Vector2(0.f, 0.f);
    arrCube[1].normal = Vector3(0.f, 1.f, 0.f);

    arrCube[2].pos = Vector3(0.5f, 0.5f, -0.5f);
    arrCube[2].uv = Vector2(0.f, 0.f);
    arrCube[2].normal = Vector3(0.f, 1.f, 0.f);

    arrCube[3].pos = Vector3(-0.5f, 0.5f, -0.5f);
    arrCube[3].uv = Vector2(0.f, 0.f);
    arrCube[3].normal = Vector3(0.f, 1.f, 0.f);


    // ¾Æ·§ ¸é	
    arrCube[4].pos = Vector3(-0.5f, -0.5f, -0.5f);
    arrCube[4].uv = Vector2(0.f, 0.f);
    arrCube[4].normal = Vector3(0.f, -1.f, 0.f);

    arrCube[5].pos = Vector3(0.5f, -0.5f, -0.5f);
    arrCube[5].uv = Vector2(0.f, 0.f);
    arrCube[5].normal = Vector3(0.f, -1.f, 0.f);

    arrCube[6].pos = Vector3(0.5f, -0.5f, 0.5f);
    arrCube[6].uv = Vector2(0.f, 0.f);
    arrCube[6].normal = Vector3(0.f, -1.f, 0.f);

    arrCube[7].pos = Vector3(-0.5f, -0.5f, 0.5f);
    arrCube[7].uv = Vector2(0.f, 0.f);
    arrCube[7].normal = Vector3(0.f, -1.f, 0.f);

    // ¿ÞÂÊ ¸é
    arrCube[8].pos = Vector3(-0.5f, 0.5f, 0.5f);
    arrCube[8].uv = Vector2(0.f, 0.f);
    arrCube[8].normal = Vector3(-1.f, 0.f, 0.f);

    arrCube[9].pos = Vector3(-0.5f, 0.5f, -0.5f);
    arrCube[9].uv = Vector2(0.f, 0.f);
    arrCube[9].normal = Vector3(-1.f, 0.f, 0.f);

    arrCube[10].pos = Vector3(-0.5f, -0.5f, -0.5f);
    arrCube[10].uv = Vector2(0.f, 0.f);
    arrCube[10].normal = Vector3(-1.f, 0.f, 0.f);

    arrCube[11].pos = Vector3(-0.5f, -0.5f, 0.5f);
    arrCube[11].uv = Vector2(0.f, 0.f);
    arrCube[11].normal = Vector3(-1.f, 0.f, 0.f);

    // ¿À¸¥ÂÊ ¸é
    arrCube[12].pos = Vector3(0.5f, 0.5f, -0.5f);
    arrCube[12].uv = Vector2(0.f, 0.f);
    arrCube[12].normal = Vector3(1.f, 0.f, 0.f);

    arrCube[13].pos = Vector3(0.5f, 0.5f, 0.5f);
    arrCube[13].uv = Vector2(0.f, 0.f);
    arrCube[13].normal = Vector3(1.f, 0.f, 0.f);

    arrCube[14].pos = Vector3(0.5f, -0.5f, 0.5f);
    arrCube[14].uv = Vector2(0.f, 0.f);
    arrCube[14].normal = Vector3(1.f, 0.f, 0.f);

    arrCube[15].pos = Vector3(0.5f, -0.5f, -0.5f);
    arrCube[15].uv = Vector2(0.f, 0.f);
    arrCube[15].normal = Vector3(1.f, 0.f, 0.f);

    // µÞ ¸é
    arrCube[16].pos = Vector3(0.5f, 0.5f, 0.5f);
    arrCube[16].uv = Vector2(0.f, 0.f);
    arrCube[16].normal = Vector3(0.f, 0.f, 1.f);

    arrCube[17].pos = Vector3(-0.5f, 0.5f, 0.5f);
    arrCube[17].uv = Vector2(0.f, 0.f);
    arrCube[17].normal = Vector3(0.f, 0.f, 1.f);

    arrCube[18].pos = Vector3(-0.5f, -0.5f, 0.5f);
    arrCube[18].uv = Vector2(0.f, 0.f);
    arrCube[18].normal = Vector3(0.f, 0.f, 1.f);

    arrCube[19].pos = Vector3(0.5f, -0.5f, 0.5f);
    arrCube[19].uv = Vector2(0.f, 0.f);
    arrCube[19].normal = Vector3(0.f, 0.f, 1.f);

    // ¾Õ ¸é
    arrCube[20].pos = Vector3(-0.5f, 0.5f, -0.5f);
    arrCube[20].uv = Vector2(0.f, 0.f);
    arrCube[20].normal = Vector3(0.f, 0.f, -1.f);

    arrCube[21].pos = Vector3(0.5f, 0.5f, -0.5f);
    arrCube[21].uv = Vector2(0.f, 0.f);
    arrCube[21].normal = Vector3(0.f, 0.f, -1.f);

    arrCube[22].pos = Vector3(0.5f, -0.5f, -0.5f);
    arrCube[22].uv = Vector2(0.f, 0.f);
    arrCube[22].normal = Vector3(0.f, 0.f, -1.f);

    arrCube[23].pos = Vector3(-0.5f, -0.5f, -0.5f);
    arrCube[23].uv = Vector2(0.f, 0.f);
    arrCube[23].normal = Vector3(0.f, 0.f, -1.f);

    // ÀÎµ¦½º
    for (int i = 0; i < 12; i += 2)
    {
        vectorIndex.push_back(i * 2);
        vectorIndex.push_back(i * 2 + 1);
        vectorIndex.push_back(i * 2 + 2);

        vectorIndex.push_back(i * 2);
        vectorIndex.push_back(i * 2 + 2);
        vectorIndex.push_back(i * 2 + 3);
    }

    mesh = new Mesh;
    mesh->Create(arrCube, sizeof(tVertex) * 24, vectorIndex.data(), sizeof(UINT) * (UINT)vectorIndex.size());
    AddRes(L"CubeMesh", mesh);

    vectorVertex.clear();
    vectorIndex.clear();

    // ===============
    // Sphere Mesh(±¸)
    // ===============
    radius = 1.f;

    // Top
    vertex.pos = Vector3(0.f, radius, 0.f);
    vertex.uv = Vector2(0.5f, 0.f);
    vertex.normal = vertex.pos;
    vertex.normal.Normalize();
    vertex.tangent = Vector3(1.f, 0.f, 0.f);
    vertex.binormal = Vector3(0.f, 0.f, 1.f);
    vectorVertex.push_back(vertex);

    // Body
    UINT stackCount = 40; // °¡·Î ºÐÇÒ °³¼ö
    UINT sliceCount = 40; // ¼¼·Î ºÐÇÒ °³¼ö

    float stackAngle = XM_PI / stackCount;
    float sliceAngle = XM_2PI / sliceCount;

    float uvXStep = 1.f / (float)sliceCount;
    float uvYStep = 1.f / (float)stackCount;

    for (UINT i = 1; i < stackCount; ++i)
    {
        float phi = i * stackAngle;

        for (UINT j = 0; j <= sliceCount; ++j)
        {
            float theta = j * sliceAngle;

            vertex.pos = Vector3(radius * sinf(i * stackAngle) * cosf(j * sliceAngle)
                , radius * cosf(i * stackAngle)
                , radius * sinf(i * stackAngle) * sinf(j * sliceAngle));
            vertex.uv = Vector2(uvXStep * j, uvYStep * i);
            vertex.normal = vertex.pos;
            vertex.normal.Normalize();

            vertex.tangent.x = -radius * sinf(phi) * sinf(theta);
            vertex.tangent.y = 0.f;
            vertex.tangent.z = radius * sinf(phi) * cosf(theta);
            vertex.tangent.Normalize();

            vertex.tangent.Cross(vertex.normal, vertex.binormal);
            vertex.binormal.Normalize();

            vectorVertex.push_back(vertex);
        }
    }

    // Bottom
    vertex.pos = Vector3(0.f, -radius, 0.f);
    vertex.uv = Vector2(0.5f, 1.f);
    vertex.normal = vertex.pos;
    vertex.normal.Normalize();

    vertex.tangent = Vector3(1.f, 0.f, 0.f);
    vertex.binormal = Vector3(0.f, 0.f, -1.f);
    vectorVertex.push_back(vertex);

    // ÀÎµ¦½º
    // ºÏ±ØÁ¡
    for (UINT i = 0; i < sliceCount; ++i)
    {
        vectorIndex.push_back(0);
        vectorIndex.push_back(i + 2);
        vectorIndex.push_back(i + 1);
    }

    // ¸öÅë
    for (UINT i = 0; i < stackCount - 2; ++i)
    {
        for (UINT j = 0; j < sliceCount; ++j)
        {
            // + 
            // | \
			// +--+
            vectorIndex.push_back((sliceCount + 1) * (i)+(j)+1);
            vectorIndex.push_back((sliceCount + 1) * (i + 1) + (j + 1) + 1);
            vectorIndex.push_back((sliceCount + 1) * (i + 1) + (j)+1);

            // +--+
            //  \ |
            //    +
            vectorIndex.push_back((sliceCount + 1) * (i)+(j)+1);
            vectorIndex.push_back((sliceCount + 1) * (i)+(j + 1) + 1);
            vectorIndex.push_back((sliceCount + 1) * (i + 1) + (j + 1) + 1);
        }
    }

    // ³²±ØÁ¡
    UINT bottomIdx = (UINT)vectorVertex.size() - 1;
    for (UINT i = 0; i < sliceCount; ++i)
    {
        vectorIndex.push_back(bottomIdx);
        vectorIndex.push_back(bottomIdx - (i + 2));
        vectorIndex.push_back(bottomIdx - (i + 1));
    }

    mesh = new Mesh;
    mesh->Create(vectorVertex.data(), sizeof(tVertex) * (UINT)vectorVertex.size(), vectorIndex.data(), sizeof(UINT) * (UINT)vectorIndex.size());
    mesh->SetName(L"SphereMesh");
    AddRes(mesh->GetName(), mesh);

    vectorVertex.clear();
    vectorIndex.clear();
}

void ResourceManager::CreateDefaultTexture()
{
    Ptr<Texture> tex = ResourceManager::GetInst()->Load<Texture>(L"noise01", L"texture\\noise\\noise01.png");
    ResourceManager::GetInst()->Load<Texture>(L"noise02", L"texture\\noise\\noise02.png");

    tex->UpdateData(SHADER_STAGE::ALL, 53);
    g_global.noiseResolution = Vector2((float)tex->Width(), (float)tex->Height());
}

void ResourceManager::CreateDefaultShader()
{
    GraphicsShader* shader = nullptr;

    shader = new GraphicsShader(RENDER_POV::FORWARD);
    shader->CreateVertexShader(L"shader\\std2D.fx", "VS_Std2D");
    shader->CreatePixelShader(L"shader\\std2D.fx", "PS_Std2D");

    shader->SetRSType(RASTERIZER_TYPE::CULL_NONE);
    shader->SetBlendType(BLEND_TYPE::ALPHABLEND_COVERAGE);
    shader->AddShaderParam(SHADER_PARAM::TEXTURE_0, L"Output Texture");
    AddRes(L"Std2DShader", shader);

    shader = new GraphicsShader(RENDER_POV::FORWARD);
    shader->CreateVertexShader(L"shader\\std2D.fx", "VS_Std2D");
    shader->CreatePixelShader(L"shader\\std2D.fx", "PS_Std2D");

    shader->SetRSType(RASTERIZER_TYPE::CULL_NONE);
    shader->SetBlendType(BLEND_TYPE::ALPHABLEND);
    shader->AddShaderParam(SHADER_PARAM::TEXTURE_0, L"Output Texture");
    AddRes(L"Std2D_AlphaBlendShader", shader);

    shader = new GraphicsShader(RENDER_POV::FORWARD);
    shader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D_Light");
    shader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_Light");

    shader->SetRSType(RASTERIZER_TYPE::CULL_NONE);
    shader->SetBlendType(BLEND_TYPE::ALPHABLEND_COVERAGE);
    shader->AddShaderParam(SHADER_PARAM::TEXTURE_0, L"Output Texture");

    AddRes(L"Std2DLightShader", shader);

    shader = new GraphicsShader(RENDER_POV::FORWARD);
    shader->CreateVertexShader(L"shader\\std2D.fx", "VS_Collider2D");
    shader->CreatePixelShader(L"shader\\std2D.fx", "PS_Collider2D");

    shader->SetRSType(RASTERIZER_TYPE::CULL_NONE);
    shader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
    shader->SetDSType(DEPTH_STENCIL_TYPE::NO_TEST);
    AddRes(L"Collider2DShader", shader);

    shader = new GraphicsShader(RENDER_POV::FORWARD);
    shader->CreateVertexShader(L"shader\\std2D.fx", "VS_TileMap");
    shader->CreatePixelShader(L"shader\\std2D.fx", "PS_TileMap");

    shader->SetRSType(RASTERIZER_TYPE::CULL_NONE);
    shader->AddShaderParam(SHADER_PARAM::INT_0, L"x Face Count");
    shader->AddShaderParam(SHADER_PARAM::INT_1, L"y Face Count");
    shader->AddShaderParam(SHADER_PARAM::TEXTURE_0, L"Atlas Texture");
    AddRes(L"TileMapShader", shader);

    shader = new GraphicsShader(RENDER_POV::FORWARD);
    shader->CreateVertexShader(L"shader\\std3d.fx", "VS_Std3D");
    shader->CreatePixelShader(L"shader\\std3d.fx", "PS_Std3D");

    shader->AddShaderParam(SHADER_PARAM::TEXTURE_0, L"Diffuse");
    shader->AddShaderParam(SHADER_PARAM::TEXTURE_1, L"Normal Map");

    AddRes(L"Std3DShader", shader);

    shader = new GraphicsShader(RENDER_POV::FORWARD);
    shader->CreateVertexShader(L"shader\\std2D.fx", "VS_TileMap_Light");
    shader->CreatePixelShader(L"shader\\std2D.fx", "PS_TileMap_Light");

    shader->SetRSType(RASTERIZER_TYPE::CULL_NONE);
    shader->AddShaderParam(SHADER_PARAM::INT_0, L"x Face Count");
    shader->AddShaderParam(SHADER_PARAM::INT_1, L"y Face Count");
    shader->AddShaderParam(SHADER_PARAM::TEXTURE_0, L"Atlas Texture");
    AddRes(L"TileMapLightShader", shader);

    shader = new GraphicsShader(RENDER_POV::FORWARD);
    shader->CreateVertexShader(L"shader\\particle.fx", "VS_Particle");
    shader->CreateGeometryShader(L"shader\\particle.fx", "GS_Particle");
    shader->CreatePixelShader(L"shader\\particle.fx", "PS_Particle");

    shader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
    shader->SetRSType(RASTERIZER_TYPE::CULL_NONE);
    shader->SetBlendType(BLEND_TYPE::ALPHABLEND);
    shader->SetDSType(DEPTH_STENCIL_TYPE::NO_WRITE);
    shader->AddShaderParam(SHADER_PARAM::VECTOR4_0, L"Start Color");
    shader->AddShaderParam(SHADER_PARAM::VECTOR4_1, L"End Color");
    shader->AddShaderParam(SHADER_PARAM::VECTOR4_2, L"Start Scale");
    shader->AddShaderParam(SHADER_PARAM::VECTOR4_3, L"End Scale");
    shader->AddShaderParam(SHADER_PARAM::TEXTURE_0, L"Particle Texture");
    AddRes(L"ParticleShader", shader);

    shader = new GraphicsShader(RENDER_POV::POST_EFFECT);
    shader->CreateVertexShader(L"shader\\posteffect.fx", "VS_Distortion");
    shader->CreatePixelShader(L"shader\\posteffect.fx", "PS_Distortion");

    shader->SetRSType(RASTERIZER_TYPE::CULL_NONE);
    shader->SetDSType(DEPTH_STENCIL_TYPE::NO_TEST_NO_WRITE);

    AddRes(L"DistortionShader", shader);
}

void ResourceManager::CreateDefaultMaterial()
{  
    Material* material = new Material(true);
    material->SetShader(FindRes<GraphicsShader>(L"Std2DShader"));
    AddRes<Material>(L"Std2DMaterial", material);

    material = new Material(true);
    material->SetShader(FindRes<GraphicsShader>(L"Std2D_AlphaBlendShader"));
    AddRes<Material>(L"Std2D_AlphaBlendMaterial", material);

    material = new Material(true);
    material->SetShader(FindRes<GraphicsShader>(L"Std2DLightShader"));
    AddRes<Material>(L"Std2DLightMaterial", material);

    material = new Material(true);
    material->SetShader(FindRes<GraphicsShader>(L"Collider2DShader"));
    AddRes<Material>(L"Collider2DMaterial", material);

    material = new Material(true);
    material->SetShader(FindRes<GraphicsShader>(L"TileMapShader"));
    AddRes<Material>(L"TileMapMaterial", material);

    material = new Material(true);
    material->SetShader(FindRes<GraphicsShader>(L"TileMapLightShader"));
    AddRes<Material>(L"TileMapLightMaterial", material);

    material = new Material(true);
    material->SetShader(FindRes<GraphicsShader>(L"Std3DShader"));
    AddRes<Material>(L"Std3DMaterial", material);

    material = new Material(true);
    material->SetShader(FindRes<GraphicsShader>(L"ParticleShader"));
    AddRes<Material>(L"ParticleMaterial", material);   

    material = new Material(true);
    material->SetShader(FindRes<GraphicsShader>(L"DistortionShader"));

    Ptr<Texture> postEffectTexture = FindRes<Texture>(L"PostEffectTexture");
    material->SetData(SHADER_PARAM::TEXTURE_0, postEffectTexture.Get());
    AddRes<Material>(L"DistortionMaterial", material);
}

void ResourceManager::CreateComputeShader()
{
    ComputeShader* shader = new ParticleUpdateShader;
    shader->CreateComputeShader(L"shader\\particle_update.fx", "CS_ParticleUpdate");
    AddRes(L"ParticleUpdateShader", shader);
}

void ResourceManager::InitSound()
{
    FMOD::System_Create(&Sound::g_FMOD);

    if (nullptr == Sound::g_FMOD)
    {
        assert(nullptr);
    }

    // 32°³ Ã¤³Î »ý¼º
    Sound::g_FMOD->init(32, FMOD_DEFAULT, nullptr);
}

void ResourceManager::AddRes(const wstring& _key, Res* _res, RESOURCE_TYPE _type)
{
    _res->SetKey(_key);
    m_mapRes[(UINT)_type].insert(make_pair(_key, _res));
    m_bEvent = true;
}

void ResourceManager::SubRes(const wstring& _key, RESOURCE_TYPE _type)
{
    map<wstring, Res*>::iterator iter = m_mapRes[(UINT)_type].find(_key);
    if (iter == m_mapRes[(UINT)_type].end())
    {
        assert(nullptr);
    }

    m_mapRes[(UINT)_type].erase(iter);
}

Ptr<Texture> ResourceManager::CreateTexture(const wstring& _key, UINT _width, UINT _height, DXGI_FORMAT _format, UINT _bindFlag)
{
    assert(nullptr == FindRes<Texture>(_key));

    Texture* texture = new Texture(true);
    texture->Create(_width, _height, _format, _bindFlag);
    AddRes<Texture>(_key, texture);
    m_bEvent = true;

    return texture;
}

Ptr<Texture> ResourceManager::CreateTexture(const wstring& _key, ComPtr<ID3D11Texture2D> _texture)
{
    assert(nullptr == FindRes<Texture>(_key));

    Texture* texture = new Texture(true);
    texture->Create(_texture);
    AddRes<Texture>(_key, texture);
    m_bEvent = true;

    return texture;
}

Ptr<Texture> ResourceManager::CreateTexture_Changeable(const wstring& _key, UINT _width, UINT _height, DXGI_FORMAT _format, UINT _bindFlag)
{
    assert(nullptr == FindRes<Texture>(_key));

    Texture* texture = new Texture;
    texture->Create(_width, _height, _format, _bindFlag);
    AddRes<Texture>(_key, texture);
    m_bEvent = true;

    return texture;
}

Ptr<Texture> ResourceManager::CreateTexture_Changeable(const wstring& _key, ComPtr<ID3D11Texture2D> _texture)
{
    assert(nullptr == FindRes<Texture>(_key));

    Texture* texture = new Texture;
    texture->Create(_texture);
    AddRes<Texture>(_key, texture);
    m_bEvent = true;

    return texture;
}

void ResourceManager::GetResName(RESOURCE_TYPE _type, vector<wstring>& _output)
{
    for (const auto& pair : m_mapRes[(UINT)_type])
    {
        _output.push_back(pair.first);
    }
}
