// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawnExercise.h"

// Sets default values
AMyPawnExercise::AMyPawnExercise()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyStaticMesh"));
	SetRootComponent(MyStaticMeshComponent);
	MyStaticMeshComponent->SetRelativeLocation(FVector(0,0,0));
	MyStaticMeshComponent->SetCollisionProfileName(TEXT("Pawn"));


	//Pawn 需要创建UColliderMovementComponent 才能使用AddMovementInput
	MovementComponent = CreateDefaultSubobject<UColliderMovementComponent>(TEXT("MoveComponent"));
	MovementComponent->UpdatedComponent = RootComponent;

	ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule'"));
	if(Mesh.Succeeded())
	{
		MyStaticMeshComponent->SetStaticMesh(Mesh.Object);
		MyStaticMeshComponent->SetRelativeScale3D(FVector(0.8f,0.8f,0.8f));
	}

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 400.f;
	SpringArmComponent->SetRelativeRotation(FRotator(-45,0,0));
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent,USpringArmComponent::SocketName);
	
	

	
	AutoPossessPlayer =EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AMyPawnExercise::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawnExercise::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator MouseRoation = GetActorRotation();
	MouseRoation.Yaw += MouseInput.Y;
	SetActorRotation(MouseRoation);

	FRotator SpringRotaton = SpringArmComponent->GetRelativeRotation();
	SpringRotaton.Pitch = FMath::Clamp(SpringRotaton.Pitch += MouseInput.X,-80.f,15.f);
	SpringArmComponent->SetRelativeRotation(SpringRotaton);
	
	FString Mystring =TEXT("Hello World");
	UE_LOG(LogTemp,Warning,TEXT("%s"),*Mystring);
}

// Called to bind functionality to input
void AMyPawnExercise::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this,&AMyPawnExercise::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"),this,&AMyPawnExercise::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("CameraPitch"),this,&AMyPawnExercise::MouseX);
	PlayerInputComponent->BindAxis(TEXT("CameraYaw"),this,&AMyPawnExercise::MouseY);
}
void AMyPawnExercise::MoveForward(float InputF)
{
	FVector Forward = GetActorForwardVector();
	AddMovementInput(Forward* InputF);
}
void AMyPawnExercise::MoveRight(float InputR)
{
	FVector Right = GetActorRightVector();
	AddMovementInput(Right * InputR );
}
void AMyPawnExercise::MouseX(float AxisX)
{
	MouseInput.X = AxisX;
}
void AMyPawnExercise::MouseY(float AxisY)
{
	MouseInput.Y = AxisY;
}




