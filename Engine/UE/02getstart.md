
从0创建新工程, 基于 UE5.7

`文件->新建项目->空白项目->C++`

## 新建关卡

建新工程后默认创建一个的未命名关卡, 关卡是一个自带 Landscape, Lighting 和 HLOD 的开放世界, 这对于现在来说还是有点复杂

我们更改为基本关卡, `文件->新建关卡->基本` 这样当前未命名的关卡就只有 `Lighting` 和 一个 `Floor` 了

按 `Ctrl + S` 保存当前关卡, 然后运行游戏, 会发现明明什么都没做，却能前后左右移动和移动视角了, 并且观察右边的项目大纲, 相比未运行时多出了很多游戏实例

比如 `DefaultPawn0, GameModeBase0, PlayerController0, PlayerCameraManager0` 等, 这些到底是什么? 怎么就凭空出现了?


## GameMode

GameMode 是一个用于设置游玩模式的类, 不同的游玩模式可以设置不同的默认 Pawn 类, 玩家控制类, 玩家状态类等

可通过 `编辑->项目设置->地图和模式->默认模式` 中设置默认的 GameMode, 可以看到默认模式为 `GameModeBase` 这也是为什么运行游戏关卡时会出现 `GameModeBase0` 实例

又因为每个 GameMode 都需要配置不同的默认 Pawn 类, 玩家控制类等, 所以 `DefaultPawn0, PlayerController0` 等实例也就被创建进来

DefaultPawn 是引擎自带的一个 Pawn 类, 可用于视角控制和移动, 这就是为何一开始什么都没做却能移动和控制视角

不同的地图关卡可以配置不同的 GameMode, 以实现不同的游戏玩法, 在右侧的 `世界场景设置` (若无则勾选 `窗口->世界场景设置`) 中可以找到 `游戏模式->游戏模式重载`

这是专门为该关卡配置的游戏模式, 如果设置为 None 则会使用全局默认的 GameMode

我们现在创建一个自定义的 GameMode:

```cpp
// MyGameMode.h
#include "MyGameMode.h"
#include "MyCubePawn.h"
#include "MyCubeCharacter.h"

AMyGameMode::AMyGameMode()
{
    // 可以在 c++ 中指定默认 Pawn 类, 但更灵活的方式是使用蓝图
    // 让蓝图继承 AMyGameMode 然后就可以编辑默认 Pawn 类了
    // DefaultPawnClass = AMyCubeCharacter::StaticClass();
}


// MyGameMode.cpp
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameMode.generated.h"

UCLASS()
class AMyGameMode : public AGameModeBase
{
    GENERATED_BODY()

  public:
    AMyGameMode();
};
```

新建一个可控制的 Character 类, 使用简单的输入控制移动, 然后将它作为 MyGameMode 的默认 Pawn 类

```cpp
// MyCubeCharacter.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "MyCubeCharacter.generated.h"

UCLASS()
class DEMO_API AMyCubeCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AMyCubeCharacter();
    virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* CubeMesh;

    UPROPERTY(VisibleAnywhere)
    USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere)
    UCameraComponent* FollowCamera;
};

// MyCubeCharacter.cpp

#include "MyCubeCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerController.h"

AMyCubeCharacter::AMyCubeCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // 创建方块, 附加到角色根组件 (Character 的根组件是胶囊体)
    CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
    CubeMesh->SetupAttachment(RootComponent);
    
    // 加载默认模型
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeAsset.Succeeded())
    {
        CubeMesh->SetStaticMesh(CubeAsset.Object);
    }

    // 弹簧臂和相机
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);

    // 相机
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(SpringArm);

    // 弹簧臂距离
    SpringArm->TargetArmLength = 500.0f;
}

void AMyCubeCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    float MoveX = 0;
    float MoveY = 0;

    // 简单输入
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        if (PC->IsInputKeyDown(EKeys::W)) MoveY = 1;
        if (PC->IsInputKeyDown(EKeys::S)) MoveY = -1;
        if (PC->IsInputKeyDown(EKeys::D)) MoveX = 1;
        if (PC->IsInputKeyDown(EKeys::A)) MoveX = -1;
    }

    // 移动角色
    AddMovementInput(GetActorForwardVector(), MoveY);
    AddMovementInput(GetActorRightVector(), MoveX);
}
```

## 蓝图和C++

C++ 定义的类不支持直接在编辑器中配置, 而转成蓝图后就可以配置了, 所以可以将实现的 C++ 类 MyGameMode, PlayerController, MyCubeCharacter 转成蓝图类, 再使用方便编辑

## 增强输入系统

增强输入功能把物理输入和 `输入操作` 逻辑分离开来, 并且输入配置可作为一种资产, 而不是写死在代码里面或需通过配置编辑器来配置输入, 同时每个关卡地图支持载入不同的输入操作。提高了输入的灵活性。

首先创建一个 Input 文件夹用于存放输入资产, 然后进入该文件夹, `右击->输入->输入操作` 创建一个 `InputAction` 命名为 `IA_Move` ,  用于定义移动操作

打开 `IA_Move` 输入操作, 在 `值类型` 中选择 `Vector2D`, 这样就可以同时绑定两个轴向输入了

接下来创建一个 `InputMappingContext` 输入映射上下文 (IMC), 命名为 `IMC_Default`, 用于定义默认的输入映射, 然后进入选择 `映射->默认按键映射` 点击 + 号添加对输入操作的映射

选择刚才创建的输入操作 `IA_Move` 然后添加对 Move 操作的按键映射, 可以点击键盘图标案后按下键盘或手柄就可以自动识别了, 这里可以选择添加 4 按键 WASD 映射

