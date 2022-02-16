#include "pch.h"
#include "ScriptManager.h"

#include "BattlePhaseScript.h"
#include "CameraScript.h"
#include "CrusaderScript.h"
#include "CultistBrawlerScript.h"
#include "CultistWitchScript.h"
#include "DoorScript.h"
#include "EffectScript.h"
#include "HighwaymanScript.h"
#include "HPBarScript.h"
#include "ItemManagerScript.h"
#include "ItemScript.h"
#include "LightBarScript.h"
#include "LightScript.h"
#include "MadmanScript.h"
#include "PartyIconScript.h"
#include "PewLargeScript.h"
#include "PewMediumScript.h"
#include "PewSmallScript.h"
#include "PlagueDoctorScript.h"
#include "SkeletonArbalistScript.h"
#include "SkeletonCommonScript.h"
#include "SkeletonCourtierScript.h"
#include "SkeletonDefenderScript.h"
#include "SkillButtonScript.h"
#include "SlotScript.h"
#include "StoreItemScript.h"
#include "TextUIScript.h"
#include "TownObjectScript.h"
#include "UIScript.h"
#include "VestalScript.h"
#include "WallScript.h"

void ScriptManager::GetScriptInfo(vector<wstring>& _vector)
{
	_vector.push_back(L"BattlePhaseScript");
	_vector.push_back(L"CameraScript");
	_vector.push_back(L"CrusaderScript");
	_vector.push_back(L"CultistBrawlerScript");
	_vector.push_back(L"CultistWitchScript");
	_vector.push_back(L"DoorScript");
	_vector.push_back(L"EffectScript");
	_vector.push_back(L"HighwaymanScript");
	_vector.push_back(L"HPBarScript");
	_vector.push_back(L"ItemManagerScript");
	_vector.push_back(L"ItemScript");
	_vector.push_back(L"LightBarScript");
	_vector.push_back(L"LightScript");
	_vector.push_back(L"MadmanScript");
	_vector.push_back(L"PartyIconScript");
	_vector.push_back(L"PewLargeScript");
	_vector.push_back(L"PewMediumScript");
	_vector.push_back(L"PewSmallScript");
	_vector.push_back(L"PlagueDoctorScript");
	_vector.push_back(L"SkeletonArbalistScript");
	_vector.push_back(L"SkeletonCommonScript");
	_vector.push_back(L"SkeletonCourtierScript");
	_vector.push_back(L"SkeletonDefenderScript");
	_vector.push_back(L"SkillButtonScript");
	_vector.push_back(L"SlotScript");
	_vector.push_back(L"StoreItemScript");
	_vector.push_back(L"TextUIScript");
	_vector.push_back(L"TownObjectScript");
	_vector.push_back(L"UIScript");
	_vector.push_back(L"VestalScript");
	_vector.push_back(L"WallScript");
}

Script * ScriptManager::GetScript(const wstring& _scriptName)
{
	if (L"BattlePhaseScript" == _scriptName)
		return new BattlePhaseScript;
	if (L"CameraScript" == _scriptName)
		return new CameraScript;
	if (L"CrusaderScript" == _scriptName)
		return new CrusaderScript;
	if (L"CultistBrawlerScript" == _scriptName)
		return new CultistBrawlerScript;
	if (L"CultistWitchScript" == _scriptName)
		return new CultistWitchScript;
	if (L"DoorScript" == _scriptName)
		return new DoorScript;
	if (L"EffectScript" == _scriptName)
		return new EffectScript;
	if (L"HighwaymanScript" == _scriptName)
		return new HighwaymanScript;
	if (L"HPBarScript" == _scriptName)
		return new HPBarScript;
	if (L"ItemManagerScript" == _scriptName)
		return new ItemManagerScript;
	if (L"ItemScript" == _scriptName)
		return new ItemScript;
	if (L"LightBarScript" == _scriptName)
		return new LightBarScript;
	if (L"LightScript" == _scriptName)
		return new LightScript;
	if (L"MadmanScript" == _scriptName)
		return new MadmanScript;
	if (L"PartyIconScript" == _scriptName)
		return new PartyIconScript;
	if (L"PewLargeScript" == _scriptName)
		return new PewLargeScript;
	if (L"PewMediumScript" == _scriptName)
		return new PewMediumScript;
	if (L"PewSmallScript" == _scriptName)
		return new PewSmallScript;
	if (L"PlagueDoctorScript" == _scriptName)
		return new PlagueDoctorScript;
	if (L"SkeletonArbalistScript" == _scriptName)
		return new SkeletonArbalistScript;
	if (L"SkeletonCommonScript" == _scriptName)
		return new SkeletonCommonScript;
	if (L"SkeletonCourtierScript" == _scriptName)
		return new SkeletonCourtierScript;
	if (L"SkeletonDefenderScript" == _scriptName)
		return new SkeletonDefenderScript;
	if (L"SkillButtonScript" == _scriptName)
		return new SkillButtonScript;
	if (L"SlotScript" == _scriptName)
		return new SlotScript;
	if (L"StoreItemScript" == _scriptName)
		return new StoreItemScript;
	if (L"TextUIScript" == _scriptName)
		return new TextUIScript;
	if (L"TownObjectScript" == _scriptName)
		return new TownObjectScript;
	if (L"UIScript" == _scriptName)
		return new UIScript;
	if (L"VestalScript" == _scriptName)
		return new VestalScript;
	if (L"WallScript" == _scriptName)
		return new WallScript;
	return nullptr;
}

