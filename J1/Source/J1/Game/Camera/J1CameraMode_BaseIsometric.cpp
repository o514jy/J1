#include "J1CameraMode_BaseIsometric.h"
#include "Curves/CurveVector.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(J1CameraMode_BaseIsometric)

UJ1CameraMode_BaseIsometric::UJ1CameraMode_BaseIsometric(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UJ1CameraMode_BaseIsometric::UpdateView(float DeltaTime)
{
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;

	// TargetOffsetCurve가 override되어있다면, Curve의 값을 가져와서 적용 진행
	// - Camera 관점에서 Character의 어느 부분에 Target으로 할지 결정하는 것으로 이해하면 된다.
	if (TargetOffsetCurve)
	{
		const FVector TargetOffset = TargetOffsetCurve->GetVectorValue(PivotRotation.Pitch);
		View.Location = PivotLocation + PivotRotation.RotateVector(TargetOffset);
	}
}
