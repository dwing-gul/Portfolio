#include "pch.h"
#include "CScriptManager.h"

#include "CAIPointScript.h"
#include "CAya.h"
#include "CBarUIScript.h"
#include "CBulletScript.h"
#include "CButtonUIScript.h"
#include "CCameraScript.h"
#include "CEffectScript.h"
#include "CFontUIScript.h"
#include "CHitBoxScript.h"
#include "CHyunwooScript.h"
#include "CIGItemObjScript.h"
#include "CItemBoxUIScript.h"
#include "CItemSpawnScript.h"
#include "CLayerUpdateBox.h"
#include "CLiDailin.h"
#include "CLightScript.h"
#include "CMonsterCreateScript.h"
#include "CMonsterScript.h"
#include "CMouseRay.h"
#include "CObjectScript.h"
#include "CPictureUIScript.h"
#include "CPlayerScript.h"
#include "CSkillUIScript.h"
#include "CSwitchUIScript.h"
#include "CTrackingUIScript.h"

void CScriptManager::GetScriptInfo(vector<wstring>& _vector)
{
	_vector.push_back(L"CAIPointScript");
	_vector.push_back(L"CAya");
	_vector.push_back(L"CBarUIScript");
	_vector.push_back(L"CBulletScript");
	_vector.push_back(L"CButtonUIScript");
	_vector.push_back(L"CCameraScript");
	_vector.push_back(L"CEffectScript");
	_vector.push_back(L"CFontUIScript");
	_vector.push_back(L"CHitBoxScript");
	_vector.push_back(L"CHyunwooScript");
	_vector.push_back(L"CIGItemObjScript");
	_vector.push_back(L"CItemBoxUIScript");
	_vector.push_back(L"CItemSpawnScript");
	_vector.push_back(L"CLayerUpdateBox");
	_vector.push_back(L"CLiDailin");
	_vector.push_back(L"CLightScript");
	_vector.push_back(L"CMonsterCreateScript");
	_vector.push_back(L"CMonsterScript");
	_vector.push_back(L"CMouseRay");
	_vector.push_back(L"CObjectScript");
	_vector.push_back(L"CPictureUIScript");
	_vector.push_back(L"CPlayerScript");
	_vector.push_back(L"CSkillUIScript");
	_vector.push_back(L"CSwitchUIScript");
	_vector.push_back(L"CTrackingUIScript");
}

CScript * CScriptManager::GetScript(const wstring& _scriptName)
{
	if (L"CAIPointScript" == _scriptName)
		return new CAIPointScript;
	if (L"CAya" == _scriptName)
		return new CAya;
	if (L"CBarUIScript" == _scriptName)
		return new CBarUIScript;
	if (L"CBulletScript" == _scriptName)
		return new CBulletScript;
	if (L"CButtonUIScript" == _scriptName)
		return new CButtonUIScript;
	if (L"CCameraScript" == _scriptName)
		return new CCameraScript;
	if (L"CEffectScript" == _scriptName)
		return new CEffectScript;
	if (L"CFontUIScript" == _scriptName)
		return new CFontUIScript;
	if (L"CHitBoxScript" == _scriptName)
		return new CHitBoxScript;
	if (L"CHyunwooScript" == _scriptName)
		return new CHyunwooScript;
	if (L"CIGItemObjScript" == _scriptName)
		return new CIGItemObjScript;
	if (L"CItemBoxUIScript" == _scriptName)
		return new CItemBoxUIScript;
	if (L"CItemSpawnScript" == _scriptName)
		return new CItemSpawnScript;
	if (L"CLayerUpdateBox" == _scriptName)
		return new CLayerUpdateBox;
	if (L"CLiDailin" == _scriptName)
		return new CLiDailin;
	if (L"CLightScript" == _scriptName)
		return new CLightScript;
	if (L"CMonsterCreateScript" == _scriptName)
		return new CMonsterCreateScript;
	if (L"CMonsterScript" == _scriptName)
		return new CMonsterScript;
	if (L"CMouseRay" == _scriptName)
		return new CMouseRay;
	if (L"CObjectScript" == _scriptName)
		return new CObjectScript;
	if (L"CPictureUIScript" == _scriptName)
		return new CPictureUIScript;
	if (L"CPlayerScript" == _scriptName)
		return new CPlayerScript;
	if (L"CSkillUIScript" == _scriptName)
		return new CSkillUIScript;
	if (L"CSwitchUIScript" == _scriptName)
		return new CSwitchUIScript;
	if (L"CTrackingUIScript" == _scriptName)
		return new CTrackingUIScript;
	return nullptr;
}

