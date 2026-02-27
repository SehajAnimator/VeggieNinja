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


#include "cPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/Engine.h"

// Sets default values
AcPlayer::AcPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Root Init
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	
	// Custom Components
	playerBase = CreateDefaultSubobject<UStaticMeshComponent>("PlayerBase");
	
	baseWeapon = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset (TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMeshAsset.Succeeded()) playerBase->SetStaticMesh(CubeMeshAsset.Object);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/res/QuestionGun_Baked/StaticMeshes/QuestionGun.QuestionGun"));
	if (MeshAsset.Succeeded())baseWeapon->SetStaticMesh(MeshAsset.Object);
	
    playerBase->SetSimulatePhysics(true);
    playerBase->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    playerBase->SetCollisionProfileName(TEXT("PhysicsActor"));
	playerBase->SetEnableGravity(true);
	playerBase->SetMassOverrideInKg(NAME_None, 10.f);
	playerBase->SetLinearDamping(1);
	playerBase->SetAngularDamping(1);
    playerBase->SetupAttachment(RootComponent);
	playerBase->SetVisibility(false);
	
	bottomCollider = CreateDefaultSubobject<UStaticMeshComponent>("BottomCollider");
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneAsset(TEXT("/Engine/BasicShapes/Plane.Plane"));
	if (PlaneAsset.Succeeded()) bottomCollider->SetStaticMesh(PlaneAsset.Object);
	
	bottomCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	bottomCollider->SetCollisionProfileName(TEXT("NoCollision"));
	bottomCollider->SetupAttachment(playerBase);
	bottomCollider->SetRelativeLocation(FVector(0, 0, -56));

    playerCamera = CreateDefaultSubobject<UCameraComponent>("DefaultCamera");
	playerCamera->SetupAttachment(playerBase);
	playerCamera->bAutoActivate = false;
	playerCamera->SetRelativeLocation(FVector3d(0.0f, 0.0f, 300.f));
	
	baseWeapon->SetupAttachment(playerCamera);
	baseWeapon->SetRelativeLocation(FVector3d(150.0f, 100.0f, -75.f));
	baseWeapon->SetCollisionProfileName("NoCollision");
}

// Called when the game starts or when spawned
void AcPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// Init Code
	playerController = GetWorld()->GetFirstPlayerController();
	// Player Camera
	playerCamera->Activate(true);
}

// Called every frame
void AcPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Update Code
	GetWorld()->GetFirstPlayerController()->GetInputMouseDelta(mouseX, mouseY);
	this->UpdateCamera();
	this->CheckMovement();
	this->UpdateMovement();

	if (isGrounded(bottomCollider))
	{
		if (GEngine != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(
			-1,
			0.1f,
			FColor::Green,
			TEXT("YES")
			);
		}
	} else
	{
		if (GEngine != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(
			-1,
			0.1f,
			FColor::Red,
			TEXT("NO")
			);
		}
	}
}

// Called to bind functionality to input
void AcPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Custom Methods
void AcPlayer::UpdateCamera()
{
	FRotator camRot = this->playerRotation;
	camRot.Add(mouseY * this->GetSensitivity(), mouseX * this->GetSensitivity(), 0);
	this->playerRotation = camRot;
	
	FRotator baseRotYaw = camRot;
	baseRotYaw.Roll = 0;
	baseRotYaw.Pitch = 0;
	playerBase->SetRelativeRotation(baseRotYaw, false, nullptr, ETeleportType::TeleportPhysics);
	
	FRotator camRotPitch = camRot;
	camRotPitch.Roll = 0;
	camRotPitch.Yaw = 0;
	camRotPitch.Pitch = FMath::Clamp(camRot.Pitch, -90, 90);
	playerCamera->SetRelativeRotation(camRotPitch);
}

void AcPlayer::CheckMovement()
{
	goForward = false;
	goLeft = false;
	goRight = false;
	goBackward = false;
	
	if (playerController->IsInputKeyDown(EKeys::W)) goForward = true;
	if (playerController->IsInputKeyDown(EKeys::Up)) goForward = true;
	if (playerController->IsInputKeyDown(EKeys::A)) goLeft = true;
	if (playerController->IsInputKeyDown(EKeys::Left)) goLeft = true;
	if (playerController->IsInputKeyDown(EKeys::S)) goBackward = true;
	if (playerController->IsInputKeyDown(EKeys::Down)) goBackward = true;
	if (playerController->IsInputKeyDown(EKeys::D)) goRight = true;
	if (playerController->IsInputKeyDown(EKeys::Right)) goRight = true;
	if (playerController->IsInputKeyDown(EKeys::SpaceBar)) canJump = true;
}

void AcPlayer::UpdateMovement()
{
	// Clamp Forces Beforehand
	FVector currVel = playerBase->GetPhysicsLinearVelocity();
	FVector moveVel = FVector(currVel.X, currVel.Y, 0);

	if (moveVel.Size() > playerAttributes.maxMoveVelocity)
	{
		FVector newVel = currVel.GetSafeNormal() * playerAttributes.maxMoveVelocity;
		FVector cVel = FVector(newVel.X, newVel.Y, currVel.Z);
		playerBase->SetPhysicsLinearVelocity(cVel);
	}
	
	// Add Forces
	if (goForward) playerBase->AddForce(playerBase->GetForwardVector() * this->playerAttributes.moveSpeed, NAME_None, true);
	if (goLeft) playerBase->AddForce(playerBase->GetRightVector() * -this->playerAttributes.moveSpeed, NAME_None, true);
	if (goBackward) playerBase->AddForce(playerBase->GetForwardVector() * -this->playerAttributes.moveSpeed, NAME_None, true);
	if (goRight) playerBase->AddForce(playerBase->GetRightVector() * this->playerAttributes.moveSpeed, NAME_None, true);
	
	if (canJump) {
		canJump = false;
		playerBase->AddImpulse(playerBase->GetUpVector() * this->playerAttributes.jumpForce, NAME_None, true);
	}
}

// Setters
void AcPlayer::SetSensitivity(float value)
{
	this->cameraSensitivity = value;
}

// Getters
float AcPlayer::GetSensitivity()
{
	return this->cameraSensitivity;
}

bool AcPlayer::isGrounded(USceneComponent* comp)
{
	FVector loc = comp->GetComponentLocation();
	FVector endLoc = loc;
	endLoc.Z -= 5;
	
	TArray<FHitResult> hitResults;
	GetWorld()->LineTraceMultiByChannel(hitResults ,loc, endLoc, ECC_Visibility);
	
	return hitResults.Num() > 0;
}
