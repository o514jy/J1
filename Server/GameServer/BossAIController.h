#pragma once

#include "MonsterAIController.h"

class BossAIController : public MonsterAIController
{
public:
	BossAIController();
	~BossAIController();

public:
	/** initialize **/
	virtual void SetInfo(ObjectRef owner) override;

public:
	/** setter & getter **/


public:
	/** finite state machine **/
	virtual void UpdateTick() override;

	virtual void UpdateIdle() override;
	virtual void UpdateRun() override;
	virtual void UpdateSkill() override;
	virtual void UpdateGimmick();
	virtual void UpdateDead() override;

public:
	/** information **/

	int32 _activeGimmickId;
};

