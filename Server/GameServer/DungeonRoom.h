#pragma once

#include "RoomBase.h"

class DungeonRoom : public RoomBase
{
public:
	DungeonRoom();
	~DungeonRoom();

public:
	DungeonRoomRef GetRoomRef();

public:
	/** tick **/
	virtual void UpdateTick() override;
};

