/**
 * @file ThreadedTCPClient.cpp
 * Implements the ThreadedTCPClient class.
 *  @date Jan 22, 2014
 *  @author: Alper Sinan Akyurek
 */

#include "ThreadedTCPClient.h"

ThreadedTCPClient::ThreadedTCPClient( void ) : m_started( true )
{
    this->m_allowingMutex.lock();
    this->m_thread = std::thread( &ThreadedTCPClient::ExecutionBody, this );
    this->m_thread.detach();
}

ThreadedTCPClient::ThreadedTCPClient( TCPClient && client ) : TCPClient( std::move( client ) ), m_started( true )
{
    this->m_allowingMutex.lock();
    this->m_thread = std::thread( &ThreadedTCPClient::ExecutionBody, this );
    this->m_thread.detach();
}

ThreadedTCPClient::~ThreadedTCPClient( void ) throw()
{
    this->m_started = false;
    this->m_allowingMutex.lock();
}

void
ThreadedTCPClient::ExecutionBody( void )
{
    while ( this->m_started )
    {
        const TNumberOfBytes bufferSize = 10000;
        char buffer[bufferSize];
        memset( buffer, 0x00, bufferSize );
        TNumberOfBytes receivedBytes = this->ReceiveData( buffer, bufferSize );
        this->m_allowingMutex.lock();
        this->m_notification( std::ref( buffer ), receivedBytes );
        this->m_allowingMutex.unlock();
    }
}

void
ThreadedTCPClient::SetNotificationCallback( TNotification & notification )
{
    if ( !this->m_notification.IsNull() )
    {
        this->m_allowingMutex.lock();
    }
    this->m_notification = notification;
    if ( !this->m_notification.IsNull() )
    {
        this->m_allowingMutex.unlock();
    }
}
