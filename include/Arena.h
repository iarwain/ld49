/**
 * @file Arena.h
 * @date 2-Oct-2021
 */

#ifndef __ARENA_H__
#define __ARENA_H__

#include "Object.h"


/** Arena Class
 */
class Arena : public Object
{
public:


protected:

                void            OnCreate();
                void            OnDelete();
                void            Update(const orxCLOCK_INFO &_rstInfo);


private:
};

#endif // __ARENA_H__
