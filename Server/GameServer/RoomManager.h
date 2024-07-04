#pragma once

#include "JobQueue.h"

class RoomManager : public JobQueue
{
public:
	RoomManager();
	~RoomManager();

public:
	void InitializeAllRoom();

public:
	void AllUpdateTickStart();

public:
	bool ChangeRoom(ObjectRef object, RoomBaseRef destRoom);

public:
	RoomBaseRef GetRoom(uint64 roomId);

	StartRoomRef GetStartRoom();

private:
	USE_LOCK;

	unordered_map<uint64, RoomBaseRef> _rooms;
};

