#include "GrappleComponent.h"
#include "Components/TimelineComponent.h"
#include "GrapplingPoint.h"
#include "CharacterBase.h"
#include "CableComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UGrappleComponent::UGrappleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// GrapplingHook setup
	GrapplingHook = CreateDefaultSubobject<UCableComponent>(TEXT("GrapplingHook"));
	GrapplingHook->bVisible = false;
	GrapplingHook->EndLocation = FVector::ZeroVector;
	GrapplingHook->CableLength = 0;

	ThrowGrapplingHookTimeline = CreateDefaultSubobject<UTimelineComponent>("ThrowGrapplingHookTimeline");
	GrappleActorMoveTimeline = CreateDefaultSubobject<UTimelineComponent>("GrappleActorMoveTimeline");
}


// Called when the game starts
void UGrappleComponent::BeginPlay()
{
	Super::BeginPlay();
	LaunchedToPoint = false;
	ACharacterBase* Player = Cast<ACharacterBase>(GetOwner());
	if (Player)
	{
		GrapplingHook->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "GrapplingHook");
	}


	grappleTimer = 10;
	PlayAnim = true;
}


// Called every frame
void UGrappleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindGrapplingPoints();

	if (bIsGrappling)
	{
		(LaunchCharacterTowardsTarget());
	}

	GrappleShootNow();
}

void UGrappleComponent::FindGrapplingPoints()
{
	if (!bIsGrappling)
	{
		if (ensure(GrapplingPointBlueprint))
		{
			TArray<AActor*>SpawnedGrapplingPoints;
			UGameplayStatics::GetAllActorsOfClass(this, GrapplingPointBlueprint, SpawnedGrapplingPoints);

			if (SpawnedGrapplingPoints.Num() != 0)
			{
				for (AActor* GrapplingPointActor : SpawnedGrapplingPoints)
				{
					AGrapplingPoint* GrapplingPoint = Cast<AGrapplingPoint>(GrapplingPointActor);

					if (GrapplingPoint)
					{
						if (CheckIfTooFar(GrapplingPoint) == false)
						{
							if (GrapplingPoint->WasRecentlyRendered())
							{
								AddToGrapplingPoints(GrapplingPoint);

							}
							else
							{
								RemoveFromGrapplingPoints(GrapplingPoint);
							}
							
						}
						ChangeColor(GrapplingPoint);
					}
				}
			}
		}
	}
}

void UGrappleComponent::ChangeColor(AGrapplingPoint* GrapplingPoint)
{
	GrapplingPoint->ChangeToBaseMat();
	if (GetClosestGrapplingPoint())
	{
		GetClosestGrapplingPoint()->ChangeToGrapplingMat();
	}
}

void UGrappleComponent::AddToGrapplingPoints(class AGrapplingPoint* GrapplingPoint)
{
	ACharacterBase* Player = Cast<ACharacterBase>(GetOwner());
	if (!Player) { return; }

	float Distance = (Player->GetActorLocation() - GrapplingPoint->GetActorLocation()).Size();

	if (Distance <= MaxGrapplingDistance)
	{
		FVector ControllerForwardVector = FVector(Player->GetControlRotation().Vector().X, Player->GetControlRotation().Vector().Y, 0.f).GetSafeNormal();
		FVector GrapplingPointDirection = FVector((Player->GetActorLocation() - GrapplingPoint->GetActorLocation()).X,
			(Player->GetActorLocation() - GrapplingPoint->GetActorLocation()).Y, 0.f).GetSafeNormal();

		float Angle = FMath::Acos(FMath::Abs(FVector::DotProduct(ControllerForwardVector, GrapplingPointDirection)));


		if (FVector::DotProduct(-ControllerForwardVector, GrapplingPointDirection) >= 0)
		{
			if (GrapplingPoints.Contains(GrapplingPoint))
			{
				int32 Index = GrapplingPoints.Find(GrapplingPoint);
				Angles[Index] = Angle;
			}
			else
			{
				GrapplingPoints.Add(GrapplingPoint);
				Angles.Add(Angle);
			}
		}
	}

}

