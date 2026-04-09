#include "LootObjectBTN.h"

#include "EditorStyleSet.h"
#include "InventoryEcosystem/05_LootSystem/Data/LootData.h"

FText SLootObjectBTN::GetText() const
{
	return LootSpawner_Name;
}

void SLootObjectBTN::Construct(const FArguments& InArgs)
{
	LootObjectGUID = InArgs._LootObjectGUID;
	LootAssetEditor = InArgs._LootAssetEditorRef;

	if (auto TextData = LootAssetEditor.Pin()->GetLootDataRef()->GetLootSettingsData().Find(LootObjectGUID))
	{
		LootSpawner_Name = TextData->LootSpawner_Name;
	}
	OnClicked = InArgs._OnClicked;
	
	this->ChildSlot
	.Padding( FMargin(0) )
	[
		
		SNew(SBox)
		.HeightOverride(25.f)
		.WidthOverride(100.f)
		[
			SAssignNew(CategoryBTN, SButton)
			.ButtonStyle( FCoreStyle::Get(), "NoBorder")
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			.ClickMethod( EButtonClickMethod::MouseDown )
			.OnClicked( this, &SLootObjectBTN::Clicked )
			.ContentPadding(0.f)
			.ForegroundColor( FSlateColor::UseForeground() )
			.IsFocusable( false )
			[
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
				.BorderBackgroundColor(FLinearColor(0.0f, 0.65f, 0.8f))
				[
					SNew(SBorder)
					.Padding(3.f)
					.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
					.BorderBackgroundColor(this, &SLootObjectBTN::GetColor)
					[
						SNew(STextBlock)
						.Text( this, &SLootObjectBTN::GetText )
					]
				]
			]
		]
	];
}

FSlateColor SLootObjectBTN::GetColor() const
{
	return ((CategoryBTN.Get() && CategoryBTN.Get()->IsHovered()) ? FLinearColor(0.0f, 0.65f, 0.8f) : FLinearColor(0.1f, 0.37f, 0.48f));
}

FReply SLootObjectBTN::Clicked()
{
	if (OnClicked.IsBound())
	{
		FReply Reply = OnClicked.Execute();
#if WITH_ACCESSIBILITY
		FSlateApplicationBase::Get().GetAccessibleMessageHandler()->OnWidgetEventRaised(FSlateAccessibleMessageHandler::FSlateWidgetAccessibleEventArgs(AsShared(), EAccessibleEvent::Activate));
#endif
		return Reply;
	}
	else
	{
		return FReply::Handled();
	}
}
