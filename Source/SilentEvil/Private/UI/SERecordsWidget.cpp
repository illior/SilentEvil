// Silent Evil Game, All Rights Reserved.

#include "UI/SERecordsWidget.h"

#include "Components/SEInventoryComponent.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UI/SERecordTitleWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogSERecords, All, All);

void USERecordsWidget::UpdataData()
{
	CreateRecords();

	CurrentIndex = 0;
	if (Records.Num() > 0)
	{
		Records[0]->SetState(ESEButtonState::Hovered);
	
		CurrentRecord = Records[0]->GetRecord();
		ShowPages();

		LeftArrow->SetVisibility(ESlateVisibility::Visible);
		RightArrow->SetVisibility(ESlateVisibility::Visible);

		Pages->SetVisibility(ESlateVisibility::Visible);
		RecordText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		LeftArrow->SetVisibility(ESlateVisibility::Hidden);
		RightArrow->SetVisibility(ESlateVisibility::Hidden);

		Pages->SetVisibility(ESlateVisibility::Hidden);
		RecordText->SetVisibility(ESlateVisibility::Hidden);
	}
}

bool USERecordsWidget::Cancel()
{
	Records.Empty();
	CurrentIndex = 0;

	return true;
}

void USERecordsWidget::MoveVertical(float Value)
{
	int32 NextIndex = CurrentIndex - Value;

	if (NextIndex < 0 || NextIndex >= Records.Num())
	{
		return;
	}

	Records[CurrentIndex]->SetState(ESEButtonState::Normal);

	CurrentIndex = NextIndex;
	Records[CurrentIndex]->SetState(ESEButtonState::Hovered);
	RecordTitles->ScrollWidgetIntoView(Records[CurrentIndex]);

	CurrentRecord = Records[CurrentIndex]->GetRecord();
	ShowPages();
}

void USERecordsWidget::MoveHorizontal(float Value)
{
	int32 NextIndex = CurrentPage + Value;

	if (NextIndex < 0 || NextIndex >= CurrentRecord.Pages.Num())
	{
		return;
	}

	CurrentPage = NextIndex;
	if (CurrentPage == 0)
	{
		LeftArrow->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		LeftArrow->SetVisibility(ESlateVisibility::Visible);
	}

	if (CurrentPage + 1 == CurrentRecord.Pages.Num())
	{
		RightArrow->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		RightArrow->SetVisibility(ESlateVisibility::Visible);
	}

	RecordText->SetText(CurrentRecord.Pages[CurrentPage]);
	Pages->SetText(FText::FromString(FString::Printf(TEXT("%i/%i"), CurrentPage + 1, CurrentRecord.Pages.Num())));
}

void USERecordsWidget::Apply()
{

}

void USERecordsWidget::CreateRecords()
{
	const APawn* Character = GetOwningPlayerPawn();
	if (Character == nullptr)
	{
		return;
	}

	USEInventoryComponent* CharacterInventory = Character->GetComponentByClass<USEInventoryComponent>();
	if (CharacterInventory == nullptr)
	{
		return;
	}

	RecordTitles->ClearChildren();

	TArray<FSEReadableRecord> Documents = CharacterInventory->GetRecords();
	if (Documents.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Empty records"));

		return;
	}

	for (int i = 0; i < Documents.Num(); i++)
	{
		USERecordTitleWidget* RecordWidget = CreateWidget<USERecordTitleWidget>(GetWorld(), RecordWidgetClass);
		if (RecordWidget == nullptr)
		{
			continue;
		}

		RecordWidget->SetRecord(Documents[i]);

		UPanelSlot* PanelSlot = RecordTitles->AddChild(RecordWidget);
		if (PanelSlot != nullptr && PanelSlot->IsA<UScrollBoxSlot>())
		{
			UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(PanelSlot);
			ScrollBoxSlot->SetPadding(RecordPadding);
		}

		Records.Add(RecordWidget);
	}

	UPanelSlot* PanelSlot = Records[Records.Num() - 1]->Slot;
	if (PanelSlot != nullptr && PanelSlot->IsA<UScrollBoxSlot>())
	{
		UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(PanelSlot);
		FMargin LastSlotPadding = RecordPadding;
		LastSlotPadding.Bottom = 0.0f;
		ScrollBoxSlot->SetPadding(LastSlotPadding);
	}
}

void USERecordsWidget::ShowPages()
{
	CurrentPage = 0;

	LeftArrow->SetVisibility(ESlateVisibility::Hidden);

	if (CurrentRecord.Pages.Num() > 1)
	{
		RightArrow->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		RightArrow->SetVisibility(ESlateVisibility::Hidden);
	}
	
	RecordText->SetText(CurrentRecord.Pages[CurrentPage]);
	Pages->SetText(FText::FromString(FString::Printf(TEXT("%i/%i"), CurrentPage + 1, CurrentRecord.Pages.Num())));
}
