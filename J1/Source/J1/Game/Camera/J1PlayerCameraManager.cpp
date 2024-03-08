#include "J1PlayerCameraManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(J1PlayerCameraManager)

AJ1PlayerCameraManager::AJ1PlayerCameraManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultFOV = J1_CAMERA_DEFAULT_FOV;
	ViewPitchMin = J1_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = J1_CAMERA_DEFAULT_PITCH_MAX;
}
