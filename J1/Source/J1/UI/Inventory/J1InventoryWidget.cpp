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
        // ��ư�� ���� �� �巡�� ����
        InventoryNameButton->OnPressed.AddDynamic(this, &UJ1InventoryWidget::OnButtonPressed);
        // ��ư���� ���� �� �� �巡�� ����
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

    // ���� ��Ŀ ������ ��������
    UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(RootSizeBox->Slot);
    if (CanvasSlot)
    {
        FVector2D AnchorPosition = CanvasSlot->GetAnchors().Minimum; // ��Ŀ�� �ּҰ� (0, 0) ~ (1, 1) ���� ����
        FVector2D SlotSize = CanvasSlot->GetSize(); // ���� ������ ũ��

        // ��Ŀ ��ġ�� �������� ���콺�� ������� ��ġ�� ���
        FVector2D AnchoredPosition = MousePosition - AnchorPosition * SlotSize;

        MousePos = AnchoredPosition - CanvasSlot->GetPosition(); // ��Ŀ �������� ���콺 ��ġ ����
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

    // ���� ��Ŀ ������ ��������
    UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(RootSizeBox->Slot);
    if (CanvasSlot)
    {
        FVector2D AnchorPosition = CanvasSlot->GetAnchors().Minimum; // ��Ŀ�� �ּҰ�
        FVector2D SlotSize = CanvasSlot->GetSize(); // ������ ũ��

        // ��Ŀ ��ġ�� �������� ���콺 ��ġ�� ��ȯ
        FVector2D AnchoredPosition = MousePosition - AnchorPosition * SlotSize;

        // ��Ŀ�� ����Ͽ� ��ġ ������Ʈ
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
            Viewport->GetMousePos(IntMousePos); // ����Ʈ �󿡼� ���콺�� ���� ��ǥ�� �����ɴϴ�.

            // FIntPoint�� FVector2D�� ��ȯ
            OutPos = FVector2D(IntMousePos.X, IntMousePos.Y);
            return true;
        }
    }

    return false;
}

