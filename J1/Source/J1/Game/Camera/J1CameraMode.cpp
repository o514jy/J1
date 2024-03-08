#include "J1CameraMode.h"
#include "Camera/J1PlayerCameraManager.h"
#include "Camera/J1CameraComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(J1CameraMode)

FJ1CameraModeView::FJ1CameraModeView()
	: Location(ForceInit)
	, Rotation(ForceInit)
	, ControlRotation(ForceInit)
	, FieldOfView(J1_CAMERA_DEFAULT_FOV)
{
}

void FJ1CameraModeView::Blend(const FJ1CameraModeView& Other, float OtherWeight)
{
	if (OtherWeight <= 0.0f)
	{
		return;
	}
	else if (OtherWeight >= 1.0f)
	{
		// Weight가 1.0이면 Other를 덮어쓰면 된다
		*this = Other;
		return;
	}

	// Location + OtherWeight * (Other.Location - Location);
	Location = FMath::Lerp(Location, Other.Location, OtherWeight);

	// Location과 같은 방식 Lerp (ControlRotation과 FieldOfView도 같음)
	const FRotator DeltaRotation = (Other.Rotation - Rotation).GetNormalized();
	Rotation = Rotation + (OtherWeight * DeltaRotation);

	const FRotator DeltaControlRotation = (Other.ControlRotation - ControlRotation).GetNormalized();
	ControlRotation = ControlRotation + (OtherWeight * DeltaControlRotation);

	FieldOfView = FMath::Lerp(FieldOfView, Other.FieldOfView, OtherWeight);
}

UJ1CameraMode::UJ1CameraMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FieldOfView = J1_CAMERA_DEFAULT_FOV;
	ViewPitchMin = J1_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = J1_CAMERA_DEFAULT_PITCH_MAX;

	BlendTime = 0.0f;
	BlendAlpha = 1.0f;
	BlendWeight = 1.0f;

	BlendFunction = EJ1CameraModeBlendFunction::EaseOut;
	BlendExponent = 4.0f;
}

UJ1CameraComponent* UJ1CameraMode::GetJ1CameraComponent() const
{
	// 우리가 앞서 UJ1CameraMode를 생성하는 곳은 UJ1CameraModeStack이었다.
	// - 해당 코드를 보면 GetOuter()를 J1CameraMode의 J1CameraComponent로 설정하였다.
	// - UJ1CameraModeStack::GetCameraModeInstance() 확인
	return CastChecked<UJ1CameraComponent>(GetOuter());
}

AActor* UJ1CameraMode::GetTargetActor() const
{
	const UJ1CameraComponent* J1CameraComponent = GetJ1CameraComponent();
	return J1CameraComponent->GetTargetActor();
}

FVector UJ1CameraMode::GetPivotLocation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		// BaseEyeHeight를 고려하여, ViewLocation을 반환한다.
		return TargetPawn->GetPawnViewLocation();
	}

	return TargetActor->GetActorLocation();
}

FRotator UJ1CameraMode::GetPivotRotation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		// GetViewRotation() 확인, 보통 Pawn의 ControlRotation을 반환
		return TargetPawn->GetViewRotation();
	}

	return TargetActor->GetActorRotation();
}

void UJ1CameraMode::UpdateView(float DeltaTime)
{
	// CameraMode를 가지고 있는 CameraComponent의 Owner인 Character(Pawn)을 활용하여, PivotLocation/Rotation을 반환한다.
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	// Pitch(위아래 X축 회전) 값에 대해 Min/Max를 Clamp시킨다.
	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	// FJ1CameraModeView에 PivotLocation/Rotation 설정
	View.Location = PivotLocation;
	View.Rotation = PivotRotation;

	// PivotRotation을 똑같이 ControlRotation으로 활용
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;

	// 정리하면 Character의 Location과 ControlRotation을 활용하여 View를 업데이트한다.
}

