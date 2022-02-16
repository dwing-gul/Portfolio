#include "pch.h"
#include "CAnimator2D.h"
#include "CAnimation2D.h"
#include "CDevice.h"
#include "CConstBuffer.h"

CAnimator2D::CAnimator2D() :
    CComponent(COMPONENT_TYPE::ANIMATOR2D),
    m_curAnim(nullptr)
{
}

CAnimator2D::CAnimator2D(const CAnimator2D& _origin) :
    CComponent(_origin),
    m_curAnim(nullptr)
{
    for (auto& pair : _origin.m_mapAnim)
    {
        pair.second->m_animator = this;
        m_mapAnim.insert(make_pair(pair.first, pair.second->Clone()));
    }

    if (nullptr != _origin.m_curAnim)
    {
        wstring curAnimName = _origin.m_curAnim->GetName();
        m_curAnim = m_mapAnim.find(curAnimName)->second;
    }
}

CAnimator2D::~CAnimator2D()
{
    Safe_Delete_Map(m_mapAnim);
}

void CAnimator2D::lateUpdate()
{
    if (nullptr == m_curAnim)
    {
        return;
    }

    if (m_curAnim->IsFinish() && m_curAnim->IsRepeat())
    {
        m_curAnim->Reset();
    }

    m_curAnim->lateUpdate();
}

CAnimation2D* CAnimator2D::FindAnim(const wstring& _name)
{
    map<wstring, CAnimation2D*>::iterator iter = m_mapAnim.find(_name);

    if (m_mapAnim.end() == iter)
    {
        return nullptr;
    }

    return iter->second;
}

void CAnimator2D::CreateAnim(const wstring& _name, Ptr<CTexture> _atlas, Vec2 _leftTop, Vec2 _frameSize, Vec2 _baseSize, float _duration, int _frameCount)
{
    CAnimation2D* anim = FindAnim(_name);
    assert(!anim);

    anim = new CAnimation2D;
    anim->SetName(_name);
    anim->Create(_atlas, _leftTop, _frameSize, _baseSize, _duration, _frameCount);
    anim->m_animator = this;
    m_mapAnim.insert(make_pair(_name, anim));
}

void CAnimator2D::CreateAnimation_Tool(const wstring& _strName, Ptr<CTexture> _pAtlas, vector<tAnimFrame>& _vecAnimFrm)
{
    CAnimation2D* pAnim = FindAnim(_strName);
    if (pAnim)
    {
        vector<tAnimFrame>& vecAnim = pAnim->GetVectorAnimFrame();
        vecAnim.clear();
        for (size_t i = 0; i < _vecAnimFrm.size(); i++)
        {
            vecAnim.push_back(_vecAnimFrm[i]);
        }
    }

    pAnim = new CAnimation2D;
    pAnim->SetName(_strName);
    pAnim->Create_Tool(_pAtlas, _vecAnimFrm);
    m_mapAnim.insert(make_pair(_strName, pAnim));
}

void CAnimator2D::Play(const wstring& _name, bool _bRepeat)
{
    m_curAnim = FindAnim(_name);

    if (nullptr != m_curAnim)
        m_curAnim->SetRepeat(_bRepeat);
}

void CAnimator2D::UpdateData()
{
    if (m_curAnim)
        m_curAnim->UpdateData();
}

void CAnimator2D::Clear()
{
    static const CConstBuffer* buffer = CDevice::GetInst()->GetCB(CONST_BUFFER::ANIM2D_INFO);

    tAnim2DInfo info{};
    buffer->SetData(&info);
    buffer->UpdateData();
}

void CAnimator2D::LoadAnim(const wstring& _relativePath)
{
    CAnimation2D* newAnim = new CAnimation2D;
    newAnim->Load(_relativePath);
    m_mapAnim.insert(make_pair(newAnim->GetName(), newAnim));
}

void CAnimator2D::SaveToScene(FILE* _file)
{
    CComponent::SaveToScene(_file);
    UINT animCount = (UINT)m_mapAnim.size();
    SaveToFile(&animCount, _file);

    map<wstring, CAnimation2D*>::iterator iter = m_mapAnim.begin();
    for (; iter != m_mapAnim.end(); ++iter)
    {
        iter->second->SaveToScene(_file);
    }

    UINT curAnim = 0;
    if (m_curAnim)
    {
        bool repeat = m_curAnim->IsRepeat();
        curAnim = 1;
        SaveToFile(&curAnim, _file);
        SaveToFile(&repeat, _file);
        SaveWStringToFile(m_curAnim->GetName(), _file);
    }
    else
        SaveToFile(&curAnim, _file);
}

void CAnimator2D::LoadFromScene(FILE* _file)
{
    CComponent::LoadFromScene(_file);
    UINT animCount = 0;
    LoadFromFile(&animCount, _file);

    for (UINT i = 0; i < animCount; ++i)
    {
        CAnimation2D* anim = new CAnimation2D;
        anim->LoadFromScene(_file);
        anim->m_animator = this;
        m_mapAnim.insert(make_pair(anim->GetName(), anim));
    }

    UINT curAnim = 0;
    bool repeat;
    LoadFromFile(&curAnim, _file);
    if (curAnim)
    {
        LoadFromFile(&repeat, _file);

        wstring animName;
        LoadWStringFromFile(animName, _file);

        m_curAnim = FindAnim(animName);
    }

    Play(m_curAnim->GetName(), repeat);
}

