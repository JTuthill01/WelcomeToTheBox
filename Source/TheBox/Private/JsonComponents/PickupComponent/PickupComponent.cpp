#include "JsonComponents/PickupComponent/PickupComponent.h"
#include "JsonUtilities.h"

// Sets default values for this component's properties
UPickupComponent::UPickupComponent() = default;

void UPickupComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializerList();
}

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

		WeaponBlueprintFilePathString = DataObject->GetStringField("WeaponBlueprintPath");

		PType = DataObject->GetIntegerField("PickupType");

		PHealthType = DataObject->GetIntegerField("PickupHealthType");

		PAmmoType = DataObject->GetIntegerField("PickupAmmoType");

		PGrenadeType = DataObject->GetIntegerField("PickupGrenadeType");

		HealthValue = DataObject->GetNumberField("HealthPackValue");

		ArmValue = DataObject->GetNumberField("ArmorValue");

		PAmmoValue = DataObject->GetIntegerField("PistolAmmoValue");

		RAmmoValue = DataObject->GetIntegerField("RifleAmmoValue");

		SAmmoValue = DataObject->GetIntegerField("ShotgunAmmoValue");

		GrenadeValue = DataObject->GetIntegerField("GrenadeValue");
	}
}

void UPickupComponent::WeaponParser()
{
	/* Creates a string ref to wherever the json file(s) are */
	const FString JsonFilePath = FPaths::ProjectContentDir() + "/JsonFiles/WeaponPickup.json";
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

		PickupNameString = DataObject->GetStringField("WeaponName");

		PickupSoundFilePath = DataObject->GetStringField("PickupSFXPath");

		WeaponBlueprintFilePathString = DataObject->GetStringField("WeaponBlueprintPath");

		PType = DataObject->GetIntegerField("PickupType");

		PWeaponType = DataObject->GetIntegerField("PickupWeaponType");
	}
}

FString UPickupComponent::LoadParser(FString WeaponString)
{
	FString OutPathString;

	/* Creates a string ref to wherever the json file(s) are */
	const FString JsonFilePath = FPaths::ProjectContentDir() + "/JsonFiles/WeaponBPFilePaths.json";
	FString JsonString; /* Json converted to FString */

	FFileHelper::LoadFileToString(JsonString, *JsonFilePath); /* Remember to dereference file path */

	/* Create a json object to store the information from the json string */
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

	/* The json reader is used to deserialize the json object later on */
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		/* Gets whatever "object" from the json file you choose */
		TSharedPtr<FJsonObject> DataObject = JsonObject->GetObjectField(WeaponString);

		/* Passes the string out, set in Pickup */
		OutPathString = DataObject->GetStringField("BPFilePath");
	}

	return OutPathString;
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

	 PWeaponType = 0;

	 PGrenadeType = 0;

	 /* Floats */

	 HealthValue = 0.F;

	 ArmValue = 0.F;

	 /* int32 */

	 PAmmoValue = 0;

	 RAmmoValue = 0;

	 SAmmoValue = 0;

	 GrenadeValue = 0;
}



