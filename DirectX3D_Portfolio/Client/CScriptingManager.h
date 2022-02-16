#pragma once

class CGameObject;

class CScriptingManager :
	public CSingleton<CScriptingManager>
{
	SINGLE(CScriptingManager);
private :
	float m_gameTime; // 진행시간(낮/밤)
	float m_totalTime; // 전체 진행시간
	bool m_bDayChange;

	CGameObject* m_DayFont;
	CGameObject* m_Day_N_Night_Font;
	CGameObject* m_dayIcon;
	CGameObject* m_light;
	CGameObject* m_levelFont;
	CGameObject* m_hpBar;
	CGameObject* m_spBar;
	CGameObject* m_expBar;
	CGameObject* m_hpBarFont;
	CGameObject* m_spBarFont;

	CGameObject* m_AttackPowerFont;
	CGameObject* m_DefenseFont;
	CGameObject* m_ExtraAttackFont;
	CGameObject* m_DMGReductionFont;
	CGameObject* m_SkillDMGIncFont;
	CGameObject* m_SkillDMGReductionFont;
	CGameObject* m_MoveSpeedFont;
	CGameObject* m_AttackSpeedFont;

public :
	void init();
	void progress();

	void PlayerCountFont();
	void DayNTimeFont();
	void PlayerLevelFont();
	void PlayerBarRatio();
	void TrackingBarRatio();
	void PlayerStatFont();

};

