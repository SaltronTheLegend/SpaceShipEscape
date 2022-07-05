// Copyright Akif Hoda 2022
#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber(){
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay(){
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}
void UGrabber::SetupInputComponent(){
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent){
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}
void UGrabber::FindPhysicsHandle(){
	// Checking for Physics Handle Component
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle){
		// Physics handle not found
		UE_LOG(LogTemp, Error, TEXT("No physics handle component found on %s!"), *GetOwner()->GetName());
	}
}
void UGrabber::Grab(){
	//UE_LOG(LogTemp, Warning, TEXT("Grabber pressed"));

	// to only raycast when key is pressed then try and reach any actors with physics body collision channel set.
    FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
    AActor* ActorHit = HitResult.GetActor();
	// If we hit something attach a physics handle.
    if(ActorHit){
	    // attach  physics handle
		if(!PhysicsHandle) {return;}
	    PhysicsHandle->GrabComponentAtLocation(
		    ComponentToGrab,
		    NAME_None,
		    GetPlayerReach()
	);
	}
}


void UGrabber::Release(){
	//UE_LOG(LogTemp, Warning, TEXT("Grabber released"));
	if(!PhysicsHandle) {return;}
	// remove/release the physics handle.
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
	// If the physics handle is attached.
	if(PhysicsHandle->GrabbedComponent){
	    // Move the object we are holding.
		PhysicsHandle->SetTargetLocation(GetPlayerReach());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const{
	// Ray-cast to a certain distance(reach)
	FHitResult Hit;
	FCollisionQueryParams TraceParms(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerWorldPosition(),
		GetPlayerReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        TraceParms
	);

	AActor* ActorHit =  Hit.GetActor();
	if (ActorHit)
	    UE_LOG(LogTemp, Warning, TEXT("Line Trace has hit: %s"), *(ActorHit->GetName()))
	
	return Hit;
}

FVector UGrabber::GetPlayerReach() const{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	// return a line from player showing the reach
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

FVector UGrabber::GetPlayerWorldPosition() const{
	// get player viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	// draw a line from player showing the reach
	return PlayerViewPointLocation;
}
