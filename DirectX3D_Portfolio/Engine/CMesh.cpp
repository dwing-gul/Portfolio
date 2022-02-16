#include "pch.h"
#include "CMesh.h"
#include "CDevice.h"
#include "CStructuredBuffer.h"
#include "CInstancingBuffer.h"

CMesh::CMesh() :
	CRes(RESOURCE_TYPE::MESH),
	m_vertexSys(nullptr),
	m_boneFrameData(nullptr),
	m_boneOffset(nullptr),
	m_vertexCount(0),
	m_vertexDesc{}
{
}

CMesh::CMesh(bool _bDefault) :
	CRes(RESOURCE_TYPE::MESH, _bDefault),
	m_vertexSys(nullptr),
	m_boneFrameData(nullptr),
	m_boneOffset(nullptr),
	m_vertexCount(0),
	m_vertexDesc{}
{
}

CMesh::~CMesh()
{
	Safe_Delete_Ptr(m_vertexSys);
	Safe_Delete_Ptr(m_boneFrameData);
	Safe_Delete_Ptr(m_boneOffset);

	for (int i = 0; i < (int)m_vecIndexInfo.size(); ++i)
	{
		if (nullptr != m_vecIndexInfo[i].indexSysMem)
		{
			delete m_vecIndexInfo[i].indexSysMem;
		}
	}
}

