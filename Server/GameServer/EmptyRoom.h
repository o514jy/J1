#pragma once

#include "RoomBase.h"

class EmptyRoom : public RoomBase
{
public:
	EmptyRoom();
	virtual ~EmptyRoom();

public:
	virtual void Push(JobRef job, bool pushOnly = false);
	virtual void PushAfter(JobRef job, uint64 tickAfter, weak_ptr<JobQueue> owner);

	virtual bool EnterRoom(ObjectRef object, bool randPos = true, FVector3 spawnPos = FVector3());
	virtual bool LeaveRoom(ObjectRef object);

	virtual void Broadcast(SendBufferRef sendBuffer, uint64 exceptId = 0);
};

