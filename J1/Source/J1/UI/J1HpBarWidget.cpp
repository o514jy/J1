#include "J1HpBarWidget.h"
#include "Components/ProgressBar.h"

UJ1HpBarWidget::UJ1HpBarWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UJ1HpBarWidget::~UJ1HpBarWidget()
{
}

void UJ1HpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UJ1HpBarWidget::SetHpRatio(float InRatio)
{
	if (HpBar != nullptr)
	{
		HpBar->SetPercent(InRatio);
	}
}