CMesh* CMesh::CreateFromContainer(FBXLoader& _loader, int index)
{
	const tContainer* container = &_loader.GetContainer(index);

	UINT vertexCount = (UINT)container->vecPos.size();

	D3D11_BUFFER_DESC vertexDesc = {};

	vertexDesc.ByteWidth = sizeof(tVertex) * vertexCount;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	if (D3D11_USAGE_DYNAMIC == vertexDesc.Usage)
		vertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sub = {};
	sub.pSysMem = malloc(vertexDesc.ByteWidth);
	tVertex* sys = (tVertex*)sub.pSysMem;
	for (UINT i = 0; i < vertexCount; ++i)
	{
		sys[i].pos = container->vecPos[i];
		sys[i].UV = container->vecUV[i];
		sys[i].normal = container->vecNormal[i];
		sys[i].tangent = container->vecTangent[i];
		sys[i].binormal = container->vecBinormal[i];
		sys[i].weights = container->vecWeights[i];
		sys[i].indices = container->vecIndices[i];
	}

	ComPtr<ID3D11Buffer> vertexBuffer = NULL;
	if (FAILED(DEVICE->CreateBuffer(&vertexDesc, &sub, vertexBuffer.GetAddressOf())))
	{
		return NULL;
	}

	CMesh* mesh = new CMesh;
	mesh->m_vertexBuffer = vertexBuffer;
	mesh->m_vertexDesc = vertexDesc;
	mesh->m_vertexSys = sys;
	mesh->m_vertexCount = vertexCount;

	// �ε��� ����
	UINT indexBufferCount = (UINT)container->vecIndex.size();
	D3D11_BUFFER_DESC indexDesc = {};

	for (UINT i = 0; i < indexBufferCount; ++i)
	{
		indexDesc.ByteWidth = (UINT)container->vecIndex[i].size() * sizeof(UINT); // Index Format �� R32_UINT �̱� ����
		indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexDesc.Usage = D3D11_USAGE_DEFAULT;
		if (D3D11_USAGE_DYNAMIC == indexDesc.Usage)
			indexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		void* sysMem = malloc(indexDesc.ByteWidth);
		memcpy(sysMem, container->vecIndex[i].data(), indexDesc.ByteWidth);
		sub.pSysMem = sysMem;

		ComPtr<ID3D11Buffer> indexBuffer = nullptr;
		if (FAILED(DEVICE->CreateBuffer(&indexDesc, &sub, indexBuffer.GetAddressOf())))
		{
			return NULL;
		}

		tIndexInfo info = {};
		info.indexBufferDesc = indexDesc;
		info.indexCount = (UINT)container->vecIndex[i].size();
		info.indexSysMem = sysMem;
		info.indexBuffer = indexBuffer;

		mesh->m_vecIndexInfo.push_back(info);
	}

	// Animation3D
	if (!container->bAnimation)
		return mesh;

	vector<tBone*>& vecBone = _loader.GetBones();
	UINT frameCount = 0;
	for (UINT i = 0; i < vecBone.size(); ++i)
	{
		tMTBone bone = {};
		bone.depth = vecBone[i]->depth;
		bone.parentIndex = vecBone[i]->parentIndex;
		bone.boneMatrix = GetMatrixFromFbxMatrix(vecBone[i]->boneMatrix);
		bone.offsetMatrix = GetMatrixFromFbxMatrix(vecBone[i]->offsetMatrix);
		bone.boneName = vecBone[i]->boneName;

		for (UINT j = 0; j < vecBone[i]->vecKeyFrame.size(); ++j)
		{
			tMTKeyFrame tKeyframe = {};
			tKeyframe.time = vecBone[i]->vecKeyFrame[j].time;
			tKeyframe.frame = j;
			tKeyframe.translate.x = (float)vecBone[i]->vecKeyFrame[j].transformMatrix.GetT().mData[0];
			tKeyframe.translate.y = (float)vecBone[i]->vecKeyFrame[j].transformMatrix.GetT().mData[1];
			tKeyframe.translate.z = (float)vecBone[i]->vecKeyFrame[j].transformMatrix.GetT().mData[2];

			tKeyframe.scale.x = (float)vecBone[i]->vecKeyFrame[j].transformMatrix.GetS().mData[0];
			tKeyframe.scale.y = (float)vecBone[i]->vecKeyFrame[j].transformMatrix.GetS().mData[1];
			tKeyframe.scale.z = (float)vecBone[i]->vecKeyFrame[j].transformMatrix.GetS().mData[2];

			tKeyframe.rot.x = (float)vecBone[i]->vecKeyFrame[j].transformMatrix.GetQ().mData[0];
			tKeyframe.rot.y = (float)vecBone[i]->vecKeyFrame[j].transformMatrix.GetQ().mData[1];
			tKeyframe.rot.z = (float)vecBone[i]->vecKeyFrame[j].transformMatrix.GetQ().mData[2];
			tKeyframe.rot.w = (float)vecBone[i]->vecKeyFrame[j].transformMatrix.GetQ().mData[3];

			bone.vecKeyFrame.push_back(tKeyframe);
		}

		frameCount = max(frameCount, (UINT)bone.vecKeyFrame.size());

		mesh->m_vecBones.push_back(bone);
	}

	vector<tAnimClip*>& vecAnimClip = _loader.GetAnimClip();

	for (UINT i = 0; i < vecAnimClip.size(); ++i)
	{
		tMTAnimClip tClip = {};

		tClip.animName = vecAnimClip[i]->name;
		tClip.startTime = vecAnimClip[i]->startTime.GetSecondDouble();
		tClip.endTime = vecAnimClip[i]->endTime.GetSecondDouble();
		tClip.timeLength = tClip.endTime - tClip.startTime;

		tClip.startFrame = (int)vecAnimClip[i]->startTime.GetFrameCount(vecAnimClip[i]->mode);
		tClip.endFrame = (int)vecAnimClip[i]->endTime.GetFrameCount(vecAnimClip[i]->mode);
		tClip.frameLength = tClip.endFrame - tClip.startFrame;
		tClip.mode = vecAnimClip[i]->mode;

		mesh->m_vecAnimClip.push_back(tClip);
	}

	// Animation �� �ִ� Mesh ��� BoneTexture �����α�
	if (mesh->IsAnimMesh())
	{
		// BoneOffet ���
		vector<Matrix> vecOffset;
		vector<tFrameTransform> vecFrameTransform;
		vecFrameTransform.resize((UINT)mesh->m_vecBones.size() * frameCount);

		for (size_t i = 0; i < mesh->m_vecBones.size(); ++i)
		{
			vecOffset.push_back(mesh->m_vecBones[i].offsetMatrix);

			for (size_t j = 0; j < mesh->m_vecBones[i].vecKeyFrame.size(); ++j)
			{
				vecFrameTransform[(UINT)mesh->m_vecBones.size() * j + i] = tFrameTransform{ Vec4(mesh->m_vecBones[i].vecKeyFrame[j].translate, 0.f), 
																							Vec4(mesh->m_vecBones[i].vecKeyFrame[j].scale, 0.f), 
																							mesh->m_vecBones[i].vecKeyFrame[j].rot };
			}
		}

		mesh->m_boneOffset = new CStructuredBuffer;
		mesh->m_boneOffset->Create(STRUCTURED_TYPE::READONLY, sizeof(Matrix), (UINT)vecOffset.size(), vecOffset.data());

		mesh->m_boneFrameData = new CStructuredBuffer;
		mesh->m_boneFrameData->Create(STRUCTURED_TYPE::READ_WRITE, sizeof(tFrameTransform), (UINT)vecOffset.size() * frameCount, vecFrameTransform.data());
	}

	return mesh;
}

