/**
 * @file Bullet.h
 * @date 2-Oct-2021
 */

#ifndef __BULLET_H__
#define __BULLET_H__

#include "Object.h"

/** Bullet Class
 */
class Bullet : public Object
{
public:

                void            Die();

                void            SetDirection(const orxVECTOR &_rvDirection);

                orxVECTOR       vDirection;
                orxU32          u32ID;
                orxS32          s32X, s32Y;
                orxBOOL         bActive;


protected:

                void            OnCreate();
                void            OnDelete();
                void            Update(const orxCLOCK_INFO &_rstInfo);


private:
};

#endif // __BULLET_H__
