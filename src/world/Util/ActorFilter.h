#ifndef _WORLD_UTIL_H
#define _WORLD_UTIL_H

#include <stdint.h>
#include <string>
#include <functional>
#include <ForwardsZone.h>
#include <Util/Util.h>

namespace Sapphire::World::Util
{
  class ActorFilter
  {
  public:
    ActorFilter() = default;
    virtual ~ActorFilter() = default;
    virtual bool conditionApplies( Entity::GameObject& actor ) = 0;
  };

  using ActorFilterPtr = std::shared_ptr< ActorFilter >;

  /////////////////////////////////////////////////////////////////////////////

  class ActorFilterInConeRange : public ActorFilter
  {
    Common::FFXIVARR_POSITION3 m_startPos;
    float m_range;
    float m_rotation;
    float m_coneEdgeAngle;
  public:
    ActorFilterInConeRange( Common::FFXIVARR_POSITION3 startPos, float range, float rotation, float coneEdgeAngle);
    bool conditionApplies( Entity::GameObject& actor ) override;
  };

  /////////////////////////////////////////////////////////////////////////////
 
  class ActorFilterInRange : public ActorFilter
  {
    Common::FFXIVARR_POSITION3 m_startPos;
    float m_range;
  public:
    ActorFilterInRange( Common::FFXIVARR_POSITION3 startPos, float range );
    bool conditionApplies( Entity::GameObject& actor ) override;
  };

  /////////////////////////////////////////////////////////////////////////////

  class ActorFilterInRectangleRange : public ActorFilter
  {
    Common::FFXIVARR_POSITION3 m_startPos;
    float m_range;
    float m_width;
    float m_rotation;
  public:
    ActorFilterInRectangleRange( Common::FFXIVARR_POSITION3 startPos, float range, float width, float rotation );
    bool conditionApplies( Entity::GameObject& actor ) override;
  };

  /////////////////////////////////////////////////////////////////////////////

  class ActorFilterSingleTarget : public ActorFilter
  {
    uint32_t m_actorId;
  public:
    explicit ActorFilterSingleTarget( uint32_t actorId );
    bool conditionApplies( Entity::GameObject& actor ) override;
  };
}

#endif
