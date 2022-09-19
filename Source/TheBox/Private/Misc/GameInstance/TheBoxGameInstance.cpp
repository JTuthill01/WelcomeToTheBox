#include "Misc/GameInstance/TheBoxGameInstance.h"
#include "JsonUtilities.h"
#include "NiagaraFunctionLibrary.h"

UTheBoxGameInstance::UTheBoxGameInstance() = default;

UTheBoxGameInstance::UTheBoxGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UTheBoxGameInstance::Init() 
{ 
	Super::Init(); 

	IconLoader();

	FireFXLoader();

	AmmoEjectFXLoader();
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

void UTheBoxGameInstance::FireFXLoader()
{
	TObjectPtr<UNiagaraSystem> AK47 = LoadObject<UNiagaraSystem>(this, TEXT("/Game/Gun_VFX/Niagara_FX/Muzzle_Flashes/NS_AR_Muzzle_flashes_01"));
	TObjectPtr<UNiagaraSystem> TT33 = LoadObject<UNiagaraSystem>(this, TEXT("/Game/Gun_VFX/Niagara_FX/Muzzle_Flashes/NS_Pistol_Muzzle_flashes_01"));
	TObjectPtr<UNiagaraSystem> L86 = LoadObject<UNiagaraSystem>(this, TEXT("/Game/Gun_VFX/Niagara_FX/Muzzle_Flashes/NS_AR_Muzzle_flashes_01"));
	TObjectPtr<UNiagaraSystem> AmericanShotgun = LoadObject<UNiagaraSystem>(this, TEXT("/Game/Gun_VFX/Niagara_FX/Muzzle_Flashes/NS_Shotgun_Muzzle_flashes_02"));
	TObjectPtr<UNiagaraSystem> BelgianAR = LoadObject<UNiagaraSystem>(this, TEXT("/Game/Muzzle_Master/PARTICLE_System/NS_MuzzleFlash_03"));
	TObjectPtr<UNiagaraSystem> GermanSMG = LoadObject<UNiagaraSystem>(this, TEXT("/Game/Muzzle_Master/PARTICLE_System/NS_MuzzleFlash_03"));
	TObjectPtr<UNiagaraSystem> HandCannon = LoadObject<UNiagaraSystem>(this, TEXT("/Game/100_MuzzleFlashes/Muzzle_Flash/MuzzleFlash_Niagara/P_MuzzleFlash7"));
	TObjectPtr<UNiagaraSystem> SKS = LoadObject<UNiagaraSystem>(this, TEXT("/Game/100_MuzzleFlashes/Muzzle_Flash/MuzzleFlash_Niagara/P_MuzzleFlash73"));
	TObjectPtr<UNiagaraSystem> XM82 = LoadObject<UNiagaraSystem>(this, TEXT("/Game/Gun_VFX/Niagara_FX/Muzzle_Flashes/NS_SMG_Muzzle_flashes_02"));
	TObjectPtr<UNiagaraSystem> Bulldog = LoadObject<UNiagaraSystem>(this, TEXT("/Game/Gun_VFX/Niagara_FX/Muzzle_Flashes/NS_Shotgun_Muzzle_flashes_02"));
	TObjectPtr<UNiagaraSystem> AK74 = LoadObject<UNiagaraSystem>(this, TEXT("/Game/100_MuzzleFlashes/Muzzle_Flash/MuzzleFlash_Niagara/P_MuzzleFlash69"));
	TObjectPtr<UNiagaraSystem> NavySMG = LoadObject<UNiagaraSystem>(this, TEXT("/Game/Gun_VFX/Niagara_FX/Muzzle_Flashes/NS_SMG_Muzzle_flashes_01"));
	TObjectPtr<UNiagaraSystem> ItalianShotgun = LoadObject<UNiagaraSystem>(this, TEXT("/Game/100_MuzzleFlashes/Muzzle_Flash/MuzzleFlash_Niagara/P_MuzzleFlash99"));
	TObjectPtr<UNiagaraSystem> SVD = LoadObject<UNiagaraSystem>(this, TEXT("/Game/100_MuzzleFlashes/Muzzle_Flash/MuzzleFlash_Niagara/P_MuzzleFlash10"));
	TObjectPtr<UNiagaraSystem> ShortStrokeAR = LoadObject<UNiagaraSystem>(this, TEXT("/Game/Gun_VFX/Niagara_FX/Muzzle_Flashes/NS_AR_Muzzle_flashes_02"));

	FireFXMap.Emplace(EWeaponName::EWN_AK47, AK47);
	FireFXMap.Emplace(EWeaponName::EWN_TT33, TT33);
	FireFXMap.Emplace(EWeaponName::EWN_L86, L86);
	FireFXMap.Emplace(EWeaponName::EWN_AmericanShotgun, AmericanShotgun);
	FireFXMap.Emplace(EWeaponName::EWN_BelgianAR, BelgianAR);
	FireFXMap.Emplace(EWeaponName::EWN_GermanSMG, GermanSMG);
	FireFXMap.Emplace(EWeaponName::EWN_HandCannon, HandCannon);
	FireFXMap.Emplace(EWeaponName::EWN_SKS, SKS);
	FireFXMap.Emplace(EWeaponName::EWN_XM82, XM82);
	FireFXMap.Emplace(EWeaponName::EWN_Bulldog, Bulldog);
	FireFXMap.Emplace(EWeaponName::EWN_AK74, AK74);
	FireFXMap.Emplace(EWeaponName::EWN_NavySMG, NavySMG);
	FireFXMap.Emplace(EWeaponName::EWN_ItalianShotgun, ItalianShotgun);
	FireFXMap.Emplace(EWeaponName::EWN_SVD, SVD);
	FireFXMap.Emplace(EWeaponName::EWN_ShortStrokeAR, ShortStrokeAR);
}

void UTheBoxGameInstance::AmmoEjectFXLoader()
{
	TObjectPtr<UNiagaraSystem> AK47 = LoadObject<UNiagaraSystem>(this, TEXT("/Game/Gun_VFX/Niagara_FX/Bullet_ejection/NS_AR_Ejection_Effect"));
	TObjectPtr<UNiagaraSystem> TT33 = LoadObject<UNiagaraSystem>(this, TEXT("/Game/Gun_VFX/Niagara_FX/Bullet_ejection/NS_Pistol_Ejection_Effect"));
	TObjectPtr<UNiagaraSystem> L86 = LoadObject<UNiagaraSystem>(this, TEXT("/Game/Gun_VFX/Niagara_FX/Bullet_ejection/NS_AR_Ejection_Effect"));
	TObjectPtr<UNiagaraSystem> AmericanShotgun = LoadObject<UNiagaraSystem>(this, TEXT("/Game/Gun_VFX/Niagara_FX/Bullet_ejection/NS_Shotgun_Ejection_Effect"));
	TObjectPtr<UNiagaraSystem> BelgianAR = LoadObject<UNiagaraSystem>(this, TEXT("/Game/Gun_VFX/Niagara_FX/Bullet_ejection/NS_AR_Ejection_Effect"));
	TObjectPtr<UNiagaraSystem> GermanSMG = LoadObject<UNiagaraSystem>(this, TEXT("/Game/Gun_VFX/Niagara_FX/Bullet_ejection/NS_SMG_Ejection_Effect"));
	TObjectPtr<UNiagaraSystem> HandCannon = LoadObject<UNiagaraSystem>(this, TEXT("/Game/Gun_VFX/Niagara_FX/Bullet_ejection/NS_Pistol_Ejection_Effect"));
	TObjectPtr<UNiagaraSystem> SKS = LoadObject<UNiagaraSystem>(this, TEXT("/Game/Gun_VFX/Niagara_FX/Bullet_ejection/NS_Sniper_Ejection_Effect"));
	TObjectPtr<UNiagaraSystem> XM82 = LoadObject<UNiagaraSystem>(this, TEXT("/Game/Gun_VFX/Niagara_FX/Bullet_ejection/NS_SMG_Ejection_Effect"));
	TObjectPtr<UNiagaraSystem> Bulldog = LoadObject<UNiagaraSystem>(this, TEXT(""));
	TObjectPtr<UNiagaraSystem> AK74 = LoadObject<UNiagaraSystem>(this, TEXT("/Game/Gun_VFX/Niagara_FX/Bullet_ejection/NS_SMG_Ejection_Effect"));
	TObjectPtr<UNiagaraSystem> NavySMG = LoadObject<UNiagaraSystem>(this, TEXT("/Game/Gun_VFX/Niagara_FX/Bullet_ejection/NS_SMG_Ejection_Effect"));
	TObjectPtr<UNiagaraSystem> ItalianShotgun = LoadObject<UNiagaraSystem>(this, TEXT("/Game/Gun_VFX/Niagara_FX/Bullet_ejection/NS_Shotgun_Ejection_Effect"));
	TObjectPtr<UNiagaraSystem> SVD = LoadObject<UNiagaraSystem>(this, TEXT("/Game/Gun_VFX/Niagara_FX/Bullet_ejection/NS_Sniper_Ejection_Effect"));
	TObjectPtr<UNiagaraSystem> ShortStrokeAR = LoadObject<UNiagaraSystem>(this, TEXT("/Game/Gun_VFX/Niagara_FX/Bullet_ejection/NS_AR_Ejection_Effect"));

	AmmoEjectFXMap.Emplace(EWeaponName::EWN_AK47, AK47);
	AmmoEjectFXMap.Emplace(EWeaponName::EWN_TT33, TT33);
	AmmoEjectFXMap.Emplace(EWeaponName::EWN_L86, L86);
	AmmoEjectFXMap.Emplace(EWeaponName::EWN_AmericanShotgun, AmericanShotgun);
	AmmoEjectFXMap.Emplace(EWeaponName::EWN_BelgianAR, BelgianAR);
	AmmoEjectFXMap.Emplace(EWeaponName::EWN_GermanSMG, GermanSMG);
	AmmoEjectFXMap.Emplace(EWeaponName::EWN_HandCannon, HandCannon);
	AmmoEjectFXMap.Emplace(EWeaponName::EWN_SKS, SKS);
	AmmoEjectFXMap.Emplace(EWeaponName::EWN_XM82, XM82);
	AmmoEjectFXMap.Emplace(EWeaponName::EWN_Bulldog, Bulldog);
	AmmoEjectFXMap.Emplace(EWeaponName::EWN_AK74, AK74);
	AmmoEjectFXMap.Emplace(EWeaponName::EWN_NavySMG, NavySMG);
	AmmoEjectFXMap.Emplace(EWeaponName::EWN_ItalianShotgun, ItalianShotgun);
	AmmoEjectFXMap.Emplace(EWeaponName::EWN_SVD, SVD);
	AmmoEjectFXMap.Emplace(EWeaponName::EWN_ShortStrokeAR, ShortStrokeAR);
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


