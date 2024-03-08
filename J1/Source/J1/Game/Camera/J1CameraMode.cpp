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
		// Weight�� 1.0�̸� Other�� ����� �ȴ�
		*this = Other;
		return;
	}

	// Location + OtherWeight * (Other.Location - Location);
	Location = FMath::Lerp(Location, Other.Location, OtherWeight);

	// Location�� ���� ��� Lerp (ControlRotation�� FieldOfView�� ����)
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
	// �츮�� �ռ� UJ1CameraMode�� �����ϴ� ���� UJ1CameraModeStack�̾���.
	// - �ش� �ڵ带 ���� GetOuter()�� J1CameraMode�� J1CameraComponent�� �����Ͽ���.
	// - UJ1CameraModeStack::GetCameraModeInstance() Ȯ��
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
		// BaseEyeHeight�� ����Ͽ�, ViewLocation�� ��ȯ�Ѵ�.
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
		// GetViewRotation() Ȯ��, ���� Pawn�� ControlRotation�� ��ȯ
		return TargetPawn->GetViewRotation();
	}

	return TargetActor->GetActorRotation();
}

void UJ1CameraMode::UpdateView(float DeltaTime)
{
	// CameraMode�� ������ �ִ� CameraComponent�� Owner�� Character(Pawn)�� Ȱ���Ͽ�, PivotLocation/Rotation�� ��ȯ�Ѵ�.
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	// Pitch(���Ʒ� X�� ȸ��) ���� ���� Min/Max�� Clamp��Ų��.
	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	// FJ1CameraModeView�� PivotLocation/Rotation ����
	View.Location = PivotLocation;
	View.Rotation = PivotRotation;

	// PivotRotation�� �Ȱ��� ControlRotation���� Ȱ��
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;

	// �����ϸ� Character�� Location�� ControlRotation�� Ȱ���Ͽ� View�� ������Ʈ�Ѵ�.
}

void UJ1CameraMode::UpdateBlending(float DeltaTime)
{
	// BlendAlpha�� DeltaTime�� ���� ���
	if (BlendTime > 0.f)
	{
		// BlendTime�� Blending ���� �� �ð�(��)
		// - BlendAlpha�� 0 -> 1�� ��ȭ�ϴ� ����
		// - DeltaTime�� Ȱ���Ͽ�, BlendTime�� 1�� �� ���, ���� ������ ����
		BlendAlpha += (DeltaTime / BlendTime);
	}
	else
	{
		BlendAlpha = 1.0f;
	}

	// BlendWeight�� [0,1]�� BlendFunction�� �°� �����
	const float Exponent = (BlendExponent > 0.0f) ? BlendExponent : 1.0f;
	switch (BlendFunction)
	{
	case EJ1CameraModeBlendFunction::Linear: // ����, �����ϰ� ���� �ٲ�
		BlendWeight = BlendAlpha;
		break;
	case EJ1CameraModeBlendFunction::EaseIn: // ���� �Լ�, ó���� �ϸ��ϴٰ� ���߿� ���ĸ�
		BlendWeight = FMath::InterpEaseIn(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	case EJ1CameraModeBlendFunction::EaseOut: // �α� �Լ�, ó���� ���ĸ��ٰ� ���߿� �ϸ�
		BlendWeight = FMath::InterpEaseOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	case EJ1CameraModeBlendFunction::EaseInOut: // 0.5�� �������� ���� ���� �Լ�, �ڴ� �α� �Լ�, ó���� ���� �ϸ��ϰ� �߰��� ���ĸ�
		BlendWeight = FMath::InterpEaseInOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	default:
		checkf(false, TEXT("UpdateBlending: Invalid BlendFunction [%d]/n"), (uint8)BlendFunction);
		break;
	}

}

void UJ1CameraMode::UpdateCameraMode(float DeltaTime)
{
	// Actor�� Ȱ���Ͽ�, Pivot[Location|Rotation]�� ����Ͽ�, View�� ������Ʈ
	UpdateView(DeltaTime);

	// BlendWeight�� DeltaTime�� Ȱ���Ͽ� BlendAlpha�� ����� �� BlendFunction�� �°� ��-�����ؼ� ���� ���
	UpdateBlending(DeltaTime);
}

UJ1CameraModeStack::UJ1CameraModeStack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

UJ1CameraMode* UJ1CameraModeStack::GetCameraModeInstance(TSubclassOf<UJ1CameraMode>& CameraModeClass)
{
	check(CameraModeClass);

	// CameraModeInstances���� ���� �����Ǿ��ִ��� Ȯ�� �� ��ȯ�Ѵ�
	for (UJ1CameraMode* CameraMode : CameraModeInstances)
	{
		// CameraMode�� UClass�� ���Ѵ�
		// - �� CameraMode�� Ŭ���� Ÿ�Կ� �ϳ��� ����� �ȴ�.
		if ((CameraMode != nullptr) && (CameraMode->GetClass() == CameraModeClass))
		{
			return CameraMode;
		}
	}

	// CameraModeClass�� �˸´� CameraMode�� �ν��Ͻ��� ���ٸ� �����Ѵ�.
	// ���� CameraMode�� Outer�� CameraModeStack������ �Ѵܰ� �� �÷��� CameraModeStack�� Outer�� �����ش�.
	UJ1CameraMode* NewCameraMode = NewObject<UJ1CameraMode>(GetOuter()/*CameraComponent*/, CameraModeClass, NAME_None, RF_NoFlags);
	check(NewCameraMode);

	// ���⼭ �츮�� CameraModeInstances�� CameraModeClass�� �´� �ν��Ͻ��� �����ϴ� �����̳ʶ�� ���� �� �� �ִ�.
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

	// CameraModeClass�� �´� CameraMode�� Instance�� �����´�.
	UJ1CameraMode* CameraMode = GetCameraModeInstance(CameraModeClass);
	check(CameraMode);

	int32 StackSize = CameraModeStack.Num();
	if ((StackSize > 0) && (CameraModeStack[0] == CameraMode))
	{
		// CameraModeStack[0] ���� �ֱٿ� �̹� CameraMode�� Stacking�Ǿ����Ƿ� �׳� ����
		// ���� �ֱٿ� �־�� CameraMode�� ���� ���� �Ͱ� ���ٸ� ��ŵ�Ѵٴ� ��
		return;
	}

	// BlendWeight�� 0���� 1���� ���� ����, 1�̸� Stack���� ���� �غ� �Ǿ��ٴ� ���̴�.
	// ExistingStackIndex�� CameraModeStack���� CameraMode�� �´� Index�� ã��
	// ExistingStackContribution�� ������ �Ʒ��� ���� BlendWeight���� ã�� ���� �ʱⰪ���� 1.0���� ����
	// BlendWeight�� ������ ������ ����� ���� ExistingStackContribution�̰� �� ���ϸ� 1�� �ȴ�.
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
			// BlendWeight�� CameraMode�� �߰�������
			// - ���⼭ ExistingStackContribution�� �츮�� ã�� CameraMode�� ã�����ϱ�
			// ������ BlendWeight�� �Բ� BlendWeight�� ���ϸ鼭 ������ �������´�.
			ExistingStackContribution *= CameraMode->BlendWeight;
			break;
		}
		else
		{
			// �츮�� ���ϴ� CameraMode�� �ƴϴϱ� InvBlendWeight = (1.0 - BlendWeight)�� ���ؼ� ������Ų��.
			ExistingStackContribution *= (1.0f - CameraModeStack[StackIndex]->BlendWeight);
		}
	}
	// �ش� ������ ���� ������ �ռ� ������� ǥ�� Ȯ���غ��� �����غ���

	// �츮�� CameraMode�� Top���� �ݿ��ϱ� ���� �翬�� �߰��� �־��ٸ� �����ϰ� �ٽ� Push����� �Ѵ�.
	if (ExistingStackIndex != INDEX_NONE)
	{
		CameraModeStack.RemoveAt(ExistingStackIndex);
		StackSize--;
	}
	else
	{
		// �����ٸ� Contribution�� 0���� �������ش�.
		ExistingStackContribution = 0.0f;
	}

	// BlendTime�� 0���� ũ�ٴ� ���� Blend�� �� �ð����� �������� �ǹ�. ���� ExistingStackContribution�� �����Ѵ�.
	// - Blend�� �������� �ʴ´ٸ�, BlendWeight�� 1.0���� �־� ���� �ִ� CameraMode�� �����Ѵ�.
	const bool bShouldBlend = ((CameraMode->BlendTime > 0.f) && (StackSize > 0));
	const float BlendWeight = (bShouldBlend ? ExistingStackContribution : 1.0f);
	CameraMode->BlendWeight = BlendWeight;

	CameraModeStack.Insert(CameraMode, 0);

	// �������� �׻� 1.0�� �Ǿ�� �Ѵ�
	CameraModeStack.Last()->BlendWeight = 1.0f;
}

