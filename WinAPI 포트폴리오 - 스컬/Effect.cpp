#include "stdafx.h"
#include "Effect.h"
#include "Animator.h"
#include "Animation.h"

Effect::Effect() :
	m_magnification(Vector2(1.5f, 1.5f))
{
}

Effect::~Effect()
{
}

void Effect::update()
{
	if (!IsDead() && nullptr != GetAnimator())
	{
		if (GetAnimator()->GetCurAnim()->IsFinish())
		{
			DeleteObject(this);
		}
	}
}

void Effect::render(HDC _dc)
{
	if (nullptr != GetAnimator())
	{
		GetAnimator()->EffectRender(_dc);
	}
}
