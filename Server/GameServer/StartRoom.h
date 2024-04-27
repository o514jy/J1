#pragma once

#include "RoomBase.h"

class StartRoom : public RoomBase
{
public:
	StartRoom();
	virtual ~StartRoom();

public:
	StartRoomRef GetRoomRef();

public:
	/** tick **/
	virtual void UpdateTick() override;

public:
	/** setter & getter **/
	BossRef GetSevarog();

	virtual void SetRoomState(Protocol::RoomState state);

public:
	/** in game logic **/
	virtual bool EnterRoom(ObjectRef object, bool randPos = true) override;
	virtual bool LeaveRoom(ObjectRef object) override;

	void SpawnBoss();
	
protected:
	/** information **/
	BossRef _sevarog;
};

