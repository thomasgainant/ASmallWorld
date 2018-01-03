// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
//#include "../../Source/LD38/Game.h"
#include "Tile.generated.h"

class UGame;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LD38_API UTile : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTile();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UPROPERTY(EditAnywhere)
	int coorX;
	UPROPERTY(EditAnywhere)
	int coorY;

	static enum TileType { ocean, plain, mountain, forest, ice };

	UPROPERTY(EditAnywhere)
	int currentType;

	UGame* uGame;

	TArray<UTile*> neighbours;

	UMaterialInstanceDynamic* materialInstance;

	void refreshColor();

	bool hasOceanNeighbourTile();

	void setTileType(TileType newType);

	UFUNCTION(BlueprintCallable, Category = "LD38")
	void OnClicked();

	FScriptDelegate clickDelegate;
	
};