void UGrappleComponent::RemoveFromGrapplingPoints(class AGrapplingPoint* GrapplingPoint)
{
	ACharacterBase* Player = Cast<ACharacterBase>(GetOwner());
	if (!Player) { return; }

	float Distance = (Player->GetActorLocation() - GrapplingPoint->GetActorLocation()).Size();

	if (GrapplingPoints.Contains(GrapplingPoint))
	{
		int32 Index = GrapplingPoints.Find(GrapplingPoint);
		GrapplingPoints.RemoveAt(Index);
		Angles.RemoveAt(Index);
	}

	//if (Distance >= MaxGrapplingDistance)
	//{
	//	int32 Index = GrapplingPoints.Find(GrapplingPoint);
	//	GrapplingPoints.RemoveAt(Index);
	//	Angles.RemoveAt(Index);
	//}
}
/*
bool UGrappleComponent::CheckIfTooFar(class AGrapplingPoint* GrapplingPoint)
{
	ACharacterBase* Player = Cast<ACharacterBase>(GetOwner());	
	float Distance = (Player->GetActorLocation() - GrapplingPoint->GetActorLocation()).Size();

	if ((Distance >= MaxGrapplingDistance))
	{
		RemoveFromGrapplingPoints(GrapplingPoint);
		return true;
	}

	return false;

}
*/

bool UGrappleComponent::CheckIfTooFar(class AGrapplingPoint* GrapplingPoint)
{
	ACharacterBase* Player = Cast<ACharacterBase>(GetOwner());
	float Distance = (Player->GetActorLocation() - GrapplingPoint->GetActorLocation()).Size();

	FVector ControllerForwardVector = FVector(Player->GetControlRotation().Vector().X, Player->GetControlRotation().Vector().Y, 0.f).GetSafeNormal();
	FVector GrapplingPointDirection = FVector((Player->GetActorLocation() - GrapplingPoint->GetActorLocation()).X,
		(Player->GetActorLocation() - GrapplingPoint->GetActorLocation()).Y, 0.f).GetSafeNormal();

	float Angle = FMath::Acos(FMath::Abs(FVector::DotProduct(ControllerForwardVector, GrapplingPointDirection)));



	if ((Distance >= MaxGrapplingDistance) || (FVector::DotProduct(-ControllerForwardVector, GrapplingPointDirection) <= 0))
	{
		RemoveFromGrapplingPoints(GrapplingPoint);
		return true;
	}


	return false;
}


AGrapplingPoint* UGrappleComponent::GetClosestGrapplingPoint() const
{
	if ((Angles.Num() != 0) && (GrapplingPoints.Num() != 0))
	{
		float MinAngle = Angles[0];
		for (const float Angle : Angles)
		{
			MinAngle = (Angle < MinAngle) ? Angle : MinAngle;
		}

		return GrapplingPoints[Angles.Find(MinAngle)];
	}

	return nullptr;
}

//THROWING THE GRAPPLE
void UGrappleComponent::Grapple()
{

	AGrapplingPoint* GrapplingPoint = GetClosestGrapplingPoint();
	ACharacterBase* Player = Cast<ACharacterBase>(GetOwner());
	if (!Player) { return; }
	
	

	//horizontal raycast
	FHitResult Out1;
	FVector ControllerForwardVector = Player->GetActorLocation();
	FCollisionQueryParams  CollisionP1;

	if (Player->Stamina > 0.3)
	{
		
		//DrawDebugLine(GetWorld(), Start1, End1, FColor::Red, false, 1, 0, 1);
		if (GrapplingPoint != nullptr)
		{

			bool IsBlocked = GetWorld()->LineTraceSingleByChannel(Out1, ControllerForwardVector, GrapplingPoint->GetActorLocation(), ECC_Visibility, CollisionP1);

			DrawDebugLine(GetWorld(), ControllerForwardVector, GrapplingPoint->GetActorLocation(), FColor::Green, true, 1, 0, 1);

			if (IsBlocked == true)
			{

				if (Out1.Actor->ActorHasTag("Grapple") == true)
				{

					if (GrapplingPoint)
					{
						GrapplingHook->SetVisibility(true);


						if (ensure(ThrowTimeCurve))
						{
							// get the ThrowHook time
							float MinTime = 0.f, CurveLength = 0.f;
							ThrowTimeCurve->GetTimeRange(MinTime, CurveLength);

							// setup the Timeline
							ThrowGrapplingHookTimeline->SetLooping(false);
							ThrowGrapplingHookTimeline->SetIgnoreTimeDilation(true);
							ThrowGrapplingHookTimeline->SetTimelineLength(CurveLength);

							// set the ThrowHookDelegate to be bound to ThrowGrapplingHook
							FOnTimelineFloat ThrowHookDelegate;
							ThrowHookDelegate.BindUFunction(this, "ThrowGrapplingHook");
							ThrowGrapplingHookTimeline->AddInterpFloat(ThrowTimeCurve, ThrowHookDelegate, "Value");

							// launch the player when the grappling hook is at target location
							FOnTimelineEvent TimelineFinished;
							//TimelineFinished.BindUFunction(this, FName("LaunchCharacterTowardsTarget"));
							ThrowGrapplingHookTimeline->SetTimelineFinishedFunc(TimelineFinished);

							bIsGrappling = true;
							ThrowGrapplingHookTimeline->PlayFromStart();
						}
					}
				}
			}
			//else
			//{
			//
			//}

		}
	}
}


