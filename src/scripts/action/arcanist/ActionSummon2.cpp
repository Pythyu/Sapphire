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

#include "Manager/PartyMgr.h"
#include "Network/Util/PacketUtil.h"
#include "Network/CommonActorControl.h"

using namespace Sapphire;
using namespace Sapphire::World::Action;

// Saphire Carbuncle 1011
// Emerald Carbuncle 1012
// Ifrit-egi 1013
// Titan-egi 1014
// Garuda-edi 1015
// Eos 1008
// Selene 1009
class ActionSummon2 : public Sapphire::ScriptAPI::ActionScript
{
public:
  ActionSummon2() : Sapphire::ScriptAPI::ActionScript( 170 )
  {
  }

  void onExecute( Sapphire::World::Action::Action& action ) override
  {
    auto& teriMgr = Common::Service< World::Manager::TerritoryMgr >::ref();
    //auto& playerMgr = Common::Service< World::Manager::PlayerMgr >::ref();
    //auto player = playerMgr.getPlayer(action.getSourceChara()->getId());
    auto player = action.getSourceChara()->getAsPlayer();
    auto instance = teriMgr.getTerritoryByGuId( player->getTerritoryId() );
    auto PartyMgr =  Common::Service< World::Manager::PartyMgr >::ref();

    player->removePlayerPet(instance);
    auto petId = 1011;
    auto baseActionId = 633;
    auto petNameId = 1400;
    std::string petName = "Saphire Carbuncle";
    if(player->getLevel() >= 35)
    {
      petId = 1014;
      baseActionId = 641;
      petName = "Titan-Egi";
      petNameId = 1403;
    }

    std::shared_ptr<Common::BNPCInstanceObject> carbuncle_instance(new Common::BNPCInstanceObject());
    carbuncle_instance->NameId = petNameId;
    carbuncle_instance->territoryType = instance->getTerritoryTypeId();
    carbuncle_instance->bnpcName = "bnpcCarbuncle";
    carbuncle_instance->instanceId = instance->getGuId();
    carbuncle_instance->nameOffset = 0;
    carbuncle_instance->ActiveType = 1;
    carbuncle_instance->BaseId = petId;
    carbuncle_instance->Level = player->getLevel();



    auto spawned = player->generatePlayerPetNpc(player, instance->getNextActorId(), petName,carbuncle_instance, *instance);

    if( spawned != nullptr )
    {
      spawned->init();
      spawned->m_flags = 0;
      spawned->setTriggerOwnerId( player->getId() );
      auto newGambitPack = World::AI::make_GambitTimeLinePack( -1 );
      auto baseAction = World::Action::make_Action( spawned->getAsChara(), baseActionId, 0 );
      baseAction->setMRange(25);
      newGambitPack->addTimeLine( World::AI::make_TopHateTargetCondition(), baseAction, 3 );
      spawned->updateGambitTimeline(newGambitPack);
      spawned->setObeyGambitPack(newGambitPack);
      spawned->setSicGambitPack(newGambitPack);
      spawned->setSteadyGambitPack(World::AI::make_GambitTimeLinePack( -1 ));


      auto playerPos = player->getPos();
      auto pNavi = instance->getNaviProvider();
      spawned->setPos( playerPos.x, playerPos.y, playerPos.z );
      spawned->sendPositionUpdate();
      instance->updateActorPosition( *spawned );
      instance->pushActor( spawned );
      pNavi->updateAgentMaxSpeed(*spawned, 4.f);

      //PartyMgr.onJoinPet(*player);
      Network::Util::Packet::sendActorControl( *player, player->getId(), Sapphire::Network::ActorControl::SetPetEntityId, 0, spawned->getId(), spawned->getId(), 1);
      Network::Util::Packet::sendActorControl( *player, player->getId(), Sapphire::Network::ActorControl::ShowPetHotbar, spawned->getId(), 2, 8, 17);
    }
    else
    {
      printf( "Unable to spawn carbuncle\n" );
    }

  }
};

EXPOSE_SCRIPT( ActionSummon2 );