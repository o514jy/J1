#pragma once

#include "Camera/CameraComponent.h"
#include "J1CameraComponent.generated.h"

/* forward declaration */
class UJ1CameraModeStack;
class UJ1CameraMode;
/* template forward declaration */
template <class TClass> class TSubclassOf;

/* (return type, delegate_name) */
// UJ1CameraMode�� ��ȯ���ִ� �̸��� FJ1CameraModeDelegate�� Delegate
DECLARE_DELEGATE_RetVal(TSubclassOf<UJ1CameraMode>, FJ1CameraModeDelegate);

UCLASS()
class UJ1CameraComponent : public UCameraComponent
{
	GENERATED_BODY()
public:
	UJ1CameraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Actor�� �پ��ִ� Component�߿� CameraComponent�� ã�Ƽ� ��ȯ
	static UJ1CameraComponent* FindCameraComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UJ1CameraComponent>() : nullptr); }

	/*
	* CameraComponent interface
	*/
	// CameraModeStack�� ù��° CameraMode�� ���� �־��ش�.
	virtual void OnRegister() final;
	// Tick���� Camera�� Update�ϴ� �Լ�
	// DesiredView : ī�޶��� SRT�� ����� ���¸� ��Ÿ����. output���� �̰� ���� ��������.
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) final;

	/*
	* member methods
	*/
	// CameraComponent�� Outer�� J1Character(Target Actor)�̴�.
	AActor* GetTargetActor() const { return GetOwner(); }
	// HeroComponent�� �ִ� PawnData���� DefaultCameraMode�� �����ͼ�(DetermineCameraModeDelegate) 
	// CameraModeStack�� �߰��Ͽ�, CameraMode ������ �غ��Ѵ�.
	void UpdateCameraModes();

	/*
	* member variables
	*/
	/* ī�޶��� blending ����� �����ϴ� stack */
	UPROPERTY()
	TObjectPtr<UJ1CameraModeStack> CameraModeStack;

	/* ���� CameraMode�� �������� Delegate */
	FJ1CameraModeDelegate DetermineCameraModeDelegate;
};