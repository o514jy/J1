#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "Game/Controllers/J1AIController.h"
#include "J1CreatureController.generated.h"

class AJ1Creature;

UCLASS()
class J1_API AJ1CreatureController : public AJ1AIController
{
	GENERATED_BODY()
public:
	AJ1CreatureController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	~AJ1CreatureController();

public:
	/** setter & getter **/
	TObjectPtr<AJ1Creature> GetOwner() { return Cast<AJ1Creature>(GetPawn()); }
	
public:
	/** network **/
	virtual void ProcessMove(const Protocol::PosInfo& posInfo) override;

	//virtual void RegisterNotifyPos() override;
	virtual void ProcessNotifyPos(const Protocol::PosInfo& posInfo) override;

public:
	/** finite state machine **/
	virtual void Tick(float DeltaTime) override;

	virtual void UpdateIdle();
	virtual void UpdateRun();
	virtual void UpdateSkill();
	virtual void UpdateDead();
};
