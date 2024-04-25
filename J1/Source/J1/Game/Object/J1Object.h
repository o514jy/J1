#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "GameFramework/Actor.h"
#include "J1Object.generated.h"

UCLASS()
class J1_API AJ1Object : public AActor
{
public:
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AJ1Object();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	/** setter & getter **/
	void SetMoveState(const Protocol::MoveState& State);
	Protocol::MoveState GetMoveState() { return PosInfo->state(); }

	void SetPosInfo(const Protocol::PosInfo& Info, bool Forced = false);
	Protocol::PosInfo* GetPosInfo() { return PosInfo; }

	void SetObjectInfo(const Protocol::ObjectInfo& InObjectInfo);
	Protocol::ObjectInfo* GetObjectInfo() { return ObjectInfo; }

public:
	/** initialize info **/
	virtual void SetInfo(Protocol::ObjectInfo& InObjectInfo);

protected:
	/** information **/
	Protocol::ObjectInfo* ObjectInfo; // 정보

	Protocol::PosInfo* PosInfo; // 위치 정보

	uint64 ObjectId; // instance id
	int32 TemplateId; // data id

	//TObjectPtr<UObjectData> ObjectData;
};
