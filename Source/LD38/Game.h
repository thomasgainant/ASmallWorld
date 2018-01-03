// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "../../Source/LD38/ASWCharacter.h"
#include "../../Source/LD38/Tile.h"
#include "../../Source/LD38/NeighboursComputer.h"
#include "Runtime/Core/Public/Containers/Array.h"
#include "Game.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LD38_API UGame : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGame();

	APlayerController* playerController;
	TArray<UTile*> tiles;
	UASWCharacter* player;
	TArray<UASWCharacter*> ai;

	int movingAI = 0;
	int numberOfDeadAI = 0;

	bool waitForMovingPlayer;
	bool waitForMovingAI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LD38")
	bool gamewon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LD38")
	bool gameover;

	bool playerCanMove;

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	NeighboursComputer* computer;
	bool computingNeighbours;
	int32 currentTileToRefreshIndex;

	void refreshNeighboursForTile(int index);

	UFUNCTION(BlueprintCallable, Category = "LD38")
	void ConfirmMovement();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LD38")
	int currentYear;

	void NewTurn();

	void ChangeWorld();

	UTile* GetTileAt(int coorX, int coorY);
	
};
