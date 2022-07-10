#include "Json/JsonParser.h"
#include "JsonUtilities.h"

// Sets default values
AJsonParser::AJsonParser() = default;

// Called when the game starts or when spawned
void AJsonParser::BeginPlay()
{
	Super::BeginPlay();
	
	/* Creates a string ref to wherever the json file(s) are */
	const FString JsonFilePath = FPaths::ProjectContentDir() + "/JsonFiles/PData.json";
	FString JsonString; /* Json converted to FString */

	FFileHelper::LoadFileToString(JsonString, *JsonFilePath); /* Remember to dereference file path */

	/* Create a json object to store the information from the json string */
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

	/* The json reader is used to deserialize the json object later on */
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		/* Gets whatever "object" from the json file you choose */
		TSharedPtr<FJsonObject> DataObject = JsonObject->GetObjectField("Data");

		/* Grab various properties and print them out */
		FString Temp = DataObject->GetStringField("Name");

		GEngine->AddOnScreenDebugMessage(-1, 10.F, FColor::Orange, TEXT("Name is: ") + Temp);

		FString TempStr = FString::FromInt(DataObject->GetIntegerField("HealthPackValue"));

		GEngine->AddOnScreenDebugMessage(-1, 10.F, FColor::FromHex(TEXT("527D1A")), TEXT("Health pack value is: ") + TempStr);

		FString Str = FString::FromInt(DataObject->GetIntegerField("ShotgunAmmoValue"));

		GEngine->AddOnScreenDebugMessage(-1, 10.F, FColor::FromHex(TEXT("527D1A")), TEXT("Shotgun ammo value is: ") + Str);
	}
}


