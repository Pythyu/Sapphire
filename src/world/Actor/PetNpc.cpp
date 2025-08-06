#include "BNpc.h"
#include "PetNpc.h"
#include <Util/Util.h>
#include "Player.h"
#include "Util/UtilMath.h"
#include "Manager/TerritoryMgr.h"
#include "Service.h"
#include "Action/Action.h"

#include "Territory/Territory.h"
#include "Navi/NaviProvider.h"

#include <AI/GambitRule.h>
#include <AI/GambitPack.h>
#include <AI/GambitTargetCondition.h>
#include <AI/Fsm/StateMachine.h>
#include <AI/Fsm/Condition.h>
#include <AI/Fsm/StateIdle.h>
#include <AI/Fsm/StateFollow.h>
#include <AI/Fsm/StateRoam.h>
#include <AI/Fsm/StateCombat.h>
#include <AI/Fsm/StateRetreat.h>
#include <AI/Fsm/StateDead.h>


using namespace Sapphire::Entity;

PetNpc::PetNpc() : BNpc()
{
}


PetNpc::PetNpc( PlayerPtr owner, uint32_t id, std::shared_ptr< Common::BNPCInstanceObject > pInfo, const Territory& zone ) : BNpc(id, pInfo, zone)
{
  m_playerOwner = owner;
  m_movingToPlayer = false;
  m_followRange = .8f;
}

PetNpc::PetNpc( PlayerPtr owner, uint32_t id, std::string petName, std::shared_ptr< Common::BNPCInstanceObject > pInfo, const Territory& zone, uint32_t hp, Common::BNpcType type ) : BNpc(id, pInfo, zone, hp, type)
{
  m_playerOwner = owner;
  m_movingToPlayer = false;
  m_followRange = .8f;
  std::strcpy(m_name, petName.c_str());
}

PetNpc::~PetNpc() = default;

PlayerPtr PetNpc::getOwner()
{
  return m_playerOwner;
}

void PetNpc::setOwner(PlayerPtr owner)
{
  m_playerOwner = owner;
}

void PetNpc::update( uint64_t tickCount )
{
  BNpc::update(tickCount);
}

const PlayerPtr& PetNpc::getPlayerOwner() const
{
  return m_playerOwner;
}

void PetNpc::petBehavior(PetBaseAction action)
{
  switch( action )
  {
    case Away:
      hateListClear();
      break;
    case Heel:
      removeFlag(Immobile);
      break;
    case Stay:
      setFlag(Immobile);
      break;
    case Guard:
      removeFlag(NoAggro);
      updateGambitTimeline(m_ObeyGambitPack->getAsTimeLine());
      break;
    case Steady:
      setFlag(NoAggro);
      updateGambitTimeline(m_SteadyGambitPack->getAsTimeLine());
      break;
    case Sic:
      removeFlag(NoAggro);
      updateGambitTimeline(m_SicGambitPack->getAsTimeLine());
      break;
    case Obey:
      removeFlag(NoAggro);
      updateGambitTimeline(m_ObeyGambitPack->getAsTimeLine());
      break;


    default:
      Logger::debug("PetAction {0} isn't implemented yet !\n", action);
      break;
  }
}

void PetNpc::init()
{
  setFlag(NoDeaggro);
  BNpc::init();
  auto stateFollow = World::AI::Fsm::make_StateFollow();
  auto stateIdle = m_fsm->getState(0);
  auto CombatTransition = stateIdle->getIndexedTransition(0);
  auto stateCombat = CombatTransition->getTargetState();

  stateFollow->addTransition(stateIdle, World::AI::Fsm::make_isTargetInFollowRange());
  stateFollow->addTransition(stateCombat, World::AI::Fsm::make_HateListHasEntriesCondition());

  stateIdle->addTransition(stateFollow, World::AI::Fsm::make_isTargetOutOfFollowRange());
}
const Sapphire::World::AI::GambitPackPtr& PetNpc::getObeyGambitPack() const
{
  return m_ObeyGambitPack;
}
const Sapphire::World::AI::GambitPackPtr& PetNpc::getSicGambitPack() const
{
  return m_SicGambitPack;
}
void PetNpc::setObeyGambitPack( const Sapphire::World::AI::GambitPackPtr& mObeyGambitPack )
{
  m_ObeyGambitPack = mObeyGambitPack;
}
void PetNpc::setSicGambitPack( const Sapphire::World::AI::GambitPackPtr& mSicGambitPack )
{
  m_SicGambitPack = mSicGambitPack;
}
void PetNpc::setSteadyGambitPack( const Sapphire::World::AI::GambitPackPtr& mSteadyGambitPack )
{
  m_SteadyGambitPack = mSteadyGambitPack;
}
