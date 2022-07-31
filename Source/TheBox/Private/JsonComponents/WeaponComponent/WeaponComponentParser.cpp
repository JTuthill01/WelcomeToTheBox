#include "JsonComponents/WeaponComponent/WeaponComponentParser.h"
#include "JsonUtilities.h"

// Sets default values for this component's properties
UWeaponComponentParser::UWeaponComponentParser() = default;

void UWeaponComponentParser::WeaponParser(FWeaponDataStats& WeaponData, FWeaponStringData& OutString, uint8& OutUintToEnum, uint8& OutType)
{
	/* Creates a string ref to wherever the json file(s) are */
	const FString JsonFilePath = FPaths::ProjectContentDir() + "/JsonFiles/WeaponStatsTable.json";
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
		
		WeaponData.CurrentMagTotal = DataObject->GetIntegerField("CurrentMagTotal");

		WeaponData.MaxMagTotal = DataObject->GetIntegerField("MaxMagTotal");

		WeaponData.CurrentTotalAmmo = DataObject->GetIntegerField("CurrentTotalAmmo");

		WeaponData.MaxTotalAmmo = DataObject->GetIntegerField("MaxTotalAmmo");

		WeaponData.LowAmmo = DataObject->GetIntegerField("LowAmmo");

		WeaponData.CrosshairIndex = DataObject->GetIntegerField("CrosshairIndex");

		WeaponData.WeaponIndex = DataObject->GetIntegerField("WeaponIndex");

		WeaponData.DamageAmount = DataObject->GetNumberField("DamageAmount");

		WeaponData.CriciticalHitChance = DataObject->GetNumberField("CriciticalHitChance");

		WeaponData.DamageRadius = DataObject->GetNumberField("DamageRadius");

		WeaponData.WeaponName = FName(*DataObject->GetStringField("WeaponName"));

		OutString.IconPath = DataObject->GetStringField("Icon");

		OutString.RackSlideSoundPath = DataObject->GetStringField("RackSlideSound");

		OutString.MagOutSoundPath = DataObject->GetStringField("MagOutSound");

		OutString.MagInSoundPath = DataObject->GetStringField("MagInSound");

		OutString.FireSoundPath = DataObject->GetStringField("FireSound");

		OutString.AmmoEjectPath = DataObject->GetStringField("AmmoEject");

		OutString.FireFXPath = DataObject->GetStringField("FireFx");

		OutUintToEnum = DataObject->GetNumberField("FireType");

		OutType = DataObject->GetNumberField("WeaponType");
	}
}

