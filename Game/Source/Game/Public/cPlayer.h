// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "cPlayer.generated.h"

UCLASS()
class GAME_API AcPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AcPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Custom vars
	// Rootscene
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* rootScene;
	// Default Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	class UCameraComponent* defCamera;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
