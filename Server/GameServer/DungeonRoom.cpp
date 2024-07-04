#include "pch.h"
#include "DungeonRoom.h"

DungeonRoom::DungeonRoom()
{
}

DungeonRoom::~DungeonRoom()
{
}

DungeonRoomRef DungeonRoom::GetRoomRef()
{
	return static_pointer_cast<DungeonRoom>(shared_from_this());
}

void DungeonRoom::UpdateTick()
{
	__super::UpdateTick();

	//cout << "Update StartRoom" << "\n";
	DoTimer(TICK_COUNT, &DungeonRoom::UpdateTick);
}
