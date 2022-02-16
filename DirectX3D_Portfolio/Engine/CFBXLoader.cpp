#include "pch.h"
#include "CFBXLoader.h"
#include "CMesh.h"
#include "CResourceManager.h"
#include "CPathManager.h"

FBXLoader::FBXLoader() :
	m_manager(NULL),
	m_scene(NULL),
	m_importer(NULL),
	m_bHasAnim(false)
{
}

FBXLoader::~FBXLoader()
{
	m_scene->Destroy();
	m_manager->Destroy();

	Safe_Delete_Vec(m_vecBone);
	Safe_Delete_Vec(m_vecAnimClip);

	for (int i = 0; i < m_arrAnimName.Size(); ++i)
	{
		Safe_Delete_Ptr(m_arrAnimName[i]);
	}
}

void FBXLoader::init()
{
	m_manager = FbxManager::Create();

	if (NULL == m_manager)
	{
		assert(NULL);
	}

	FbxIOSettings* IOSettings = FbxIOSettings::Create(m_manager, IOSROOT);
	m_manager->SetIOSettings(IOSettings);
	m_scene = FbxScene::Create(m_manager, "");
	if (NULL == m_scene)
		assert(NULL);
}

void FBXLoader::LoadFbx(const wstring& _path)
{
	m_vecContainer.clear();

	m_importer = FbxImporter::Create(m_manager, "");

	//wstring str = wstring_convert<codecvt_utf8<wchar_t>>().from_bytes(strName.c_str());
	string path(_path.begin(), _path.end());

	if (!m_importer->Initialize(path.c_str(), -1, m_manager->GetIOSettings()))
		assert(nullptr);

	m_importer->Import(m_scene);

	/*FbxAxisSystem originAxis = FbxAxisSystem::eMax;
	originAxis = m_pScene->GetGlobalSettings().GetAxisSystem();
	FbxAxisSystem DesireAxis = FbxAxisSystem::DirectX;
	DesireAxis.ConvertScene(m_pScene);
	originAxis = m_pScene->GetGlobalSettings().GetAxisSystem();*/

	m_scene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::Max);

	// Bone 정보 읽기
	LoadSkeleton(m_scene->GetRootNode());

	// Animation 이름정보 
	m_scene->FillAnimStackNameArray(m_arrAnimName);

	// Animation Clip 정보
	LoadAnimationClip();

	// 삼각화(Triangulate)
	Triangulate(m_scene->GetRootNode());

	// 메쉬 데이터 얻기
	LoadMeshDataFromNode(m_scene->GetRootNode());

	//애니메이션 작업이 끝난 후 실제 측정한 애니메이션 프레임 카운트 만큼 본에 애니메이션 행렬을 넣어준다.
	//본개수

	if (!m_vecAllKeyFrame.empty())
	{
		for (int i = 0; i < m_vecBone.size(); ++i)
		{
			//클립 개수
			for (int j = 0; j < m_vecAllKeyFrame[i].size(); ++j)
			{
				//애니메이션 행렬 개수
				for (int k = 0; k < m_vecAllKeyFrame[i][j].size(); ++k)
				{
					if (k > m_vecAnimFrame[j] - 1 || m_vecAnimFrame[j] == 0)
						break;
					m_vecBone[i]->vecKeyFrame.push_back(m_vecAllKeyFrame[i][j][k]);
				}
			}
		}

		//애니메이션 클립에 들어가 있는 시간을 다시 계산
		FbxTime   time = 0;
		time.SetFrame(1, m_vecAnimClip[0]->mode);

		for (int i = 0; i < (int)m_vecAnimFrame.size(); ++i)
		{
			wstring name = m_vecAnimClip[i]->name;
			//타임 길이는 프레임 개수, endtime은 fbx 타임 1초 * 프레임 길이 
			m_vecAnimClip[i]->timeLength = m_vecAnimFrame[i];
			m_vecAnimClip[i]->endTime = m_vecAnimClip[i]->timeLength * time.Get();
		}

	}

	m_importer->Destroy();

	// 필요한 텍스쳐 로드
	LoadTexture();

	// 필요한 메테리얼 생성
	CreateMaterial();
}

void FBXLoader::LoadMeshDataFromNode(FbxNode* _node)
{
	// 노드의 메쉬정보 읽기
	FbxNodeAttribute* attribute = _node->GetNodeAttribute();

	UINT mCount = _node->GetMaterialCount();
	FbxNodeAttribute::EType type;
	if(attribute)
		type = attribute->GetAttributeType();
	if (attribute && FbxNodeAttribute::eMesh == attribute->GetAttributeType())
	{
		FbxMesh* mesh = _node->GetMesh();
		if (NULL != mesh)
		{
			LoadMesh(mesh);
		}
	}
	else
	{
		if (!m_bHasAnim)
		{
			// 메쉬가 없는 더미 노드의 이름, 부모의 이름, 자식들의 이름, 트랜스폼 정보를 얻어와서 컨테이너 벡터에 넣음
			string name = _node->GetName();

			if (name != "RootNode")
			{
				m_vecContainer.push_back(tContainer{});
				tContainer& container = m_vecContainer[m_vecContainer.size() - 1];

				container.name = StringToWString(name);

				string parentName = _node->GetParent()->GetName();
				container.parentName = StringToWString(parentName);

				for (int i = 0; i < _node->GetChildCount(); ++i)
				{
					string childName = _node->GetChild(i)->GetName();

					container.vecChildName.push_back(StringToWString(childName));
				}

				container.bMesh = false;

				FbxDouble3 T = _node->LclTranslation.Get();
				FbxDouble3 S = _node->LclScaling.Get();
				FbxDouble3 R = _node->LclRotation.Get();

				container.translate = Vec3((float)T.mData[0], (float)T.mData[1], (float)T.mData[2]);
				container.scale = Vec3((float)S.mData[0], (float)S.mData[1], (float)S.mData[2]);
				container.rotation = Vec3((float)R.mData[0], (float)R.mData[1], (float)R.mData[2]);
				container.rotation = (container.rotation * XM_PI) / 180.f;
			}
		}
	}

	// 해당 노드의 재질정보 읽기
	UINT materialCount = _node->GetMaterialCount();
	if (materialCount > 0)
	{
		for (UINT i = 0; i < materialCount; ++i)
		{
			FbxSurfaceMaterial* materialSurface = _node->GetMaterial(i);
			LoadMaterial(materialSurface);
		}
	}

	// 자식 노드 정보 읽기
	int childCount = _node->GetChildCount();
	for (int i = 0; i < childCount; ++i)
	{
		LoadMeshDataFromNode(_node->GetChild(i));
	}
}

