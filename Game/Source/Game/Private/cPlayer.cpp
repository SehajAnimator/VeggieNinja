// Fill out your copyright notice in the Description page of Project Settings.


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
	if (CubeMeshAsset.Succeeded())
	{
		playerBase->SetStaticMesh(CubeMeshAsset.Object);
		
	}
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/res/QuestionGun_Baked/StaticMeshes/QuestionGun.QuestionGun"));
	if (MeshAsset.Succeeded())
	{
		baseWeapon->SetStaticMesh(MeshAsset.Object);
	}
	
    playerBase->SetSimulatePhysics(true);
    playerBase->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    playerBase->SetCollisionProfileName(TEXT("PhysicsActor"));
	playerBase->SetEnableGravity(true);
    playerBase->SetupAttachment(RootComponent);
	playerBase->SetVisibility(false);

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
	if (goForward) playerBase->SetPhysicsLinearVelocity(playerBase->GetForwardVector() * this->playerAttributes.moveSpeed);
	if (goLeft) playerBase->SetPhysicsLinearVelocity(playerBase->GetRightVector() * -this->playerAttributes.moveSpeed);
	if (goBackward) playerBase->SetPhysicsLinearVelocity(playerBase->GetForwardVector() * -this->playerAttributes.moveSpeed);
	if (goRight) playerBase->SetPhysicsLinearVelocity(playerBase->GetRightVector() * this->playerAttributes.moveSpeed);
	if (canJump)
	{
		canJump = false;
		playerBase->SetPhysicsLinearVelocity(playerBase->GetUpVector() * this->playerAttributes.jumpForce);
	}
}

// Setters
void AcPlayer::SetSensitivity(float value)
{
	this->cameraSensitivity = value;
}

float AcPlayer::GetSensitivity()
{
	return this->cameraSensitivity;
}

