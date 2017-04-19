/**
 * @file ConnectionManager.cpp
 * This file implements the ConnectionManager class.
 *  @date Oct 13, 2013
 *  @author: Alper Sinan Akyurek
 */

#include "ConnectionManager.h"

static SmartPointer<ConnectionManager> connectionManager = CreateObject<ConnectionManager>();

SmartPointer<ConnectionManager> &
GetConnectionManager( void )
{
    return ( connectionManager );
}

ConnectionManager::ConnectionManager( void )
{
    LOG_FUNCTION_START();
    this->m_server.SetPort( 26999 );
    LOG_FUNCTION_END();
}

void
ConnectionManager::Initialize( void )
{
    LOG_FUNCTION_START();
    this->m_server.SetNotificationCallback( CreateFunctionPointer( GetConnectionManager(), &ConnectionManager::IncomingConnection ) );
    LOG_FUNCTION_END();
}

void
ConnectionManager::IncomingConnection( const SmartPointer<ThreadedTCPConnectedClient> & newClient )
{
    LOG_FUNCTION_START();
    LogPrint( "Creating a client manager for the new connection" );
    auto newClientManager = CreateObject<ClientManager>( newClient );
    newClientManager->Initialize( newClientManager );
    LOG_FUNCTION_END();
}
