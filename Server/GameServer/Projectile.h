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
	virtual void SetInfo(CreatureRef owner, SkillBaseRef ownerSkill, int32 templateId, float spawnPosX, float spawnPosY, float spawnPosZ);

public:
	/** impact event **/
	void OnImpactTimeHandler();
	void OnImpactEvent(int32 impactCount);

	void OnDurationCompleteHandler();

public:
	/** spawn projectile **/
	void SpawnProjectile();

public:
	/** check area **/
	bool IsInCircleArea(ObjectRef object, float effectRadius);

public:
	/** information **/
	ProjectileDataRef _projectileData;

	CreatureRef _owner;
	SkillBaseRef _ownerSkill;

	int32 _impactCount;
};