Script * ScriptManager::GetScript(UINT _scriptType)
{
	switch (_scriptType)
	{
	case (UINT)SCRIPT_TYPE::BATTLEPHASESCRIPT:
		return new BattlePhaseScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERASCRIPT:
		return new CameraScript;
		break;
	case (UINT)SCRIPT_TYPE::CRUSADERSCRIPT:
		return new CrusaderScript;
		break;
	case (UINT)SCRIPT_TYPE::CULTISTBRAWLERSCRIPT:
		return new CultistBrawlerScript;
		break;
	case (UINT)SCRIPT_TYPE::CULTISTWITCHSCRIPT:
		return new CultistWitchScript;
		break;
	case (UINT)SCRIPT_TYPE::DOORSCRIPT:
		return new DoorScript;
		break;
	case (UINT)SCRIPT_TYPE::EFFECTSCRIPT:
		return new EffectScript;
		break;
	case (UINT)SCRIPT_TYPE::HIGHWAYMANSCRIPT:
		return new HighwaymanScript;
		break;
	case (UINT)SCRIPT_TYPE::HPBARSCRIPT:
		return new HPBarScript;
		break;
	case (UINT)SCRIPT_TYPE::ITEMMANAGERSCRIPT:
		return new ItemManagerScript;
		break;
	case (UINT)SCRIPT_TYPE::ITEMSCRIPT:
		return new ItemScript;
		break;
	case (UINT)SCRIPT_TYPE::LIGHTBARSCRIPT:
		return new LightBarScript;
		break;
	case (UINT)SCRIPT_TYPE::LIGHTSCRIPT:
		return new LightScript;
		break;
	case (UINT)SCRIPT_TYPE::MADMANSCRIPT:
		return new MadmanScript;
		break;
	case (UINT)SCRIPT_TYPE::PARTYICONSCRIPT:
		return new PartyIconScript;
		break;
	case (UINT)SCRIPT_TYPE::PEWLARGESCRIPT:
		return new PewLargeScript;
		break;
	case (UINT)SCRIPT_TYPE::PEWMEDIUMSCRIPT:
		return new PewMediumScript;
		break;
	case (UINT)SCRIPT_TYPE::PEWSMALLSCRIPT:
		return new PewSmallScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAGUEDOCTORSCRIPT:
		return new PlagueDoctorScript;
		break;
	case (UINT)SCRIPT_TYPE::SKELETONARBALISTSCRIPT:
		return new SkeletonArbalistScript;
		break;
	case (UINT)SCRIPT_TYPE::SKELETONCOMMONSCRIPT:
		return new SkeletonCommonScript;
		break;
	case (UINT)SCRIPT_TYPE::SKELETONCOURTIERSCRIPT:
		return new SkeletonCourtierScript;
		break;
	case (UINT)SCRIPT_TYPE::SKELETONDEFENDERSCRIPT:
		return new SkeletonDefenderScript;
		break;
	case (UINT)SCRIPT_TYPE::SKILLBUTTONSCRIPT:
		return new SkillButtonScript;
		break;
	case (UINT)SCRIPT_TYPE::SLOTSCRIPT:
		return new SlotScript;
		break;
	case (UINT)SCRIPT_TYPE::STOREITEMSCRIPT:
		return new StoreItemScript;
		break;
	case (UINT)SCRIPT_TYPE::TEXTUISCRIPT:
		return new TextUIScript;
		break;
	case (UINT)SCRIPT_TYPE::TOWNOBJECTSCRIPT:
		return new TownObjectScript;
		break;
	case (UINT)SCRIPT_TYPE::UISCRIPT:
		return new UIScript;
		break;
	case (UINT)SCRIPT_TYPE::VESTALSCRIPT:
		return new VestalScript;
		break;
	case (UINT)SCRIPT_TYPE::WALLSCRIPT:
		return new WallScript;
		break;
	}
	return nullptr;
}

