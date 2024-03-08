#include "J1CameraComponent.h"
#include "Camera/J1CameraMode.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(J1CameraComponent)

UJ1CameraComponent::UJ1CameraComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, CameraModeStack(nullptr)
{

}

void UJ1CameraComponent::OnRegister()
{
	Super::OnRegister();
	if (!CameraModeStack)
	{
		// �ʱ�ȭ (BeginPlay�� ����)�� ���� �ʿ���� ��ü�� NewObject�� �Ҵ�
		CameraModeStack = NewObject<UJ1CameraModeStack>(this);
	}
}

void UJ1CameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	check(CameraModeStack);

	// �ش� �Լ��� HeroComponent�� �ִ� PawnData���� DefaultCameraMode�� �����ͼ� 
	// CameraModeStack�� �߰��Ͽ�, CameraMode ������ �غ��Ѵ�.
	UpdateCameraModes();

	// EvaluateStack�� CameraModeStack�� �ִ� CameraMode�� ������Ʈ(+����)�ϰ�
	// CameraModeStack�� Bottom-Top���� ������Ʈ�� CameraMode���� Lerp�� �������ش�.
	// - �̿� ���� ����� CameraModeView�� ĳ�̵ȴ�.
	FJ1CameraModeView CameraModeView;
	CameraModeStack->EvaluateStack(DeltaTime, CameraModeView);

	if (APawn* TargetPawn = Cast<APawn>(GetTargetActor()))
	{
		if (APlayerController* PC = TargetPawn->GetController<APlayerController>())
		{
			// PlayerController�� ControlRotation�� ���� CameraModeView�� ControlRotation���� ������Ʈ������
			// - SetControlRotation�� �ܿ�⺸�� �ѹ� �ڵ带 ����
			//   - �ش� �Լ��� PC�� Possess�ϰ��ִ� Pawn�� Root Component�� ControlRotation�� �ݿ��Ѵ�. (== �����ϰ��ִ� ĳ���Ϳ� ȸ���� ��Ų��)
			// ī�޶��� ���� ���� ��ŭ �װͿ� ���� ĳ������ ���⵵ ���缭 �ٲ��ش�.
			PC->SetControlRotation(CameraModeView.ControlRotation);
		}
	}

	// Camera�� Location�� Rotation�� �ݿ�����
	// - �ð� ������ SetWorldLocationAndRotation�� ���鼭 -> UpdateChildTransform���� ���� ���� ��õ�Ѵ�.
	// - SceneGraph ���� ������Ʈ�� ������ �� �ִ�.
	// �����ϰ� �����ϸ�, CameraComponent�� ���� Parent�� SceneGraph ���踦 ����Ͽ� ������Ʈ�� �����Ѵ�.
	// Camera�� ���� ������ ������Ʈ���� ���� ������ش�.
	SetWorldLocationAndRotation(CameraModeView.Location, CameraModeView.Rotation);

	// FOV ������Ʈ
	FieldOfView = CameraModeView.FieldOfView;

	/*
	* ���⼭ �츮�� ControlRotation vs Rotation�� �������� ������ �� �ִ�.
	* - ControlRotation�� PC�� �����ϴ� Pawn�� Rotation�� ������ ���̴�.
	* - Rotation�� Camera�� �ݿ��ϴ� Rotation�̴�.
	*/

	// FMinimalViewInfo�� ������Ʈ ���ش�
	// - CameraComponent�� ��ȭ ������ �ݿ��ؼ� ���� ���������� �ݿ��ϰ� �ȴ�.
	DesiredView.Location = CameraModeView.Location;
	DesiredView.Rotation = CameraModeView.Rotation;
	DesiredView.FOV = CameraModeView.FieldOfView;
	DesiredView.OrthoWidth = OrthoWidth;
	DesiredView.OrthoNearClipPlane = OrthoNearClipPlane;
	DesiredView.OrthoFarClipPlane = OrthoFarClipPlane;
	DesiredView.AspectRatio = AspectRatio;
	DesiredView.bConstrainAspectRatio = bConstrainAspectRatio;
	DesiredView.bUseFieldOfViewForLOD = bUseFieldOfViewForLOD;
	DesiredView.ProjectionMode = ProjectionMode;
	DesiredView.PostProcessBlendWeight = PostProcessBlendWeight;
	if (PostProcessBlendWeight > 0.0f)
	{
		DesiredView.PostProcessSettings = PostProcessSettings;
	}
}

void UJ1CameraComponent::UpdateCameraModes()
{
	check(CameraModeStack);

	// DetermineCameraModeDelegate�� CameraMode Class�� ��ȯ�Ѵ�.
	if (DetermineCameraModeDelegate.IsBound())
	{
		// ī�޶� ��带 �޾ƿͼ� ������ Stack�� �־��ش�.
		if (TSubclassOf<UJ1CameraMode> CameraMode = DetermineCameraModeDelegate.Execute())
		{
			CameraModeStack->PushCameraMode(CameraMode);
		}
	}
}
