// Fill out your copyright notice in the Description page of Project Settings.


#include "Collider.h"

#include <Windows.Data.Text.h>

#include "ColliderMovementComponent.h"

// Sets default values
ACollider::ACollider()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereMesh"));
	//設定root 為RootComponent
	SetRootComponent(SphereComponent);
	
	SphereComponent->SetSphereRadius(40.f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	MyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MyMeshComponent->SetupAttachment(GetRootComponent());

	//尋找與加載文件裡的模型 （只能在構造函數裡宣告）
	static  ConstructorHelpers::FObjectFinder<UStaticMesh> MeshComponentAsset(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	
	//當文件尋找成功 便Setstaticmensh。。。。。 
	if(MeshComponentAsset.Succeeded())
	{
		MyMeshComponent->SetStaticMesh(MeshComponentAsset.Object);
		MyMeshComponent->SetRelativeLocation(FVector(0.f,0.f,-40.f));
		MyMeshComponent->SetWorldScale3D(FVector(0.8f,0.8f,0.8f));
	}
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeRotation(FRotator(-45.f,0.f,0.f));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3;

	Camera =CreateDefaultSubobject<UCameraComponent>("Camera");

	//把Camera 變成 SpringArm的子物件
	Camera->SetupAttachment(SpringArm,USpringArmComponent::SocketName);
	

	OurMovementComponent = CreateDefaultSubobject<UColliderMovementComponent>(TEXT("OurMovementComponent"));
	//把OurMovementComponent 設定為Updatedcomponent 到 Rootcomponent
	OurMovementComponent->UpdatedComponent = RootComponent;

	CameraInput = FVector2d(0.f,0.f);
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ACollider::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += CameraInput.X;
	SetActorRotation(NewRotation);
	
	FRotator NewSpringArmRotation = SpringArm->GetComponentRotation();
	NewSpringArmRotation.Pitch =  FMath::Clamp(NewSpringArmRotation.Pitch +=CameraInput.Y,-80,15.f);
	SpringArm->SetWorldRotation(NewSpringArmRotation);
}

// Called to bind functionality to input
void ACollider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this,&ACollider::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"),this,&ACollider::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("CameraPitch"),this,&ACollider::PitchCamera);
	PlayerInputComponent->BindAxis(TEXT("CameraYaw"),this,&ACollider::YawCamera);
}

void ACollider::MoveForward(float value)
{
	FVector Forward = GetActorForwardVector();
	if(OurMovementComponent)
	{
		OurMovementComponent->AddInputVector(Forward * value );
	}
}

void ACollider::MoveRight(float value)
{
	FVector Right = GetActorRightVector();
	
	if(OurMovementComponent)
	{
		OurMovementComponent->AddInputVector(Right * value );
	}
}

UPawnMovementComponent* ACollider::GetMovementComponent() const
{
	return  OurMovementComponent;
}

void ACollider::PitchCamera(float CameraY)
{
	CameraInput.Y = CameraY;
}

void ACollider::YawCamera(float CameraX)
{
	CameraInput.X = CameraX;
}

