// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "MyBossAIController.generated.h"

/**
 * 
 */
UCLASS()
class JELLYGAME_API AMyBossAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMyBossAIController(FObjectInitializer const& object_initializer = FObjectInitializer::Get());
	void BeginPlay() override;
	void OnPossess(APawn* const pawn) override;
	class UBlackboardComponent* get_blackboard() const;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTreeComponent* behaviorTreeComp;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTree* behaviortree;

	class UBlackboardComponent* blackboard;

	class UAISenseConfig_Sight* sight_config;

	UFUNCTION()
		void onUpdated(TArray<AActor*> const& updated_actors);
	UFUNCTION()
		void setupPerceptionSystem();
	UFUNCTION()
		void onTargetDetected(AActor* actor, FAIStimulus const stimulus);
};
