#pragma once

#include <Common.h>

#include "Forwards.h"
#include "ForwardsZone.h"
#include "Chara.h"
#include "Npc.h"
#include <set>
#include <map>
#include <queue>

#include "BNpc.h"

namespace Sapphire::Entity
{

  class PetNpc : public BNpc
  {
  public:
    PetNpc();

    PetNpc( PlayerPtr owner, uint32_t id, std::shared_ptr< Common::BNPCInstanceObject > pInfo, const Territory& zone );
    PetNpc( PlayerPtr owner, uint32_t id, std::shared_ptr< Common::BNPCInstanceObject > pInfo, const Territory& zone, uint32_t hp, Common::BNpcType type );

    PlayerPtr getOwner();
    void setOwner(PlayerPtr owner);

    void update( uint64_t tickCount) override;
    void init() ;


    virtual ~PetNpc() override;

  public:
    const PlayerPtr& getPlayerOwner() const;

    float m_followRange;

  private:
    Entity::PlayerPtr m_playerOwner;

    bool m_movingToPlayer;


  };

}
