#include "pch.h"
#include "MeshRender.h"
#include "GameObject.h"
#include "Transform.h"
#include "Collider2D.h"
#include "Animator2D.h"

MeshRender::MeshRender() :
    Component(COMPONENT_TYPE::MESH_RENDER)
{
}

MeshRender::~MeshRender()
{
}

void MeshRender::render()
{
    if (nullptr == m_mesh || nullptr == m_material)
        return;

    GetTransform()->UpdateData();

    if (GetAnimator2D())
    {
        GetAnimator2D()->UpdateData();
    }
    else
    {
        Animator2D::Clear();
    }

    m_mesh->UpdateData();
    m_material->UpdateData();

    if(m_material != ResourceManager::GetInst()->FindRes<Material>(L"Collider2DMaterial").Get())
        m_mesh->render();

    m_material->Clear();
}

Ptr<Material> MeshRender::GetCloneMaterial()
{
    if (nullptr == m_material)
        return nullptr;

    m_material = m_material->Clone();
    return m_material;
}

void MeshRender::SaveToScene(FILE* _file)
{
    Component::SaveToScene(_file);
    SaveResourceToFile(m_mesh, _file);
    SaveResourceToFile(m_material, _file);
}

void MeshRender::LoadFromScene(FILE* _file)
{
    Component::LoadFromScene(_file);
    LoadResourceFromFile(m_mesh, _file);
    LoadResourceFromFile(m_material, _file);
}
