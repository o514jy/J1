#pragma once

#include "BaseAIController.h"

class MonsterAIController : public BaseAIController
{
public:
	MonsterAIController();
	~MonsterAIController();

public:
	/** initialize **/
	virtual void SetInfo(ObjectRef owner) override;

public:
	/** finite state machine **/
	virtual void UpdateTick() override;

	virtual void UpdateIdle() override;
	virtual void UpdateRun() override;
	virtual void UpdateSkill() override;
	virtual void UpdateDead() override;

};