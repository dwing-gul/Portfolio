#include "stdafx.h"
#include "HPBar.h"
#include "Texture.h"
#include "Player.h"
#include "Monster.h"

HPBar::HPBar() :
	m_target(nullptr)
{
}

HPBar::~HPBar()
{
}

void HPBar::update()
{
}

void HPBar::render(HDC _dc)
{
	if (nullptr != GetTexture())
	{
		Texture* tex = GetTexture();
		UINT width = tex->GetWidth();
		UINT height = tex->GetHeight();
		float length = float(width) * 8.37f;
		Vector2 pos = GetFinalPos();

		if (m_target->GetType() == GROUP_TYPE::PLAYER)
		{
			float ratio = ((Player*)m_target)->GetHPRatio();

			BLENDFUNCTION blend = {};
			blend.AlphaFormat = AC_SRC_ALPHA;
			blend.BlendFlags = 0;
			blend.BlendOp = AC_SRC_OVER;
			blend.SourceConstantAlpha = 255;

			AlphaBlend(_dc, (int)pos.x, (int)pos.y, int(length * ratio), height,
				tex->GetTextureDC(), 0, 0, width, height, blend);
		}
		else if (m_target->GetType() == GROUP_TYPE::MONSTER)
		{
			float ratio = ((Monster*)m_target)->GetHPRatio();

			BLENDFUNCTION blend = {};
			blend.AlphaFormat = AC_SRC_ALPHA;
			blend.BlendFlags = 0;
			blend.BlendOp = AC_SRC_OVER;
			blend.SourceConstantAlpha = 255;

			AlphaBlend(_dc, (int)pos.x, (int)pos.y, int(length * ratio), height,
				tex->GetTextureDC(), 0, 0, width, height, blend);
		}
		else if(m_target->GetType() == GROUP_TYPE::BOSS_BODY)
		{
			float ratio = ((Monster*)m_target)->GetHPRatio();
			length = float(width) * 29.5f;
			float yLength = float(height) * 1.85f;

			BLENDFUNCTION blend = {};
			blend.AlphaFormat = AC_SRC_ALPHA;
			blend.BlendFlags = 0;
			blend.BlendOp = AC_SRC_OVER;
			blend.SourceConstantAlpha = 255;

			AlphaBlend(_dc, (int)pos.x, (int)pos.y, int(length * ratio), int(yLength),
				tex->GetTextureDC(), 0, 0, int(float(width) - 1.0005f), height, blend);
		}
	}
}
