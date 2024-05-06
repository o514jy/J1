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

	void NotifyAndGenerateOtherSafeZone(Protocol::Direction dir);

public:
	/** helper **/
	SafeZoneRef GenerateSafeZone(Protocol::Direction dir);

	vector<uint64> GetSafePlayerList();
	
public:
	/** event **/
	virtual void OnEvent(int32 eventCount) override;

	virtual void OnDurationCompleteHandler() override;

public:
	/** information **/
	map<Protocol::Direction, SafeZoneRef> _gimmickList;
};