void CMesh::Create(void* _vertexSys, UINT _vertexBufferSize, void* _indexSys, UINT _indexBufferSize)
{
	// GPU �޸𸮿� Vertex Buffer �����
	m_vertexDesc.ByteWidth = _vertexBufferSize;				// GPU �� ������ ���� �޸� ũ��
	m_vertexDesc.Usage = D3D11_USAGE_DEFAULT;				// ���� �Ұ���
	m_vertexDesc.CPUAccessFlags = 0;						// ���� �Ұ���
	m_vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// Vertex Buffer �뵵

	D3D11_SUBRESOURCE_DATA subDesc = {};
	subDesc.pSysMem = _vertexSys;							// �ʱ� ������

	HRESULT hr = DEVICE->CreateBuffer(&m_vertexDesc, &subDesc, m_vertexBuffer.GetAddressOf());

	// sysmem ����
	m_vertexSys = malloc(_vertexBufferSize);
	memcpy(m_vertexSys, _vertexSys, _vertexBufferSize);

	m_vertexCount = _vertexBufferSize / sizeof(tVertex);

	// GPU �޸𸮿� Index Buffer �����
	tIndexInfo info = {};
	info.indexCount = _indexBufferSize / sizeof(UINT);;
	info.indexBufferDesc.ByteWidth = sizeof(UINT) * info.indexCount;	// GPU �� ������ ���� �޸� ũ��
	info.indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;				// ���� �Ұ��� ����
	info.indexBufferDesc.CPUAccessFlags = 0;						// ���� �Ұ��� ����
	info.indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;		// Index Buffer �뵵
	info.indexSysMem = malloc(_indexBufferSize);
	memcpy(info.indexSysMem, _indexSys, _indexBufferSize);

	subDesc = {};
	subDesc.pSysMem = _indexSys;						// �ʱ� ������

	hr = DEVICE->CreateBuffer(&info.indexBufferDesc, &subDesc, info.indexBuffer.GetAddressOf());

	m_vecIndexInfo.push_back(info);
}

