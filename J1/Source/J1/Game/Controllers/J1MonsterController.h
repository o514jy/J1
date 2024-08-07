#pragma once

#include "CoreMinimal.h"
#include "Game/Controllers/J1CreatureController.h"
#include "J1MonsterController.generated.h"

class AJ1Monster;

UCLASS()
class J1_API AJ1MonsterController : public AJ1CreatureController
{
	GENERATED_BODY()
public:
	AJ1MonsterController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	~AJ1MonsterController();

public:
	virtual void BeginPlay() override;

public:
	TObjectPtr<AJ1Monster> GetOwner() { return Cast<AJ1Monster>(GetPawn()); }

public:
	/** finite state machine **/
	virtual void Tick(float DeltaTime) override;

	virtual void UpdateIdle();
	virtual void UpdateRun();
	virtual void UpdateSkill();
	virtual void UpdateDead();
};
