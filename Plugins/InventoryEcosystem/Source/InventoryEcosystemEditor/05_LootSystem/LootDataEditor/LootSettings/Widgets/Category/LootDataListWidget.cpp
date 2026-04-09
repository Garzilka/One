#include "LootDataListWidget.h"

#include "EditorStyleSet.h"
#include "NavigationSystemTypes.h"
#include "InventoryEcosystemEditor/05_LootSystem/LootDataEditor/LootSettings/Widgets/Buttons/CategoryBTN.h"
#include "InventoryEcosystemEditor/05_LootSystem/LootDataEditor/LootSettings/Widgets/Buttons/LootObjectBTN.h"

void SLootDataListWidget::Construct(const FArguments& InArgs)
{
	CategoryData = InArgs._CategoryData;
	LootAssetEditor = InArgs._LootAssetEditorRef;

	this->ChildSlot
	[
		SNew(SBox)
		.Padding(10.f, 0.f, 0.f, 0.f)
		[
			SNew(SBox)
			.WidthOverride(300.f)
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Left)
			[
				CreateChildren(CategoryData, 0)
			]
		]
	];
}

TSharedRef<SWidget> SLootDataListWidget::CreateArray(TArray<FName> InLootSettings)
{
	TSharedRef<SVerticalBox> ResultBox = SNew(SVerticalBox);
	for (FName ObjectGUID : InLootSettings)		
	{
		ResultBox->AddSlot()
		.AutoHeight()
		.Padding(0.f, 0.f, 0.f, 5.f)
		[
			SNew(SBox)
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Left)
			[
				SNew(SLootObjectBTN)
				.LootObjectGUID(ObjectGUID)
				.LootAssetEditorRef(LootAssetEditor)
			]
		];
	}
	return ResultBox;
}



TSharedRef<SWidget> SLootDataListWidget::CreateChildren(FLootCategoryData Category, int32 Idx)
{
	int32 Next = Idx + 1;
	TSharedRef<SVerticalBox> ResultBox = SNew(SVerticalBox);
	for (auto Data : Category.Children)
	{
		FLootCategoryData NextCategory = (Data.Value.Get() != nullptr ? *Data.Value.Get() : FLootCategoryData());
		
		ResultBox->AddSlot()
		.Padding(0.f, 8.f, 0.f, 0.f)
		.AutoHeight()
		[
			SAssignNew(CategoryExpand, SExpandableArea)
			.HeaderContent()
			[
				SNew(SBox)
				.VAlign(VAlign_Top)
				.HAlign(HAlign_Left)
				[
					SNew(SCategoryBTN)
					.CategoryName(Data.Key)
					.LootAssetEditorRef(LootAssetEditor)
				]
			]
			.BodyContent()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.Padding(0.f, 5.f, 0.f, 0.f)
				[
					SNew(SBox)
					.HeightOverride(2.f)
					[
						SNew(SBorder)
						.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
						.BorderBackgroundColor(FLinearColor(0.2f, 0.2f, 0.2f))
					]
				]				
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SBox)
						.WidthOverride(2.f)
						[
							SNew(SBorder)
							.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
							.BorderBackgroundColor(FLinearColor(0.2f, 0.2f, 0.2f))
						]
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(10*Next, 10.f, 0.f, 2.f)
						[
							SNew(SBox)
							[
								SNew(SLootDataListWidget)
								.CategoryData(NextCategory)
								.LootAssetEditorRef(LootAssetEditor)
							]
						]
						+ SVerticalBox::Slot()
						.Padding(1, 5.f, 0.f, 0.f)
						.AutoHeight()
						[
							SNew(SBox)
							.VAlign(VAlign_Top)
							.HAlign(HAlign_Left)
							[
								CreateArray(Data.Value.Get()->ObjectGUIDS)
							]
						]
					]
				]
				+ SVerticalBox::Slot()
				[
					SNew(SBox)
					.HeightOverride(2.f)
					[
						SNew(SBorder)
						.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
						.BorderBackgroundColor(FLinearColor(0.2f, 0.2f, 0.2f))
					]
				]
			]
		];
	}
	
	return ResultBox;
}
