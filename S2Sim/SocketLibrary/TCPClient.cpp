/**
 * @file TCPClient.cpp
 * Implements the TCPClient class.
 *  @date Jan 21, 2014
 *  @author: Alper Sinan Akyurek
 */

#include "TCPClient.h"

TCPClient::TCPClient( void )
{
}

TCPClient::~TCPClient( void )
{
}

TCPClient::TNumberOfBytes
TCPClient::SendData( const TBuffer buffer, const TNumberOfBytes length )
{
    return ( send( this->m_socketId, buffer, length, 0 ) );
}

TCPClient::TNumberOfBytes
TCPClient::ReceiveData( TBuffer buffer, const TNumberOfBytes receptionLength )
{
    return ( recv( this->m_socketId, buffer, receptionLength, 0 ) );
}

bool
TCPClient::Connect( IPAddress & address )
{
    if ( connect( this->m_socketId, ( struct sockaddr* )address, IPAddress::GetSocketSize() ) < 0 )
    {
        return ( false );
    }
    return ( true );
}

/*TCPClient::TCPClient( const TCPClient & copy ) : SocketBase<SOCK_STREAM>( copy )
{
}*/

TCPClient::TCPClient( TCPClient && move ) : SocketBase<SOCK_STREAM>( std::move( move ) )
{
}

/*TCPClient &
TCPClient::operator = ( const TCPClient & rhs )
{
    if ( this == &rhs )
    {
        return ( *this );
    }
    *( ( SocketBase<SOCK_STREAM>* )this ) = ( SocketBase<SOCK_STREAM> )rhs;
}

TCPClient &
TCPClient::operator = ( TCPClient && rhs )
{
    if ( this == &rhs )
    {
        return ( *this );
    }
    *( ( SocketBase<SOCK_STREAM>* )this ) = std::move( ( SocketBase<SOCK_STREAM> )rhs );
}*/
