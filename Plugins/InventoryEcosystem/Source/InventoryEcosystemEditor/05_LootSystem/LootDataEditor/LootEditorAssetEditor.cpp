#include "LootEditorAssetEditor.h"

#include "EditorStyleSet.h"
#include "ApplicationMode/LootData/LootEditorAppMode_LootData.h"
#include "ApplicationMode/LootProfile/LootEditorAppMode_LootProfile.h"
#include "ApplicationMode/LootSettings/LootEditorAppMode_LootSettings.h"
#include "InventoryEcosystem/05_LootSystem/Data/LootData.h"


#pragma region ToolKit

FName FLootEditorAssetEditor::GetToolkitFName() const
{
	return FName("Loot Editor");
}

FText FLootEditorAssetEditor::GetBaseToolkitName() const
{
	return GetToolkitName();
}

FString FLootEditorAssetEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("Loot Editor");
}

FLinearColor FLootEditorAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(FColor::FromHex("00AFFF"));
}

#pragma endregion

#pragma region Editor

void FLootEditorAssetEditor::InitLootAssetEditor(const EToolkitMode::Type Mode,	const TSharedPtr<class IToolkitHost>& InitToolkitHost, ULootData* InLootData)
{
	LootDataRef = InLootData;

	InitAssetEditor(Mode, InitToolkitHost, FName("Loot Editor"), FTabManager::FLayout::NullLayout, true, true, LootDataRef.Get(), false);
	
	TSharedRef<FExtender> ToolkitExtender = MakeShareable(new FExtender);
	ToolkitExtender->AddToolBarExtension("Asset", EExtensionHook::Position::After, GetToolkitCommands(), FToolBarExtensionDelegate::CreateSP(this, &FLootEditorAssetEditor::BuildToolBar));
	AddToolbarExtender(ToolkitExtender);

	

	const  TSharedPtr<FTabManager::FLayout> LootSettings_ModeLayout = FTabManager::NewLayout("LootEditor_LootSeetings_Ver_0")
	->AddArea
	(
		FTabManager::NewPrimaryArea()
		->SetOrientation(Orient_Vertical)
		->Split
		(
		FTabManager::NewStack()
			->SetSizeCoefficient(0.1f)
			->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
			->SetHideTabWell(true)
		)
		->Split
		(
		FTabManager::NewSplitter()
			->SetOrientation(Orient_Horizontal)
			->Split
			(
			FTabManager::NewStack()
			->SetSizeCoefficient(0.3f)
			->SetHideTabWell(true)
			->AddTab(LootEditorTabs::LootSettings_List, ETabState::OpenedTab)
			)
			->Split
			(
			FTabManager::NewStack()
			->SetSizeCoefficient(0.7f)
			->SetHideTabWell(true)
			->AddTab(LootEditorTabs::LootSettings_Workflow, ETabState::OpenedTab)
			)
		)
	);

	const  TSharedPtr<FTabManager::FLayout> LootData_ModeLayout = FTabManager::NewLayout("LootEditor_LootData_Ver_0")
	->AddArea
	(
		FTabManager::NewPrimaryArea()
		->SetOrientation(Orient_Vertical)
		->Split
		(
		FTabManager::NewStack()
			->SetSizeCoefficient(0.1f)
			->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
			->SetHideTabWell(true)
		)
		->Split
		(
		FTabManager::NewSplitter()
			->SetOrientation(Orient_Horizontal)
			->Split
			(
			FTabManager::NewStack()
			->SetSizeCoefficient(0.4f)
			->SetHideTabWell(true)
			->AddTab(LootEditorTabs::LootData_List, ETabState::OpenedTab)
			)
			->Split
			(
			FTabManager::NewStack()
			->SetSizeCoefficient(0.4f)
			->SetHideTabWell(true)
			->AddTab(LootEditorTabs::LootData_Workflow, ETabState::OpenedTab)
			)
		)
	);

	const  TSharedPtr<FTabManager::FLayout> LootProfile_ModeLayout = FTabManager::NewLayout("LootEditor_LootProfile_Ver_0")
	->AddArea
	(
		FTabManager::NewPrimaryArea()
		->SetOrientation(Orient_Vertical)
		->Split
		(
		FTabManager::NewStack()
			->SetSizeCoefficient(0.1f)
			->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
			->SetHideTabWell(true)
		)
		->Split
		(
		FTabManager::NewSplitter()
			->SetOrientation(Orient_Horizontal)
			->Split
			(
			FTabManager::NewStack()
			->SetSizeCoefficient(0.4f)
			->SetHideTabWell(true)
			->AddTab(LootEditorTabs::LootProfile_List, ETabState::OpenedTab)
			)
			->Split
			(
			FTabManager::NewStack()
			->SetSizeCoefficient(0.4f)
			->SetHideTabWell(true)
			->AddTab(LootEditorTabs::LootProfile_Workflow, ETabState::OpenedTab)
			)
		)
	);
	
	AddApplicationMode(LootEditorAppModes::LootSettings, MakeShareable(new FLootEditorAppMode_LootSettings(SharedThis(this), LootEditorAppModes::LootSettings, LootSettings_ModeLayout)));
	AddApplicationMode(LootEditorAppModes::LootData, MakeShareable(new FLootEditorAppMode_LootData(SharedThis(this), LootEditorAppModes::LootData, LootData_ModeLayout)));
	AddApplicationMode(LootEditorAppModes::LootProfile, MakeShareable(new FLootEditorAppMode_LootProfile(SharedThis(this), LootEditorAppModes::LootProfile, LootProfile_ModeLayout)));

	

	SetCurrentMode(LootEditorAppModes::LootSettings);
}

