#include "pch.h"
#include "ToolScene.h"
#include "SceneSaveLoad.h"
#include <Engine/CollisionManager.h>
#include <Engine/Scene.h>
#include <Engine/GameObject.h>
#include <Engine/Transform.h>
#include <Engine/MeshRender.h>
#include <Engine/Camera.h>
#include <Engine/Light2D.h>
#include <Script/CameraScript.h>
#include <Script/LightScript.h>

ToolScene::ToolScene()
{
	SetName("ToolScene");
}

ToolScene::~ToolScene()
{
}

void ToolScene::CreateScene()
{
	Scene* sample = nullptr;
	sample = new Scene;
	sample->SetSceneType(SCENE_TYPE::TOOL);

	GameObject* camera = new GameObject;
	camera->SetName(L"Camera");
	camera->AddComponent(new Transform);
	camera->AddComponent(new Camera);
	camera->AddComponent(new CameraScript);
	camera->SetObjectType(OBJECT_TYPE::CAMERA);
	camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::PERSPECTIVE);
	camera->GetCamera()->SetLayerCheckAll();
	camera->GetTransform()->SetLocalPos(Vector3(0.f, 0.f, 0.f));
	
	const vector<Script*> camScript = camera->GetScripts();
	Script::CreateObject(sample, camera, camera->GetTransform()->GetLocalPos(), 0);
	

	GameObject* light = new GameObject;
	light->SetName(L"Light");
	light->AddComponent(new Transform);
	light->AddComponent(new Light2D);
	light->AddComponent(new LightScript);
	light->SetObjectType(OBJECT_TYPE::LIGHT);
	light->GetLight2D()->SetDiffuseColor(Vector3(1.f, 1.f, 1.f));
	light->GetLight2D()->SetLightType(LIGHT_TYPE::POINT);
	light->GetLight2D()->SetRange(450.f);
	light->GetLight2D()->SetLightPos(camera->GetTransform()->GetLocalPos());
	Script::CreateObject(sample, light, light->GetTransform()->GetLocalPos(), 0);

	Ptr<Mesh> mesh = ResourceManager::GetInst()->FindRes<Mesh>(L"CubeMesh");
	Ptr<Material> stdMaterial = ResourceManager::GetInst()->FindRes<Material>(L"Std3DMaterial");

	GameObject* obj = new GameObject;
	obj->SetName(L"Player");
	obj->AddComponent(new Transform);
	obj->AddComponent(new MeshRender);

	obj->GetMeshRender()->SetMesh(mesh);
	obj->GetMeshRender()->SetMaterial(stdMaterial);

	obj->GetTransform()->SetLocalPos(Vector3(0.f, 0.f, 100.f));
	obj->GetTransform()->SetLocalScale(Vector3(80.f, 80.f, 80.f));
	obj->GetTransform()->SetLocalRot(Vector3(0.f, 0.f, 0.f));
	Script::CreateObject(sample, obj, obj->GetTransform()->GetLocalPos(), 0);

	CollisionManager::GetInst()->CollisionCheck(0, 1);

	ChangeScene(sample);

}
