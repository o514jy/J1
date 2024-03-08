#include "J1PlayerController.h"
#include "J1/Game/Camera/J1PlayerCameraManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(J1PlayerController)

AJ1PlayerController::AJ1PlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerCameraManagerClass = AJ1PlayerCameraManager::StaticClass();
}
