#include "Misc/GameInstance/TheBoxGameInstance.h"
#include "JsonUtilities.h"
#include "NiagaraFunctionLibrary.h"

UTheBoxGameInstance::UTheBoxGameInstance() = default;

UTheBoxGameInstance::UTheBoxGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UTheBoxGameInstance::Init() 
{ 
	Super::Init(); 

	IconLoader();
}

void UTheBoxGameInstance::IconLoader()
{
	Icon = LoadObject<UTexture2D>(this, TEXT("/Game/_Main/Icons/L86"));

	TObjectPtr<UTexture2D> AK47 = LoadObject<UTexture2D>(this, TEXT("/Game/_Main/Icons/AK47"));
	TObjectPtr<UTexture2D> TT33 = LoadObject<UTexture2D>(this, TEXT("/Game/_Main/Icons/TT33"));
	TObjectPtr<UTexture2D> AmericanShotgun = LoadObject<UTexture2D>(this, TEXT("/Game/_Main/Icons/American"));
	TObjectPtr<UTexture2D> BelgianAR = LoadObject<UTexture2D>(this, TEXT("/Game/_Main/Icons/BelgianAR"));
	TObjectPtr<UTexture2D> GermanSMG = LoadObject<UTexture2D>(this, TEXT("/Game/_Main/Icons/GermanSMG"));
	TObjectPtr<UTexture2D> HandCannon = LoadObject<UTexture2D>(this, TEXT("/Game/_Main/Icons/HC"));
	TObjectPtr<UTexture2D> SKS = LoadObject<UTexture2D>(this, TEXT("/Game/_Main/Icons/SKS"));
	TObjectPtr<UTexture2D> XM82 = LoadObject<UTexture2D>(this, TEXT("/Game/_Main/Icons/XM82"));
	TObjectPtr<UTexture2D> Bulldog = LoadObject<UTexture2D>(this, TEXT("/Game/_Main/Icons/Bulldog"));
	TObjectPtr<UTexture2D> AK74 = LoadObject<UTexture2D>(this, TEXT("/Game/_Main/Icons/AK74U"));
	TObjectPtr<UTexture2D> NavySMG = LoadObject<UTexture2D>(this, TEXT("/Game/_Main/Icons/NavySMG"));
	TObjectPtr<UTexture2D> ItalianShotgun = LoadObject<UTexture2D>(this, TEXT("/Game/_Main/Icons/Italian"));
	TObjectPtr<UTexture2D> SVD = LoadObject<UTexture2D>(this, TEXT("/Game/_Main/Icons/SVD"));
	TObjectPtr<UTexture2D> ShortStrokeAR = LoadObject<UTexture2D>(this, TEXT("/Game/_Main/Icons/ShortStroke"));

	IconMap.Emplace(EWeaponName::EWN_L86, Icon);
	IconMap.Emplace(EWeaponName::EWN_AK47, AK47);
	IconMap.Emplace(EWeaponName::EWN_TT33, TT33);
	IconMap.Emplace(EWeaponName::EWN_AmericanShotgun, AmericanShotgun);
	IconMap.Emplace(EWeaponName::EWN_BelgianAR, BelgianAR);
	IconMap.Emplace(EWeaponName::EWN_GermanSMG, GermanSMG);
	IconMap.Emplace(EWeaponName::EWN_HandCannon, HandCannon);
	IconMap.Emplace(EWeaponName::EWN_SKS, SKS);
	IconMap.Emplace(EWeaponName::EWN_XM82, XM82);
	IconMap.Emplace(EWeaponName::EWN_Bulldog, Bulldog);
	IconMap.Emplace(EWeaponName::EWN_AK74, AK74);
	IconMap.Emplace(EWeaponName::EWN_NavySMG, NavySMG);
	IconMap.Emplace(EWeaponName::EWN_ItalianShotgun, ItalianShotgun);
	IconMap.Emplace(EWeaponName::EWN_SVD, SVD);
	IconMap.Emplace(EWeaponName::EWN_ShortStrokeAR, ShortStrokeAR);
}

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

UTheBoxGameInstance* UTheBoxGameInstance::SetInstance_Implementation() { return this; }


