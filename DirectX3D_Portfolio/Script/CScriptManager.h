#pragma once

#include <vector>
#include <string>

enum class SCRIPT_TYPE
{
	AIPOINTSCRIPT,
	AYA,
	BARUISCRIPT,
	BULLETSCRIPT,
	BUTTONUISCRIPT,
	CAMERASCRIPT,
	EFFECTSCRIPT,
	FONTUISCRIPT,
	HITBOXSCRIPT,
	HYUNWOOSCRIPT,
	IGITEMOBJSCRIPT,
	ITEMBOXUISCRIPT,
	ITEMSPAWNSCRIPT,
	LAYERUPDATEBOX,
	LIDAILIN,
	LIGHTSCRIPT,
	MONSTERCREATESCRIPT,
	MONSTERSCRIPT,
	MOUSERAY,
	OBJECTSCRIPT,
	PICTUREUISCRIPT,
	PLAYERSCRIPT,
	SKILLUISCRIPT,
	SWITCHUISCRIPT,
	TRACKINGUISCRIPT,
	END,
};

using namespace std;

class CScript;

class CScriptManager
{
public:
	static void GetScriptInfo(vector<wstring>& _vector);
	static CScript * GetScript(const wstring& _scriptName);
	static CScript * GetScript(UINT _scriptType);
	static const wchar_t * GetScriptName(CScript * _script);
};
