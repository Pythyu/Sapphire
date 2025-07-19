#include <Script/NativeScriptApi.h>
#include <ScriptObject.h>
#include <Actor/Player.h>
#include "Actor/Chara.h"
#include <Action/CommonAction.h>
#include <Action/Action.h>
#include <StatusEffect/StatusEffect.h>

#include "Manager/TerritoryMgr.h"
#include "Territory/Territory.h"
#include "Navi/NaviProvider.h"

#include <AI/GambitRule.h>
#include <AI/GambitPack.h>
#include <AI/GambitTargetCondition.h>
#include <AI/Fsm/StateMachine.h>
#include <AI/Fsm/Condition.h>
#include <AI/Fsm/StateIdle.h>

using namespace Sapphire;
using namespace Sapphire::World::Action;

// Saphire Carbuncle 1011
// Emerald Carbuncle 1012
// Ifrit-egi 1013
// Titan-egi 1014
// Garuda-edi 1015
// Eos 1008
// Selene 1009
class ActionSummon : public Sapphire::ScriptAPI::ActionScript
{
public:
  uint32_t currentBaseId = 205;

  ActionSummon() : Sapphire::ScriptAPI::ActionScript( 165 )
  {
  }

  void onExecute( Sapphire::World::Action::Action& action ) override
  {
    auto& teriMgr = Common::Service< World::Manager::TerritoryMgr >::ref();
    auto player = action.getSourceChara()->getAsPlayer();
    auto instance = teriMgr.getTerritoryByGuId( player->getTerritoryId() );
    std::shared_ptr<Common::BNPCInstanceObject> carbuncle_instance(new Common::BNPCInstanceObject());
    carbuncle_instance->NameId = 1401;
    carbuncle_instance->territoryType = instance->getTerritoryTypeId();
    carbuncle_instance->bnpcName = "bnpcCarbuncle";
    carbuncle_instance->instanceId = instance->getGuId();
    carbuncle_instance->nameOffset = 0;
    carbuncle_instance->ActiveType = 1;
    carbuncle_instance->BaseId = 1012;
    carbuncle_instance->Level = player->getLevel();



    auto spawned = player->generatePlayerPetNpc(player, instance->getNextActorId(), carbuncle_instance, *instance);

    if( spawned != nullptr )
    {
      spawned->init();
      spawned->m_flags = 0;
      spawned->setTriggerOwnerId( player->getId() );
      auto newGambitPack = World::AI::make_GambitTimeLinePack( -1 );
      newGambitPack->addTimeLine( World::AI::make_TopHateTargetCondition(), World::Action::make_Action( spawned->getAsChara(), 637, 0 ), 3 );
      newGambitPack->addTimeLine( World::AI::make_TopHateTargetCondition(), World::Action::make_Action( spawned->getAsChara(), 637, 0 ), 6 );
      newGambitPack->addTimeLine( World::AI::make_TopHateTargetCondition(), World::Action::make_Action( spawned->getAsChara(), 637, 0 ), 9 );
      newGambitPack->addTimeLine( World::AI::make_TopHateTargetCondition(), World::Action::make_Action( spawned->getAsChara(), 637, 0 ), 12 );
      spawned->updateGambitTimeline(newGambitPack);
      auto playerPos = player->getPos();
      auto pNavi = instance->getNaviProvider();
      spawned->setPos( playerPos.x, playerPos.y, playerPos.z );
      spawned->sendPositionUpdate();
      instance->updateActorPosition( *spawned );
      instance->pushActor( spawned );
      pNavi->updateAgentMaxSpeed(*spawned, 4.f);
    }
    else
    {
      printf( "Unable to spawn carbuncle\n" );
    }

  }
};

EXPOSE_SCRIPT( ActionSummon );