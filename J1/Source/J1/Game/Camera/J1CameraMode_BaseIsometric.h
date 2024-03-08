#pragma once

#include "J1CameraMode.h"
#include "J1CameraMode_BaseIsometric.generated.h"

/* forward declarations */
class UCurveVector;

// abstract�̱⶧���� blueprint���� ��ӹ޾Ƽ� �����ؾ��Ѵ�.
UCLASS(Abstract, Blueprintable)
class UJ1CameraMode_BaseIsometric : public UJ1CameraMode
{
	GENERATED_BODY()
public:
	UJ1CameraMode_BaseIsometric(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/*
	* UJ1CameraMode's interface
	*/
	// Character�� Location�� ControlRotation�� Ȱ���Ͽ� View�� ������Ʈ�Ѵ�.
	// �ٸ� CurveData�� �ִٸ� location�� offset�� �����Ű�� �۾��� �߰��ȴ�.
	virtual void UpdateView(float DeltaTime) override;

	/*
	* member variables
	*/
	// Curve���� ���� Pitch�� ���¿� ���� offset�� ���Ѵ�.
	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	TObjectPtr<const UCurveVector> TargetOffsetCurve;
};