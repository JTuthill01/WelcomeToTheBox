#include "JsonComponents/PickupComponent/PickupComponent.h"
#include "JsonUtilities.h"

// Sets default values for this component's properties
UPickupComponent::UPickupComponent() = default;

void UPickupComponent::Parser()
{
	/* Creates a string ref to wherever the json file(s) are */
	const FString JsonFilePath = FPaths::ProjectContentDir() + "/JsonFiles/PickupTable.json";
	FString JsonString; /* Json converted to FString */

	FFileHelper::LoadFileToString(JsonString, *JsonFilePath); /* Remember to dereference file path */

	/* Create a json object to store the information from the json string */
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

	/* The json reader is used to deserialize the json object later on */
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		/* Gets whatever "object" from the json file you choose */
		TSharedPtr<FJsonObject> DataObject = JsonObject->GetObjectField(ObjectString);

		MeshFilePathString = DataObject->GetStringField("PickupMesh");

		IconFilePathString = DataObject->GetStringField("Icon");

		NameString = DataObject->GetStringField("PickupName");

		WidgetTextString = DataObject->GetStringField("PickupWidgetText");

		PType = DataObject->GetIntegerField("PickupType");

		PHealthType = DataObject->GetIntegerField("PickupHealthType");

		PAmmoType = DataObject->GetIntegerField("PickupAmmoType");

		HealthValue = DataObject->GetNumberField("HealthPackValue");

		ArmValue = DataObject->GetNumberField("ArmorValue");

		PAmmoValue = DataObject->GetIntegerField("PistolAmmoValue");

		RAmmoValue = DataObject->GetIntegerField("RifleAmmoValue");

		SAmmoValue = DataObject->GetIntegerField("ShotgunAmmoValue");
	}
}

void UPickupComponent::WeaponPickupParser()
{
	/* Creates a string ref to wherever the json file(s) are */
	const FString JsonFilePath = FPaths::ProjectContentDir() + "/JsonFiles/WeaponPickupTable.json";
	FString JsonString; /* Json converted to FString */

	FFileHelper::LoadFileToString(JsonString, *JsonFilePath); /* Remember to dereference file path */

	/* Create a json object to store the information from the json string */
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

	/* The json reader is used to deserialize the json object later on */
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		/* Gets whatever "object" from the json file you choose */
		TSharedPtr<FJsonObject> DataObject = JsonObject->GetObjectField(WeaponObjectString);

		MeshFilePathString = DataObject->GetStringField("PickupMesh");

		IconFilePathString = DataObject->GetStringField("Icon");

		PickupNameString = DataObject->GetStringField("WeaponPickupName");

		WidgetTextString = DataObject->GetStringField("PickupWidgetText");

		PType = DataObject->GetNumberField("PickupType");
	}
}

void UPickupComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializerList();
}

void UPickupComponent::InitializerList()
{
			/* FStrings */
	 MeshFilePathString = FString();

	 IconFilePathString  = FString();

	 PickupNameString  = FString();

	 NameString  = FString();

	 WidgetTextString  = FString();

	 /* uint8's */

	 PType = 0;

	 PHealthType = 0;

	 PAmmoType = 0;

	 /* Floats */

	 HealthValue = 0.F;

	 ArmValue = 0.F;

	 /* int32 */

	 PAmmoValue = 0;

	 RAmmoValue = 0;

	 SAmmoValue = 0;
}



