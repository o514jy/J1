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

public:
	/** in game logic **/
	virtual bool EnterRoom(ObjectRef object, bool randPos = true, FVector3 spawnPos = FVector3()) override;

public:
	/** information **/


	// temp
	virtual void SetRoomState(Protocol::RoomState state);
	MonsterRef GetMonster();
	vector<MonsterRef> GetMonsters();
	MonsterRef _monster = nullptr;
	void SpawnMonster();
};

