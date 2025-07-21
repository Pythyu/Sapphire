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
  m_followRange = .5f;
}

PetNpc::PetNpc( PlayerPtr owner, uint32_t id, std::shared_ptr< Common::BNPCInstanceObject > pInfo, const Territory& zone, uint32_t hp, Common::BNpcType type ) : BNpc(id, pInfo, zone, hp, type)
{
  m_playerOwner = owner;
  m_movingToPlayer = false;
  m_followRange = .5f;
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