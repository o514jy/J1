#pragma once

#include "GimmickBase.h"

class FindSafeZone : public GimmickBase
{
public:
	FindSafeZone();
	~FindSafeZone();

public:
	/** initialize **/
	virtual void SetInfo(BossRef owner, int32 templateId) override;

public:
	/** process gimmick **/
	virtual void DoGimmick() override;

public:
	/** helper **/
	SafeZoneRef GenerateSafeZone(Protocol::Direction dir);
	
public:
	/** event **/
	virtual void OnEvent(int32 eventCount) override;

public:
	/** information **/
	map<Protocol::Direction, SafeZoneRef> _gimmickList;
};

