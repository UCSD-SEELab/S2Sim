/**
 * @file ThreadedTCPConnectedClient.cpp
 * Implements the ThreadedTCPConnectedClient class.
 *  @date Jan 22, 2014
 *  @author: Alper Sinan Akyurek
 */

#include "ThreadedTCPConnectedClient.h"

ThreadedTCPConnectedClient::ThreadedTCPConnectedClient( const TSocketId socketId, const IPAddress & clientAddress ) : TCPConnectedClient( socketId, clientAddress ),
                                                                                                                      m_started( true )
{
    LOG_FUNCTION_START();
    this->m_allowingMutex.lock();
    this->m_thread = std::thread( &ThreadedTCPConnectedClient::ExecutionBody, this );
    this->m_thread.detach();
    LOG_FUNCTION_END();
}

ThreadedTCPConnectedClient::~ThreadedTCPConnectedClient( void )
{
    LOG_FUNCTION_START();
    this->m_deathMutex.lock();
    LOG_FUNCTION_END();
}

void
ThreadedTCPConnectedClient::ExecutionBody( void )
{
    LOG_FUNCTION_START();
    while ( this->m_started )
    {
        this->m_allowingMutex.lock();
        const TNumberOfBytes bufferSize = 10000;
        char buffer[bufferSize];
        memset( buffer, 0x00, bufferSize );
        TNumberOfBytes receivedBytes = this->ReceiveData( buffer, bufferSize );
        if ( !this->m_notification.IsNull() )
        {
            ( *this->m_notification )( ( TBuffer )buffer, receivedBytes );
        }
        else
        {
            ErrorPrint( "TCP Connected Client Thread: We should not be here" );
        }
        if ( receivedBytes != 0 )
        {
            this->m_allowingMutex.unlock();
        }
    }
    this->m_deathMutex.unlock();
    this->m_myselfBeforeDeath.NullifyPointer();
    LOG_FUNCTION_END();
}

void
ThreadedTCPConnectedClient::SetNotificationCallback( TNotification && notification )
{
    LOG_FUNCTION_START();
    this->m_deathMutex.lock();
    if ( notification.IsNull() )
    {
        if ( this->m_notification.IsNull() )
        {
            this->m_notification = std::move( notification );
        }
        else
        {
            this->m_allowingMutex.lock();
            this->m_notification = std::move( notification );
        }
    }
    else
    {
        if ( this->m_notification.IsNull() )
        {
            this->m_notification = std::move( notification );
            this->m_allowingMutex.unlock();
        }
        else
        {
            this->m_allowingMutex.lock();
            this->m_notification = std::move( notification );
            this->m_allowingMutex.unlock();
        }
    }
    this->m_deathMutex.unlock();
    LOG_FUNCTION_END();
}

void
ThreadedTCPConnectedClient::StopThread( const SmartPointer<ThreadedTCPConnectedClient> & myself )
{
    LOG_FUNCTION_START();
    this->m_deathMutex.lock();
    this->m_myselfBeforeDeath = myself;
    this->ForceCloseSocket();
    this->m_started = false;
    this->m_notification.NullifyPointer();
    LOG_FUNCTION_END();
}
