#include "J1AIController.h"
#include "J1/Game/Object/J1Creature.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

AJ1AIController::AJ1AIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AJ1AIController::ProcessMove(const Protocol::PosInfo& posInfo)
{
	//FVector location;
	//location.X = posInfo.dest_x();
	//location.Y = posInfo.dest_y();
	//location.Z = posInfo.dest_z();
	//UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, location);
   
	
}

void AJ1AIController::RegisterNotifyPos()
{
}

void AJ1AIController::ProcessNotifyPos(const Protocol::PosInfo& posInfo)
{
	
}
