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

                void            Die();
                void            IncreaseEnergy();

                orxU32          u32ID;
                orxS32          s32X, s32Y;


protected:

                void            OnCreate();
                void            OnDelete();
                void            Update(const orxCLOCK_INFO &_Info);

                orxBOOL         OnShader(orxSHADER_EVENT_PAYLOAD &_rstPayload);


private:

                orxFLOAT        fEnergy, fMaxEnergy, fEnergyRate;
};

#endif // __PLAYER_H__
