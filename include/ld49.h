/**
 * @file ld49.h
 * @date 2-Oct-2021
 */

#ifndef __ld49_H__
#define __ld49_H__

#define __NO_SCROLLED__
#include "Scroll.h"

/** Game Class
 */
class ld49 : public Scroll<ld49>
{
public:


private:

                orxSTATUS       Bootstrap() const;

                void            Update(const orxCLOCK_INFO &_rstInfo);

                orxSTATUS       Init();
                orxSTATUS       Run();
                void            Exit();
                void            BindObjects();


private:
};

#endif // __ld49_H__
