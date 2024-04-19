#pragma once

class RoomManager
{
public:
	RoomManager();
	~RoomManager();

public:
	void InitializeAllRoom();

public:
	void AllUpdateTickStart();

public:
	RoomBaseRef GetRoom(uint64 roomId);

	StartRoomRef GetStartRoom();

private:
	USE_LOCK;

	unordered_map<uint64, RoomBaseRef> _rooms;
};

