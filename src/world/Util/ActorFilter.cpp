#include "ActorFilter.h"
#include "Actor/GameObject.h"
#include "Util/Util.h"
#include "Util/UtilMath.h"
#include "Actor/Chara.h"


Sapphire::World::Util::ActorFilterInConeRange::ActorFilterInConeRange( Common::FFXIVARR_POSITION3 startPos, float range, float rotation, float coneEdgeAngle) :
    m_startPos( startPos ),
    m_range( range),
    m_rotation(rotation),
    m_coneEdgeAngle(coneEdgeAngle)
{
}

bool Sapphire::World::Util::ActorFilterInConeRange::conditionApplies( Entity::GameObject& actor )
{
  if(Sapphire::Common::Util::distance( m_startPos, actor.getPos() ) > m_range)
    return false;

  float minAngle = m_rotation - m_coneEdgeAngle;
  float maxAngle = m_rotation + m_coneEdgeAngle;
  float actualAngle = Sapphire::Common::Util::calcAngFrom(actor.getPos().x, actor.getPos().y,  m_startPos.x, m_startPos.y);
  printf("[ActorFilterInConeRange] minAngle=%.2f maxAngle=%.2f actualAngle=%.2f\n", minAngle, maxAngle, actualAngle);
  return minAngle <= actualAngle && maxAngle <= actualAngle;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

Sapphire::World::Util::ActorFilterInRange::ActorFilterInRange( Common::FFXIVARR_POSITION3 startPos,
                                                               float range ) :
  m_startPos( startPos ),
  m_range( range )
{
}

bool Sapphire::World::Util::ActorFilterInRange::conditionApplies( Entity::GameObject& actor )
{
  return Sapphire::Common::Util::distance( m_startPos, actor.getPos() ) <= m_range;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

Sapphire::World::Util::ActorFilterSingleTarget::ActorFilterSingleTarget( uint32_t actorId ) :
  m_actorId( actorId )
{
}

bool Sapphire::World::Util::ActorFilterSingleTarget::conditionApplies( Sapphire::Entity::GameObject& actor )
{
  return actor.getId() == m_actorId;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

Sapphire::World::Util::ActorFilterInRectangleRange::ActorFilterInRectangleRange( Common::FFXIVARR_POSITION3 startPos,
                                                               float range, float width, float rotation ) :
                                                                               m_startPos( startPos ),
                                                                               m_range( range ),
                                                                               m_width( width ),
                                                                               m_rotation(rotation)
{
}

bool Sapphire::World::Util::ActorFilterInRectangleRange::conditionApplies( Entity::GameObject& actor )
{
  auto actorPos = actor.getPos();
  auto actorChara = actor.getAsChara();
  if(actorChara == nullptr)
    return false;

  Common::FFXIVARR_POSITION3 firstCorner {m_startPos.x + cos(m_rotation + PI/2.f)*(m_width/2.f),m_startPos.y, m_startPos.z + sin(m_rotation + PI/2.f)*(m_width/2.f)};
  Common::FFXIVARR_POSITION3 secondCorner {m_startPos.x + cos(m_rotation - PI/2.f)*(m_width/2.f),m_startPos.y, m_startPos.z + sin(m_rotation - PI/2.f)*(m_width/2.f)};
  Common::FFXIVARR_POSITION3 secondFarCorner {secondCorner.x + cos(m_rotation) * m_range, secondCorner.y, secondCorner.z + sin(m_rotation) * m_range };
  Common::FFXIVARR_POSITION3 firstFarCorner {firstCorner.x + cos(m_rotation) * m_range, firstCorner.y, firstCorner.z + sin(m_rotation) * m_range };
  Common::FFXIVARR_POSITION3 middleCorner {m_startPos.x + cos(m_rotation)*m_range,m_startPos.y, m_startPos.z + sin(m_rotation)*m_range};

  return Common::Util::checkLineCircleCollision(actorPos.x, actorPos.z, actorChara->getRadius(), firstCorner, firstFarCorner) ||
         Common::Util::checkLineCircleCollision(actorPos.x, actorPos.z, actorChara->getRadius(), actorPos, middleCorner) ||
         Common::Util::checkLineCircleCollision(actorPos.x, actorPos.z, actorChara->getRadius(), secondCorner, secondFarCorner);
}