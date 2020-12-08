#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "CharacterBase.h"
#include "GrappleComponent.h"
#include "Materials/MaterialInstance.h"

#include "GrapplingPoint.generated.h"

UCLASS()
class MYTEMPLATE_API AGrapplingPoint : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Point = nullptr;
//ADDED
	UPROPERTY(VisibleAnywhere)
	USceneComponent *Scene =  nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "GamePlay")
		UMaterialInstance* BaseMaterial = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "GamePlay")
		UMaterialInstance* GrapplingMaterial = nullptr;

	UFUNCTION(BlueprintCallable)
		void ChangeToBaseMat(float DeltaTime);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	AGrapplingPoint();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USphereComponent* Collider;

	ACharacterBase* Character;
	


	void ChangeToGrapplingMat();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 otherBodyIndex, bool bfromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	float TimerBeforeColorChanges;
	bool ColorChanged;
};