void UJ1CameraMode::UpdateBlending(float DeltaTime)
{
	// BlendAlpha를 DeltaTime을 통해 계산
	if (BlendTime > 0.f)
	{
		// BlendTime은 Blending 과정 총 시간(초)
		// - BlendAlpha는 0 -> 1로 변화하는 과정
		// - DeltaTime을 활용하여, BlendTime을 1로 볼 경우, 진행 정도를 누적
		BlendAlpha += (DeltaTime / BlendTime);
	}
	else
	{
		BlendAlpha = 1.0f;
	}

	// BlendWeight를 [0,1]를 BlendFunction에 맞게 재매핑
	const float Exponent = (BlendExponent > 0.0f) ? BlendExponent : 1.0f;
	switch (BlendFunction)
	{
	case EJ1CameraModeBlendFunction::Linear: // 직선, 일정하게 값이 바뀜
		BlendWeight = BlendAlpha;
		break;
	case EJ1CameraModeBlendFunction::EaseIn: // 지수 함수, 처음엔 완만하다가 나중에 가파름
		BlendWeight = FMath::InterpEaseIn(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	case EJ1CameraModeBlendFunction::EaseOut: // 로그 함수, 처음에 가파르다가 나중에 완만
		BlendWeight = FMath::InterpEaseOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	case EJ1CameraModeBlendFunction::EaseInOut: // 0.5를 기준으로 앞은 지수 함수, 뒤는 로그 함수, 처음과 끝은 완만하고 중간이 가파름
		BlendWeight = FMath::InterpEaseInOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	default:
		checkf(false, TEXT("UpdateBlending: Invalid BlendFunction [%d]/n"), (uint8)BlendFunction);
		break;
	}

}

void UJ1CameraMode::UpdateCameraMode(float DeltaTime)
{
	// Actor를 활용하여, Pivot[Location|Rotation]을 계산하여, View를 업데이트
	UpdateView(DeltaTime);

	// BlendWeight를 DeltaTime을 활용하여 BlendAlpha를 계산한 후 BlendFunction에 맞게 재-매핑해서 최종 계산
	UpdateBlending(DeltaTime);
}

UJ1CameraModeStack::UJ1CameraModeStack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

UJ1CameraMode* UJ1CameraModeStack::GetCameraModeInstance(TSubclassOf<UJ1CameraMode>& CameraModeClass)
{
	check(CameraModeClass);

	// CameraModeInstances에서 먼저 생성되어있는지 확인 후 반환한다
	for (UJ1CameraMode* CameraMode : CameraModeInstances)
	{
		// CameraMode는 UClass를 비교한다
		// - 즉 CameraMode는 클래스 타입에 하나만 생기게 된다.
		if ((CameraMode != nullptr) && (CameraMode->GetClass() == CameraModeClass))
		{
			return CameraMode;
		}
	}

	// CameraModeClass에 알맞는 CameraMode의 인스턴스가 없다면 생성한다.
	// 원래 CameraMode의 Outer는 CameraModeStack이지만 한단계 더 올려서 CameraModeStack의 Outer로 맞춰준다.
	UJ1CameraMode* NewCameraMode = NewObject<UJ1CameraMode>(GetOuter()/*CameraComponent*/, CameraModeClass, NAME_None, RF_NoFlags);
	check(NewCameraMode);

	// 여기서 우리는 CameraModeInstances는 CameraModeClass에 맞는 인스턴스를 관리하는 컨테이너라는 것을 알 수 있다.
	CameraModeInstances.Add(NewCameraMode);

	return NewCameraMode;
}

void UJ1CameraModeStack::PushCameraMode(TSubclassOf<UJ1CameraMode>& CameraModeClass)
{
	// CameraModeClass ex) CM_ThirdPerson
	if (!CameraModeClass)
	{
		return;
	}

	// CameraModeClass에 맞는 CameraMode의 Instance를 가져온다.
	UJ1CameraMode* CameraMode = GetCameraModeInstance(CameraModeClass);
	check(CameraMode);

	int32 StackSize = CameraModeStack.Num();
	if ((StackSize > 0) && (CameraModeStack[0] == CameraMode))
	{
		// CameraModeStack[0] 가장 최근에 이미 CameraMode가 Stacking되었으므로 그냥 리턴
		// 가장 최근에 넣어둔 CameraMode가 지금 넣을 것과 같다면 스킵한다는 뜻
		return;
	}

	// BlendWeight는 0부터 1까지 가는 과정, 1이면 Stack에서 나갈 준비가 되었다는 것이다.
	// ExistingStackIndex는 CameraModeStack에서 CameraMode에 맞는 Index를 찾음
	// ExistingStackContribution은 위에서 아래로 최종 BlendWeight값을 찾기 위해 초기값으로 1.0으로 설정
	// BlendWeight의 각각의 비율을 계산한 것이 ExistingStackContribution이고 다 더하면 1이 된다.
	int32 ExistingStackIndex = INDEX_NONE;
	float ExistingStackContribution = 1.0f;

	/*
	* BlendWeight	|	ExistingStackContribution	|	ExistingStackContribution (accumulated)
	* 0.1f			|	(1.0f) * 0.1f = 0.1f		|	(1.0f - 0.1f) = 0.9f
	* 0.3f			|	(0.9f) * 0.3f = 0.27f		|	(1.0f - 0.3f) * 0.9f = 0.63f
	* 0.6f			|	(0.63f) * 0.6f = 0.378f		|	(1.0f - 0.6f) * 0.63f = 0.252f
	* 1.0f			|	(0.252f) * 1.0f = 0.252f	|
	*				|	0.1f + 0.27f + 0.378f + 0.252f = 1.0f
	*/
	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		if (CameraModeStack[StackIndex] == CameraMode)
		{
			ExistingStackIndex = StackIndex;
			// BlendWeight를 CameraMode에 추가해주자
			// - 여기서 ExistingStackContribution은 우리가 찾는 CameraMode를 찾았으니까
			// 누적된 BlendWeight와 함께 BlendWeight를 곱하면서 루프를 빠져나온다.
			ExistingStackContribution *= CameraMode->BlendWeight;
			break;
		}
		else
		{
			// 우리가 원하는 CameraMode가 아니니까 InvBlendWeight = (1.0 - BlendWeight)를 곱해서 누적시킨다.
			ExistingStackContribution *= (1.0f - CameraModeStack[StackIndex]->BlendWeight);
		}
	}
	// 해당 루프의 동작 원리는 앞서 적어놓은 표를 확인해보며 이해해보자

	// 우리는 CameraMode를 Top으로 반영하기 위해 당연히 중간에 있었다면 제거하고 다시 Push해줘야 한다.
	if (ExistingStackIndex != INDEX_NONE)
	{
		CameraModeStack.RemoveAt(ExistingStackIndex);
		StackSize--;
	}
	else
	{
		// 없었다면 Contribution은 0으로 셋팅해준다.
		ExistingStackContribution = 0.0f;
	}

	// BlendTime이 0보다 크다는 것은 Blend를 얼마 시간동안 진행함을 의미. 따라서 ExistingStackContribution을 적용한다.
	// - Blend를 적용하지 않는다면, BlendWeight를 1.0으로 넣어 새로 넣는 CameraMode만 적용한다.
	const bool bShouldBlend = ((CameraMode->BlendTime > 0.f) && (StackSize > 0));
	const float BlendWeight = (bShouldBlend ? ExistingStackContribution : 1.0f);
	CameraMode->BlendWeight = BlendWeight;

	CameraModeStack.Insert(CameraMode, 0);

	// 마지막엔 항상 1.0이 되어야 한다
	CameraModeStack.Last()->BlendWeight = 1.0f;
}

