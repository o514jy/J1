#include "UI/J1PopupWidget.h"
#include "UI/J1UIManager.h"

void UJ1PopupWidget::SetbIsOpened(bool InFlag)
{
	if (InFlag == true)
		SetVisibility(ESlateVisibility::Visible);
	else
		SetVisibility(ESlateVisibility::Hidden);

	bIsOpened = InFlag;
}

bool UJ1PopupWidget::GetbIsOpened()
{
	return bIsOpened;
}

void UJ1PopupWidget::ClosePopupWidget()
{
	GetManager(UI)->ClosePopupWidget(UIType);
}