void FBXLoader::LoadMesh(FbxMesh* _fbxMesh)
{
	m_vecContainer.push_back(tContainer{});
	tContainer& container = m_vecContainer[m_vecContainer.size() - 1];

	string name = _fbxMesh->GetNode()->GetName();
	container.name = wstring(name.begin(), name.end());

	// 더미 노드에서 했던것과 같음 이건 메쉬가 있는 노드들
	string parentName = _fbxMesh->GetNode()->GetParent()->GetName();
	container.parentName = StringToWString(parentName);

	for (int i = 0; i < _fbxMesh->GetNode()->GetChildCount(); ++i)
	{
		string childName = _fbxMesh->GetNode()->GetChild(i)->GetName();

		container.vecChildName.push_back(StringToWString(childName));
	}

	container.bMesh = true;

	FbxVector4	vector1 = { 1, 0, 0, 0 };
	FbxVector4	vector2 = { 0, 0, 1, 0 };
	FbxVector4	vector3 = { 0, 1, 0, 0 };
	FbxVector4	vector4 = { 0, 0, 0, 1 };
	FbxAMatrix	reflectMatrix;
	reflectMatrix.mData[0] = vector1;
	reflectMatrix.mData[1] = vector2;
	reflectMatrix.mData[2] = vector3;
	reflectMatrix.mData[3] = vector4;

	FbxDouble3 T = _fbxMesh->GetNode()->LclTranslation.Get();
	FbxDouble3 S = _fbxMesh->GetNode()->LclScaling.Get();
	FbxDouble3 R = _fbxMesh->GetNode()->LclRotation.Get();

	container.translate = Vec3((float)T.mData[0], (float)T.mData[1], (float)T.mData[2]);
	container.scale = Vec3((float)S.mData[0], (float)S.mData[1], (float)S.mData[2]);
	container.rotation = Vec3((float)R.mData[0], (float)R.mData[1], (float)R.mData[2]);
	container.rotation = (container.rotation * XM_PI) / 180.f;

	int vertexCount = _fbxMesh->GetControlPointsCount();
	container.Resize(vertexCount);

	FbxVector4* fbxPos = _fbxMesh->GetControlPoints();

	for (int i = 0; i < vertexCount; ++i)
	{
		if (!m_bHasAnim)
		{
			container.vecPos[i].x = (float)fbxPos[i].mData[0];
			container.vecPos[i].y = (float)fbxPos[i].mData[1];
			container.vecPos[i].z = (float)fbxPos[i].mData[2];
		}
		else
		{
			container.vecPos[i].x = (float)fbxPos[i].mData[0];
			container.vecPos[i].y = (float)fbxPos[i].mData[2];
			container.vecPos[i].z = (float)fbxPos[i].mData[1];
		}
	}

	Vec3 Min = container.vecPos[0];
	Vec3 Max = container.vecPos[0];

	// 메쉬의 xyz 길이 구하기 및 메쉬의 중앙의 위치 구하기
	for (int i = 0; i < vertexCount; ++i)
	{
		Min.x = min(Min.x, container.vecPos[i].x);
		Min.y= min(Min.y, container.vecPos[i].y);
		Min.z = min(Min.z, container.vecPos[i].z);

		Max.x = max(Max.x, container.vecPos[i].x);
		Max.y = max(Max.y, container.vecPos[i].y);
		Max.z = max(Max.z, container.vecPos[i].z);
	}

	container.meshLength = Vec3(abs(Max.x - Min.x), abs(Max.y - Min.y), abs(Max.z - Min.z));
	container.meshCenter = Vec3((Max.x + Min.x) / 2.f, (Max.y + Min.y) / 2.f, (Max.z + Min.z) / 2.f);
	
	// 폴리곤 개수
	int polygonCount = _fbxMesh->GetPolygonCount();

	// 재질의 개수 ( ==> SubSet 개수 ==> Index Buffer Count)
	int materialCount = _fbxMesh->GetNode()->GetMaterialCount();
	container.vecIndex.resize(materialCount);

	// 정점 정보가 속한 subset 을 알기위해서...
	FbxGeometryElementMaterial* material = _fbxMesh->GetElementMaterial();

	// 폴리곤을 구성하는 정점 개수
	int polygonsize = _fbxMesh->GetPolygonSize(0);
	if (3 != polygonsize)
		assert(NULL); // Polygon 구성 정점이 3개가 아닌 경우

	UINT arrIndex[3] = {};
	UINT vertexOrder = 0; // 폴리곤 순서로 접근하는 순번

	for (int i = 0; i < polygonCount; ++i)
	{
		for (int j = 0; j < polygonsize; ++j)
		{
			// i 번째 폴리곤에, j 번째 정점
			int index = _fbxMesh->GetPolygonVertex(i, j);
			arrIndex[j] = index;

			GetTangent(_fbxMesh, &container, index, vertexOrder);
			GetBinormal(_fbxMesh, &container, index, vertexOrder);
			GetNormal(_fbxMesh, &container, index, vertexOrder);
			GetUV(_fbxMesh, &container, index, _fbxMesh->GetTextureUVIndex(i, j));

			++vertexOrder;
		}
		UINT subsetIndex = material->GetIndexArray().GetAt(i);
		container.vecIndex[subsetIndex].push_back(arrIndex[0]);
		container.vecIndex[subsetIndex].push_back(arrIndex[2]);
		container.vecIndex[subsetIndex].push_back(arrIndex[1]);
	}

	LoadAnimationData(_fbxMesh, &container);
}

