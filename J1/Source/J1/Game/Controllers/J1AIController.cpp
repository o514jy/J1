#include "J1AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

AJ1AIController::AJ1AIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AJ1AIController::ProcessMove(const Protocol::PosInfo& posInfo)
{
	FVector location;
	location.X = posInfo.dest_x();
	location.Y = posInfo.dest_y();
	location.Z = posInfo.dest_z();
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, location);
}
