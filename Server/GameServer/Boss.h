#pragma once

#include "Monster.h"

class Boss : public Monster
{
public:
	Boss();
	~Boss();

public:
	/** update tick **/
	virtual void UpdateTick() override;

public:
	/** initialize **/
	virtual void SetInfo(int32 templateId) override;

public:
	/** setter & getter **/
	GimmickComponentRef GetGimmickComponent();

	BossDataRef GetBossData() { return static_pointer_cast<BossData>(_creatureData); }

	virtual void Clear() override;

public:
	/** component **/
	GimmickComponentRef _gimmickComponent;

};

