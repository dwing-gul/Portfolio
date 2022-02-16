#include "stdafx.h"
#include "NPC.h"
#include "Animator.h"
#include "Texture.h"
#include "Scene.h"
#include "SceneManager.h"

NPC::NPC()
{
	CreateAnimator();
}

NPC::~NPC()
{
}

void NPC::update()
{
}

void NPC::render(HDC _dc)
{
	Vector2 pos = GetPos();
	Scene* curScene = SceneManager::GetInst()->GetCurScene();
	if (curScene->GetType() == SCENE_TYPE::TOOL)
	{
		if (nullptr == GetTexture())
		{
			Rectangle(_dc, (int)(pos.x)
				, (int)(pos.y)
				, (int)(pos.x + 10)
				, (int)(pos.y + 10));
		}
	}

	if (nullptr != GetAnimator())
		GetAnimator()->render(_dc);

	Obj::render(_dc);
}

