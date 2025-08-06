#include <cstdint>
#include "ForwardsZone.h"
#include "Actor/BNpc.h"
#include "State.h"

#pragma once

namespace Sapphire::World::AI::Fsm
{

  class CombatAllIn
  {
  public:
    static void onUpdate( Entity::BNpc& bnpc, uint64_t tickCount );
  };

  class CombatKeepRangeIfPossible
  {
  public:
    static void onUpdate( Entity::BNpc& bnpc, uint64_t tickCount );
  };

  class CombatInPlace
  {
  public:
    static void onUpdate( Entity::BNpc& bnpc, uint64_t tickCount );
  };

  class StateCombat : public State
  {
  public:
    virtual ~StateCombat() = default;

    void onUpdate( Entity::BNpc& bnpc, uint64_t tickCount );
    void onEnter( Entity::BNpc& bnpc );
    void onExit( Entity::BNpc& bnpc );

    std::string getName();
  };
}