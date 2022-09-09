#include "Misc/GameInstance/TheBoxGameInstance.h"
#include "JsonUtilities.h"

UTheBoxGameInstance::UTheBoxGameInstance() = default;

UTheBoxGameInstance::UTheBoxGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UTheBoxGameInstance::Init() { Super::Init(); }

FString UTheBoxGameInstance::LoadParser(FString WeaponString)
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


