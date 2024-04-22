#pragma once
#include "BuffBase.h"
class BuffInstant : public BuffBase
{
public:
	BuffInstant();
	~BuffInstant();

public:
	/** process **/
	virtual void ApplyBuff() override;
};

