#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Templates/SharedPointer.h"

#include "InventoryEcosystem/05_LootSystem/Data/LootData.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

class FLootEditorAssetEditor;


struct FLootCategoryData
{
	TArray<FName> ObjectGUIDS;
	TMap<FName, TSharedPtr<FLootCategoryData>> Children;
};

class INVENTORYECOSYSTEMEDITOR_API FLootEditorTabFactory_LootSettingsList : public FWorkflowTabFactory
{
public:

	FLootEditorTabFactory_LootSettingsList(TSharedRef<FLootEditorAssetEditor> InLootEditorAsset);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

	UPROPERTY()
	TMap<FName, TSharedPtr<FLootCategoryData>> Children;
	
	TSharedPtr<FLootCategoryData> AddRoot(TMap<FName, TSharedPtr<FLootCategoryData>>& CategoryData, FName& RootName) const
	{
		auto Root = MakeShared<FLootCategoryData>();
		CategoryData.Add(RootName, Root);
		return Root;
	}
	
	TSharedPtr<FLootCategoryData> AddChild(TMap<FName, TSharedPtr<FLootCategoryData>>& Root, TArray<FName> PathTo) const
	{
		if (PathTo.Num() == 0)
		{
			return nullptr;
		}
		
		TSharedPtr<FLootCategoryData> HasPath = FindByPath(Root, PathTo);
		if (HasPath.IsValid()) return HasPath;

		TArray<FName> WaysLeft = PathTo;
		TArray<FName> CurrentPath;
		TMap<FName, TSharedPtr<FLootCategoryData>>* CurrentRoot = &Root;
		
		//Создаем|Ищем категорию
		for (auto Path : PathTo)
		{
			CurrentPath.Add(Path);
			TSharedPtr<FLootCategoryData> CurrentCategory = FindByPath(*CurrentRoot, {Path});
			
			if (!CurrentCategory.IsValid())
			{
				CurrentCategory = AddRoot(*CurrentRoot, {Path});
			}
			
			WaysLeft.RemoveAt(0);
			if (WaysLeft.Num() == 0) return CurrentCategory;
			
			CurrentRoot = &CurrentCategory.Get()->Children;
		}
		
		return FindByPath(Root, PathTo);
	}
	
	TSharedPtr<FLootCategoryData> FindByPath(TMap<FName, TSharedPtr<FLootCategoryData>>& CategoryData, TArray<FName> CopyPathParts) const
	{
		if (CopyPathParts.Num() == 0)
		{
			return nullptr;
		}

		//Тут мы убираем текущий для вызова рекурсии
		TArray<FName> NextPaths = CopyPathParts;
		NextPaths.RemoveAt(0);
		TSharedPtr<FLootCategoryData>* Current = CategoryData.Find(CopyPathParts[0]);
		if (!Current) return nullptr;
		
		if (NextPaths.Num() == 0 && Current)
		{
			return *Current;
		}
		
		return FindByPath(Current->Get()->Children, NextPaths);
	}
private:

	TWeakPtr<FLootEditorAssetEditor> LootAssetEditor;
};