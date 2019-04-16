// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralSplineMesh.h"
#include "Engine/Classes/Engine/StaticMesh.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"
#include "Engine/Classes/Components/SplineComponent.h"
#include "Engine/Classes/Components/SplineMeshComponent.h"
#include "Engine/Engine.h"

#define PRINT(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, x);}

// Sets default values
AProceduralSplineMesh::AProceduralSplineMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProceduralSplineMesh::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProceduralSplineMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma optimize("", off)
void AProceduralSplineMesh::OnConstruction(const FTransform & Transform)
{
	size_t SplineEdgesCount = Spline->GetNumberOfSplinePoints() - 1;
	size_t MeshLength = 0;
	if (SplineMesh)
		MeshLength = SplineMesh->GetBoundingBox().GetSize().Z;

	size_t SplineLength = Spline->GetSplineLength();
	
	PRINT(FString("Spline: ") + FString::FromInt(SplineLength))

	for (size_t i = 0; i < SplineEdgesCount; ++i)
	{
		USplineMeshComponent* SplineMeshComp = NewObject<USplineMeshComponent>(this);
		SplineMeshComp->SetMobility(EComponentMobility::Movable);
		SplineMeshComp->SetupAttachment(Spline);
		SplineMeshComp->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		SplineMeshComp->SetStaticMesh(SplineMesh);
		SplineMeshComp->ForwardAxis = ESplineMeshAxis::Z;
		FVector StartLocationAtPoint;
		FVector StartTangentAtPoint;
		FVector EndLocationAtPoint;
		FVector EndTangentAtPoint;
		Spline->GetLocationAndTangentAtSplinePoint(i, StartLocationAtPoint, StartTangentAtPoint, ESplineCoordinateSpace::Type::Local);
		Spline->GetLocationAndTangentAtSplinePoint(i + 1, EndLocationAtPoint, EndTangentAtPoint, ESplineCoordinateSpace::Type::Local);
		SplineMeshComp->SetStartAndEnd(StartLocationAtPoint, StartTangentAtPoint, EndLocationAtPoint, EndTangentAtPoint);
	}
	RegisterAllComponents();
}
#pragma optimize("", on)