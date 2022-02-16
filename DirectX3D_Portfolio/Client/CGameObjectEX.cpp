#include "pch.h"
#include "CGameObjectEX.h"
#include <Engine/CComponent.h>

CGameObjectEX::CGameObjectEX()
{
}

CGameObjectEX::~CGameObjectEX()
{
}

void CGameObjectEX::finalUpdate()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		CComponent* component = GetComponent((COMPONENT_TYPE)i);
		if (nullptr == component)
			continue;

		component->finalUpdate();
	}

	const vector<CGameObject*>& vectorChild = GetChilds();
	for (size_t i = 0; i < vectorChild.size(); ++i)
	{
		vectorChild[i]->finalUpdate_ex();
	}
}