void FLootEditorAssetEditor::BuildToolBar(FToolBarBuilder& ToolbarBuilder)
{
	ToolbarBuilder.BeginSection("LootEditorModes");
	{
		// Edit Grid button
		ToolbarBuilder.AddToolBarButton(
			FUIAction(
				FExecuteAction::CreateSP(this, &FLootEditorAssetEditor::SetCurrentMode, LootEditorAppModes::LootSettings),
				FCanExecuteAction(),
				FIsActionChecked::CreateLambda([this]() {
					return GetCurrentMode() == LootEditorAppModes::LootSettings;
				})
			),
			NAME_None,
			FText::FromString("Loot Settings"),
			FText::FromString(""),
			FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"),
			EUserInterfaceActionType::ToggleButton
		);

		// Place Items button
		ToolbarBuilder.AddToolBarButton(
			FUIAction(
				FExecuteAction::CreateSP(this, &FLootEditorAssetEditor::SetCurrentMode, LootEditorAppModes::LootData),
				FCanExecuteAction(),
				FIsActionChecked::CreateLambda([this]() {
					return GetCurrentMode() == LootEditorAppModes::LootData;
				})
			),
			NAME_None,
			FText::FromString("Loot Data"),
			FText::FromString(""),
			FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"),
			EUserInterfaceActionType::ToggleButton
		);

		ToolbarBuilder.AddToolBarButton(
			FUIAction(
				FExecuteAction::CreateSP(this, &FLootEditorAssetEditor::SetCurrentMode, LootEditorAppModes::LootProfile),
				FCanExecuteAction(),
				FIsActionChecked::CreateLambda([this]() {
					return GetCurrentMode() == LootEditorAppModes::LootProfile;
				})
			),
			NAME_None,
			FText::FromString("Profile"),
			FText::FromString(""),
			FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"),
			EUserInterfaceActionType::ToggleButton
		);
	}
	ToolbarBuilder.EndSection();
}

void FLootEditorAssetEditor::AddReferencedObjects(FReferenceCollector& Collector)
{
	
	Collector.AddReferencedObject(LootDataRef);
}

FString FLootEditorAssetEditor::GetReferencerName() const
{
	if (IsValid(LootDataRef.Get()))
	{
		return LootDataRef.Get()->GetName();
	}
	return "Error Reference Name";
}
#pragma endregion

#pragma region TabSpawner

void FLootEditorAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);
}

void FLootEditorAssetEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FWorkflowCentricApplication::UnregisterTabSpawners(InTabManager);
}

void FLootEditorAssetEditor::RegisterToolBarTab(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
}

TSharedRef<SWidget> FLootEditorAssetEditor::SpawnTab_Graph()
{
	return SNew(SBox);
}

TSharedRef<SWidget> FLootEditorAssetEditor::SpawnTab_CategoryList()
{
	return SNew(SBox);
}

#pragma endregion