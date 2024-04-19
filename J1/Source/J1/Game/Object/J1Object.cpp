// Fill out your copyright notice in the Description page of Project Settings.


#include "J1Object.h"

// Sets default values
AJ1Object::AJ1Object()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AJ1Object::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJ1Object::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJ1Object::SetMoveState(const Protocol::MoveState& State)
{
	if (PosInfo->state() == State)
		return;

	PosInfo->set_state(State);

	// TODO
}

void AJ1Object::SetPosInfo(const Protocol::PosInfo& Info, bool Forced)
{
	if (PosInfo->object_id() != 0)
	{
		assert(PosInfo->object_id() == Info.object_id());
	}

	PosInfo->CopyFrom(Info);

	if (Forced == true)
	{
		FVector Location(Info.x(), Info.y(), Info.z());
		SetActorLocation(Location);
		return;
	}

	SetMoveState(Info.state());
}

void AJ1Object::SetObjectInfo(const Protocol::ObjectInfo& InObjectInfo)
{
	if (ObjectInfo->object_id() == 0)
		return;

	ObjectInfo->CopyFrom(InObjectInfo);
}

void AJ1Object::SetInfo(Protocol::ObjectInfo& InObjectInfo)
{
	if (InObjectInfo.object_id() == 0)
		return;

	ObjectInfo->CopyFrom(InObjectInfo);

	ObjectId = ObjectInfo->object_id();

	SetPosInfo(ObjectInfo->pos_info(), true);


}
