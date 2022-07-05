
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NewPawn.generated.h"

UCLASS()
class BUILDINGESCAPE_API ANewPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ANewPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
