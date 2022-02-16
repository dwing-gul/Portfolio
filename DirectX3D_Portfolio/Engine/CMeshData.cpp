#include "pch.h"
#include "CMeshData.h"
#include "CPathManager.h"
#include "CResourceManager.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CGameObject.h"
#include "CAnimator3D.h"
#include "CCollider3D.h"
#include "CScript.h"

CMeshData::CMeshData() :
	CRes(RESOURCE_TYPE::MESHDATA)
{
}

CMeshData::~CMeshData()
{
}

CMeshData* CMeshData::LoadFromFBX(const wstring& _filePath, bool _bHasAnim)
{
	wstring fullPath = CPathManager::GetInst()->GetContentPath();
	fullPath += _filePath;

	FBXLoader loader;
	loader.init();
	loader.SetHasAnim(_bHasAnim);
	loader.LoadFbx(fullPath);

	// 메쉬 가져오기
	CMesh* mesh = nullptr;
	mesh = CMesh::CreateFromContainer(loader);

	// ResMgr 에 메쉬 등록
	wstring meshName = L"mesh\\";
	meshName += path(fullPath).stem();
	meshName += L".mesh";

	mesh->SetName(meshName);
	mesh->SetRelativePath(meshName);

	CResourceManager::GetInst()->AddRes<CMesh>(mesh->GetName(), mesh);

	vector<Ptr<CMaterial>> vecMaterial;

	// 메테리얼 가져오기
	for (UINT i = 0; i < loader.GetContainer(0).vecMaterial.size(); ++i)
	{
		// 예외처리 (material 이름이 입력 안되어있을 수도 있다.)
		Ptr<CMaterial> material = CResourceManager::GetInst()->FindRes<CMaterial>(loader.GetContainer(0).vecMaterial[i].materialName);
		vecMaterial.push_back(material);
	}

	CMeshData* meshData = new CMeshData;
	meshData->m_mesh = mesh;
	wstring meshDataName = L"";
	meshDataName += path(fullPath).stem();
	meshData->SetName(meshDataName);
	meshData->m_vecMaterial = vecMaterial;
	meshData->m_mesh->SetMeshCenterPos(loader.GetContainer(0).meshCenter);
	meshData->m_mesh->SetMeshXYZLength(loader.GetContainer(0).meshLength);

	if (nullptr != mesh)
	{
		meshData->m_vecBoolMesh.push_back(true);
	}


	return meshData;
}

vector<CMeshData*>& CMeshData::LoadFromFBX_vector(const wstring& _filePath, bool _bHasAnim)
{
	static vector<CMeshData*> vecMeshData;

	wstring filePath = CPathManager::GetInst()->GetContentPath();
	filePath += _filePath;

	FBXLoader loader;
	loader.init();
	loader.SetHasAnim(_bHasAnim);
	loader.LoadFbx(filePath);

	vecMeshData.clear();

	for (int i = 0; i < (int)loader.GetContainerCount(); ++i)
	{
		CMesh* mesh = nullptr;
		vector<Ptr<CMaterial>> vecMaterial;

		CMeshData* meshData = new CMeshData;
		meshData->SetName(loader.GetContainer(i).name);
		bool bMesh = false;

		if (loader.GetContainer(i).bMesh)
		{
			bMesh = true;

			// 메쉬 가져오기
			mesh = CMesh::CreateFromContainer(loader, i);

			// ResMgr 에 메쉬 등록
			wstring meshName = L"mesh\\";
			meshName += loader.GetContainer(i).name;
			meshName += L".mesh";

			mesh->SetName(meshName);
			mesh->SetRelativePath(meshName);

			if (nullptr == CResourceManager::GetInst()->FindRes<CMesh>(mesh->GetName()))
				CResourceManager::GetInst()->AddRes<CMesh>(mesh->GetName(), mesh);
			else
			{
				wstring key = mesh->GetName();
				delete mesh;
				mesh = CResourceManager::GetInst()->FindRes<CMesh>(key).Get();
			}

			mesh->SetMeshCenterPos(loader.GetContainer(i).meshCenter);
			mesh->SetMeshXYZLength(loader.GetContainer(i).meshLength);

			// 메테리얼 가져오기
			for (UINT j = 0; j < loader.GetContainer(i).vecMaterial.size(); ++j)
			{
				// 예외처리 (material 이름이 입력 안되어있을 수도 있다.)
				Ptr<CMaterial> material = CResourceManager::GetInst()->FindRes<CMaterial>(loader.GetContainer(i).vecMaterial[j].materialName);
				vecMaterial.push_back(material);
			}

			meshData->m_mesh = mesh;
			meshData->m_vecMaterial = vecMaterial;
		}
		else
		{
			bMesh = false;
		}

		wstring name = meshData->GetName();

		wstring relativePath = L"meshdata\\";
		relativePath += name + L".mdat";

		meshData->SetKey(name);
		meshData->SetRelativePath(relativePath);

		meshData->m_vecParentName.push_back(loader.GetContainer(i).parentName);
		meshData->m_vecChildName.push_back(loader.GetContainer(i).vecChildName);

		tFrameTransform transform = {};
		transform.translate = loader.GetContainer(i).translate;
		transform.scale = loader.GetContainer(i).scale;
		transform.rot = loader.GetContainer(i).rotation;

		meshData->m_vecBoolMesh.push_back(bMesh);
		meshData->AddTransInformation(transform);

		if (nullptr == CResourceManager::GetInst()->FindRes<CMeshData>(name))
		{
			CResourceManager::GetInst()->AddRes<CMeshData>(name, meshData);
			vecMeshData.push_back(meshData);
		}
		else
		{
			CMeshData* MD = CResourceManager::GetInst()->FindRes<CMeshData>(name).Get();

			if (nullptr == MD->m_mesh)
			{
				MD->m_mesh = mesh;
				MD->m_vecMaterial = vecMaterial;
			}

			MD->m_vecParentName.push_back(loader.GetContainer(i).parentName);
			MD->m_vecChildName.push_back(loader.GetContainer(i).vecChildName);
			MD->m_vecBoolMesh.push_back(bMesh);
			MD->AddTransInformation(transform);
			delete meshData;
		}
	}

	return vecMeshData;
}

