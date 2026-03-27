#include "InteractSeetingsCustomization.h"

#include "DetailWidgetRow.h"
#include "EditorStyleSet.h"
#include "IDetailChildrenBuilder.h"
#include "InventoryEcosystem/02_InteractSystem/Data/InteractData.h"

TSharedRef<IPropertyTypeCustomization> FInteractSeetingsCustomization::MakeInstance()
{
	return MakeShareable(new FInteractSeetingsCustomization());
}

void FInteractSeetingsCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	HeaderRow
	.NameContent()
	[
		StructPropertyHandle->CreatePropertyNameWidget()
	];
}

void FInteractSeetingsCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	PropertyHandle_ActionMenu = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FInteractSettings, ActionMenu));
	PropertyHandle_InteractionTime = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FInteractSettings, InteractionTime));
	PropertyHandle_TypeTimeDegradation = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FInteractSettings, TypeTimeDegradation));
	PropertyHandle_SpeedOfRevers = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FInteractSettings, SpeedOfRevers));
	PropertyHandle_InteractionDistance = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FInteractSettings, InteractionDistance));

	bool CheckProperty = PropertyHandle_ActionMenu.IsValid();
	CheckProperty &= PropertyHandle_InteractionTime.IsValid();
	CheckProperty &= PropertyHandle_TypeTimeDegradation.IsValid();
	CheckProperty &= PropertyHandle_SpeedOfRevers.IsValid();
	CheckProperty &= PropertyHandle_InteractionDistance.IsValid();
	if (!CheckProperty) return;
	
	StructBuilder.AddCustomRow(FText::FromString("Panel"))
	.NameContent()	
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		[
			PropertyHandle_InteractionDistance->CreatePropertyNameWidget()
		]
		+ SVerticalBox::Slot()
		[
			PropertyHandle_ActionMenu->CreatePropertyNameWidget()
		]
		+ SVerticalBox::Slot()
		[
			SNew(SBox)			
			.Visibility(this, &FInteractSeetingsCustomization::GetVisibility_InteractionTime)
			[
				PropertyHandle_InteractionTime->CreatePropertyNameWidget()
			]
		]
		+ SVerticalBox::Slot()
		[
			SNew(SBox)			
			.Visibility(this, &FInteractSeetingsCustomization::GetVisibility_TypeTimeDegradation)
			[
				PropertyHandle_TypeTimeDegradation->CreatePropertyNameWidget()
			]
		]
		+ SVerticalBox::Slot()
		[
			SNew(SBox)			
			.Visibility(this, &FInteractSeetingsCustomization::GetVisibility_SpeedOfRevers)
			[
				PropertyHandle_SpeedOfRevers->CreatePropertyNameWidget()
			]
		]
	]
	.ValueContent()
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		[
			PropertyHandle_InteractionDistance->CreatePropertyValueWidget()
		]
		+ SVerticalBox::Slot()
		[
			PropertyHandle_ActionMenu->CreatePropertyValueWidget()
		]
		+ SVerticalBox::Slot()
		[
			SNew(SBox)			
			.Visibility(this, &FInteractSeetingsCustomization::GetVisibility_InteractionTime)
			[
				PropertyHandle_InteractionTime->CreatePropertyValueWidget()
			]
		]
		+ SVerticalBox::Slot()
		[
			SNew(SBox)			
			.Visibility(this, &FInteractSeetingsCustomization::GetVisibility_TypeTimeDegradation)
			[
				PropertyHandle_TypeTimeDegradation->CreatePropertyValueWidget()
			]
		]
		+ SVerticalBox::Slot()
		[
			SNew(SBox)			
			.Visibility(this, &FInteractSeetingsCustomization::GetVisibility_SpeedOfRevers)
			[
				PropertyHandle_SpeedOfRevers->CreatePropertyValueWidget()
			]
		]
	];
}

EVisibility FInteractSeetingsCustomization::GetVisibility_InteractionTime() const
{
	if (!PropertyHandle_ActionMenu.IsValid()) return EVisibility::Collapsed;

	int32 Mask = 0;
	PropertyHandle_ActionMenu->GetValue(Mask);
	bool Result = (Mask & (uint8)EInteractType::EIT_Pressed || Mask & (uint8)EInteractType::EIT_Released || Mask & (uint8)EInteractType::EIT_CirculPressed|| Mask & (uint8)EInteractType::EIT_QTE);
	return (Result ? EVisibility::Visible : EVisibility::Collapsed);
}

EVisibility FInteractSeetingsCustomization::GetVisibility_TypeTimeDegradation() const
{
	return GetVisibility_InteractionTime();
}

EVisibility FInteractSeetingsCustomization::GetVisibility_SpeedOfRevers() const
{
	if (GetVisibility_InteractionTime() == EVisibility::Collapsed) return EVisibility::Collapsed;
	if (!PropertyHandle_TypeTimeDegradation.IsValid()) return EVisibility::Collapsed;

	uint8 LEnum = 0;
	PropertyHandle_TypeTimeDegradation->GetValue(LEnum);
	return (static_cast<EInteractTypeTimeDegradation>(LEnum) == EInteractTypeTimeDegradation::EITTD_Reverse ? EVisibility::Visible : EVisibility::Collapsed);
}
