/**
 * @file Object.h
 * @date 2-Oct-2021
 */

#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "ld49.h"

/** Object Class
 */
class Object : public ScrollObject
{
public:

    virtual     void            Die()               {bDead = orxTRUE;}

                orxBOOL         bDead;
                orxBOOL         bUnstable;


protected:

                void            OnCreate();
                void            OnDelete();
                void            Update(const orxCLOCK_INFO &_rstInfo);

                orxBOOL         OnShader(orxSHADER_EVENT_PAYLOAD &_rstPayload);


private:
};

#endif // __OBJECT_H__
