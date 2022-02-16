#include "pch.h"
#include "Animator2D.h"
#include "Animation2D.h"
#include "Texture.h"
#include "Device.h"
#include "ConstBuffer.h"

Animator2D::Animator2D() :
    Component(COMPONENT_TYPE::ANIMATOR2D),
    m_curAnim(nullptr)
{
}

Animator2D::Animator2D(const Animator2D& _origin) :
    Component(_origin),
    m_curAnim(nullptr)
{
    for (const auto& pair : _origin.m_mapAnim)
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

Animator2D::~Animator2D()
{
    Safe_Delete_Map(m_mapAnim);
}

void Animator2D::lateUpdate()
{
    if (nullptr == m_curAnim)
        return;

    if (m_curAnim->IsFinish() && m_curAnim->IsRepeat())
    {
        m_curAnim->Reset();
    }

    m_curAnim->lateUpdate();
}

Animation2D* Animator2D::FindAnim(const wstring& _name)
{
    map<wstring, Animation2D*>::iterator iter = m_mapAnim.find(_name);

    if (m_mapAnim.end() == iter)
    {
        return nullptr;
    }

    return iter->second;
}

void Animator2D::CreateAnim(const wstring& _name, Ptr<Texture> _atlasTexture, Vector2 _leftTop, Vector2 _frameSize, Vector2 _baseSize, float _duration, int _frameCount)
{
    Animation2D* anim = FindAnim(_name);
    assert(!anim);

    anim = new Animation2D;
    anim->SetName(_name);
    anim->Create(_atlasTexture, _leftTop, _frameSize, _baseSize, _duration, _frameCount);
    anim->m_animator = this;
    m_mapAnim.insert(make_pair(_name, anim));
}

void Animator2D::LoadAnim(const wstring& _relativePath)
{
    Animation2D* anim = new Animation2D;
    anim->Load(_relativePath);
    m_mapAnim.insert(make_pair(anim->GetName(), anim));
}

void Animator2D::Play(const wstring& _name, bool _bRepeat)
{
    m_curAnim = FindAnim(_name);

    if(nullptr != m_curAnim)
        m_curAnim->SetRepeat(_bRepeat);
}

void Animator2D::UpdateData()
{
    if (m_curAnim)
    {
        m_curAnim->UpdateData();
    }
}

void Animator2D::Clear()
{
    static const ConstBuffer* CB = Device::GetInst()->GetCB(CONST_BUFFER::ANIM2D_INFO);

    tAnim2DInfo info = {};
    CB->SetData(&info);
    CB->UpdateData();
}

void Animator2D::SaveToScene(FILE* _file)
{
    Component::SaveToScene(_file);
    UINT animCount = (UINT)m_mapAnim.size();
    SaveToFile(&animCount, _file);

    map<wstring, Animation2D*>::iterator iter = m_mapAnim.begin();
    for (; iter != m_mapAnim.end(); ++iter)
    {
        iter->second->SaveToScene(_file);
    }

    UINT curAnim = 0;
    if (m_curAnim)
    {
        curAnim = 1;
        SaveToFile(&curAnim, _file);
        SaveWStringToFile(m_curAnim->GetName(), _file);
    }
    else
        SaveToFile(&curAnim, _file);
}

void Animator2D::LoadFromScene(FILE* _file)
{
    Component::LoadFromScene(_file);
    UINT animCount = 0;
    LoadFromFile(&animCount, _file);

    for (UINT i = 0; i < animCount; ++i)
    {
        Animation2D* anim = new Animation2D;
        anim->LoadFromScene(_file);
        anim->m_animator = this;
        m_mapAnim.insert(make_pair(anim->GetName(), anim));
    }

    UINT curAnim = 0;
    LoadFromFile(&curAnim, _file);
    if (curAnim)
    {

        wstring animName;
        LoadWStringFromFile(animName, _file);

        m_curAnim = FindAnim(animName);
    }

    if (nullptr == m_curAnim)
        return;

    Play(m_curAnim->GetName());
}