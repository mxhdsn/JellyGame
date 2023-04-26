// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_FollowPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "MyBossAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFrameWork/Character.h"
#include "blackboardkeys.h"

UMyBTTask_FollowPlayer::UMyBTTask_FollowPlayer(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UMyBTTask_FollowPlayer::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	// Get player character and npc controller
	ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	auto const cont = Cast<AMyBossAIController>(owner_comp.GetAIOwner());

	// Get player location
	FVector const playerlocation = player->GetActorLocation();
	if (search_random)
	{
		FNavLocation loc;
		// get navigation system and generate random location near the player
		UNavigationSystemV1* const nav_sys = UNavigationSystemV1::GetCurrent(GetWorld());
		if (nav_sys->GetRandomPointInNavigableRadius(playerlocation, search_radius, loc, nullptr))
		{
			cont->get_blackboard()->SetValueAsVector(bb_keys::PatrolLocation, loc.Location);
		}

	}
	else
	{
		cont->get_blackboard()->SetValueAsVector(bb_keys::PatrolLocation, playerlocation);
	}
	//Finish with succeed
	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}