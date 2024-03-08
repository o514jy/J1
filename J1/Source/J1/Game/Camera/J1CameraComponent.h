#pragma once

#include "Camera/CameraComponent.h"
#include "J1CameraComponent.generated.h"

/* forward declaration */
class UJ1CameraModeStack;
class UJ1CameraMode;
/* template forward declaration */
template <class TClass> class TSubclassOf;

/* (return type, delegate_name) */
// UJ1CameraMode를 반환해주는 이름이 FJ1CameraModeDelegate인 Delegate
DECLARE_DELEGATE_RetVal(TSubclassOf<UJ1CameraMode>, FJ1CameraModeDelegate);

UCLASS()
class UJ1CameraComponent : public UCameraComponent
{
	GENERATED_BODY()
public:
	UJ1CameraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Actor에 붙어있는 Component중에 CameraComponent를 찾아서 반환
	static UJ1CameraComponent* FindCameraComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UJ1CameraComponent>() : nullptr); }

	/*
	* CameraComponent interface
	*/
	// CameraModeStack에 첫번째 CameraMode를 만들어서 넣어준다.
	virtual void OnRegister() final;
	// Tick마다 Camera를 Update하는 함수
	// DesiredView : 카메라의 SRT를 비롯한 상태를 나타낸다. output또한 이걸 통해 내보낸다.
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) final;

	/*
	* member methods
	*/
	// CameraComponent의 Outer는 J1Character(Target Actor)이다.
	AActor* GetTargetActor() const { return GetOwner(); }
	// HeroComponent에 있는 PawnData에서 DefaultCameraMode를 가져와서(DetermineCameraModeDelegate) 
	// CameraModeStack에 추가하여, CameraMode 블렌딩을 준비한다.
	void UpdateCameraModes();

	/*
	* member variables
	*/
	/* 카메라의 blending 기능을 지원하는 stack */
	UPROPERTY()
	TObjectPtr<UJ1CameraModeStack> CameraModeStack;

	/* 현재 CameraMode를 가져오는 Delegate */
	FJ1CameraModeDelegate DetermineCameraModeDelegate;
};