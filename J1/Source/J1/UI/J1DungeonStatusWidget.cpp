#include "UI/J1DungeonStatusWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

UJ1DungeonStatusWidget::UJ1DungeonStatusWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UJ1DungeonStatusWidget::~UJ1DungeonStatusWidget()
{
}

void UJ1DungeonStatusWidget::NativeConstruct()
{
	if (ClearRatioProgressBar == nullptr || ClearRatioText == nullptr)
		return;

	ClearRatioProgressBar->SetPercent(0.f);
	FText RatioText = FText::AsPercent(0.f);
	ClearRatioText->SetText(RatioText);
}

void UJ1DungeonStatusWidget::SetClearRatio(float InRatio)
{
	if (ClearRatioProgressBar == nullptr || ClearRatioText == nullptr)
		return;

	ClearRatioProgressBar->SetPercent(InRatio);

	FText RatioText = FText::AsPercent(InRatio);
	ClearRatioText->SetText(RatioText);
}
