#include "JsonComponents/WeaponComponent/WeaponComponentParser.h"
#include "JsonUtilities.h"

// Sets default values for this component's properties
UWeaponComponentParser::UWeaponComponentParser() : MeshFilePathString(FString()), CurrentMagTotal(0), MaxMagTotal(0), CurrentTotalAmmo(0), MaxTotalAmmo(0), LowAmmo(0), CrosshairIndex(0), WeaponIndex(0),
	DamageAmount(0.F), CriciticalHitChance(0.F), DamageRadius(0.F), NameOfWeapon(NAME_None), FireType(EWeaponFireType::EWFT_None), Icon(nullptr), AmmoEject(nullptr), FireFX(nullptr), FireSound(nullptr),
	RackSlideSound(nullptr), MagOutSound(nullptr), MagInSound(nullptr)
{
}

void UWeaponComponentParser::WeaponParser()
{
	/* Creates a string ref to wherever the json file(s) are */
	const FString JsonFilePath = FPaths::ProjectContentDir() + "/JsonFiles/PTable.json";
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

		CurrentMagTotal = DataObject->GetIntegerField("CurrentMagTotal");

		MaxMagTotal = DataObject->GetIntegerField("MaxMagTotal");

		CurrentTotalAmmo = DataObject->GetIntegerField("CurrentTotalAmmo");

		MaxTotalAmmo = DataObject->GetIntegerField("MaxTotalAmmo");

		LowAmmo = DataObject->GetIntegerField("LowAmmo");

		CrosshairIndex = DataObject->GetIntegerField("CrosshairIndex");

		WeaponIndex = DataObject->GetIntegerField("WeaponIndex");
	}
}

