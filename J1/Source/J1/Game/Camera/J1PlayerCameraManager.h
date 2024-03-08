#pragma once

#include "Camera/PlayerCameraManager.h"
#include "J1PlayerCameraManager.generated.h"

/*
* Controller�� ���ε��� CameraManager
*/
#define J1_CAMERA_DEFAULT_FOV (90.0f)
#define J1_CAMERA_DEFAULT_PITCH_MIN (-89.0f)
#define J1_CAMERA_DEFAULT_PITCH_MAX (89.0f)

UCLASS()
class AJ1PlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
public:
	AJ1PlayerCameraManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};