int CMesh::Load(const wstring& _filePath)
{
	FILE* file = nullptr;
	_wfopen_s(&file, _filePath.c_str(), L"rb");

	// Ű��, �����
	wstring key, relativePath;
	LoadWStringFromFile(key, file);
	LoadWStringFromFile(relativePath, file);

	SetKey(key);
	SetRelativePath(relativePath);

	// ����������
	UINT byteSize = 0;
	fread(&byteSize, sizeof(int), 1, file);

	m_vertexSys = (tVertex*)malloc(byteSize);
	fread(m_vertexSys, 1, byteSize, file);
	LoadFromFile(&m_vertexCount, file);

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = byteSize;
	desc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA subData = {};
	subData.pSysMem = m_vertexSys;

	if (FAILED(DEVICE->CreateBuffer(&desc, &subData, m_vertexBuffer.GetAddressOf())))
	{
		assert(nullptr);
	}

	// �ε��� ����
	UINT materialCount = 0;
	fread(&materialCount, sizeof(int), 1, file);

	for (UINT i = 0; i < materialCount; ++i)
	{
		tIndexInfo info = {};
		fread(&info, sizeof(tIndexInfo), 1, file);

		UINT byteWidth = info.indexCount * GetSizeOfFormat(DXGI_FORMAT_R32_UINT);

		void* pSysMem = malloc(byteWidth);
		info.indexSysMem = pSysMem;
		fread(info.indexSysMem, byteWidth, 1, file);

		subData.pSysMem = info.indexSysMem;

		if (FAILED(DEVICE->CreateBuffer(&info.indexBufferDesc, &subData, info.indexBuffer.GetAddressOf())))
		{
			assert(nullptr);
		}

		m_vecIndexInfo.push_back(info);
	}

	// �޽��� �߽� ���� �� �޽��� xyz ���� ���� ����
	LoadFromFile(&m_centerPos, file);
	LoadFromFile(&m_xyzLength, file);

	// Animation3D ���� �б�
	int count = 0;
	fread(&count, sizeof(int), 1, file);
	for (int i = 0; i < count; ++i)
	{
		tMTAnimClip clip = {};

		LoadWStringFromFile(clip.animName, file);
		fread(&clip.startTime, sizeof(double), 1, file);
		fread(&clip.endTime, sizeof(double), 1, file);
		fread(&clip.timeLength, sizeof(double), 1, file);
		fread(&clip.mode, sizeof(int), 1, file);
		fread(&clip.updateTime, sizeof(float), 1, file);
		fread(&clip.startFrame, sizeof(int), 1, file);
		fread(&clip.endFrame, sizeof(int), 1, file);
		fread(&clip.frameLength, sizeof(int), 1, file);

		m_vecAnimClip.push_back(clip);
	}

	count = 0;
	fread(&count, sizeof(int), 1, file);
	m_vecBones.resize(count);

	UINT _frameCount = 0;
	for (int i = 0; i < count; ++i)
	{
		LoadWStringFromFile(m_vecBones[i].boneName, file);
		fread(&m_vecBones[i].depth, sizeof(int), 1, file);
		fread(&m_vecBones[i].parentIndex, sizeof(int), 1, file);
		fread(&m_vecBones[i].boneMatrix, sizeof(Matrix), 1, file);
		fread(&m_vecBones[i].offsetMatrix, sizeof(Matrix), 1, file);

		UINT frameCount = 0;
		fread(&frameCount, sizeof(int), 1, file);
		m_vecBones[i].vecKeyFrame.resize(frameCount);
		_frameCount = max(_frameCount, frameCount);
		for (UINT j = 0; j < frameCount; ++j)
		{
			fread(&m_vecBones[i].vecKeyFrame[j], sizeof(tMTKeyFrame), 1, file);
		}
	}

	// Animation �� �ִ� Mesh ��� Bone StructuredBuffer �����
	if (m_vecAnimClip.size() > 0 && m_vecBones.size() > 0)
	{
		wstring boneName = GetName();

		// BoneOffet ���
		vector<Matrix> vecOffsetMatrix;
		vector<tFrameTransform> vecFrameTransform;
		vecFrameTransform.resize((UINT)m_vecBones.size() * _frameCount);

		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			vecOffsetMatrix.push_back(m_vecBones[i].offsetMatrix);

			for (size_t j = 0; j < m_vecBones[i].vecKeyFrame.size(); ++j)
			{
				vecFrameTransform[(UINT)m_vecBones.size() * j + i] = tFrameTransform{ Vec4(m_vecBones[i].vecKeyFrame[j].translate, 0.f), 
																	Vec4(m_vecBones[i].vecKeyFrame[j].scale, 0.f), Vec4(m_vecBones[i].vecKeyFrame[j].rot) };
			}
		}

		m_boneOffset = new CStructuredBuffer;
		m_boneOffset->Create(STRUCTURED_TYPE::READONLY, sizeof(Matrix), (UINT)vecOffsetMatrix.size(), vecOffsetMatrix.data());

		m_boneFrameData = new CStructuredBuffer;
		m_boneFrameData->Create(STRUCTURED_TYPE::READONLY, sizeof(tFrameTransform), (UINT)vecOffsetMatrix.size() * (UINT)_frameCount, vecFrameTransform.data());
	}

	fclose(file);

	return S_OK;
}

