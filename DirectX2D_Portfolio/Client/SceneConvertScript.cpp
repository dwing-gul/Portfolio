#include "pch.h"
#include "SceneConvertScript.h"
#include "SceneConverter.h"
#include "PartyManager.h"
#include "SceneConfig.h"
#include <Engine/Collider2D.h>

SceneConvertScript::SceneConvertScript() :
	Script(-2)
{
}

SceneConvertScript::~SceneConvertScript()
{
}

void SceneConvertScript::update()
{
	if (GetCollider2D()->IsObjectHovered() && IsMouseClicked())
	{
		const vector<Prefab*>& vectorPlayerParty = PartyManager::GetInst()->GetWaitingPlayer();

		if (vectorPlayerParty.empty())
			return;
		else
		{
			int count = 0;
			for (size_t i = 0; i < vectorPlayerParty.size(); ++i)
			{
				if (nullptr != vectorPlayerParty[i])
				{
					++count;
				}
			}

			if (0 == count)
				return;
		}


		const vector<SceneConfig*>& vectorSceneConfig = SceneConverter::GetInst()->GetSceneConfigurations();
		SceneConfig* curConfig = SceneConverter::GetInst()->GetCurSceneConfig();

		size_t index = 0;
		for (; index < vectorSceneConfig.size(); ++index)
		{
			if (curConfig == vectorSceneConfig[index])
			{
				++index;
				break;
			}
		}

		if(index < vectorSceneConfig.size())
			SceneConverter::GetInst()->MoveNextScene(vectorSceneConfig[index]);
	}
}