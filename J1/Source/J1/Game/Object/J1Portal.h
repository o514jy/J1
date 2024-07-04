#pragma once

#include "CoreMinimal.h"
#include "Game/Object/J1Env.h"
#include "J1Portal.generated.h"

UCLASS()
class J1_API AJ1Portal : public AJ1Env
{
	GENERATED_BODY()
public:
	AJ1Portal();
	~AJ1Portal();

public:
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);
	
public:
	/** network **/
	//void ProcessTeleport(const Protocol::S_TELEPORT& TeleportPkt);

public:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* CollisionComponent;
};
