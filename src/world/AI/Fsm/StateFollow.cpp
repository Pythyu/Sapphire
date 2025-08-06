#include "StateFollow.h"
#include "Actor/BNpc.h"
#include "Logging/Logger.h"
#include <Service.h>
#include <Manager/TerritoryMgr.h>

#include "Actor/PetNpc.h"
#include "Actor/Player.h"

#include <Territory/Territory.h>
#include <Navi/NaviProvider.h>

using namespace Sapphire::World;

void AI::Fsm::StateFollow::onUpdate( Sapphire::Entity::BNpc& bnpc, uint64_t tickCount )
{
  auto& teriMgr = Common::Service< World::Manager::TerritoryMgr >::ref();
  auto pZone = teriMgr.getTerritoryByGuId( bnpc.getTerritoryId() );
  auto pNaviProvider = pZone->getNaviProvider();
  auto petNpc = bnpc.getAsPetNpc();
  auto targetPlayer = petNpc->getPlayerOwner();


  if(!bnpc.hasFlag( Entity::Immobile ) && Common::Util::distance( bnpc.getPos(), targetPlayer->getPos() ) + targetPlayer->getRadius() > petNpc->m_followRange )
  {
    if( pNaviProvider )
      pNaviProvider->setMoveTarget( bnpc, targetPlayer->getPos() );
    bnpc.moveTo( targetPlayer->getPos() );
  }

}

void AI::Fsm::StateFollow::onEnter( Sapphire::Entity::BNpc& bnpc )
{
}

void AI::Fsm::StateFollow::onExit( Sapphire::Entity::BNpc& bnpc )
{
}

std::string AI::Fsm::StateFollow::getName()
{
  return "StateFollow";
}

