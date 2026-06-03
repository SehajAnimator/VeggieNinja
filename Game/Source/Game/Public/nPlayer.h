// // Copyright (c) 2026 Studios SehajAvastha// // This file is part of VeggieNinja.// // Licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.// You may not use this file except in compliance with the License.// // You are free to share and adapt this work for academic and research purposes only,// provided that proper attribution is given and derivatives are licensed under the same terms.// Commercial use is strictly prohibited.// // Full license text: https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "nPlayer.generated.h"

UCLASS()
class GAME_API AnPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AnPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
