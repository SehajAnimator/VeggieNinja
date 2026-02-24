// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "cPlayer.generated.h"

class PlayerAttributes
{
public:
	float moveSpeed = 250.f;
};

UCLASS()
class GAME_API AcPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AcPlayer();

private:
	FRotator playerRotation = {};
	float cameraSensitivity = 3.f;
	float mouseX = 0, mouseY = 0;
	PlayerAttributes playerAttributes;
	APlayerController* playerController;
	
	bool goForward = false, goBackward = false, goRight = false, goLeft = false;
	bool canJump = false, isJumping = false;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Custom Component
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default")
	UStaticMeshComponent* playerBase;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default")
	UCameraComponent* playerCamera;
	
	// Custom Methods
	void UpdateCamera();
	void CheckMovement();
	void UpdateMovement();
	// Setters
	void SetSensitivity(float value);
	// Getters
	float GetSensitivity();
};
