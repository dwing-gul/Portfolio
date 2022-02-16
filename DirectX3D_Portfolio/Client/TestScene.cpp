#include "pch.h"
#include "TestScene.h"
#include <Engine/CSceneManager.h>
#include <Engine/CGameObject.h>
#include <Engine/CScene.h>
#include <Engine/CTransform.h>
#include <Engine/CCollider2D.h>
#include <Engine/CCollider3D.h>
#include <Engine/CTerrain.h>
#include <Engine/CCamera.h>
#include <Engine/CScript.h>
#include <Engine/CLight3D.h>
#include <Engine/CNaviMesh.h>
#include <Engine/CCollisionManager.h>
#include <Engine/CParticleSystem.h>
#include <Engine/CAnimator2D.h>

#include <Script/CPlayerScript.h>
#include <Script/CSwitchUIScript.h>
#include <Script/CLayerUpdateBox.h>
#include <Script/CItemBoxUIScript.h>
#include <Script/CButtonUIScript.h>
#include <Script/CFontUIScript.h>
#include <Script/CBarUIScript.h>
#include <Script/CSkillUIScript.h>
#include <Script/CPictureUIScript.h>
#include <Script/CTrackingUIScript.h>
#include <Script/CLiDailin.h>
#include <Script/CAya.h>
#include <Script/CHyunwooScript.h>
#include <Script/CEffectScript.h>
#include <Script/CLightScript.h>
#include <Script/CCameraScript.h>
#include <Script/CBulletScript.h>
#include <Script/CMouseRay.h>
#include <Script/CMonsterCreateScript.h>
#include <Script/CMonsterManager.h>
#include <Script/CAIPointScript.h>

void TestScene::CreateTestScene()
{
	CScene* newScene = nullptr;

	newScene = new CScene;

	CGameObject* camera = new CGameObject;
	camera->SetName(L"MainCamera");
	camera->AddComponent(new CTransform);
	camera->AddComponent(new CCamera);
	camera->AddComponent(new CCameraScript);
	camera->Camera()->SetProjectionType(PROJECTION_TYPE::PERSPECTIVE);
	camera->Camera()->SetLayerCheckAll();
	camera->Camera()->SetLayerCheck(30);
	camera->Transform()->SetLocalPos(Vector3(0.f, 0.f, 0.f));
	camera->Transform()->SetLocalRot(Vec3(66.f * XM_PI / 180.f, -50.f * XM_PI / 180.f, 0.f));

	CScript::CreateObject(camera, camera->Transform()->GetLocalPos(), 0);

	CGameObject* UIcamera = new CGameObject;
	UIcamera->SetName(L"UICamera");
	UIcamera->AddComponent(new CTransform);
	UIcamera->AddComponent(new CCamera);
	UIcamera->Camera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
	UIcamera->Camera()->SetLayerCheck(31);
	UIcamera->Camera()->SetLayerCheck(30);
	UIcamera->Camera()->SetCameraType(CAMERA_TYPE::UI);
	UIcamera->Transform()->SetLocalPos(Vector3(0.f, 0.f, 0.f));

	CScript::CreateObject(UIcamera, 0);

	CGameObject* light3DObj = new CGameObject;
	light3DObj->SetName(L"Light3D");
	light3DObj->AddComponent(new CTransform);
	light3DObj->AddComponent(new CMeshRender);
	light3DObj->AddComponent(new CLight3D);
	light3DObj->AddComponent(new CLightScript);

	light3DObj->Light3D()->SetDiffuseColor(Vec3(1.f, 1.f, 1.f));
	light3DObj->Light3D()->SetSpecularColor(Vec3(0.3f, 0.3f, 0.3f));
	light3DObj->Light3D()->SetAmbientColor(Vec3(0.3f, 0.3f, 0.3f));
	light3DObj->Light3D()->SetRange(10000.f);
	light3DObj->Light3D()->SetLightType(LIGHT_TYPE::DIR);

	light3DObj->Transform()->SetLocalRot(Vec3(XM_PI / 4.f, XM_PI / 4.f, 0.f));

	CScript::CreateObject(light3DObj, Vec3(0.f, 300.f, 0.f), 0);

	CPrefab* itemspwan = nullptr;
	CGameObject* pTest = nullptr;

	itemspwan = CResourceManager::GetInst()->Load<CPrefab>(L"ItemSpawn", L"prefab\\ItemSpawn.prefab").Get();
	pTest = itemspwan->Instantiate();
	CScript::CreateObject(pTest, pTest->Transform()->GetLocalPos(), 1);
	
	itemspwan = CResourceManager::GetInst()->Load<CPrefab>(L"RootUI", L"prefab\\RootUI.prefab").Get();
	pTest = itemspwan->Instantiate();
	CScript::CreateObject(pTest, pTest->Transform()->GetLocalPos(), 31);

	itemspwan = CResourceManager::GetInst()->Load<CPrefab>(L"AlleyMap", L"prefab\\AlleyMap.prefab").Get();
	pTest = itemspwan->Instantiate();
	CScript::CreateObject(pTest, pTest->Transform()->GetLocalPos(), 0);

	itemspwan = CResourceManager::GetInst()->Load<CPrefab>(L"NaviMesh", L"prefab\\NaviMesh.prefab").Get();
	pTest = itemspwan->Instantiate();
	CScript::CreateObject(pTest, pTest->Transform()->GetLocalPos(), 0);

	CGameObject* mouse = new CGameObject;
	mouse->SetName(L"Mouse");
	mouse->AddComponent(new CTransform);
	mouse->AddComponent(new CMouseRay);
	CScript::CreateObject(mouse, Vec3(0.f, 300.f, 0.f), 0);

	CGameObject* obj;
	CPrefab* Aya = CResourceManager::GetInst()->Load<CPrefab>(L"Aya", L"prefab\\Aya.prefab").Get();
	CPrefab* LiDailin = CResourceManager::GetInst()->Load<CPrefab>(L"LiDailin", L"prefab\\LiDailin.prefab").Get();
	CPrefab* Hyunwoo = CResourceManager::GetInst()->Load<CPrefab>(L"Hyunwoo", L"prefab\\Hyunwoo.prefab").Get();

	obj = Aya->Instantiate();
	obj->SetName(L"Player");
	obj->Transform()->SetLocalPos(Vec3(-10000.f, 10.f, 12000.f));
	CScript::CreateObject(obj, obj->Transform()->GetLocalPos());

	obj = Hyunwoo->Instantiate();
	obj->SetName(L"Hyunwoo");
	obj->Transform()->SetLocalPos(Vec3(-14000.f, 10.f, 11000.f));
	((CPlayerScript*)obj->GetScripts()[0])->SetPlayerType(PLAYER_TYPE::AI);
	CScript::CreateObject(obj, obj->Transform()->GetLocalPos());

	obj = LiDailin->Instantiate();
	obj->SetName(L"LiDailin");
	obj->Transform()->SetLocalPos(Vec3(-10000.f, 10.f, 16000.f));
	((CPlayerScript*)obj->GetScripts()[0])->SetPlayerType(PLAYER_TYPE::AI);
	CScript::CreateObject(obj, obj->Transform()->GetLocalPos());

	CCollisionManager::GetInst()->CollisionCheck(0, 0);
	CCollisionManager::GetInst()->CollisionCheck(0, 1);
	CSceneManager::GetInst()->ChangeScene(newScene);
}