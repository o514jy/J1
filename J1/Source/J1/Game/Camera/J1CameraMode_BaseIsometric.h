#pragma once

#include "J1CameraMode.h"
#include "J1CameraMode_BaseIsometric.generated.h"

/* forward declarations */
class UCurveVector;

// abstract이기때문에 blueprint에서 상속받아서 정의해야한다.
UCLASS(Abstract, Blueprintable)
class UJ1CameraMode_BaseIsometric : public UJ1CameraMode
{
	GENERATED_BODY()
public:
	UJ1CameraMode_BaseIsometric(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/*
	* UJ1CameraMode's interface
	*/
	// Character의 Location과 ControlRotation을 활용하여 View를 업데이트한다.
	// 다만 CurveData가 있다면 location에 offset을 적용시키는 작업이 추가된다.
	virtual void UpdateView(float DeltaTime) override;

	/*
	* member variables
	*/
	// Curve값을 통해 Pitch의 상태에 따라 offset이 변한다.
	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	TObjectPtr<const UCurveVector> TargetOffsetCurve;
};