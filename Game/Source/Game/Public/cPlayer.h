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
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "cPlayer.generated.h"

class PlayerAttributes
{
public:
	float moveSpeed = 3000.f;
	float jumpForce = 250.f;
	float maxMoveVelocity = 4500.f;
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
	bool canJump = false;
	
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
	UStaticMeshComponent* bottomCollider;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default")
	UStaticMeshComponent* baseWeapon;
	
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
	bool isGrounded(USceneComponent* comp);
};
