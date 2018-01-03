// Fill out your copyright notice in the Description page of Project Settings.

#include "LD38.h"
#include "Tile.h"
#include "../../Source/LD38/Game.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"


// Sets default values for this component's properties
UTile::UTile()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	//neighbours = new UTile*[4];
	UTile* tableinit[] = { NULL, NULL, NULL, NULL };
	neighbours.Append(tableinit, ARRAY_COUNT(tableinit));	
}


// Called when the game starts
void UTile::BeginPlay()
{
	Super::BeginPlay();

	clickDelegate.BindUFunction(this, FName(TEXT("OnClicked")));

	//currentType = ocean;

	TArray<UStaticMeshComponent*> Components;
	this->GetOwner()->GetComponents<UStaticMeshComponent>(Components);
	Components[0]->SetMobility(EComponentMobility::Movable);
	UStaticMeshComponent* mesh = Components[0];
	materialInstance = mesh->CreateAndSetMaterialInstanceDynamic(0);
	//materialInstance = UMaterialInstanceDynamic::Create(mesh->GetMaterial(0), this);

	//mesh->OnClicked.AddDynamic(this, &UTile::OnClick);
	GetOwner()->OnClicked.Add(clickDelegate);

	/*TArray<UStaticMeshComponent*> Components;
	this->GetOwner()->GetComponents<UStaticMeshComponent>(Components);
	materialInstance = Components[0]->GetMaterial(0);*/
	refreshColor();

	//mesh->SetMaterial(0, materialInstance);
	//FLinearColor* color = NULL;
	//materialInstance->GetVectorParameterValue(TEXT("MainColor"), *color);
	
	//UE_LOG(LogTemp, Warning, TEXT("Tile color: %d"), color->R);
}


// Called every frame
void UTile::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//refreshColor();
}

void UTile::refreshColor(){
	switch (currentType) {
	case ocean:
		materialInstance->SetVectorParameterValue(TEXT("MainColor"), FLinearColor(4.0f / 225.0f, 115.0f / 225.0f, 187.0f / 225.0f, 255.0f/225.0f));
		break;
	case plain:
		materialInstance->SetVectorParameterValue(TEXT("MainColor"), FLinearColor(119.0f / 225.0f, 174.0f / 225.0f, 108.0f / 225.0f, 255.0f / 225.0f));
		break;
	case mountain:
		materialInstance->SetVectorParameterValue(TEXT("MainColor"), FLinearColor(127.0f / 225.0f, 114.0f / 225.0f, 79.0f / 225.0f, 255.0f / 225.0f));
		break;
	case forest:
		materialInstance->SetVectorParameterValue(TEXT("MainColor"), FLinearColor(70.0f / 225.0f, 123.0f / 225.0f, 60.0f / 225.0f, 255.0f / 225.0f));
		break;
	case ice:
		materialInstance->SetVectorParameterValue(TEXT("MainColor"), FLinearColor(220.0f / 225.0f, 220.0f / 225.0f, 220.0f / 225.0f, 255.0f / 225.0f));
		break;
	}
}

bool UTile::hasOceanNeighbourTile() {
	bool result = false;

	for (int i = 0; i < neighbours.Num(); i++){
		if(neighbours[i]!= NULL && neighbours[i]->currentType == UTile::TileType::ocean){
			result = true;
			break;
		}
	}

	return result;
}

void UTile::setTileType(TileType newType) {
	currentType = newType;

	refreshColor();
}

void UTile::OnClicked() {
	//UE_LOG(LogTemp, Warning, TEXT("Tile click"));
	if (uGame->playerCanMove){
		int coorXToMove = coorX - uGame->player->currentXCoor;
		int coorYToMove= coorY - uGame->player->currentYCoor;

		if(coorXToMove > 1){
			coorXToMove = 1;
		}
		else if (coorXToMove < -1) {
			coorXToMove = -1;
		}

		if (coorYToMove > 1) {
			coorYToMove = 1;
		}
		else if (coorYToMove < -1) {
			coorYToMove = -1;
		}

		uGame->playerCanMove = false;
		uGame->waitForMovingPlayer = true;
		uGame->player->moveTo(coorXToMove, coorYToMove);
	}
}