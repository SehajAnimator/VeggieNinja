// Fill out your copyright notice in the Description page of Project Settings.


#include "cPlayer.h"
#include "Camera/CameraComponent.h"
#include "Engine/Engine.h"

// Sets default values
AcPlayer::AcPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Constructor Manual Gen
		// Root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
		// Camera
	defCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("DefaultCamera"));
	defCamera->SetupAttachment(RootComponent);
	defCamera->SetRelativeLocation(FVector(0.f, 0.f, 50.f));

	GAreScreenMessagesEnabled = true;
}

// Called when the game starts or when spawned
void AcPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AcPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update Loop
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Test"));
	}

	UE_LOG(LogTemp, Error, TEXT("tick working"));
}

// Called to bind functionality to input
void AcPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

