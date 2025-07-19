#pragma once

#include <Common.h>
#include <memory>

#include "ForwardsZone.h"
#include <set>
#include <map>
#include <queue>

namespace Sapphire::Entity
{

  /*!
  \class GameObject
  \brief Base class for all actor/objects

  */
  class GameObject : public std::enable_shared_from_this< GameObject >
  {

  protected:
    /*! Position of the object */
    Common::FFXIVARR_POSITION3 m_pos{};
    /*! Rotation of the object */
    float m_rot{};
    /*! Id of the actor */
    uint32_t m_id{};
    /*! Type of the actor */
    Common::ObjKind m_objKind;
    /*! Id of the territory type the actor currently is in */
    uint32_t m_territoryTypeId{};
    /*! Specific GUId of the zone the actor currently is in */
    uint32_t m_territoryId{};

    /*! list of various actors in range */
    std::set< GameObjectPtr > m_inRangeActor;
    std::set< PlayerPtr > m_inRangePlayers;
    std::set< BNpcPtr > m_inRangeBNpc;

    /*! Parent cell in the zone */
    Common::CellId m_cellId;

  public:
    explicit GameObject( Common::ObjKind type );

    virtual ~GameObject() {};

    virtual void spawn( PlayerPtr pTarget ) {}

    virtual void despawn( PlayerPtr pTarget ) {}

    uint32_t getId() const;

    void setId( uint32_t id );

    Common::ObjKind getObjKind() const;

    Common::FFXIVARR_POSITION3& getPos();
    const Common::FFXIVARR_POSITION3& getPos() const;

    void setPos( const Common::FFXIVARR_POSITION3& pos, bool broadcastUpdate = true );

    void setPos( float x, float y, float z, bool broadcastUpdate = true );

    float getRot() const;

    void setRot( float rot );

    bool isChara() const;

    bool isPlayer() const;

    bool isEventNpc() const;

    bool isBattleNpc() const;

    bool isRetainer() const;

    bool isCompanion() const;

    bool isEventObj() const;

    bool isHousingEventObj() const;

    bool isAetheryte() const;

    ///// IN RANGE LOGIC ///////////////////////////////
    virtual void onRemoveInRangeActor( GameObject& pActor ) {}

    // check if another actor is in the actors in range set
    bool isInRangeSet( GameObjectPtr pActor ) const;

    CharaPtr getClosestChara();

    // add an actor to in range set
    void addInRangeActor( GameObjectPtr pActor );

    // remove an actor from the in range set
    void removeInRangeActor( GameObject& actor );

    // return true if there is at least one actor in the in range set
    bool hasInRangeActor() const;

    void removeFromInRange();

    // clear the whole in range set, this does no cleanup
    virtual void clearInRangeSet();

    std::set< GameObjectPtr > getInRangeActors( bool includeSelf = false );

    std::set< uint64_t > getInRangePlayerIds( bool includeSelf = false );

    ////////////////////////////////////////////////////

    CharaPtr getAsChara();

    PlayerPtr getAsPlayer();

    EventObjectPtr getAsEventObj();

    BNpcPtr getAsBNpc();

    PetNpcPtr getAsPetNpc();

    uint32_t getTerritoryTypeId() const;
    void setTerritoryTypeId( uint32_t territoryTypeId );

    uint32_t getTerritoryId() const;
    void setTerritoryId( uint32_t territoryTypeId );

    // get the current cell of a region the actor is in
    Common::CellId getCellId() const;
    // set the current cell
    void setCellId( Common::CellId cellId );

  };

}