void UGrappleComponent::LaunchCharacterTowardsTarget()
{
	ACharacterBase* Player = Cast<ACharacterBase>(GetOwner());
	if (!Player) { return; }

	AGrapplingPoint* Target = GetClosestGrapplingPoint();
	
	
	if (Target && GrapplingHook)
	{
		FVector LaunchVelocity = FVector::ZeroVector;
		bool bHasSolution = UGameplayStatics::SuggestProjectileVelocity_CustomArc(this,
			LaunchVelocity,
			Player->GetActorLocation(),
			Target->GetActorLocation(),
			0.0f, 0.5f);

		if (bHasSolution)
		{

			PlayerLocation = Player->GetActorLocation();
			bool VChecker = UKismetMathLibrary::EqualEqual_VectorVector(GetClosestGrapplingPoint()->GetActorLocation(), PlayerLocation, 80);		//I CHANGED IT TO 80
			
			if (VChecker == false)		//if it hasnt collided with the grapple point
			{

				grappleTimer -= 0.1f;


				//Animation playing
				if (PlayAnim == true)
				{
					float grappleAnim = 0.7f;
					Player->PlayAnimMontage(Player->GrappleAnim, 1, NAME_None);


					//delay
					GetWorld()->GetTimerManager().SetTimer(GrappleShootDelay, this, &UGrappleComponent::GrappleShootNow, grappleAnim, false);

					//play sound
					UWorld* WorldContextObject = GetWorld();
					UGameplayStatics::PlaySound2D(WorldContextObject, Player->GrappleSound, 1.0f, 1.0f, 0, NULL, NULL);
					PlayAnim = false;
				}

				

				if (LaunchedToPoint == false)
				{
					Player->Stamina -= 0.3f;
					LaunchVel = ((GetClosestGrapplingPoint()->GetActorLocation() - PlayerLocation)) *1.5f ;
					LaunchedToPoint = true;
					
				}

				/*
				if (grappleAnim  > 0)
				{
					Player->LaunchCharacter(LaunchVel, true, true);
					PlayAnim = true;
				}*/
				
				else if(grappleTimer<=0)//if he hasnt reached the point and the timers depleted!!!!!!!!!!					
				{
					GrapplingHook->SetVisibility(false);
					bIsGrappling = false;
					
					GrapplingHook->SetWorldLocation(Player->GetMesh()->GetSocketLocation("GrapplingHook"));

					Player->LaunchCharacter(LaunchVel * 0.5f, true, true);
					PlayAnim = true;
				
					LaunchedToPoint = false;
					grappleTimer = 10;
				}
				
				
			}
			else if (VChecker == true)
			{
				GrapplingHook->SetVisibility(false);
				bIsGrappling = false;
				GrapplingHook->SetWorldLocation(Player->GetMesh()->GetSocketLocation("GrapplingHook"));
				
				Player->LaunchCharacter(LaunchVel*0.5f, true, true);
				PlayAnim = true;
				
				LaunchedToPoint = false;
				grappleTimer = 10;
			}
		}
	}






}

void UGrappleComponent::ThrowGrapplingHook(float Value)
{
	if (GrapplingHook && GetClosestGrapplingPoint())
	{
		FVector NewLocation = FMath::Lerp<FVector, float>(GrapplingHook->GetComponentLocation(), GetClosestGrapplingPoint()->GetActorLocation(), Value);
		GrapplingHook->SetWorldLocation(NewLocation);
	}
}

void UGrappleComponent::GrappleShootNow()
{
	ACharacterBase* Player = Cast<ACharacterBase>(GetOwner());
	if (!Player) { return; }
	
		Player->LaunchCharacter(LaunchVel, true, true);
		PlayAnim = true;
	



	//RESET THE TIMER
	GetWorld()->GetTimerManager().ClearTimer(GrappleShootDelay);

}
