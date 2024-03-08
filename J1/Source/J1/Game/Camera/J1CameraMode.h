#pragma once

#include "J1CameraMode.generated.h"

/* forward declaration */
class UJ1CameraComponent;

/*
* [0,1]�� BlendFunction�� �°� ������� ���� Ÿ��
*/
UENUM(BlueprintType)
enum class EJ1CameraModeBlendFunction : uint8
{
	Linear,
	/*
	* EaseIn/Out�� exponent ���� ���� �����ȴ�
	*/
	EaseIn,
	EaseOut, // default ��, ���Ⱑ �ް��ϰ� ���ٰ� �ڷ� ������ �ϸ�������.
	EaseInOut,
	COUNT
};

/*
* FJ1CameraModeView
*/
struct FJ1CameraModeView
{
	FJ1CameraModeView();

	// �ڽŰ� �� Top�� �ִ� CameraMode�� Weight�� �°� Lerp�� �����Ѵ�.
	void Blend(const FJ1CameraModeView& Other, float OtherWeight);

	FVector Location;
	FRotator Rotation; // Camera�� Rotation
	FRotator ControlRotation; // Camera�� ����� �Ǵ� Actor�� Rotation
	float FieldOfView;
};

/* Camera Blending ��� ���� */
UCLASS(Abstract, NotBlueprintable)
class UJ1CameraMode : public UObject
{
	GENERATED_BODY()
public:
	UJ1CameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// CameraMode�� Outer�� CameraComponent�̱⶧���� �װ� �̿��ؼ� CameraComponent�� �ҷ��´�.
	UJ1CameraComponent* GetJ1CameraComponent() const;
	// CameraComponent�� �̿��ؼ� TargetActor�� �����´�. CameraMode->CameraComponent->Actor
	AActor* GetTargetActor() const;
	// TargetActor�� BaseEyeHeight�� ����ؼ� ViewLocation�� ��ȯ�Ѵ�.
	FVector GetPivotLocation() const;
	// TargetActor�� GetViewRotation�� ���� Eye�� ��ġ�� ����ؼ� Rotation�� ��ȯ�Ѵ�.
	FRotator GetPivotRotation() const;
	// ī�޶� ��忡 �°� �ٸ��� override�ؼ� �����Ų��.
	// Character�� Location�� ControlRotation�� Ȱ���Ͽ� View�� ������Ʈ�Ѵ�.
	virtual void UpdateView(float DeltaTime);
	// BlendWeight�� DeltaTime�� Ȱ���Ͽ� BlendAlpha�� ����� �� BlendFunction�� �°� ��-�����ؼ� ���� ���
	void UpdateBlending(float DeltaTime);
	// CameraMode�� ������Ʈ�Ѵ�. UpdateView�� UpdateBlending �Լ� ȣ��
	void UpdateCameraMode(float DeltaTime);

	/* CameraMode�� ���� ������ CameraModeView */
	// UpdateView�� ���� �� ���� �׻� Update�ȴ�.
	FJ1CameraModeView View;

	/* Camera Mode�� FOV */
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", Clampmax = "170.0"))
	float FieldOfView;

	/* View�� ���� Pitch [Min, Max] */
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", Clampmax = "89.9"))
	float ViewPitchMin;

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", Clampmax = "89.9"))
	float ViewPitchMax;

	/* �󸶵��� Blend�� �������� �ð��� �ǹ� */
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;

	/* �������� Blend �� [0, 1] */
	float BlendAlpha;

	/*
	* �ش� CameraMode�� ���� Blend ��
	* �ռ� BlendAlpha�� ���� ���� �����Ͽ� ���� BlendWeight�� ���
	*/
	// �ð��� �帧, 0�� �������� �ֱ�, 1���������� ������
	float BlendWeight;

	/*
	* EaseIn/Out�� ����� Exponent
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent;

	/* Blend function */
	EJ1CameraModeBlendFunction BlendFunction;
};

/* Camera Blending�� ����ϴ� ��ü */
UCLASS()
class UJ1CameraModeStack : public UObject
{
	GENERATED_BODY()
public:
	UJ1CameraModeStack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/*
	* member methods
	*/
	// PushCameraMode���� ����� �Լ�
	// Class�� �´� CameraMode�� Instance�� ��ȯ���ش�. ������ ���� Instances�� �ְ� ��ȯ�Ѵ�.
	UJ1CameraMode* GetCameraModeInstance(TSubclassOf<UJ1CameraMode>& CameraModeClass);
	// Class�� �´� CameraMode�� Blending ������ ����ؼ� �������� Stack�� �� ���� �ִ´�.
	void PushCameraMode(TSubclassOf<UJ1CameraMode>& CameraModeClass);
	// Top -> Bottom [0 -> Num]���� ���������� Stack�� �ִ� CameraMode ������Ʈ
	void UpdateStack(float DeltaTime);
	// Bottom -> Top���� CameraModeStack�� �ִ� CameraMode�� �� ���� CameraMode�� ���� Blending ����
	void BlendStack(FJ1CameraModeView& OutCameraModeView) const;
	// Stack ��� ����, UpdateStack�� BlendStack�� ȣ���Ѵ�.
	void EvaluateStack(float DeltaTime, FJ1CameraModeView& OutCameraModeView);

	/*
	* member variables
	*/

	// CameraModeClass�� �ν��Ͻ�ȭ ���Ѽ� ������ �ְ� ������ �ʿ��� �� �ϳ��� CameraModeStack�� �����鼭 ����

	/* ������ CameraMode�� ���� */
	UPROPERTY()
	TArray<TObjectPtr<UJ1CameraMode>> CameraModeInstances;

	/* Camera Matrix Blend ������Ʈ ���� ť */
	UPROPERTY()
	TArray<TObjectPtr<UJ1CameraMode>> CameraModeStack;
};