void UJ1CameraModeStack::UpdateStack(float DeltaTime)
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	// CameraModeStack을 순회하며, CameraMode를 업데이트한다.
	int32 RemoveCount = 0;
	int32 RemoveIndex = INDEX_NONE;
	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		UJ1CameraMode* CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		// UpdateCameraMode를 확인해보자
		// 각각 카메라에 대한 BlendWeight를 업데이트한다.
		CameraMode->UpdateCameraMode(DeltaTime);

		// 만약 하나라도 CameraMode의 BlendWeight가 1.0에 도달했다면, 그 이후 CameraMode를 제거한다.
		// 1 2 3 4 5 중에 3이 1.0이면 그 뒤에 4,5를 지우는 느낌.
		if (CameraMode->BlendWeight >= 1.0f)
		{
			RemoveIndex = (StackIndex + 1);
			RemoveCount = (StackSize - RemoveIndex);
			break;
		}
	}

	if (RemoveCount > 0)
	{
		// 생각해보면 이거 때문에 Pop하고 Push를 안한거일지도?
		// 뒤에꺼를 지우는 것이 복사가 덜 일어나서 더 효율적이니까
		CameraModeStack.RemoveAt(RemoveIndex, RemoveCount);
	}
}

void UJ1CameraModeStack::BlendStack(FJ1CameraModeView& OutCameraModeView) const
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	// CameraModeStack은 Bottom -> Top 순서로 Blend를 진행한다.
	const UJ1CameraMode* CameraMode = CameraModeStack[StackSize - 1];
	check(CameraMode);

	// 참고로 가장 아래 (Bottom)은 BlendWeight가 1.0이다.
	OutCameraModeView = CameraMode->View;

	// 이미 Index = [StackSize - 1] 이미 OutCameraModeView로 지정했으므로, StackSize - 2부터 순회한다.
	for (int32 StackIndex = (StackSize - 2); StackIndex >= 0; --StackIndex)
	{
		CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		// J1CameraModeView Blend 함수를 구현하자:
		OutCameraModeView.Blend(CameraMode->View, CameraMode->BlendWeight);
	}
}

void UJ1CameraModeStack::EvaluateStack(float DeltaTime, FJ1CameraModeView& OutCameraModeView)
{
	// Top -> Bottom [0 -> Num]까지 순차적으로 Stack에 있는 CameraMode 업데이트
	UpdateStack(DeltaTime);

	// Bottom -> Top까지 CameraModeStack에 대해 Blending 진행
	BlendStack(OutCameraModeView);
}