const wchar_t * ScriptManager::GetScriptName(Script * _script)
{
	switch ((SCRIPT_TYPE)_script->GetScriptType())
	{
	case SCRIPT_TYPE::BATTLEPHASESCRIPT:
		return L"BattlePhaseScript";
		break;

	case SCRIPT_TYPE::CAMERASCRIPT:
		return L"CameraScript";
		break;

	case SCRIPT_TYPE::CRUSADERSCRIPT:
		return L"CrusaderScript";
		break;

	case SCRIPT_TYPE::CULTISTBRAWLERSCRIPT:
		return L"CultistBrawlerScript";
		break;

	case SCRIPT_TYPE::CULTISTWITCHSCRIPT:
		return L"CultistWitchScript";
		break;

	case SCRIPT_TYPE::DOORSCRIPT:
		return L"DoorScript";
		break;

	case SCRIPT_TYPE::EFFECTSCRIPT:
		return L"EffectScript";
		break;

	case SCRIPT_TYPE::HIGHWAYMANSCRIPT:
		return L"HighwaymanScript";
		break;

	case SCRIPT_TYPE::HPBARSCRIPT:
		return L"HPBarScript";
		break;

	case SCRIPT_TYPE::ITEMMANAGERSCRIPT:
		return L"ItemManagerScript";
		break;

	case SCRIPT_TYPE::ITEMSCRIPT:
		return L"ItemScript";
		break;

	case SCRIPT_TYPE::LIGHTBARSCRIPT:
		return L"LightBarScript";
		break;

	case SCRIPT_TYPE::LIGHTSCRIPT:
		return L"LightScript";
		break;

	case SCRIPT_TYPE::MADMANSCRIPT:
		return L"MadmanScript";
		break;

	case SCRIPT_TYPE::PARTYICONSCRIPT:
		return L"PartyIconScript";
		break;

	case SCRIPT_TYPE::PEWLARGESCRIPT:
		return L"PewLargeScript";
		break;

	case SCRIPT_TYPE::PEWMEDIUMSCRIPT:
		return L"PewMediumScript";
		break;

	case SCRIPT_TYPE::PEWSMALLSCRIPT:
		return L"PewSmallScript";
		break;

	case SCRIPT_TYPE::PLAGUEDOCTORSCRIPT:
		return L"PlagueDoctorScript";
		break;

	case SCRIPT_TYPE::SKELETONARBALISTSCRIPT:
		return L"SkeletonArbalistScript";
		break;

	case SCRIPT_TYPE::SKELETONCOMMONSCRIPT:
		return L"SkeletonCommonScript";
		break;

	case SCRIPT_TYPE::SKELETONCOURTIERSCRIPT:
		return L"SkeletonCourtierScript";
		break;

	case SCRIPT_TYPE::SKELETONDEFENDERSCRIPT:
		return L"SkeletonDefenderScript";
		break;

	case SCRIPT_TYPE::SKILLBUTTONSCRIPT:
		return L"SkillButtonScript";
		break;

	case SCRIPT_TYPE::SLOTSCRIPT:
		return L"SlotScript";
		break;

	case SCRIPT_TYPE::STOREITEMSCRIPT:
		return L"StoreItemScript";
		break;

	case SCRIPT_TYPE::TEXTUISCRIPT:
		return L"TextUIScript";
		break;

	case SCRIPT_TYPE::TOWNOBJECTSCRIPT:
		return L"TownObjectScript";
		break;

	case SCRIPT_TYPE::UISCRIPT:
		return L"UIScript";
		break;

	case SCRIPT_TYPE::VESTALSCRIPT:
		return L"VestalScript";
		break;

	case SCRIPT_TYPE::WALLSCRIPT:
		return L"WallScript";
		break;

	}
	return nullptr;
}