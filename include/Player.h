/**
 * @file Player.h
 * @date 2-Oct-2021
 */

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Object.h"
#include "Arena.h"

/** Player Class
 */
class Player : public Object
{
public:

    const       Arena          *poArena             = {};

                orxU32          u32Score = 0;
                orxU32          u32Picks = 0;


protected:

                void            OnCreate();
                void            OnDelete();
                void            Update(const orxCLOCK_INFO &_Info);


private:
};

#endif // __PLAYER_H__