void FBXLoader::LoadMaterial(FbxSurfaceMaterial* _materialSurface)
{
	tFBXMaterial tMtrlInfo{};

	string str = _materialSurface->GetName();
	tMtrlInfo.materialName = wstring(str.begin(), str.end());

	// Diff
	tMtrlInfo.material.diffuse = GetMaterialData(_materialSurface, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);

	// Amb
	tMtrlInfo.material.ambient = GetMaterialData(_materialSurface, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);

	// Spec
	tMtrlInfo.material.specular = GetMaterialData(_materialSurface, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);

	// Emisv
	tMtrlInfo.material.emissive = GetMaterialData(_materialSurface, FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor);

	// Texture Name
	tMtrlInfo.diff = GetMaterialTextureName(_materialSurface, FbxSurfaceMaterial::sDiffuse);
	tMtrlInfo.normal = GetMaterialTextureName(_materialSurface, FbxSurfaceMaterial::sNormalMap);
	tMtrlInfo.spec = GetMaterialTextureName(_materialSurface, FbxSurfaceMaterial::sSpecular);


	m_vecContainer.back().vecMaterial.emplace_back(tMtrlInfo);
}

void FBXLoader::GetTangent(FbxMesh* _mesh, tContainer* _container, int _index, int _vertexOrder)
{
	int tangentCount = _mesh->GetElementTangentCount();
	if (1 != tangentCount)
		return; // 정점 1개가 포함하는 탄젠트 정보가 2개 이상이다.

	// 탄젠트 data 의 시작 주소
	FbxGeometryElementTangent* tangent = _mesh->GetElementTangent();
	UINT tangentIndex = 0;

	if (tangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (tangent->GetReferenceMode() == FbxGeometryElement::eDirect)
			tangentIndex = _vertexOrder;
		else
			tangentIndex = tangent->GetIndexArray().GetAt(_vertexOrder);
	}
	else if (tangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (tangent->GetReferenceMode() == FbxGeometryElement::eDirect)
			tangentIndex = _index;
		else
			tangentIndex = tangent->GetIndexArray().GetAt(_index);
	}

	FbxVector4 vecTangent = tangent->GetDirectArray().GetAt(tangentIndex);

	_container->vecTangent[_index].x = (float)vecTangent.mData[0];
	_container->vecTangent[_index].y = (float)vecTangent.mData[2];
	_container->vecTangent[_index].z = (float)vecTangent.mData[1];
}

void FBXLoader::GetBinormal(FbxMesh* _mesh, tContainer* _container, int _index, int _vertexOrder)
{
	int binormalCount = _mesh->GetElementBinormalCount();
	if (1 != binormalCount)
		return; // 정점 1개가 포함하는 종법선 정보가 2개 이상이다.

	// 종법선 data 의 시작 주소
	FbxGeometryElementBinormal* binormal = _mesh->GetElementBinormal();
	UINT binormalIndex = 0;

	if (binormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (binormal->GetReferenceMode() == FbxGeometryElement::eDirect)
			binormalIndex = _vertexOrder;
		else
			binormalIndex = binormal->GetIndexArray().GetAt(_vertexOrder);
	}
	else if (binormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (binormal->GetReferenceMode() == FbxGeometryElement::eDirect)
			binormalIndex = _index;
		else
			binormalIndex = binormal->GetIndexArray().GetAt(_index);
	}

	FbxVector4 vecBinormal = binormal->GetDirectArray().GetAt(binormalIndex);

	_container->vecBinormal[_index].x = (float)vecBinormal.mData[0];
	_container->vecBinormal[_index].y = (float)vecBinormal.mData[2];
	_container->vecBinormal[_index].z = (float)vecBinormal.mData[1];
}

