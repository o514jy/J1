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

	// TargetOffsetCurve�� override�Ǿ��ִٸ�, Curve�� ���� �����ͼ� ���� ����
	// - Camera �������� Character�� ��� �κп� Target���� ���� �����ϴ� ������ �����ϸ� �ȴ�.
	if (TargetOffsetCurve)
	{
		const FVector TargetOffset = TargetOffsetCurve->GetVectorValue(PivotRotation.Pitch);
		View.Location = PivotLocation + PivotRotation.RotateVector(TargetOffset);
	}
}
