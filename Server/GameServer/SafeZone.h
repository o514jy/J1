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

	PlayerRef GetMatchedPlayer();

	virtual void Clear() override;

public:
	/** helper **/
	bool IsInSafeZone(ObjectRef object);

public:
	/** event **/
	virtual void OnImpactEvent(int32 impactCount) override;
	virtual void OnDurationCompleteHandler() override;

public:
	/** information **/
	Protocol::Direction _dir;

	PlayerRef _matchedPlayer;

	bool _callSpawn;
};

