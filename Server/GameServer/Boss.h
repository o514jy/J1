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

public:
	/** component **/
	GimmickComponentRef _gimmickComponent;

};

