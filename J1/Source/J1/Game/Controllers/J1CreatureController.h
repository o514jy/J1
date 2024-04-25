#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "Game/Controllers/J1AIController.h"
#include "J1CreatureController.generated.h"


UCLASS()
class J1_API AJ1CreatureController : public AJ1AIController
{
public:
	GENERATED_BODY()
	
public:
	/** network **/
	virtual void ProcessMove(const Protocol::PosInfo& posInfo) override;

	virtual void ProcessSkill(const Protocol::SkillSlot& slot);
};