void FBXLoader::GetNormal(FbxMesh* _mesh, tContainer* _container, int _index, int _vertexOrder)
{
	int normalCount = _mesh->GetElementNormalCount();
	if (1 != normalCount)
		return; // 정점 1개가 포함하는 종법선 정보가 2개 이상이다.

					  // 종법선 data 의 시작 주소
	FbxGeometryElementNormal* normal = _mesh->GetElementNormal();
	UINT normalIndex = 0;

	if (normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
			normalIndex = _vertexOrder;
		else
			normalIndex = normal->GetIndexArray().GetAt(_vertexOrder);
	}
	else if (normal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
			normalIndex = _index;
		else
			normalIndex = normal->GetIndexArray().GetAt(_index);
	}

	FbxVector4 vNormal = normal->GetDirectArray().GetAt(normalIndex);

	_container->vecNormal[_index].x = (float)vNormal.mData[0];
	_container->vecNormal[_index].y = (float)vNormal.mData[2];
	_container->vecNormal[_index].z = (float)vNormal.mData[1];
}

void FBXLoader::GetUV(FbxMesh* _mesh, tContainer* _container, int _index, int _UVIndex)
{
	FbxGeometryElementUV* uv = _mesh->GetElementUV();

	if (nullptr == uv)
		return;

	UINT uvIndex = 0;
	if (uv->GetReferenceMode() == FbxGeometryElement::eDirect)
		uvIndex = _index;

	UINT UVIndex = 0;
	//FbxGeometryElement 에 따라 분기처리
	//현재 구조에서 eDirect가 아니면 uv를 확인하지 못하여서 eIndexToDirect인 경우 추가 분기처리
	if (uv->GetReferenceMode() == FbxGeometryElement::eDirect)
		UVIndex = _index;
	else if (uv->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
	{
		UVIndex = _UVIndex;
	}
	else
		UVIndex = uv->GetIndexArray().GetAt(_index);

	FbxLayerElement::EReferenceMode mode = uv->GetReferenceMode();

	FbxVector2 vecUV = uv->GetDirectArray().GetAt(UVIndex);

	_container->vecUV[_index].x = (float)vecUV.mData[0];
	_container->vecUV[_index].y = 1.f - (float)vecUV.mData[1]; // fbx uv 좌표계는 좌하단이 0,0
}


Vec4 FBXLoader::GetMaterialData(FbxSurfaceMaterial* _surface, const char* _materialName, const char* _materialFactorName)
{
	FbxDouble3  material;
	FbxDouble	factor = 0.;

	FbxProperty materialProperty = _surface->FindProperty(_materialName);
	FbxProperty materialFactorProperty = _surface->FindProperty(_materialFactorName);

	if (materialProperty.IsValid() && materialFactorProperty.IsValid())
	{
		material = materialProperty.Get<FbxDouble3>();
		factor = materialFactorProperty.Get<FbxDouble>();
	}

	Vec4 returnValue = Vec4((float)material.mData[0] * (float)factor, (float)material.mData[1] * (float)factor, (float)material.mData[2] * (float)factor, (float)factor);
	return returnValue;
}

wstring FBXLoader::GetMaterialTextureName(FbxSurfaceMaterial* _surface, const char* _materialProperty)
{
	string name;

	FbxProperty TextureProperty = _surface->FindProperty(_materialProperty);
	if (TextureProperty.IsValid())
	{
		UINT count = TextureProperty.GetSrcObjectCount();

		if (1 <= count)
		{
			FbxFileTexture* fbxTex = TextureProperty.GetSrcObject<FbxFileTexture>(0);
			if (NULL != fbxTex)
				name = fbxTex->GetFileName();
		}
	}

	return wstring(name.begin(), name.end());
}

void FBXLoader::LoadTexture()
{
	path path_content = CPathManager::GetInst()->GetContentPath();

	path path_fbx_texture = path_content.wstring() + L"texture\\FBXTexture\\";
	if (false == exists(path_fbx_texture))
	{
		create_directory(path_fbx_texture);
	}

	path path_origin;
	path path_filename;
	path path_dest;

	// 노말 맵이 없으면 블렌더를 사용하여 노말맵을 만들어서 사용
	for (UINT i = 0; i < m_vecContainer.size(); ++i)
	{
		for (UINT j = 0; j < m_vecContainer[i].vecMaterial.size(); ++j)
		{
			vector<path> vectorPath;
			vectorPath.push_back(m_vecContainer[i].vecMaterial[j].diff.c_str());
			vectorPath.push_back(m_vecContainer[i].vecMaterial[j].normal.c_str());
			vectorPath.push_back(m_vecContainer[i].vecMaterial[j].spec.c_str());

			if (vectorPath[0] == L"" && vectorPath[1] != L"")
			{
				wstring texturePath = vectorPath[1].wstring();

				size_t index = texturePath.find(L".");
				wstring name;

				for (size_t i = 0; i < texturePath.size(); ++i)
				{
					if (i != index - 1)
						name += texturePath[i];
				}

				vectorPath[0] = name;
			}

			for (size_t k = 0; k < vectorPath.size(); ++k)
			{
				if (vectorPath[k] == "")
					continue;

				path_origin = vectorPath[k];
				path_filename = vectorPath[k].filename();
				path_dest = path_fbx_texture.wstring() + path_filename.wstring();

				bool esit = exists(path_origin);

				//실제 경로에 텍스쳐가 있는지 확인
				if (L"" == path_filename || false == exists(path_origin))
				{
					continue;
				}

				if (false == exists(path_dest))
				{
					copy(path_origin, path_dest);
				}

				path_dest = CPathManager::GetInst()->GetRelativePath(path_dest);
				CResourceManager::GetInst()->Load<CTexture>(path_dest, path_dest);

				switch (k)
				{
				case 0: m_vecContainer[i].vecMaterial[j].diff = path_dest; break;
				case 1: m_vecContainer[i].vecMaterial[j].normal = path_dest; break;
				case 2: m_vecContainer[i].vecMaterial[j].spec = path_dest; break;
				}
			}
		}
		//현재 경로
		//Monster.fbx 경우 Monster.fbx 폴더가 생성이 되고, 해당 텍스쳐 폴더가 texture\\FBXTexture로 이동을 하는데,
		//현재 item의 경우 텍스쳐 폴더가 나오지 않아서 \\content 를 이동 시키게 되어 폴더가 다 사라지는 겁니다.
		// path_origin 경로에 content 폴더의 경로만 들어가는 경우가 있어 bin 폴더의 삭제를 막기 위함
		path origin = path_origin;
		origin += L"\\";
		if (origin == CPathManager::GetInst()->GetContentPath())
			continue;

		path_origin = path_origin.parent_path();
		path str = path_origin;
		str += L"\\";

		if(str != CPathManager::GetInst()->GetContentPath())
			remove_all(path_origin);
	}
}

void FBXLoader::CreateMaterial()
{
	wstring key;
	wstring filePath;

	for (UINT i = 0; i < m_vecContainer.size(); ++i)
	{
		for (UINT j = 0; j < m_vecContainer[i].vecMaterial.size(); ++j)
		{
			CMaterial* material = new CMaterial;

			// Material 이름짓기
			key = m_vecContainer[i].vecMaterial[j].materialName;
			if (key.empty())
				key = path(m_vecContainer[i].vecMaterial[j].diff).stem();

			filePath = L"material\\";
			filePath += key;

			// 상대경로가 곧 이름(확장자명은 제외)
			m_vecContainer[i].vecMaterial[j].materialName = key;
			material->SetKey(key);
			material->SetRelativePath(filePath + L".mtrl");

			CGraphicsShader* shader = nullptr;

			//shader = CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"Std3DDeferredShader").Get();
			if (!m_bHasAnim)
			{
				shader = CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"Std3DDeferredShader_CULL_FRONT").Get();
			}
			else
			{
				shader = CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"Std3DDeferredShader").Get();
			}

			material->SetShader(shader);

			wstring texKey = m_vecContainer[i].vecMaterial[j].diff;
			Ptr<CTexture> tex = CResourceManager::GetInst()->FindRes<CTexture>(texKey);

			if (NULL != tex)
				material->SetData(SHADER_PARAM::TEXTURE_0, tex.Get());
			else
			{
				Vec4 diffColor = m_vecContainer[i].vecMaterial[j].material.diffuse;
				if (Vec4() != diffColor)
				{
					material->SetData(SHADER_PARAM::VEC4_0, &diffColor);
				}
			}

			texKey = m_vecContainer[i].vecMaterial[j].normal;
			tex = CResourceManager::GetInst()->FindRes<CTexture>(texKey);
			if (NULL != tex)
				material->SetData(SHADER_PARAM::TEXTURE_1, tex.Get());

			texKey = m_vecContainer[i].vecMaterial[j].spec;
			tex = CResourceManager::GetInst()->FindRes<CTexture>(texKey);
			if (NULL != tex)
				material->SetData(SHADER_PARAM::TEXTURE_2, tex.Get());

			material->SetMaterialCoefficient(m_vecContainer[i].vecMaterial[j].material.diffuse, m_vecContainer[i].vecMaterial[j].material.specular,
				m_vecContainer[i].vecMaterial[j].material.ambient, m_vecContainer[i].vecMaterial[j].material.emissive);

			// 중복되는 메테리얼인 경우 해당 메테리얼을 삭제 해준다.
			if (nullptr == CResourceManager::GetInst()->FindRes<CMaterial>(material->GetKey()))
			{
				CResourceManager::GetInst()->AddRes<CMaterial>(material->GetKey(), material);
			}
			else
				delete material;
		}
	}
}

