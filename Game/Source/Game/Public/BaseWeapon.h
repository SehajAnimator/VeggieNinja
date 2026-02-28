// Copyright (c) 2026 Studios SehajAvastha
// 
// This file is part of VeggieNinja.
// 
// Licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
// You may not use this file except in compliance with the License.
// 
// You are free to share and adapt this work for academic and research purposes only,
// provided that proper attribution is given and derivatives are licensed under the same terms.
// Commercial use is strictly prohibited.
// 
// Full license text: https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

#pragma once

#include "CoreMinimal.h"
#include "cPlayer.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class WeaponAttributes
{
public:
	int64 baseDamage;
};

UCLASS()
class GAME_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();
	
	ABaseWeapon& setPlayer(AcPlayer* player);
	
private:
	AcPlayer* cPlayer;
	WeaponAttributes attributes;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default")
	UStaticMeshComponent* baseMesh;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default")
	UBoxComponent* attackFrame;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