void CMeshData::Save(const wstring& _relativePath)
{
	wstring fileName = CPathManager::GetInst()->GetContentPath();
	fileName += _relativePath;
	SetRelativePath(_relativePath);

	FILE* file = nullptr;
	errno_t err = _wfopen_s(&file, fileName.c_str(), L"wb");
	assert(file);

	SaveWStringToFile(GetName(), file);

	UINT transformInformationCount = (UINT)m_vecTransInformation.size();
	SaveToFile(&transformInformationCount, file);

	for (size_t i = 0; i < m_vecTransInformation.size(); ++i)
	{
		SaveToFile(&m_vecTransInformation[i], file);
	}

	int parentSize = (int)m_vecParentName.size();
	SaveToFile(&parentSize, file);

	for (size_t i = 0; i < m_vecParentName.size(); ++i)
	{
		SaveWStringToFile(m_vecParentName[i], file);
	}

	int vecChildSize = (int)m_vecChildName.size();
	SaveToFile(&vecChildSize, file);

	for (size_t i = 0; i < m_vecChildName.size(); ++i)
	{
		int childSize = (int)m_vecChildName[i].size();
		SaveToFile(&childSize, file);

		for (size_t j = 0; j < m_vecChildName[i].size(); ++j)
		{
			SaveWStringToFile(m_vecChildName[i][j], file);
		}
	}

	UINT boolSize = (UINT)m_vecBoolMesh.size();
	SaveToFile(&boolSize, file);

	for (size_t i = 0; i < m_vecBoolMesh.size(); ++i)
	{
		bool boolValue = m_vecBoolMesh[i];
		SaveToFile(&boolValue, file);
	}

	bool bMesh = false;

	for (size_t i = 0; i < m_vecBoolMesh.size(); ++i)
	{
		if (m_vecBoolMesh[i] == true)
		{
			bMesh = true;
			break;
		}
	}

	UINT i = 0;

	if (bMesh)
	{
		// Mesh 를 파일로 저장
		m_mesh->Save(m_mesh->GetRelativePath());

		// Mesh Key 저장	
		// Mesh Data 저장	
		SaveResourceToFile<CMesh>(m_mesh, file);

		// material 정보 저장
		UINT materialCount = (UINT)m_vecMaterial.size();
		fwrite(&materialCount, sizeof(UINT), 1, file);

		for (; i < materialCount; ++i)
		{
			if (nullptr == m_vecMaterial[i])
				continue;

			// Material 을 파일로 저장
			m_vecMaterial[i]->Save(m_vecMaterial[i]->GetRelativePath());

			// Material 인덱스, Key, Path 저장
			fwrite(&i, sizeof(UINT), 1, file);
			SaveWStringToFile(m_vecMaterial[i]->GetKey(), file);
			SaveWStringToFile(m_vecMaterial[i]->GetRelativePath(), file);
		}
	}

	i = -1; // 마감 값
	fwrite(&i, sizeof(UINT), 1, file);

	fclose(file);
}

