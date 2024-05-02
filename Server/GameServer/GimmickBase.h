#pragma once

#include "JobQueue.h"

class GimmickBase : public JobQueue
{
public:
	GimmickBase();
	~GimmickBase();

public:
	/** initialize **/
	virtual void SetInfo(BossRef owner, int32 templateId);

public:
	/** event **/
	void OnEventTimeHandler();
	virtual void OnEvent(int32 eventCount);

	void OnDurationCompleteHandler();

public:
	/** network **/
	Protocol::C_SKILL MakeSkillPkt(int32 skillId, Protocol::SimplePosInfo simplePosInfo = Protocol::SimplePosInfo());

public:
	/** process gimmick **/
	virtual void DoGimmick();

public:
	/** information **/
	BossRef _owner;

	GimmickDataRef _gimmickData;

	int32 _eventCount;
};

