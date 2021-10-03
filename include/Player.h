/**
 * @file Player.h
 * @date 2-Oct-2021
 */

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Object.h"

/** Player Class
 */
class Player : public Object
{
public:

                orxU32          u32ID;
                orxS32          s32X, s32Y;
                orxBOOL         bDead;


protected:

                void            OnCreate();
                void            OnDelete();
                void            Update(const orxCLOCK_INFO &_Info);


private:
};

#endif // __PLAYER_H__