int CMeshData::Load(const wstring& _filePath)
{
	FILE* file = NULL;
	_wfopen_s(&file, _filePath.c_str(), L"rb");

	assert(file);

	wstring name;
	LoadWStringFromFile(name, file);
	SetName(name);

	UINT transformSize = 0;
	LoadFromFile(&transformSize, file);

	for (UINT i = 0; i < transformSize; ++i)
	{
		tFrameTransform transform = {};

		LoadFromFile(&transform, file);
		m_vecTransInformation.push_back(transform);
	}

	int parentSize = 0;
	LoadFromFile(&parentSize, file);

	for (int i = 0; i < parentSize; ++i)
	{
		wstring parentName;
		LoadWStringFromFile(parentName, file);
		m_vecParentName.push_back(parentName);
	}

	int vecChildSize = 0;
	LoadFromFile(&vecChildSize, file);

	for (size_t i = 0; i < vecChildSize; ++i)
	{
		int childSize = 0;
		LoadFromFile(&childSize, file);

		vector<wstring> vecChildName;
		for (size_t j = 0; j < childSize; ++j)
		{
			wstring childName;
			LoadWStringFromFile(childName, file);
			vecChildName.push_back(childName);
		}
		m_vecChildName.push_back(vecChildName);
	}

	UINT boolSize = 0;
	LoadFromFile(&boolSize, file);

	for (UINT i = 0; i < boolSize; ++i)
	{
		bool boolMesh = false;
		LoadFromFile(&boolMesh, file);

		m_vecBoolMesh.push_back(boolMesh);
	}

	bool bMesh = false;

	for (size_t i = 0; i < m_vecBoolMesh.size(); ++i)
	{
		if (m_vecBoolMesh[i] == true)
		{
			bMesh = true;
			break;
		}
	}

	if (bMesh)
	{
		// Mesh Load
		LoadResourceFromFile<CMesh>(m_mesh, file);
		assert(m_mesh.Get());

		// material 정보 읽기
		UINT materialCount = 0;
		fread(&materialCount, sizeof(UINT), 1, file);

		m_vecMaterial.resize(materialCount);

		for (UINT i = 0; i < materialCount; ++i)
		{
			UINT index = -1;
			fread(&index, 4, 1, file);
			if (index == -1)
				break;

			wstring key, path;
			LoadWStringFromFile(key, file);
			LoadWStringFromFile(path, file);

			Ptr<CMaterial> material = CResourceManager::GetInst()->FindRes<CMaterial>(key);
			if (nullptr == material)
				material = CResourceManager::GetInst()->Load<CMaterial>(key, path);

			m_vecMaterial[i] = material;
		}
	}

	fclose(file);

	return S_OK;
}

CGameObject* CMeshData::Instantiate()
{
	// Mesh
	// Material
	CGameObject* newObj = new CGameObject;
	newObj->AddComponent(new CTransform);
	newObj->AddComponent(new CMeshRender);

	newObj->SetMeshDataIndex(0);

	newObj->Transform()->UseFrustumCheck(true);

	newObj->MeshRender()->SetMesh(m_mesh);

	for (UINT i = 0; i < m_vecMaterial.size(); ++i)
	{
		newObj->MeshRender()->SetMaterial(m_vecMaterial[i], i);
	}

	newObj->SetName(GetName());

	if (false == m_mesh->IsAnimMesh())
		return newObj;

	CAnimator3D* animator = new CAnimator3D;
	newObj->AddComponent(animator);

	animator->SetBones(m_mesh->GetBones());
	animator->SetAnimClip(m_mesh->GetAnimClip());

	return newObj;
}

#include "CNaviMesh.h"

void CMeshData::InstantiateAndCreate(float _boundingRange, int _layerIndex)
{
	// 어차피 메쉬데이터는 이름 별로 저장되고 각 이름 별로 가지고 있는 트랜스폼 벡터의 크기가 다르니까 
	// 그냥 그 벡터의 수만큼 오브젝트를 생성해 주면됨. 그런데 그렇게 되면 메쉬데이터에서 씬에 대한
	// 크리에이트 오브젝트도 이쪽에서 같이 해줘야됨.
	// 매번 불러올 때마다 씬에서 있는지 찾을 필요도 없음.
	for (size_t i = 0; i < m_vecTransInformation.size(); ++i)
	{
		// Mesh
		// Material
		CGameObject* newObj = new CGameObject;
		newObj->SetName(GetName());
		newObj->AddComponent(new CTransform);
		newObj->AddComponent(new CMeshRender);

		newObj->SetMeshDataIndex((UINT)i);

		newObj->Transform()->SetLocalPos(m_vecTransInformation[i].translate);
		newObj->Transform()->SetLocalScale(m_vecTransInformation[i].scale);
		newObj->Transform()->SetLocalRot(m_vecTransInformation[i].rot);
		newObj->Transform()->UseFrustumCheck(true);
		newObj->Transform()->SetBoundingRange(_boundingRange);

		if (m_vecChildName.size() != 0)
		{
			for (size_t j = 0; j < m_vecChildName[i].size(); ++j)
			{
				newObj->AddChildName(m_vecChildName[i][j]);
			}
		}

		if (m_vecBoolMesh[i])
		{
			newObj->AddComponent(new CNaviMesh);
			newObj->SetObjectType(OBJECT_TYPE::NAVI_MESH);
			newObj->MeshRender()->SetMesh(m_mesh);

			for (UINT j = 0; j < m_vecMaterial.size(); ++j)
			{
				newObj->MeshRender()->SetMaterial(m_vecMaterial[j], j);
			}

			if (m_mesh->IsAnimMesh())
			{
				CAnimator3D* animator = new CAnimator3D;
				newObj->AddComponent(animator);

				animator->SetBones(m_mesh->GetBones());
				animator->SetAnimClip(m_mesh->GetAnimClip());
			}
		}

		CScript::CreateObject(newObj, newObj->Transform()->GetLocalPos(), _layerIndex);

		CScript::AddChildByName(m_vecParentName[i], newObj);
	}
}
