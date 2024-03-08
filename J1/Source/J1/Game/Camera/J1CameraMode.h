#pragma once

#include "J1CameraMode.generated.h"

/* forward declaration */
class UJ1CameraComponent;

/*
* [0,1]을 BlendFunction에 맞게 재매핑을 위한 타입
*/
UENUM(BlueprintType)
enum class EJ1CameraModeBlendFunction : uint8
{
	Linear,
	/*
	* EaseIn/Out은 exponent 값에 의해 조절된다
	*/
	EaseIn,
	EaseOut, // default 값, 기울기가 급격하게 가다가 뒤로 갈수록 완만해진다.
	EaseInOut,
	COUNT
};

/*
* FJ1CameraModeView
*/
struct FJ1CameraModeView
{
	FJ1CameraModeView();

	// 자신과 더 Top에 있는 CameraMode를 Weight에 맞게 Lerp를 진행한다.
	void Blend(const FJ1CameraModeView& Other, float OtherWeight);

	FVector Location;
	FRotator Rotation; // Camera의 Rotation
	FRotator ControlRotation; // Camera의 대상이 되는 Actor의 Rotation
	float FieldOfView;
};

/* Camera Blending 대상 유닛 */
UCLASS(Abstract, NotBlueprintable)
class UJ1CameraMode : public UObject
{
	GENERATED_BODY()
public:
	UJ1CameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// CameraMode의 Outer가 CameraComponent이기때문에 그걸 이용해서 CameraComponent를 불러온다.
	UJ1CameraComponent* GetJ1CameraComponent() const;
	// CameraComponent를 이용해서 TargetActor를 가져온다. CameraMode->CameraComponent->Actor
	AActor* GetTargetActor() const;
	// TargetActor의 BaseEyeHeight를 고려해서 ViewLocation을 반환한다.
	FVector GetPivotLocation() const;
	// TargetActor의 GetViewRotation을 통해 Eye의 위치를 고려해서 Rotation을 반환한다.
	FRotator GetPivotRotation() const;
	// 카메라 모드에 맞게 다르게 override해서 적용시킨다.
	// Character의 Location과 ControlRotation을 활용하여 View를 업데이트한다.
	virtual void UpdateView(float DeltaTime);
	// BlendWeight를 DeltaTime을 활용하여 BlendAlpha를 계산한 후 BlendFunction에 맞게 재-매핑해서 최종 계산
	void UpdateBlending(float DeltaTime);
	// CameraMode를 업데이트한다. UpdateView와 UpdateBlending 함수 호출
	void UpdateCameraMode(float DeltaTime);

	/* CameraMode에 의해 생성된 CameraModeView */
	// UpdateView를 통해 이 값이 항상 Update된다.
	FJ1CameraModeView View;

	/* Camera Mode의 FOV */
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", Clampmax = "170.0"))
	float FieldOfView;

	/* View에 대한 Pitch [Min, Max] */
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", Clampmax = "89.9"))
	float ViewPitchMin;

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", Clampmax = "89.9"))
	float ViewPitchMax;

	/* 얼마동안 Blend를 진행할지 시간을 의미 */
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;

	/* 선형적인 Blend 값 [0, 1] */
	float BlendAlpha;

	/*
	* 해당 CameraMode의 최종 Blend 값
	* 앞서 BlendAlpha의 선형 값을 매핑하여 최종 BlendWeight를 계산
	*/
	// 시간의 흐름, 0에 가까울수록 최근, 1에가까울수록 오래된
	float BlendWeight;

	/*
	* EaseIn/Out에 사용한 Exponent
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent;

	/* Blend function */
	EJ1CameraModeBlendFunction BlendFunction;
};

/* Camera Blending을 담당하는 객체 */
UCLASS()
class UJ1CameraModeStack : public UObject
{
	GENERATED_BODY()
public:
	UJ1CameraModeStack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/*
	* member methods
	*/
	// PushCameraMode에서 사용할 함수
	// Class에 맞는 CameraMode의 Instance를 반환해준다. 없으면 만들어서 Instances에 넣고 반환한다.
	UJ1CameraMode* GetCameraModeInstance(TSubclassOf<UJ1CameraMode>& CameraModeClass);
	// Class에 맞는 CameraMode를 Blending 과정을 고려해서 가져오고 Stack의 맨 위에 넣는다.
	void PushCameraMode(TSubclassOf<UJ1CameraMode>& CameraModeClass);
	// Top -> Bottom [0 -> Num]까지 순차적으로 Stack에 있는 CameraMode 업데이트
	void UpdateStack(float DeltaTime);
	// Bottom -> Top까지 CameraModeStack에 있는 CameraMode와 그 다음 CameraMode에 대해 Blending 진행
	void BlendStack(FJ1CameraModeView& OutCameraModeView) const;
	// Stack 계산 진행, UpdateStack과 BlendStack을 호출한다.
	void EvaluateStack(float DeltaTime, FJ1CameraModeView& OutCameraModeView);

	/*
	* member variables
	*/

	// CameraModeClass를 인스턴스화 시켜서 가지고 있고 블렌딩이 필요할 때 하나씩 CameraModeStack에 넣으면서 진행

	/* 생성된 CameraMode를 관리 */
	UPROPERTY()
	TArray<TObjectPtr<UJ1CameraMode>> CameraModeInstances;

	/* Camera Matrix Blend 업데이트 진행 큐 */
	UPROPERTY()
	TArray<TObjectPtr<UJ1CameraMode>> CameraModeStack;
};