#pragma once

#include "Object.h"

class Projectile : public Object
{
public:
	Projectile();
	~Projectile();

public:
	/** update tick **/
	virtual void UpdateTick() override;

public:
	/** initialize **/	

	/* by skill */
	virtual void SetInfo(CreatureRef owner, SkillBaseRef ownerSkill, int32 templateId);

	/* by gimmick */
	virtual void SetInfo(BossRef owner, GimmickBaseRef ownerGimmick, int32 templateId);

public:
	/** impact event **/
	void OnImpactTimeHandler();
	virtual void OnImpactEvent(int32 impactCount);

	void OnDurationCompleteHandler();

public:
	/** spawn projectile **/
	void SpawnProjectile();

public:
	/** gather creature in effects **/
	vector<ObjectRef> GatherObjectInEffectArea(int32 effectId);

public:
	/** process buff **/
	void ProcessBuff(vector<ObjectRef>& objects);

public:
	/** check area **/
	bool IsInCircleArea(ObjectRef object, float effectRadius);


public:
	/** information **/
	ProjectileDataRef _projectileData;

	CreatureRef _owner;

	SkillBaseRef _ownerSkill;
	GimmickBaseRef _ownerGimmick;

	int32 _impactCount;
};