void UJ1CameraModeStack::UpdateStack(float DeltaTime)
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	// CameraModeStack�� ��ȸ�ϸ�, CameraMode�� ������Ʈ�Ѵ�.
	int32 RemoveCount = 0;
	int32 RemoveIndex = INDEX_NONE;
	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		UJ1CameraMode* CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		// UpdateCameraMode�� Ȯ���غ���
		// ���� ī�޶� ���� BlendWeight�� ������Ʈ�Ѵ�.
		CameraMode->UpdateCameraMode(DeltaTime);

		// ���� �ϳ��� CameraMode�� BlendWeight�� 1.0�� �����ߴٸ�, �� ���� CameraMode�� �����Ѵ�.
		// 1 2 3 4 5 �߿� 3�� 1.0�̸� �� �ڿ� 4,5�� ����� ����.
		if (CameraMode->BlendWeight >= 1.0f)
		{
			RemoveIndex = (StackIndex + 1);
			RemoveCount = (StackSize - RemoveIndex);
			break;
		}
	}

	if (RemoveCount > 0)
	{
		// �����غ��� �̰� ������ Pop�ϰ� Push�� ���Ѱ�������?
		// �ڿ����� ����� ���� ���簡 �� �Ͼ�� �� ȿ�����̴ϱ�
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

	// CameraModeStack�� Bottom -> Top ������ Blend�� �����Ѵ�.
	const UJ1CameraMode* CameraMode = CameraModeStack[StackSize - 1];
	check(CameraMode);

	// ����� ���� �Ʒ� (Bottom)�� BlendWeight�� 1.0�̴�.
	OutCameraModeView = CameraMode->View;

	// �̹� Index = [StackSize - 1] �̹� OutCameraModeView�� ���������Ƿ�, StackSize - 2���� ��ȸ�Ѵ�.
	for (int32 StackIndex = (StackSize - 2); StackIndex >= 0; --StackIndex)
	{
		CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		// J1CameraModeView Blend �Լ��� ��������:
		OutCameraModeView.Blend(CameraMode->View, CameraMode->BlendWeight);
	}
}

void UJ1CameraModeStack::EvaluateStack(float DeltaTime, FJ1CameraModeView& OutCameraModeView)
{
	// Top -> Bottom [0 -> Num]���� ���������� Stack�� �ִ� CameraMode ������Ʈ
	UpdateStack(DeltaTime);

	// Bottom -> Top���� CameraModeStack�� ���� Blending ����
	BlendStack(OutCameraModeView);
}
