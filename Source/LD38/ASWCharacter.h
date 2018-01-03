// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "ASWCharacter.generated.h"

class UGame;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LD38_API UASWCharacter : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UASWCharacter();

	UGame* uGame;

	UPROPERTY(EditAnywhere)
	bool ai;
	UPROPERTY(EditAnywhere)
	int currentXCoor;
	UPROPERTY(EditAnywhere)
	int currentYCoor;

	FVector destination;
	bool shouldMove;
	float moveSpeed;
	bool hasMovedThisTurn;

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	bool moveTo(int coorX, int coorY);
	
};
