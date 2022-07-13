#include "JsonComponents/WeaponComponent/WeaponComponentParser.h"
#include "JsonUtilities.h"

// Sets default values for this component's properties
UWeaponComponentParser::UWeaponComponentParser() = default;

void UWeaponComponentParser::WeaponParser()
{
	/* Creates a string ref to wherever the json file(s) are */
	const FString JsonFilePath = FPaths::ProjectContentDir() + "/JsonFiles/Table.json";
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
	}
}