void CMesh::Save(const wstring& _relativePath)
{
	wstring fileName = CPathManager::GetInst()->GetContentPath();
	fileName += _relativePath;
	SetRelativePath(_relativePath);

	FILE* file = nullptr;
	errno_t err = _wfopen_s(&file, fileName.c_str(), L"wb");

	assert(file);

	// Ű��
	SaveWStringToFile(GetName(), file);

	// ��� ���
	SaveWStringToFile(GetRelativePath(), file);

	// ���� ������ ����				
	int byteSize = m_vertexDesc.ByteWidth;
	fwrite(&byteSize, sizeof(int), 1, file);
	fwrite(m_vertexSys, byteSize, 1, file);
	SaveToFile(&m_vertexCount, file);

	// �ε��� ����
	UINT materialCount = (UINT)m_vecIndexInfo.size();
	fwrite(&materialCount, sizeof(int), 1, file);

	UINT indexBufferSize = 0;
	for (UINT i = 0; i < materialCount; ++i)
	{
		fwrite(&m_vecIndexInfo[i], sizeof(tIndexInfo), 1, file);
		fwrite(m_vecIndexInfo[i].indexSysMem, m_vecIndexInfo[i].indexCount * GetSizeOfFormat(DXGI_FORMAT_R32_UINT), 1, file);
	}

	// �޽��� �߽� ���� �� �޽��� xyz ���� ���� ����
	SaveToFile(&m_centerPos, file);
	SaveToFile(&m_xyzLength, file);

	// Animation3D ���� 
	UINT count = (UINT)m_vecAnimClip.size();
	fwrite(&count, sizeof(int), 1, file);
	for (UINT i = 0; i < count; ++i)
	{
		SaveWStringToFile(m_vecAnimClip[i].animName, file);
		fwrite(&m_vecAnimClip[i].startTime, sizeof(double), 1, file);
		fwrite(&m_vecAnimClip[i].endTime, sizeof(double), 1, file);
		fwrite(&m_vecAnimClip[i].timeLength, sizeof(double), 1, file);
		fwrite(&m_vecAnimClip[i].mode, sizeof(int), 1, file);
		fwrite(&m_vecAnimClip[i].updateTime, sizeof(float), 1, file);
		fwrite(&m_vecAnimClip[i].startFrame, sizeof(int), 1, file);
		fwrite(&m_vecAnimClip[i].endFrame, sizeof(int), 1, file);
		fwrite(&m_vecAnimClip[i].frameLength, sizeof(int), 1, file);
	}

	count = (UINT)m_vecBones.size();
	fwrite(&count, sizeof(int), 1, file);

	for (UINT i = 0; i < count; ++i)
	{
		SaveWStringToFile(m_vecBones[i].boneName, file);
		fwrite(&m_vecBones[i].depth, sizeof(int), 1, file);
		fwrite(&m_vecBones[i].parentIndex, sizeof(int), 1, file);
		fwrite(&m_vecBones[i].boneMatrix, sizeof(Matrix), 1, file);
		fwrite(&m_vecBones[i].offsetMatrix, sizeof(Matrix), 1, file);

		int frameCount = (int)m_vecBones[i].vecKeyFrame.size();
		fwrite(&frameCount, sizeof(int), 1, file);

		for (int j = 0; j < m_vecBones[i].vecKeyFrame.size(); ++j)
		{
			fwrite(&m_vecBones[i].vecKeyFrame[j], sizeof(tMTKeyFrame), 1, file);
		}
	}

	fclose(file);
}

void CMesh::UpdateData(UINT _subset)
{
	UINT stride = sizeof(tVertex);
	UINT offset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	CONTEXT->IASetIndexBuffer(m_vecIndexInfo[_subset].indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void CMesh::UpdateData_Inst(UINT _subset)
{
	if (_subset >= m_vecIndexInfo.size())
		assert(nullptr);

	ID3D11Buffer*	arrBuffer[2] = { m_vertexBuffer.Get(), CInstancingBuffer::GetInst()->GetBuffer().Get() };
	UINT			stride[2] = { sizeof(tVertex), sizeof(tInstancingData) };
	UINT			offset[2] = { 0, 0 };

	CONTEXT->IASetVertexBuffers(0, 2, arrBuffer, stride, offset);
	CONTEXT->IASetIndexBuffer(m_vecIndexInfo[_subset].indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void CMesh::render(UINT _subset)
{
	CONTEXT->DrawIndexed(m_vecIndexInfo[_subset].indexCount, 0, 0);
}

void CMesh::render_particle(UINT _count)
{
	CONTEXT->DrawIndexedInstanced(m_vecIndexInfo[0].indexCount, _count, 0, 0, 0);
}

void CMesh::render_instancing(UINT _subset)
{
	UINT test = CInstancingBuffer::GetInst()->GetInstanceCount();
	CONTEXT->DrawIndexedInstanced(m_vecIndexInfo[_subset].indexCount, CInstancingBuffer::GetInst()->GetInstanceCount(), 0, 0, 0);
}

void CMesh::Reset(void* _vertexSys, UINT _vertexBufferSize, void* _indexSys, UINT _indexBufferSize)
{
	free(m_vertexSys);
	m_vertexBuffer = nullptr;

	for (size_t i = 0; i < m_vecIndexInfo.size(); ++i)
	{
		free(m_vecIndexInfo[i].indexSysMem);
		m_vecIndexInfo[i].indexBuffer = nullptr;
	}

	Create(_vertexSys, _vertexBufferSize, _indexSys, _indexBufferSize);
}
