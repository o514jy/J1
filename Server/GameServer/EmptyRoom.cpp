#include "pch.h"
#include "EmptyRoom.h"

EmptyRoom::EmptyRoom()
{
}

EmptyRoom::~EmptyRoom()
{
}

void EmptyRoom::Push(JobRef job, bool pushOnly)
{
}

void EmptyRoom::PushAfter(JobRef job, uint64 tickAfter, weak_ptr<JobQueue> owner)
{
}

bool EmptyRoom::EnterRoom(ObjectRef object, bool randPos, FVector3 spawnPos)
{
	return false;
}

bool EmptyRoom::LeaveRoom(ObjectRef object)
{
	return false;
}

void EmptyRoom::Broadcast(SendBufferRef sendBuffer, uint64 exceptId)
{
}
