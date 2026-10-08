// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"

#include "Atank.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API Atank : public ABasePawn
{
	GENERATED_BODY()
	
public:
	Atank();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* defaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* moveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* turnAction;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* armComp;

	UPROPERTY(EditAnywhere)
	float speed = 300.0;

	UPROPERTY(EditAnywhere)
	float turnRate = 100.0;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* cameraComp;

	APlayerController* playerController;
	bool isAlive;

	void MoveInput(const FInputActionValue& value);
	void TurnInput(const FInputActionValue& value);
	void HandleDestruction();
	void SetPlayerEnabled(bool enabled);
};