void FBXLoader::LoadSkeleton(FbxNode* _node)
{
	LoadSkeleton_Re(_node, 0, 0, -1);
}

void FBXLoader::LoadSkeleton_Re(FbxNode* _node, int _depth, int _index, int _parentIndex)
{
	FbxNodeAttribute* attribute = _node->GetNodeAttribute();

	if (attribute && attribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		tBone* bone = new tBone;

		string boneName = _node->GetName();

		bone->boneName = wstring(boneName.begin(), boneName.end());
		bone->depth = _depth++;
		bone->parentIndex = _parentIndex;

		m_vecBone.push_back(bone);
	}

	int childCount = _node->GetChildCount();
	for (int i = 0; i < childCount; ++i)
	{
		LoadSkeleton_Re(_node->GetChild(i), _depth, (int)m_vecBone.size(), _index);
	}
}

void FBXLoader::LoadAnimationClip()
{
	int animCount = m_arrAnimName.GetCount();

	for (int i = 0; i < animCount; ++i)
	{
		FbxAnimStack* animStack = m_scene->FindMember<FbxAnimStack>(m_arrAnimName[i]->Buffer());

		if (!animStack)
			continue;

		tAnimClip* animClip = new tAnimClip;

		string clipName = animStack->GetName();
		animClip->name = wstring(clipName.begin(), clipName.end());

		FbxTakeInfo* takeInfo = m_scene->GetTakeInfo(animStack->GetName());
		animClip->startTime = takeInfo->mLocalTimeSpan.GetStart();
		animClip->endTime = takeInfo->mLocalTimeSpan.GetStop();

		animClip->mode = m_scene->GetGlobalSettings().GetTimeMode();
		animClip->timeLength = animClip->endTime.GetFrameCount(animClip->mode) - animClip->startTime.GetFrameCount(animClip->mode);

		m_vecAnimClip.push_back(animClip);
	}
}

