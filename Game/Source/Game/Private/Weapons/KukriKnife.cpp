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

#include "Weapons/KukriKnife.h"

AKukriKnife::AKukriKnife()
{
	PrimaryActorTick.bCanEverTick = true;
	
	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset (TEXT("/Game/res/Weapons/old/KukriKnife/StaticMeshes/KukriKnife.KukriKnife"));
	if (MeshAsset.Succeeded()) baseMesh->SetStaticMesh(MeshAsset.Object);

	baseMesh->SetupAttachment(RootComponent);
}

void AKukriKnife::BeginPlay()
{
	Super::BeginPlay();
	
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		baseMesh->SetCollisionProfileName(TEXT("NoCollision"));
		baseMesh->SetRelativeScale3D(FVector(0.2, 0.2, 0.2));
	}
	
	viewOffset = FVector(150, 50, -25);
	rotOffset = FRotator(0, -90, 0);
}

void AKukriKnife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
