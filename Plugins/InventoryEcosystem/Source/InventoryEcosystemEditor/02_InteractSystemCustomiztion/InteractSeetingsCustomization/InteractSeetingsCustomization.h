#pragma once

class FInteractSeetingsCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	// BEGIN IPropertyTypeCustomization interface
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle,
		class FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle,
		class IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

protected:
	EVisibility GetVisibility_InteractionTime() const;
	EVisibility GetVisibility_TypeTimeDegradation() const;
	EVisibility GetVisibility_SpeedOfRevers() const;


	
	TSharedPtr<IPropertyHandle> PropertyHandle_ActionMenu;
	TSharedPtr<IPropertyHandle> PropertyHandle_InteractionTime;
	TSharedPtr<IPropertyHandle> PropertyHandle_TypeTimeDegradation;
	TSharedPtr<IPropertyHandle> PropertyHandle_SpeedOfRevers;
	TSharedPtr<IPropertyHandle> PropertyHandle_InteractionDistance;
};
