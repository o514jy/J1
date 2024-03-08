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
		// 초기화 (BeginPlay와 같은)가 딱히 필요없는 객체로 NewObject로 할당
		CameraModeStack = NewObject<UJ1CameraModeStack>(this);
	}
}

void UJ1CameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	check(CameraModeStack);

	// 해당 함수는 HeroComponent에 있는 PawnData에서 DefaultCameraMode를 가져와서 
	// CameraModeStack에 추가하여, CameraMode 블렌딩을 준비한다.
	UpdateCameraModes();

	// EvaluateStack은 CameraModeStack에 있는 CameraMode를 업데이트(+블렌딩)하고
	// CameraModeStack을 Bottom-Top까지 업데이트된 CameraMode들의 Lerp를 진행해준다.
	// - 이에 대한 결과는 CameraModeView에 캐싱된다.
	FJ1CameraModeView CameraModeView;
	CameraModeStack->EvaluateStack(DeltaTime, CameraModeView);

	if (APawn* TargetPawn = Cast<APawn>(GetTargetActor()))
	{
		if (APlayerController* PC = TargetPawn->GetController<APlayerController>())
		{
			// PlayerController의 ControlRotation을 계산된 CameraModeView의 ControlRotation으로 업데이트해주자
			// - SetControlRotation을 외우기보다 한번 코드를 보자
			//   - 해당 함수는 PC가 Possess하고있는 Pawn의 Root Component에 ControlRotation을 반영한다. (== 조정하고있는 캐릭터에 회전을 시킨다)
			// 카메라의 값이 변한 만큼 그것에 맞춰 캐릭터의 방향도 맞춰서 바꿔준다.
			PC->SetControlRotation(CameraModeView.ControlRotation);
		}
	}

	// Camera의 Location과 Rotation을 반영하자
	// - 시간 있으면 SetWorldLocationAndRotation을 보면서 -> UpdateChildTransform까지 보는 것을 추천한다.
	// - SceneGraph 관계 업데이트를 이해할 수 있다.
	// 간단하게 정리하면, CameraComponent에 대해 Parent의 SceneGraph 관계를 고려하여 업데이트를 진행한다.
	// Camera에 맞춰 나머지 컴포넌트들의 값도 계산해준다.
	SetWorldLocationAndRotation(CameraModeView.Location, CameraModeView.Rotation);

	// FOV 업데이트
	FieldOfView = CameraModeView.FieldOfView;

	/*
	* 여기서 우리는 ControlRotation vs Rotation의 차이점을 이해할 수 있다.
	* - ControlRotation은 PC가 조정하는 Pawn의 Rotation을 적용한 값이다.
	* - Rotation은 Camera에 반영하는 Rotation이다.
	*/

	// FMinimalViewInfo를 업데이트 해준다
	// - CameraComponent의 변화 사항을 반영해서 최종 렌더링까지 반영하게 된다.
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

	// DetermineCameraModeDelegate는 CameraMode Class를 반환한다.
	if (DetermineCameraModeDelegate.IsBound())
	{
		// 카메라 모드를 받아와서 있으면 Stack에 넣어준다.
		if (TSubclassOf<UJ1CameraMode> CameraMode = DetermineCameraModeDelegate.Execute())
		{
			CameraModeStack->PushCameraMode(CameraMode);
		}
	}
}