void FBXLoader::Triangulate(FbxNode* _node)
{
	FbxNodeAttribute* attribute = _node->GetNodeAttribute();

	if (attribute && (attribute->GetAttributeType() == FbxNodeAttribute::eMesh ||
		attribute->GetAttributeType() == FbxNodeAttribute::eNurbs || attribute->GetAttributeType() == FbxNodeAttribute::eNurbsSurface))
	{
		FbxGeometryConverter converter(m_manager);
		converter.Triangulate(attribute, true);
	}

	int childCount = _node->GetChildCount();

	for (int i = 0; i < childCount; ++i)
	{
		Triangulate(_node->GetChild(i));
	}
}

void FBXLoader::LoadAnimationData(FbxMesh* _mesh, tContainer* _container)
{
	// Animation Data 로드할 필요가 없음
	int skinCount = _mesh->GetDeformerCount(FbxDeformer::eSkin);
	if (skinCount <= 0 || m_vecAnimClip.empty())
		return;

	_container->bAnimation = true;

	m_vecAllKeyFrame.resize(m_vecBone.size());

	// Skin 개수만큼 반복을하며 읽는다.	
	for (int i = 0; i < skinCount; ++i)
	{
		FbxSkin* skin = (FbxSkin*)_mesh->GetDeformer(i, FbxDeformer::eSkin);

		if (skin)
		{
			FbxSkin::EType type = skin->GetSkinningType();
			if (FbxSkin::eRigid == type || FbxSkin::eLinear)
			{
				// Cluster 를 얻어온다
				// Cluster == Joint == 관절
				int clusterCount = skin->GetClusterCount();

				for (int j = 0; j < clusterCount; ++j)
				{
					FbxCluster* cluster = skin->GetCluster(j);

					if (!cluster->GetLink())
						continue;

					// 현재 본 인덱스를 얻어온다.
					int boneIndex = FindBoneIndex(cluster->GetLink()->GetName());
					if (-1 == boneIndex)
						assert(NULL);

					FbxAMatrix nodeTransformMatrix = GetTransform(_mesh->GetNode());

					// Weights And Indices 정보를 읽는다.
					LoadWeightsAndIndices(cluster, boneIndex, _container);

					// Bone 의 OffSet 행렬 구한다.
					LoadOffsetMatrix(cluster, nodeTransformMatrix, boneIndex, _container);

					// Bone KeyFrame 별 행렬을 구한다.
					LoadKeyframeTransform(_mesh->GetNode(), cluster, nodeTransformMatrix, boneIndex, _container);
				}
			}
		}
	}

	CheckWeightAndIndices(_mesh, _container);
}

void FBXLoader::LoadWeightsAndIndices(FbxCluster* _cluster, int _boneIndex, tContainer* _container)
{
	int indicesCount = _cluster->GetControlPointIndicesCount();

	for (int i = 0; i < indicesCount; ++i)
	{
		tWeightsAndIndices weightsAndIndices = {};

		// 각 정점에게 본 인덱스 정보와, 가중치 값을 알린다.
		weightsAndIndices.boneIndex = _boneIndex;
		weightsAndIndices.weight = _cluster->GetControlPointWeights()[i];

		int vertexIndex = _cluster->GetControlPointIndices()[i];

		_container->vecWeightIndices[vertexIndex].push_back(weightsAndIndices);
	}
}

void FBXLoader::LoadOffsetMatrix(FbxCluster* _cluster, const FbxAMatrix& _nodeTransformMatrix, int _boneIndex, tContainer* _container)
{
	FbxAMatrix clusterTransformMatrix;
	FbxAMatrix clusterLinkTransformMatrix;

	_cluster->GetTransformMatrix(clusterTransformMatrix);
	_cluster->GetTransformLinkMatrix(clusterLinkTransformMatrix);

	// Reflect Matrix
	FbxVector4 V0 = { 1, 0, 0, 0 };
	FbxVector4 V1 = { 0, 0, 1, 0 };
	FbxVector4 V2 = { 0, 1, 0, 0 };
	FbxVector4 V3 = { 0, 0, 0, 1 };

	FbxAMatrix reflectMatrix;
	reflectMatrix[0] = V0;
	reflectMatrix[1] = V1;
	reflectMatrix[2] = V2;
	reflectMatrix[3] = V3;

	FbxAMatrix offsetMatrix;
	offsetMatrix = clusterLinkTransformMatrix.Inverse() * clusterTransformMatrix * _nodeTransformMatrix;
	offsetMatrix = reflectMatrix * offsetMatrix * reflectMatrix;

	m_vecBone[_boneIndex]->offsetMatrix = offsetMatrix;
}

