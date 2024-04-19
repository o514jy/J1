#pragma once

class RoomManager : public JobQueue
{
public:
	RoomManager();
	~RoomManager();

public:
	void InitializeAllRoom();

public:
	void AllUpdateTick();

public:
	RoomBaseRef GetRoom(uint64 roomId);

	StartRoomRef GetStartRoom();

private:
	unordered_map<uint64, RoomBaseRef> _rooms;
};

