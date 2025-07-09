#include "ActorFilter.h"
#include "Actor/GameObject.h"
#include "Util/Util.h"
#include "Util/UtilMath.h"


Sapphire::World::Util::ActorFilterInConeRange::ActorFilterInConeRange( Common::FFXIVARR_POSITION3 startPos, float range, float rotation, float coneEdgeAngle) :
    m_startPos( startPos ),
    m_range( range),
    m_rotation(rotation),
    m_coneEdgeAngle(coneEdgeAngle)
{
}

bool Sapphire::World::Util::ActorFilterInConeRange::conditionApplies( const Entity::GameObject& actor )
{
  if(Sapphire::Common::Util::distance( m_startPos, actor.getPos() ) > m_range)
    return false;

  float minAngle = m_rotation - m_coneEdgeAngle;
  float maxAngle = m_rotation + m_coneEdgeAngle;
  float actualAngle = Sapphire::Common::Util::calcAngFrom(actor.getPos().x, actor.getPos().y,  m_startPos.x, m_startPos.y);
  return minAngle <= actualAngle && maxAngle <= actualAngle;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

Sapphire::World::Util::ActorFilterInRange::ActorFilterInRange( Common::FFXIVARR_POSITION3 startPos,
                                                               float range ) :
  m_startPos( startPos ),
  m_range( range )
{
}

bool Sapphire::World::Util::ActorFilterInRange::conditionApplies( const Entity::GameObject& actor )
{
  return Sapphire::Common::Util::distance( m_startPos, actor.getPos() ) <= m_range;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

Sapphire::World::Util::ActorFilterSingleTarget::ActorFilterSingleTarget( uint32_t actorId ) :
  m_actorId( actorId )
{
}

bool Sapphire::World::Util::ActorFilterSingleTarget::conditionApplies( const Sapphire::Entity::GameObject& actor )
{
  return actor.getId() == m_actorId;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////