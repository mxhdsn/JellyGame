// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FindRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "MyBossAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "blackboardKeys.h"

UBTT_FindRandomLocation::UBTT_FindRandomLocation(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Find Random Location");
}
EBTNodeResult::Type UBTT_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	AMyBossAIController* cont = Cast<AMyBossAIController>(owner_comp.GetAIOwner());
	APawn* npc = cont->GetPawn();

	FVector const origin = npc->GetActorLocation();
	FNavLocation loc;

	UNavigationSystemV1* const nav_sys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (nav_sys->GetRandomPointInNavigableRadius(origin, searchRadius, loc, nullptr))
	{
		cont->get_blackboard()->SetValueAsVector(bb_keys::PatrolLocation, loc.Location);
	}

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, npc->GetName() + ": " + loc.Location.ToString());
	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}