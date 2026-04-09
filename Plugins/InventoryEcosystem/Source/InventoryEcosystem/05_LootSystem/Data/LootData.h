// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootData.generated.h"




USTRUCT(BlueprintType)
struct FLootSettingsData
{
	GENERATED_BODY()

	UPROPERTY()
	FName Category;

	UPROPERTY()
	FText LootSpawner_Name;
};


UCLASS()
class LOOTSYSTEM_API ULootData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	ULootData()
	{
		LootSettings.Reset();
		
		FLootSettingsData Container_01;
		Container_01.Category = FName("Dev");
		Container_01.LootSpawner_Name = FText::FromString("Container_01");
		LootSettings.Add("Container_01", Container_01);
		
		FLootSettingsData Container_02;
		Container_02.Category = FName("Dev|Military");
		Container_02.LootSpawner_Name = FText::FromString("Container_02");
		LootSettings.Add("Container_02", Container_02);
		
		FLootSettingsData Container_03;
		Container_03.Category = FName("Dev|Military");
		Container_03.LootSpawner_Name = FText::FromString("Container_03");
		LootSettings.Add("Container_03", Container_03);
		
		FLootSettingsData Container_04;
		Container_04.Category = FName("Dev|Military|Eat");
		Container_04.LootSpawner_Name = FText::FromString("Container_04");
		LootSettings.Add("Container_04", Container_04);
		
		FLootSettingsData Container_05;
		Container_05.Category = FName("Dev|Vilage");
		Container_05.LootSpawner_Name = FText::FromString("Container_05");
		LootSettings.Add("Container_05", Container_05);
		
		FLootSettingsData Container_06;
		Container_06.Category = FName("Dev|Vilage");
		Container_06.LootSpawner_Name = FText::FromString("Container_06");
		LootSettings.Add("Container_06", Container_06);
		
		FLootSettingsData Container_07;
		Container_07.Category = FName("Dev|Vilage|Resource");
		Container_07.LootSpawner_Name = FText::FromString("Container_07");
		LootSettings.Add("Container_07", Container_07);
		
		FLootSettingsData Container_08;
		Container_08.Category = FName("Demo|SafetyZone");
		Container_08.LootSpawner_Name = FText::FromString("Container_08");
		LootSettings.Add("Container_08", Container_08);
		
		FLootSettingsData Container_09;
		Container_09.Category = FName("Demo|Military");
		Container_09.LootSpawner_Name = FText::FromString("Container_09");
		LootSettings.Add("Container_09", Container_09);
		
		FLootSettingsData Container_10;
		Container_10.Category = FName("Demo|Military");
		Container_10.LootSpawner_Name = FText::FromString("Container_10");
		LootSettings.Add("Container_10", Container_10);
		
		FLootSettingsData Container_11;
		Container_11.Category = FName("Demo|Military|Eat");
		Container_11.LootSpawner_Name = FText::FromString("Container_11");
		LootSettings.Add("Container_11", Container_11);
		
		FLootSettingsData Container_12;
		Container_12.Category = FName("Demo|Vilage");
		Container_12.LootSpawner_Name = FText::FromString("Container_12");
		LootSettings.Add("Container_12", Container_12);
		
		FLootSettingsData Container_13;
		Container_13.Category = FName("Demo|Vilage");
		Container_13.LootSpawner_Name = FText::FromString("Container_13");
		LootSettings.Add("Container_13", Container_13);
		
		FLootSettingsData Container_14;
		Container_14.Category = FName("Demo|Vilage|Resource");
		Container_14.LootSpawner_Name = FText::FromString("Container_14");
		LootSettings.Add("Container_14", Container_14);
	};

	/*

	
	 */
	
	TMap<FName, FLootSettingsData> GetLootSettingsData() {return LootSettings;};
protected:
	
	UPROPERTY(BlueprintReadOnly)
	TMap<FName, FLootSettingsData> LootSettings;
};
