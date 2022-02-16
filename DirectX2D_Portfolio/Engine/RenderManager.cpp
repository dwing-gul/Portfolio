#include "pch.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "Device.h"
#include "StructuredBuffer.h"
#include "ConstBuffer.h"
#include "Camera.h"
#include "Light2D.h"

RenderManager::RenderManager() :
    m_toolCam(nullptr),
    m_light2DBuffer(nullptr)
{
}

RenderManager::~RenderManager()
{
    Safe_Delete_Vector(m_vectorCam);
    Safe_Delete_Vector(m_vectorLight2D);
    Safe_Delete(m_light2DBuffer);
}

void RenderManager::init()
{
    m_light2DBuffer = new StructuredBuffer;
    m_light2DBuffer->Create(STRUCTURED_TYPE::READONLY, sizeof(tLightInfo), 5);

    Vector2 resolution = Device::GetInst()->GetRenderResolution();
    m_postEffectTexture = ResourceManager::GetInst()->CreateTexture(L"PostEffectTexture", (UINT)resolution.x, (UINT)resolution.y, DXGI_FORMAT_R8G8B8A8_UNORM,
                                                        D3D11_BIND_SHADER_RESOURCE);
}

void RenderManager::render()
{
    render_init();

    Device::GetInst()->ClearTarget();

    if (SCENE_MODE::PLAY == SceneManager::GetInst()->GetSceneMode())
    {
        render_play();
    }
    else
    {
        render_tool();
    }

    m_vectorCam.clear();
    render_clear();
}

Camera* RenderManager::GetMainCam()
{
    if (SCENE_MODE::PLAY == SceneManager::GetInst()->GetSceneMode())
    {
        if (m_vectorCam.empty())
        {
            return nullptr;
        }

        return m_vectorCam[0];
    }
    else
    {
        return m_toolCam;
    }
}

void RenderManager::CopyBackBuffer()
{
    Ptr<Texture> renderTargetTexture = ResourceManager::GetInst()->FindRes<Texture>(L"RenderTargetTexture");
    CONTEXT->CopyResource(m_postEffectTexture->GetTexture2D().Get(), renderTargetTexture->GetTexture2D().Get());
}

void RenderManager::render_init()
{
    g_global.light2DCount = (int)m_vectorLight2D.size();

    const ConstBuffer* GlobalBuffer = Device::GetInst()->GetCB(CONST_BUFFER::GLOBAL);
    GlobalBuffer->SetData(&g_global);
    GlobalBuffer->UpdateData();

    vector<tLightInfo> vectorLight2DInfo;
    for (size_t i = 0; i < m_vectorLight2D.size(); ++i)
    {
        vectorLight2DInfo.push_back(m_vectorLight2D[i]->GetLightInfo());
    }

    if (m_light2DBuffer->GetElementCount() < (UINT)vectorLight2DInfo.size())
    {
        m_light2DBuffer->Create(STRUCTURED_TYPE::READONLY, m_light2DBuffer->GetElementSize(), m_light2DBuffer->GetElementCount() * 2);
    }

    m_light2DBuffer->SetData(vectorLight2DInfo.data(), (UINT)vectorLight2DInfo.size());
    m_light2DBuffer->UpdateData(51);
}

void RenderManager::render_play()
{
    for (size_t i = 0; i < m_vectorCam.size(); ++i)
    {
        m_vectorCam[i]->SortObject();
        m_vectorCam[i]->render_forward();
        m_vectorCam[i]->render_particle();
        m_vectorCam[i]->render_tilemap();
        m_vectorCam[i]->render_posteffect();
    }
}

void RenderManager::render_tool()
{
    if (nullptr != m_toolCam)
    {
        m_toolCam->SortObject();
        m_toolCam->render_forward();
        m_toolCam->render_particle();
        m_toolCam->render_tilemap();
        m_toolCam->render_posteffect();
    }
}

void RenderManager::render_clear()
{
    m_vectorLight2D.clear();
}
