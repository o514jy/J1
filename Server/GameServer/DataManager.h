#pragma once

#include "Data.h"

class DataManager //: public JobQueue
{
public:
	DataManager();
	~DataManager();

public:
	void Init();
	
public:
	void ParseJsonData(const WCHAR* path);

public:
	/** setter & getter **/
	PlayerDataRef GetPlayerDataById(int32 id);

	SkillDataRef GetSkillDataById(int32 id);

	EffectDataRef GetEffectDataById(int32 id);

private:
	USE_LOCK;

	unordered_map<int32, PlayerDataRef> _playerData;
	
	unordered_map<int32, BossDataRef> _bossData;

	unordered_map<int32, SkillDataRef> _skillData;

	unordered_map<int32, EffectDataRef> _effectData;
};
