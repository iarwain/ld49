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
                orxBOOL         IsUnstable() const  {return fEnergy < orxFLOAT_1 ? orxTRUE : orxFALSE;}

                orxU32          u32ID;
                orxS32          s32X, s32Y;


protected:

                void            OnCreate();
                void            OnDelete();
                void            Update(const orxCLOCK_INFO &_Info);


private:

                orxU64          u64ArenaID;
                orxFLOAT        fEnergy, fMaxEnergy, fEnergyRate;
                orxBOOL         bIsAttract;
};

#endif // __PLAYER_H__
