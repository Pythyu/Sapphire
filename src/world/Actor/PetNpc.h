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

  enum PetBaseAction {
    Away = 1,
    Heel = 2,
    Place = 3,
    Stay = 4,
    Guard = 5,
    Steady = 6,
    Sic = 7,
    Obey = 8
  };

  class PetNpc : public BNpc
  {
  public:
    PetNpc();

    PetNpc( PlayerPtr owner, uint32_t id, std::shared_ptr< Common::BNPCInstanceObject > pInfo, const Territory& zone );
    PetNpc( PlayerPtr owner, uint32_t id, std::string petName, std::shared_ptr< Common::BNPCInstanceObject > pInfo, const Territory& zone, uint32_t hp, Common::BNpcType type );

    PlayerPtr getOwner();
    void setOwner(PlayerPtr owner);

    void update( uint64_t tickCount) override;
    void init() ;


    virtual ~PetNpc() override;

  public:
    const PlayerPtr& getPlayerOwner() const;
    void petBehavior(PetBaseAction action);

    void setSicGambitPack( const World::AI::GambitPackPtr& mSicGambitPack );
    const World::AI::GambitPackPtr& getSicGambitPack() const;
    void setObeyGambitPack( const World::AI::GambitPackPtr& mObeyGambitPack );
    const World::AI::GambitPackPtr& getObeyGambitPack() const;

    float m_followRange;

  private:
    Entity::PlayerPtr m_playerOwner;

    bool m_movingToPlayer;

    World::AI::GambitPackPtr m_ObeyGambitPack;
    World::AI::GambitPackPtr m_SicGambitPack;
    World::AI::GambitPackPtr m_SteadyGambitPack;

  public:
    void setSteadyGambitPack( const World::AI::GambitPackPtr& mSteadyGambitPack );
  };

}
