/**
 * @file Arena.h
 * @date 2-Oct-2021
 */

#ifndef __ARENA_H__
#define __ARENA_H__

#include "Player.h"


/** Arena Class
 */
class Arena : public Object
{
public:

                orxU32          RegisterPlayer(Player &_roPlayer);

                Player *        GetPlayer(orxU32 _u32ID) const;
                void            MovePlayer(orxU32 _u32ID, orxS32 _s32X, orxS32 _s32Y);

                orxVECTOR       vGridSize;
                orxVECTOR       vTileSize;


protected:

                void            OnCreate();
                void            OnDelete();
                void            Update(const orxCLOCK_INFO &_rstInfo);


private:

    struct Cell
    {
        ScrollObject *          poTile;
        orxU32                  u32ID;
    };


                orxVECTOR       vOrigin;

                Cell *          poGrid;
};

#endif // __ARENA_H__
