#include "pch.h"
#include "CAnimator3DGUI.h"
#include <Engine\CAnimator3D.h>


CAnimator3DGUI::CAnimator3DGUI() :
	CComponentGUI(COMPONENT_TYPE::ANIMATOR3D),
	m_bRepeat(false),
	m_repeatCount(0)
{
}

CAnimator3DGUI::~CAnimator3DGUI()
{
}


void CAnimator3DGUI::update()
{
	if (Start())
	{
		if (GetTarget())
		{
			CAnimator3D* animator3D = GetTarget()->Animator3D();
			int frameSize = animator3D->GetCurAnimMaxFrame();
			int curClipIndex = animator3D->GetCurClipIndex();
			int clipSize = animator3D->GetClipSize();
			int curFrame = animator3D->GetCurFrameIndex();
			float ratio = animator3D->GetRatio();
			float curAnimSpeed = animator3D->GetAnimSpeed();
			string animName = WStringToString(animator3D->GetCurAnimName());

			
			ImGui::PushItemWidth(100);
			ImGui::Text(animName.c_str());
			
			ImGui::Checkbox("Repeat", &m_bRepeat);

			ImGui::PushItemWidth(100);
			ImGui::Text("CurClipIndex"); ImGui::SameLine(); 
			
			if (ImGui::InputInt("##CurClipIndex", &curClipIndex))
			{
				if (clipSize > curClipIndex && 0 <= curClipIndex)
					animator3D->SetCurClipIndex(curClipIndex, m_bRepeat, m_repeatCount);
			}

			ImGui::Text("RepeatCount"); ImGui::SameLine();
			if (ImGui::InputInt("##RepeatCount", &m_repeatCount))
			{
				if (m_repeatCount < 0)
					m_repeatCount = 0;
			}

			ImGui::PushItemWidth(100);

			string frameMax = "Total frame : " + std::to_string(frameSize);
			ImGui::Text(frameMax.c_str());
			ImGui::Text("curFrameID"); ImGui::SameLine();
			if (ImGui::InputInt("##curFrameID", &curFrame))
			{
				if (frameSize > curFrame && 0 <= curFrame)
					animator3D->SetFrameIndexMotion(curFrame);
			}
			ImGui::PushItemWidth(100);
			ImGui::Text("curRatio"); ImGui::SameLine();
			if (ImGui::InputFloat("##curRatio", &ratio))
			{
				if (1.f > ratio && 0.f <= ratio)
					animator3D->SetRatio(ratio);
			}

			ImGui::PushItemWidth(100);
			ImGui::Text("curAnimSpeed"); ImGui::SameLine();
			if (ImGui::InputFloat("##curAnimSpeed", &curAnimSpeed))
			{
				if (0.f <= curAnimSpeed)
					animator3D->SetAnimSpeed(curAnimSpeed);
			}
		}

		End();
	}
}

