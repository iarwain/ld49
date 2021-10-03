/**
 * @file Arena.h
 * @date 2-Oct-2021
 */

#ifndef __ARENA_H__
#define __ARENA_H__

#include "Player.h"
#include "Bullet.h"


/** Arena Class
 */
class Arena : public ScrollObject
{
public:

                orxU32          RegisterPlayer(Player &_roPlayer, orxS32 _s32X, orxS32 _s32Y);

                Player *        GetPlayer(orxU32 _u32ID = orxU32_UNDEFINED) const;
                void            MovePlayer(orxU32 _u32ID, orxS32 _s32X, orxS32 _s32Y);

                void            ShootBullet(orxU32 _u32ID, orxS32 _s32X, orxS32 _s32Y, const orxVECTOR &_rvDirection);
                orxBOOL         MoveBullet(Bullet &_roBullet);

                orxBOOL         CheckPosition(orxS32 &_rs32X, orxS32 &_rs32Y) const;

                orxVECTOR       vGridSize;
                orxVECTOR       vTileSize;
                orxU32          u32TickSize;
                orxBOOL         bIsGameOver;


protected:

                void            OnCreate();
                void            OnDelete();
                void            Update(const orxCLOCK_INFO &_rstInfo);


private:

    struct Cell
    {
        ScrollObject *          poTile;
        orxU32                  u32Count;
    };


                orxVECTOR       vOrigin;
                Cell *          poGrid;
                orxFLOAT        fTickTime;
                orxU32          u32TickCount;
                orxBOOL         bIsAttract;
};

#endif // __ARENA_H__