CScript * CScriptManager::GetScript(UINT _scriptType)
{
	switch (_scriptType)
	{
	case (UINT)SCRIPT_TYPE::AIPOINTSCRIPT:
		return new CAIPointScript;
		break;
	case (UINT)SCRIPT_TYPE::AYA:
		return new CAya;
		break;
	case (UINT)SCRIPT_TYPE::BARUISCRIPT:
		return new CBarUIScript;
		break;
	case (UINT)SCRIPT_TYPE::BULLETSCRIPT:
		return new CBulletScript;
		break;
	case (UINT)SCRIPT_TYPE::BUTTONUISCRIPT:
		return new CButtonUIScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERASCRIPT:
		return new CCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::EFFECTSCRIPT:
		return new CEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::FONTUISCRIPT:
		return new CFontUIScript;
		break;
	case (UINT)SCRIPT_TYPE::HITBOXSCRIPT:
		return new CHitBoxScript;
		break;
	case (UINT)SCRIPT_TYPE::HYUNWOOSCRIPT:
		return new CHyunwooScript;
		break;
	case (UINT)SCRIPT_TYPE::IGITEMOBJSCRIPT:
		return new CIGItemObjScript;
		break;
	case (UINT)SCRIPT_TYPE::ITEMBOXUISCRIPT:
		return new CItemBoxUIScript;
		break;
	case (UINT)SCRIPT_TYPE::ITEMSPAWNSCRIPT:
		return new CItemSpawnScript;
		break;
	case (UINT)SCRIPT_TYPE::LAYERUPDATEBOX:
		return new CLayerUpdateBox;
		break;
	case (UINT)SCRIPT_TYPE::LIDAILIN:
		return new CLiDailin;
		break;
	case (UINT)SCRIPT_TYPE::LIGHTSCRIPT:
		return new CLightScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERCREATESCRIPT:
		return new CMonsterCreateScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::MOUSERAY:
		return new CMouseRay;
		break;
	case (UINT)SCRIPT_TYPE::OBJECTSCRIPT:
		return new CObjectScript;
		break;
	case (UINT)SCRIPT_TYPE::PICTUREUISCRIPT:
		return new CPictureUIScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::SKILLUISCRIPT:
		return new CSkillUIScript;
		break;
	case (UINT)SCRIPT_TYPE::SWITCHUISCRIPT:
		return new CSwitchUIScript;
		break;
	case (UINT)SCRIPT_TYPE::TRACKINGUISCRIPT:
		return new CTrackingUIScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptManager::GetScriptName(CScript * _script)
{
	switch ((SCRIPT_TYPE)_script->GetScriptType())
	{
	case SCRIPT_TYPE::AIPOINTSCRIPT:
		return L"CAIPointScript";
		break;

	case SCRIPT_TYPE::AYA:
		return L"CAya";
		break;

	case SCRIPT_TYPE::BARUISCRIPT:
		return L"CBarUIScript";
		break;

	case SCRIPT_TYPE::BULLETSCRIPT:
		return L"CBulletScript";
		break;

	case SCRIPT_TYPE::BUTTONUISCRIPT:
		return L"CButtonUIScript";
		break;

	case SCRIPT_TYPE::CAMERASCRIPT:
		return L"CCameraScript";
		break;

	case SCRIPT_TYPE::EFFECTSCRIPT:
		return L"CEffectScript";
		break;

	case SCRIPT_TYPE::FONTUISCRIPT:
		return L"CFontUIScript";
		break;

	case SCRIPT_TYPE::HITBOXSCRIPT:
		return L"CHitBoxScript";
		break;

	case SCRIPT_TYPE::HYUNWOOSCRIPT:
		return L"CHyunwooScript";
		break;

	case SCRIPT_TYPE::IGITEMOBJSCRIPT:
		return L"CIGItemObjScript";
		break;

	case SCRIPT_TYPE::ITEMBOXUISCRIPT:
		return L"CItemBoxUIScript";
		break;

	case SCRIPT_TYPE::ITEMSPAWNSCRIPT:
		return L"CItemSpawnScript";
		break;

	case SCRIPT_TYPE::LAYERUPDATEBOX:
		return L"CLayerUpdateBox";
		break;

	case SCRIPT_TYPE::LIDAILIN:
		return L"CLiDailin";
		break;

	case SCRIPT_TYPE::LIGHTSCRIPT:
		return L"CLightScript";
		break;

	case SCRIPT_TYPE::MONSTERCREATESCRIPT:
		return L"CMonsterCreateScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::MOUSERAY:
		return L"CMouseRay";
		break;

	case SCRIPT_TYPE::OBJECTSCRIPT:
		return L"CObjectScript";
		break;

	case SCRIPT_TYPE::PICTUREUISCRIPT:
		return L"CPictureUIScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::SKILLUISCRIPT:
		return L"CSkillUIScript";
		break;

	case SCRIPT_TYPE::SWITCHUISCRIPT:
		return L"CSwitchUIScript";
		break;

	case SCRIPT_TYPE::TRACKINGUISCRIPT:
		return L"CTrackingUIScript";
		break;

	}
	return nullptr;
}