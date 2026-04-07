#include "InteractSystemSettings.h"

#include "InteractSystem.h"

const UInteractSystemSettings* UInteractSystemSettings::Get()
{
	return GetDefault<UInteractSystemSettings>();
}
