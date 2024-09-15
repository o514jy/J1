#pragma once

class BuffBase : public enable_shared_from_this<BuffBase>
{
public:
	BuffBase();
	~BuffBase();

public:
	/** initialize **/
	virtual void SetInfo(uint64 buffId, int32 templateId, CreatureRef target, CreatureRef caster, BuffPolicyRef policy);

public:
	/** setter & getter **/
	float GetFinalAmount();

public:
	/** network **/


public:
	/** process **/
	virtual void ApplyBuff();
	virtual void RevertBuff();

public:
	/** helper **/
	uint64 GetRemainingLifetimeInTicks();
	float GetRemainingLifetimeInSeconds();

public:
	/** information **/

	// cache
	uint64 _buffId;
	int32 _templateId;
	Protocol::BuffType _buffType;

	CreatureRef _owner;
	CreatureRef _caster;
	BuffDataRef _buffData;

	BuffPolicyRef _policy;

	uint64 _despawnTick;

	float _finalAmount;

	Protocol::BuffInfo* _buffInfo;
};

