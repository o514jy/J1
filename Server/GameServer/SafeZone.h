#pragma once

#include "Projectile.h"

class SafeZone : public Projectile
{
public:
	SafeZone();
	~SafeZone();

public:
	/** update tick **/
	virtual void UpdateTick() override;

public:
	/** initialize by gimmick **/
	virtual void SetInfo(BossRef owner, GimmickBaseRef ownerGimmick, int32 templateId);

public:
	/** setter & getter **/
	void SetDir(Protocol::Direction dir);
	Protocol::Direction GetDir();

public:
	/** event **/
	virtual void OnImpactEvent(int32 impactCount) override;

public:
	/** information **/
	Protocol::Direction _dir;
};

