
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GrappleComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYTEMPLATE_API UGrappleComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		float MaxGrapplingDistance = 2000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		TSubclassOf<class AGrapplingPoint> GrapplingPointBlueprint;

	UPROPERTY()
		class UCableComponent* GrapplingHook;

	UPROPERTY()
		class UTimelineComponent* ThrowGrapplingHookTimeline = nullptr;
	UPROPERTY()
		class UTimelineComponent* GrappleActorMoveTimeline = nullptr;


	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
		UCurveFloat* ThrowTimeCurve = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
		UCurveFloat* MoveCurve = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		TArray<float> Angles;
	UPROPERTY(BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		TArray<class AGrapplingPoint*> GrapplingPoints;

	bool bIsGrappling = false;


	
	// Are we hooked on something?
	bool bHooked;
	// Is the hooked finished moving to the hit location?
	bool bHookedFinished;




protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Sets default values for this component's properties
	UGrappleComponent();




	// Called every frame
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Finds all visible grappling points
	void FindGrapplingPoints();

	//Change the color of the closest grappling point
	void ChangeColor(AGrapplingPoint* GrapplingPoint);

	UFUNCTION(BlueprintCallable)
		class AGrapplingPoint* GetClosestGrapplingPoint() const;

	void AddToGrapplingPoints(class AGrapplingPoint* GrapplingPoint);
	
	
	void RemoveFromGrapplingPoints(class AGrapplingPoint* GrapplingPoint);
	bool CheckIfTooFar(AGrapplingPoint* GrapplingPoint);
	void Grapple();

	UFUNCTION(BlueprintCallable, Category = "GamePlay")
		void LaunchCharacterTowardsTarget( float tick);

	UFUNCTION()
		void ThrowGrapplingHook(float Value);

	FVector LaunchVel;
	bool LaunchedToPoint;


	bool PlayAnim;
	
	FVector PlayerLocation;
	float GrappleTimer;
	bool GrappleNow;
	float grappleAnimTime;
	float GrappleTimerValue;


	void DetachFromGrapplingOnceHit();
};
