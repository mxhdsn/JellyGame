// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBossAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "JellyGameCharacter.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "blackboardkeys.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/Character.h"

AMyBossAIController::AMyBossAIController(FObjectInitializer const& object_initializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/Blueprints/Boss/BT_BossAI.BT_BossAI'"));
	if (obj.Succeeded())
	{
		behaviortree = obj.Object;
	}
	behaviorTreeComp = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	blackboard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));
	setupPerceptionSystem();
}
void AMyBossAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(behaviortree);
	behaviorTreeComp->StartTree(*behaviortree);
}
void AMyBossAIController::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);
	if (blackboard)
	{
		blackboard->InitializeBlackboard(*behaviortree->BlackboardAsset);
	}
}
void AMyBossAIController::onTargetDetected(AActor* actor, FAIStimulus const stimulus)
{
	if (auto const ch = Cast<AJellyGameCharacter>(actor))
	{
		get_blackboard()->SetValueAsBool(bb_keys::HasLineOfSight, stimulus.WasSuccessfullySensed());
	}
}
UBlackboardComponent* AMyBossAIController::get_blackboard() const
{
	return blackboard;
}
void AMyBossAIController::onUpdated(TArray<AActor*> const& updated_actors)
{

}
void AMyBossAIController::setupPerceptionSystem()
{
	// sight config object
	sight_config = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
	sight_config->SightRadius = 500.0f;
	sight_config->LoseSightRadius = sight_config->SightRadius + 50.0f;
	sight_config->PeripheralVisionAngleDegrees = 90.0f;
	sight_config->SetMaxAge(5.0f);
	sight_config->AutoSuccessRangeFromLastSeenLocation = 900.0f;
	sight_config->DetectionByAffiliation.bDetectEnemies = true;
	sight_config->DetectionByAffiliation.bDetectFriendlies = true;
	sight_config->DetectionByAffiliation.bDetectNeutrals = true;

	// add sight config comp to perception comp
	GetPerceptionComponent()->SetDominantSense(*sight_config->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AMyBossAIController::onTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*sight_config);
}
