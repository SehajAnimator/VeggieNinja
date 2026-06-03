// // Copyright (c) 2026 Studios SehajAvastha// // This file is part of VeggieNinja.// // Licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.// You may not use this file except in compliance with the License.// // You are free to share and adapt this work for academic and research purposes only,// provided that proper attribution is given and derivatives are licensed under the same terms.// Commercial use is strictly prohibited.// // Full license text: https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode


#include "nPlayer.h"

// Sets default values
AnPlayer::AnPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AnPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AnPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AnPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

