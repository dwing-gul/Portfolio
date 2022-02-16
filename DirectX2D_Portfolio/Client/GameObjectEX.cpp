#include "pch.h"
#include "GameObjectEX.h"
#include <Engine/Component.h>

GameObjectEX::GameObjectEX()
{
}

GameObjectEX::~GameObjectEX()
{
}

void GameObjectEX::finalUpdate()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		Component* component = GetComponent((COMPONENT_TYPE)i);
		if (nullptr == component)
		{
			continue;
		}

		component->finalUpdate();
	}

	const vector<GameObject*>& vectorChild = GetChilds();
	for (size_t i = 0; i < vectorChild.size(); ++i)
	{
		vectorChild[i]->finalUpdate();
	}
}