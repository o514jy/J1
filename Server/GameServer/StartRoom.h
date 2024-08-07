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
	PortalRef GetPortal();

	virtual void SetRoomState(Protocol::RoomState state);

public:
	/** network **/
	virtual void HandleTeleport(Protocol::C_TELEPORT pkt) override;

public:
	/** in game logic **/
	virtual bool EnterRoom(ObjectRef object, bool randPos = true, FVector3 spawnPos = FVector3()) override;
	virtual bool LeaveRoom(ObjectRef object) override;

	void SpawnBoss();

	void SpawnPortal();

public:
	// ranged temp
	MonsterRef GetRanged();
	void SpawnRanged();
	MonsterRef GetDemon();
	void SpawnDemon();
	MonsterRef _monster;
	
protected:
	/** information **/
	BossRef _sevarog; // temp

	PortalRef _portal; // temp
};