然后点进去设置触发器, 触发器是一个数组, 意思是支持多种触发事件, 比如按下、松开、重复等, 只要其中一个发生都会触发

修改器则用来修改默认行为, 因为 IA_Move 输入操作是 Vector2D 类型, 而键盘按键是 1D 类型, 默认按下只会更改 X = 1, 对应 Vector2D = (1, 0)

通过`拌合(Swizzle)输入轴值`, 或者说重排输入轴循序变成 YXZ, 这时 W 按下就会触发 Y = 1, 对应 Vector2D = (0, 1), 而 S 键需要添加2个修改, 一个是重排输入轴, 另一个是否定, 使输入变成 Y = -1, 对应 Vector2D = (0, -1)


需要在 PlayerController 类中将输入映射上下文 (IMC) 添加到增强输入系统中, 因此定义一个 MyPlayerController 类中将输入映射上下文

```cpp
// MyPlayerController.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;

/**
 *  Basic PlayerController class for a third person game
 *  Manages input mappings
 */
UCLASS(abstract)
class AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category ="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;
};


////////////////////////////////////////////////////////////////////////////////

// MyPlayerController.cpp

#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}
		}
	}
}
```

然后创建基于 My_MyPlayerController 的蓝图类 BP_MyPlayerController,  然后编辑设置 DefaultMappingContexts 为 之前创建的 IMC_Default


接着创建一个使用增强输入的第三人称控制的角色

```cpp
// MyCubeCharacter.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "MyCubeCharacter.generated.h"

UCLASS()
class DEMO_API AMyCubeCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AMyCubeCharacter();
    virtual void Tick(float DeltaTime) override;

protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();


private:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* CubeMesh;

    UPROPERTY(VisibleAnywhere)
    USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere)
    UCameraComponent* FollowCamera;
};


////////////////////////////////////////////////////////////////////////////////

// MyCubeCharacter.cpp

#include "MyCubeCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"

AMyCubeCharacter::AMyCubeCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Don't rotate when the controller rotates. Let that just affect the camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

    // Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
    // instead of recompiling to adjust them
    GetCharacterMovement()->JumpZVelocity = 500.f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
    GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

    // 创建方块, 附加到角色根组件 (Character 的根组件是胶囊体)
    CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
    CubeMesh->SetupAttachment(RootComponent);

    // 加载默认模型
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeAsset.Succeeded())
    {
        CubeMesh->SetStaticMesh(CubeAsset.Object);
    }

    // 弹簧臂和相机
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);

    // 相机
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(SpringArm);

    // 弹簧臂距离
    SpringArm->TargetArmLength = 500.0f;
}

void AMyCubeCharacter::Tick(float DeltaTime)
{
}

void AMyCubeCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    // Set up action bindings
    if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Jumping
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyCubeCharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMyCubeCharacter::StopJumping);

        // Moving
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCubeCharacter::Move);
        EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AMyCubeCharacter::Look);

        // Looking
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCubeCharacter::Look);
    }
}

void AMyCubeCharacter::Move(const FInputActionValue &Value)
{
    // input is a Vector2D
    FVector2D MovementVector = Value.Get<FVector2D>();

    // route the input
    DoMove(MovementVector.X, MovementVector.Y);
}

void AMyCubeCharacter::Look(const FInputActionValue &Value)
{
    // input is a Vector2D
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    // route the input
    DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AMyCubeCharacter::DoMove(float Right, float Forward)
{
    if (GetController() != nullptr)
    {
        // find out which way is forward
        const FRotator Rotation = GetController()->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        // get right vector
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // add movement
        AddMovementInput(ForwardDirection, Forward);
        AddMovementInput(RightDirection, Right);
    }
}

void AMyCubeCharacter::DoLook(float Yaw, float Pitch)
{
    if (GetController() != nullptr)
    {
        // add yaw and pitch input to controller
        AddControllerYawInput(Yaw);
        AddControllerPitchInput(Pitch);
    }
}

void AMyCubeCharacter::DoJumpStart()
{
    // signal the character to jump
    Jump();
}

void AMyCubeCharacter::DoJumpEnd()
{
    // signal the character to stop jumping
    StopJumping();
}
```

创建 MyGameMode 和 MyCubeCharacter 的蓝图类 BP_MyCubeCharacter 和 BP_MyGameMode, 并设置好 BP_MyCubeCharacter 中的输入操作 `MoveAction` 等

将光关卡设置为 BP_MyGameMode,  并在 `BP_MyGameMode` 中设置 `DefaultPawnClass` 为 `BP_MyCubeCharacter`, 将 `玩家控制器设置` 为 `BP_MyPlayerController`


## 总结

启动游戏时会加载默认关卡, 关卡会根据设置加载对应的 GameMode, 这时 GameMode 中设置有默认 Pawn 类和 PlayerController 类会被加载进来

其中 `Pawn 类` 实例会被加载到设置的玩家出生点, `PlayerController` 会被实例化给当前 Pawn 实例, 表示当前 `Pawn` 的控制类型。Pawn 有一个方法 GetController 可以用来获取当前 Controller

而 `PlayerController` 则可以添加之前配置号的输入操作映射 `InputMappingContext` 实例, 然后在 SetupInputComponent(); 中添加到系统中, 从而 `Pawn` 可以通过 `GetController` 获取

关于蓝图类.

C++ 构建的类是无法直接在编辑器中修改参数的, 而转化为蓝图类后却能方便地在编辑器中修改导出地参数列表地默认值, 改用蓝图地地方还是使用比较方便



