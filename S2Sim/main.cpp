/**
 * @file main.cpp
 * Main file that starts an infinite running loop on the SystemManager::AdvanceTimeStep method.
 *  @date Sep 19, 2013
 *  @author: Alper Sinan Akyurek
 */

/**
 *  @mainpage S2Sim
 * S2Sim, SmartGrid Swarm Simulator is a Smart Grid simulation tool that allows clients to cosimulate their local controls and test their outputs within the big picture of the grid. S2Sim uses OpenDSS to solve the power flow equations. More on the usage on S2Sim is written in its Interoperability Document.
 */


#include "MatlabManager.h"
#include "ConnectionManager.h"
#include "ControlManager.h"
#include "SystemManager.h"
#include "LogPrint.h"

int
main( int argc, char **argv )
{    
    LOG_FUNCTION_START();
    LogPrint( "S2Sim Started in ", argv[0] );
    
    GetConnectionManager()->Initialize();
    GetMatlabManager()->Initialize();
    GetControlManager()->Initialize();
    GetSystemManager();
    
    auto iterationNumber = 0;
    
    GetControlManager()->WaitUntilReady();
    while ( 1 )
    {
        try
        {
            GetSystemManager().AdvanceTimeStep();
        }
        catch ( ... )
        {
            ErrorPrint( "System Error, Exiting" );
            break;
        }
        
        ++iterationNumber;
        LogPrint( "Time: ", iterationNumber );
    }
    LOG_FUNCTION_END();
    return ( EXIT_SUCCESS );
}



