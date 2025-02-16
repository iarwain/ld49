/**
 * @file ld49.cpp
 * @date 2-Oct-2021
 */

#define __SCROLL_IMPL__
#include "ld49.h"
#undef __SCROLL_IMPL__
#include "orxExtensions.h"

#include "Arena.h"
#include "Bullet.h"
#include "Object.h"
#include "Player.h"

#ifdef __orxMSVC__

/* Requesting high performance dedicated GPU on hybrid laptops */
__declspec(dllexport) unsigned long NvOptimusEnablement        = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

#endif // __orxMSVC__

static orxBOOL sbRestart = orxTRUE;

/** Update function, it has been registered to be called every tick of the core clock
 */
void ld49::Update(const orxCLOCK_INFO &_rstInfo)
{
    // Should quit?
    if(orxInput_IsActive("Quit"))
    {
        // Should restart?
        sbRestart = orxInput_HasBeenActivated("Reset");

        // Send close event
        orxEvent_SendShort(orxEVENT_TYPE_SYSTEM, orxSYSTEM_EVENT_CLOSE);
    }
    // Screenshot?
    else if(orxInput_HasBeenActivated("Screenshot"))
    {
        orxScreenshot_Capture();
    }
    // Reset?
    else if(orxInput_HasBeenActivated("Reset"))
    {
        for(ScrollObject *poObject = GetNextObject();
            poObject;
            poObject = GetNextObject())
        {
            DeleteObject(poObject);
        }
        CreateObject("Select");
    }
}

/** Init function, it is called when all orx's modules have been initialized
 */
orxSTATUS ld49::Init()
{
    // Init extensions
    InitExtensions();
    
    // Push game section
    orxConfig_PushSection("Game");

    // Create all viewports
    for(orxS32 i = 0, iCount = orxConfig_GetListCount("ViewportList"); i < iCount; i++)
    {
        orxViewport_CreateFromConfig(orxConfig_GetListString("ViewportList", i));
    }

    // Disable main viewport
    orxViewport_Enable(orxViewport_Get("MainViewport"), orxFALSE);

    // Go to title
    CreateObject("Title");

    // Done!
    return orxSTATUS_SUCCESS;
}

/** Run function, it should not contain any game logic
 */
orxSTATUS ld49::Run()
{
    // Return orxSTATUS_FAILURE to instruct orx to quit
    return orxSTATUS_SUCCESS;
}

/** Exit function, it is called before exiting from orx
 */
void ld49::Exit()
{
    // Let Orx clean all our mess automatically. :)

    // Exit from extensions
    ExitExtensions();
}

/** BindObjects function, ScrollObject-derived classes are bound to config sections here
 */
void ld49::BindObjects()
{
    // Bind the Object class to the Object config section
    ScrollBindObject<Arena>("Arena");
    ScrollBindObject<Bullet>("Bullet");
    ScrollBindObject<Object>("Object");
    ScrollBindObject<Player>("Player");
}

/** Bootstrap function, it is called before config is initialized, allowing for early resource storage definitions
 */
orxSTATUS ld49::Bootstrap() const
{
    // Bootstrap extensions
    BootstrapExtensions();
    
    // Return orxSTATUS_FAILURE to prevent orx from loading the default config file
    return orxSTATUS_SUCCESS;
}

/** Main function
 */
int main(int argc, char **argv)
{
    // Should restart?
    while(sbRestart)
    {
        // Clear restart
        sbRestart = orxFALSE;

        // Execute our game
        ld49::GetInstance().Execute(argc, argv);
    }

    // Done!
    return EXIT_SUCCESS;
}
