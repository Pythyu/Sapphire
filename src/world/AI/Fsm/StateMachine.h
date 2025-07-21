#include <cstdint>
#include "ForwardsZone.h"
#include "Actor/BNpc.h"

#pragma once

namespace Sapphire::World::AI::Fsm
{
  class StateMachine
  {
  public:
    StateMachine() = default;
    ~StateMachine() = default;

    StatePtr addState( StatePtr state );
    void setCurrentState( StatePtr state );
    virtual void update( Entity::BNpc& bnpc, uint64_t tickCount );

  protected:
    std::vector< StatePtr > m_states;

  public:
    StatePtr& getState(uint32_t index);

  protected:
    StatePtr m_pCurrentState;

  public:
    const StatePtr& getCurrentState() const;
  };
}