void FBXLoader::LoadKeyframeTransform(FbxNode* _node, FbxCluster* _cluster, const FbxAMatrix& _nodeTransformMatrix, int _boneIndex, tContainer* _container)
{
	if (m_vecAnimClip.empty())
		return;

	FbxVector4	vector1 = { 1, 0, 0, 0 };
	FbxVector4	vector2 = { 0, 0, 1, 0 };
	FbxVector4	vector3 = { 0, 1, 0, 0 };
	FbxVector4	vector4 = { 0, 0, 0, 1 };
	FbxAMatrix	reflectMatrix;
	reflectMatrix.mData[0] = vector1;
	reflectMatrix.mData[1] = vector2;
	reflectMatrix.mData[2] = vector3;
	reflectMatrix.mData[3] = vector4;

	m_vecBone[_boneIndex]->boneMatrix = _nodeTransformMatrix;

	if (m_vecAnimFrame.size() == 0 || m_vecAnimClip.size() != m_vecAnimFrame.size())
	{
		m_vecAnimFrame.resize(m_vecAnimClip.size());
	}

	m_vecAllKeyFrame[_boneIndex].resize(m_vecAnimClip.size());

	FbxTime::EMode timeMode = m_scene->GetGlobalSettings().GetTimeMode();

	//각 관절 별로 애니메이션 클립에 따른 위치 정보를 저장
	for (size_t j = 0; j < m_vecAnimClip.size(); ++j)
	{
		FbxLongLong startFrame = m_vecAnimClip[j]->startTime.GetFrameCount(timeMode);
		FbxLongLong endFrame = m_vecAnimClip[j]->endTime.GetFrameCount(timeMode);

		FbxAnimEvaluator* animEvaluator = m_scene->GetAnimationEvaluator();
		FbxAnimStack* animStack = m_scene->FindMember<FbxAnimStack>(WStringToString(m_vecAnimClip[j]->name).c_str());
		m_scene->SetCurrentAnimationStack(animStack);
		bool bFrame = false;

		FbxAMatrix prveTransformMatrix;
		vector<tKeyFrame> vecKeyFrame;
		for (FbxLongLong i = startFrame; i < endFrame; ++i)
		{
			tKeyFrame frame = {};
			FbxTime   time = 0;

			time.SetFrame(i, timeMode);

			FbxAMatrix fromNodeMatrix = _node->EvaluateGlobalTransform(time) * _nodeTransformMatrix;
			FbxAMatrix curTransformMatrix = fromNodeMatrix.Inverse() * _cluster->GetLink()->EvaluateGlobalTransform(time);
			curTransformMatrix = reflectMatrix * curTransformMatrix * reflectMatrix;
			
			//혹시 모를 상황을 대비해 현재 프레임과 다음 프레임까지 비교하여 같은 경우 프레임 카운트를 넣어준다.
			if (!bFrame && curTransformMatrix == prveTransformMatrix)
			{
				if (m_vecAnimFrame[j] < i)
				{
					FbxTime   nextTime = 0;
					nextTime.SetFrame(i + 1, timeMode);

					fromNodeMatrix = _node->EvaluateGlobalTransform(nextTime) * _nodeTransformMatrix;
					FbxAMatrix nextTransformMatrix = fromNodeMatrix.Inverse() * _cluster->GetLink()->EvaluateGlobalTransform(nextTime);
					nextTransformMatrix = reflectMatrix * nextTransformMatrix * reflectMatrix;

					if (curTransformMatrix == nextTransformMatrix)
					{
						bFrame = true;
						m_vecAnimFrame[(int)j] = (int) i - 1;
					}

				}
			}

			if (!bFrame && i == (endFrame - 1) && curTransformMatrix != prveTransformMatrix && m_vecAnimFrame[j] < endFrame)
			{
				m_vecAnimFrame[j] = (int)endFrame;
			}


			frame.time = time.GetSecondDouble();
			frame.transformMatrix = curTransformMatrix;

			//원래 이전에는 bone 벡터의 애니메이션 정보를 바로 넣었는데, 나중에 넣어주게 변경
			//m_vecBone[_boneIndex]->vecKeyFrame.push_back(frame);
			vecKeyFrame.push_back(frame);
			prveTransformMatrix = curTransformMatrix;
		}
		m_vecAllKeyFrame[_boneIndex][j] = vecKeyFrame;
	}
}

int FBXLoader::FindBoneIndex(string _boneName)
{
	wstring boneName = wstring(_boneName.begin(), _boneName.end());

	for (UINT i = 0; i < m_vecBone.size(); ++i)
	{
		if (m_vecBone[i]->boneName == boneName)
			return i;
	}

	return -1;
}

FbxAMatrix FBXLoader::GetTransform(FbxNode* _node)
{
	const FbxVector4 translation = _node->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 rotation = _node->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 scale = _node->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(translation, rotation, scale);
}

void FBXLoader::CheckWeightAndIndices(FbxMesh* _mesh, tContainer* _container)
{
	vector<vector<tWeightsAndIndices>>::iterator iter = _container->vecWeightIndices.begin();

	int vertexIndex = 0;
	for (iter; iter != _container->vecWeightIndices.end(); ++iter, ++vertexIndex)
	{
		if ((*iter).size() > 1)
		{
			// 가중치 값 순으로 내림차순 정렬
			sort
			(
				(*iter).begin(), (*iter).end(),
				[](const tWeightsAndIndices& left, const tWeightsAndIndices& right)
			{
				return left.weight > right.weight;
			}
			);

			double weight = 0.f;
			for (UINT i = 0; i < (*iter).size(); ++i)
			{
				weight += (*iter)[i].weight;
			}

			// 가중치의 합이 1이 넘어가면 처음부분에 더해준다.
			double revision = 0.f;
			if (weight > 1.0)
			{
				revision = 1.0 - weight;
				(*iter)[0].weight += revision;
			}

			if ((*iter).size() >= 4)
			{
				(*iter).erase((*iter).begin() + 4, (*iter).end());
			}
		}

		// 정점 정보로 변환, 
		float weights[4] = {};
		float indices[4] = {};

		for (UINT i = 0; i < (*iter).size(); ++i)
		{
			weights[i] = (float)(*iter)[i].weight;
			indices[i] = (float)(*iter)[i].boneIndex;
		}

		memcpy(&_container->vecWeights[vertexIndex], weights, sizeof(Vec4));
		memcpy(&_container->vecIndices[vertexIndex], indices, sizeof(Vec4));
	}
}

