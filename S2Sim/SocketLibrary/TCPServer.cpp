/**
 * @file TCPServer.cpp
 * Implements the TCPServer class.
 *  @date Jan 21, 2014
 *  @author: Alper Sinan Akyurek
 */

#include "TCPServer.h"

TCPServer::TCPServer( void )
{
}

TCPServer::~TCPServer( void )
{
}

void
TCPServer::SetAddress( IPAddress& address )
{
    LOG_FUNCTION_START();
    if ( -1 == bind( this->m_socketId, ( struct sockaddr* )address, IPAddress::GetSocketSize() ) )
    {
        ErrorPrint( "Socket Bind failed! Error: ", errno );
    }
    LOG_FUNCTION_END();
}

void
TCPServer::SetPort( const IPAddress::TPort port )
{
    LOG_FUNCTION_START();
    IPAddress address;
    address.SetAddress( INADDR_ANY );
    address.SetPort( port );
    LOG_FUNCTION_END();
    this->SetAddress( address );
}

bool
TCPServer::Listen( void )
{
    LOG_FUNCTION_START();
    if ( listen( this->m_socketId, 1 ) == 0 )
    {
        LOG_FUNCTION_END();
        return ( true );
    }
    ErrorPrint( "Socket Listen failed! Error: ", errno );
    LOG_FUNCTION_END();
    return ( false );
}

SmartPointer<ThreadedTCPConnectedClient>
TCPServer::Accept( void )
{
    LOG_FUNCTION_START();
    IPAddress address;
    IPAddress::TSocketSize length = IPAddress::GetSocketSize();
    TSocketId acceptedSocketId = accept( this->m_socketId, ( struct sockaddr* )address, &length );
    if ( acceptedSocketId <= 0 )
    {
        ErrorPrint( "Socket Accept failed! Error: ", errno );
        //class ERROR_SocketAcceptFailed{};
        //throw ( ERROR_SocketAcceptFailed() );
    }
    LOG_FUNCTION_END();
    return ( CreateObject<ThreadedTCPConnectedClient>( acceptedSocketId, address ) );
}
