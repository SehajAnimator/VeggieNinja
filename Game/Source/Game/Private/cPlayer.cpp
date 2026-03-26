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
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/Engine.h"
#include "Weapons/KukriKnife.h"
#include "Components/ChildActorComponent.h"

// Sets default values
AcPlayer::AcPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Root Init
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	
	// Custom Components
	playerBase = CreateDefaultSubobject<UCapsuleComponent>("PlayerBase");
	playerBase->InitCapsuleSize(50, 100);
	
    playerBase->SetSimulatePhysics(true);
    playerBase->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    playerBase->SetCollisionProfileName(TEXT("PhysicsActor"));
	playerBase->SetEnableGravity(true);
	playerBase->SetMassOverrideInKg(NAME_None, 100.f);
	playerBase->SetLinearDamping(1);
	playerBase->SetAngularDamping(1);
    playerBase->SetupAttachment(RootComponent);
	// Physics Error Instance
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		playerBase->BodyInstance.bLockXRotation = true;
		playerBase->BodyInstance.bLockYRotation = true;
	}
	
	bottomCollider = CreateDefaultSubobject<USphereComponent>("BottomCollider");
	bottomCollider->InitSphereRadius(50);	
	bottomCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	bottomCollider->SetCollisionProfileName(TEXT("OverlapAll"));
	bottomCollider->SetupAttachment(playerBase);
	bottomCollider->SetVisibility(true);
	bottomCollider->SetRelativeLocation(FVector(0, 0, -125));

    playerCamera = CreateDefaultSubobject<UCameraComponent>("DefaultCamera");
	playerCamera->SetupAttachment(playerBase);
	playerCamera->bAutoActivate = false;
	playerCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 300.f));
	
	equippedWeapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("EquippedWeapon"));
	equippedWeapon->AttachToComponent(playerCamera, FAttachmentTransformRules::KeepRelativeTransform);
	equippedWeapon->SetChildActorClass(AKukriKnife::StaticClass());
}

// Called when the game starts or when spawned
void AcPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// Init Code
	playerController = GetWorld()->GetFirstPlayerController();
	playerController->Possess(this);
	// Player Camera
	playerCamera->Activate(true);
	
	AKukriKnife* child = Cast<AKukriKnife>(equippedWeapon->GetChildActor());
	if (child)
	{
		child->SetBase(playerBase).SetView(playerCamera);
		equippedWeapon->SetRelativeLocation(child->viewOffset);
		equippedWeapon->SetRelativeRotation(child->rotOffset);
	}
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
	camRotPitch.Pitch = FMath::Clamp(camRotPitch.Pitch, -90, 90);
	playerCamera->SetRelativeRotation(camRotPitch);
}

void AcPlayer::CheckMovement()
{
	goForward = false;
	goLeft = false;
	goRight = false;
	goBackward = false;
	
	if (playerController->IsInputKeyDown(EKeys::W)) goForward = true;
	if (playerController->IsInputKeyDown(EKeys::A)) goLeft = true;
	if (playerController->IsInputKeyDown(EKeys::S)) goBackward = true;
	if (playerController->IsInputKeyDown(EKeys::D)) goRight = true;
	if (playerController->IsInputKeyDown(EKeys::SpaceBar) && isGrounded()) canJump = true;
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
	
	// Add Movement
	FVector playerVelocity = playerBase->GetPhysicsLinearVelocity();
	if (goForward) playerVelocity += (playerBase->GetForwardVector() * this->playerAttributes.moveSpeed);
	if (goLeft) playerVelocity += (playerBase->GetRightVector() * -this->playerAttributes.moveSpeed);
	if (goBackward) playerVelocity += (playerBase->GetForwardVector() * -this->playerAttributes.moveSpeed);
	if (goRight) playerVelocity += (playerBase->GetRightVector() * this->playerAttributes.moveSpeed);
	playerBase->SetPhysicsLinearVelocity(playerVelocity);
	
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
float AcPlayer::GetSensitivity() const
{
	return this->cameraSensitivity;
}

bool AcPlayer::isGrounded()
{
	// FVector loc = comp->GetComponentLocation();
	// FVector endLoc = loc;
	// endLoc.Z -= 5;
	//
	// TArray<FHitResult> hitResults;
	// GetWorld()->LineTraceMultiByChannel(hitResults ,loc, endLoc, ECC_Visibility);
	//
	// return hitResults.Num() > 0;
	
	TArray<UPrimitiveComponent*> comp;
	bottomCollider->GetOverlappingComponents(comp);
		
	for (UPrimitiveComponent* compi : comp)
	{
		if (!compi->IsA(UCapsuleComponent::StaticClass())) return true;
	}
	return false;
}

UCapsuleComponent* AcPlayer::GetBase() const
{
	return this->playerBase;
}
