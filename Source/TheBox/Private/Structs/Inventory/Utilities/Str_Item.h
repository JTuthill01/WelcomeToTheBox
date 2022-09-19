#pragma once

#include "Enums/InventoryEnums/InventoryEnums.h"
#include "Str_Item.generated.h"

USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UStaticMesh> ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Icon)
	TObjectPtr<class UTexture2D> ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
	EInventoryType InventoryType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Description)
	FString ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Name)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variables)
	int32 MaxItemAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variables)
	bool bIsUsable;
};