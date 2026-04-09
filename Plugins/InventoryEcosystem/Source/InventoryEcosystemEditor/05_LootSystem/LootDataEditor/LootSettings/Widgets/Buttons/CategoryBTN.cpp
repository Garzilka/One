#include "CategoryBTN.h"

#include "EditorStyleSet.h"

FText SCategoryBTN::GetText() const
{
	return FText::FromName(CategoryName);
}

void SCategoryBTN::Construct(const FArguments& InArgs)
{
	StyleSet = InArgs._StyleSet;
	OnClicked = InArgs._OnClicked;
	CategoryName = InArgs._CategoryName;
	LootAssetEditor = InArgs._LootAssetEditorRef;
	
	this->ChildSlot
	.Padding( FMargin(0) )
	[
		SNew(SBox)
		.HeightOverride(25.f)
		.WidthOverride(100.f)
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
			.BorderBackgroundColor(FLinearColor(0.4f, 0.4f, 0.4f))
			[
				SNew(SBorder)
				.Padding(3.f)
				.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
				.BorderBackgroundColor(this, &SCategoryBTN::GetColor)
				[
					SAssignNew(CategoryBTN, SButton)
					.ButtonStyle( FCoreStyle::Get(), "NoBorder")
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Center)
					.ClickMethod( EButtonClickMethod::MouseDown )
					.OnClicked( this, &SCategoryBTN::Clicked )
					.ContentPadding(0.f)
					.ForegroundColor( FSlateColor::UseForeground() )
					.IsFocusable( false )
					[
						SNew(STextBlock)
						.Text( this, &SCategoryBTN::GetText )
					]
				]
			]
		]
	];
}

void SCategoryBTN::SetOnClicked(FOnClicked InOnClicked)
{
	OnClicked = InOnClicked;
}

FSlateColor SCategoryBTN::GetColor() const
{
	return ((CategoryBTN.Get() && CategoryBTN.Get()->IsHovered()) ? FLinearColor(0.4f, 0.4f, 0.4f) : FLinearColor(0.2f, 0.2f, 0.2f));
}

FMargin SCategoryBTN::GetExpanderPadding() const
{
	return FMargin( 0, 0,0,0 );
}

EVisibility SCategoryBTN::GetExpanderVisibility() const
{
	return EVisibility::Visible;
}

FReply SCategoryBTN::Clicked()
{
	// Recurse the expansion if "shift" is being pressed
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

const FSlateBrush* SCategoryBTN::GetImage() const
{
	return FEditorStyle::GetBrush("PropertyWindow.Button_AddToArray"); 
}