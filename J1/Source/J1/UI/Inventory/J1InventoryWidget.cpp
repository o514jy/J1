#include "UI/Inventory/J1InventoryWidget.h"
#include "J1EquipmentSlotsWidget.h"
#include "J1InventorySlotsWidget.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/SizeBoxSlot.h"

#include "Components/CanvasPanelSlot.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/GameViewportClient.h"

void UJ1InventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

    if (InventoryNameButton)
    {
        // 버튼을 누를 때 드래그 시작
        InventoryNameButton->OnPressed.AddDynamic(this, &UJ1InventoryWidget::OnButtonPressed);
        // 버튼에서 손을 뗄 때 드래그 중지
        InventoryNameButton->OnReleased.AddDynamic(this, &UJ1InventoryWidget::OnButtonReleased);
    }
    
}

void UJ1InventoryWidget::NativeDestruct()
{
    Super::NativeDestruct();
}

void UJ1InventoryWidget::OnButtonPressed()
{
    FVector2D MousePosition;
    MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

    // 현재 앵커 정보를 가져오기
    UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(RootSizeBox->Slot);
    if (CanvasSlot)
    {
        FVector2D AnchorPosition = CanvasSlot->GetAnchors().Minimum; // 앵커의 최소값 (0, 0) ~ (1, 1) 사이 비율
        FVector2D SlotSize = CanvasSlot->GetSize(); // 현재 슬롯의 크기

        // 앵커 위치를 기준으로 마우스의 상대적인 위치를 계산
        FVector2D AnchoredPosition = MousePosition - AnchorPosition * SlotSize;

        MousePos = AnchoredPosition - CanvasSlot->GetPosition(); // 앵커 기준으로 마우스 위치 저장
    }

    UpdatePosition();
}

void UJ1InventoryWidget::OnButtonReleased()
{
    GetWorld()->GetTimerManager().ClearTimer(UpdateHandle);
}

void UJ1InventoryWidget::UpdatePosition()
{
    GetWorld()->GetTimerManager().SetTimer(UpdateHandle, this, &UJ1InventoryWidget::UpdatePosition, 0.01f, false);

    FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

    // 현재 앵커 정보를 가져오기
    UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(RootSizeBox->Slot);
    if (CanvasSlot)
    {
        FVector2D AnchorPosition = CanvasSlot->GetAnchors().Minimum; // 앵커의 최소값
        FVector2D SlotSize = CanvasSlot->GetSize(); // 슬롯의 크기

        // 앵커 위치를 기준으로 마우스 위치를 변환
        FVector2D AnchoredPosition = MousePosition - AnchorPosition * SlotSize;

        // 앵커를 고려하여 위치 업데이트
        CanvasSlot->SetPosition(AnchoredPosition - MousePos);
    }
}

bool UJ1InventoryWidget::GetMousePositionOnViewport(FVector2D& OutPos)
{
    if (GEngine && GEngine->GameViewport)
    {
        FViewport* Viewport = GEngine->GameViewport->Viewport;
        if (Viewport)
        {
            FIntPoint IntMousePos;
            Viewport->GetMousePos(IntMousePos); // 뷰포트 상에서 마우스의 정수 좌표를 가져옵니다.

            // FIntPoint를 FVector2D로 변환
            OutPos = FVector2D(IntMousePos.X, IntMousePos.Y);
            return true;
        }
    }

    return false;
}