FbxAMatrix CalculateGlobalTransform(FbxNode* pNode)
{
	FbxAMatrix lTranlationM, lScalingM, lScalingPivotM, lScalingOffsetM, lRotationOffsetM, lRotationPivotM,
		lPreRotationM, lRotationM, lPostRotationM, lTransform;

	FbxAMatrix lParentGX, lGlobalT, lGlobalRS;

	if (!pNode)
	{
		lTransform.SetIdentity();
		return lTransform;
	}

	// Construct translation matrix
	FbxVector4 lTranslation = pNode->LclTranslation.Get();
	lTranlationM.SetT(lTranslation);

	// Construct rotation matrices
	FbxVector4 lRotation = pNode->LclRotation.Get();
	FbxVector4 lPreRotation = pNode->PreRotation.Get();
	FbxVector4 lPostRotation = pNode->PostRotation.Get();
	lRotationM.SetR(lRotation);
	lPreRotationM.SetR(lPreRotation);
	lPostRotationM.SetR(lPostRotation);

	// Construct scaling matrix
	FbxVector4 lScaling = pNode->LclScaling.Get();
	lScalingM.SetS(lScaling);

	// Construct offset and pivot matrices
	FbxVector4 lScalingOffset = pNode->ScalingOffset.Get();
	FbxVector4 lScalingPivot = pNode->ScalingPivot.Get();
	FbxVector4 lRotationOffset = pNode->RotationOffset.Get();
	FbxVector4 lRotationPivot = pNode->RotationPivot.Get();
	lScalingOffsetM.SetT(lScalingOffset);
	lScalingPivotM.SetT(lScalingPivot);
	lRotationOffsetM.SetT(lRotationOffset);
	lRotationPivotM.SetT(lRotationPivot);

	// Calculate the global transform matrix of the parent node
	FbxNode* lParentNode = pNode->GetParent();
	if (lParentNode)
	{
		lParentGX = CalculateGlobalTransform(lParentNode);
	}
	else
	{
		lParentGX.SetIdentity();
	}

	//Construct Global Rotation
	FbxAMatrix lLRM, lParentGRM;
	FbxVector4 lParentGR = lParentGX.GetR();
	lParentGRM.SetR(lParentGR);
	lLRM = lPreRotationM * lRotationM * lPostRotationM;

	//Construct Global Shear*Scaling
	//FBX SDK does not support shear, to patch this, we use:
	//Shear*Scaling = RotationMatrix.Inverse * TranslationMatrix.Inverse * WholeTranformMatrix
	FbxAMatrix lLSM, lParentGSM, lParentGRSM, lParentTM;
	FbxVector4 lParentGT = lParentGX.GetT();
	lParentTM.SetT(lParentGT);
	lParentGRSM = lParentTM.Inverse() * lParentGX;
	lParentGSM = lParentGRM.Inverse() * lParentGRSM;
	lLSM = lScalingM;

	//Do not consider translation now
	FbxTransform::EInheritType lInheritType = pNode->InheritType.Get();
	if (lInheritType == FbxTransform::eInheritRrSs)
	{
		lGlobalRS = lParentGRM * lLRM * lParentGSM * lLSM;
	}
	else if (lInheritType == FbxTransform::eInheritRSrs)
	{
		lGlobalRS = lParentGRM * lParentGSM * lLRM * lLSM;
	}
	else if (lInheritType == FbxTransform::eInheritRrs)
	{
		FbxAMatrix lParentLSM;
		FbxVector4 lParentLS = lParentNode->LclScaling.Get();
		lParentLSM.SetS(lParentLS);

		FbxAMatrix lParentGSM_noLocal = lParentGSM * lParentLSM.Inverse();
		lGlobalRS = lParentGRM * lLRM * lParentGSM_noLocal * lLSM;
	}
	else
	{
		FBXSDK_printf("error, unknown inherit type! \n");
	}

	// Construct translation matrix
	// Calculate the local transform matrix
	lTransform = lTranlationM * lRotationOffsetM * lRotationPivotM * lPreRotationM * lRotationM * lPostRotationM * lRotationPivotM.Inverse()
		* lScalingOffsetM * lScalingPivotM * lScalingM * lScalingPivotM.Inverse();
	FbxVector4 lLocalTWithAllPivotAndOffsetInfo = lTransform.GetT();
	// Calculate global translation vector according to: 
	// GlobalTranslation = ParentGlobalTransform * LocalTranslationWithPivotAndOffsetInfo
	FbxVector4 lGlobalTranslation = lParentGX.MultT(lLocalTWithAllPivotAndOffsetInfo);
	lGlobalT.SetT(lGlobalTranslation);

	//Construct the whole global transform
	lTransform = lGlobalT * lGlobalRS;

	return lTransform;
}