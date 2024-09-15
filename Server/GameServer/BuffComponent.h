#pragma once

#include "BuffPolicy.h"

class BuffComponent : public enable_shared_from_this<BuffComponent>
{
public:
	BuffComponent();
	~BuffComponent();

public:
	/** static **/
	static atomic<uint64> _buffIdGenerator;
	static unordered_map<Protocol::BuffType, BuffPolicyRef> _policies;

	static uint64 GenerateBuffId();
	static BuffBaseRef SpawnBuff(uint64 buffId, int32 templateId, CreatureRef target, CreatureRef caster, BuffPolicyRef policy);

public:
	/** initialize **/
	void SetInfo(CreatureRef owner);

public:
	/** process **/
	vector<BuffBaseRef> GenerateBuffs(vector<int32> buffIds, SkillBaseRef skill); // legacy

	void ApplyBuff(int32 templateId, CreatureRef caster);
	void ApplyBuff(BuffDataRef buffData, CreatureRef caster, bool send = true);

	void ApplyInstantBuff(BuffDataRef buffData, CreatureRef caster);
	void ApplyDurationBuff(BuffDataRef buffData, CreatureRef caster, bool send = true);
	void ApplyInfiniteBuff(BuffDataRef buffData, CreatureRef caster,  bool send = true);

	void RemoveBuff(uint64 buffId, bool send = true);
	void RemoveBuff(BuffBaseRef buff, bool send = true);
	void RemoveAllBuff();

public:
	/** network **/
	void SendApply(BuffBaseRef buff);
	void SendRemove(BuffBaseRef buff);

public:
	/** information **/
	CreatureRef _owner;

	vector<BuffBaseRef> ActiveBuffs; // legacy
	unordered_map<uint64, BuffBaseRef> _buffs;
};

