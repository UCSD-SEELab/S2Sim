/**
 * @file ThreadedTCPConnectedClient.h
 * Defeines the ThreadedTCPConnectedClient class.
 *  @date Jan 22, 2014
 *  @author: Alper Sinan Akyurek
 */

#ifndef THREADEDTCPCONNECTEDCLIENT_H_
#define THREADEDTCPCONNECTEDCLIENT_H_

#include "TCPConnectedClient.h"
#include <mutex>
#include <thread>
#include "LogPrint.h"
#include "FunctionPointer.hpp"

/**
 *  Manages the connection to an accepted client on the server side and receives data in a separate thread in the background.
 */
class ThreadedTCPConnectedClient : public TCPConnectedClient
{
    public:
    /**
     *  Defines the callback type. The function should have three inputs:
     - ThreadedTCPConnectedClient*: The function will be notified who the sender is.
     - TBuffer: The function will be provided the data received in a buffer on the stack. The function is responsible of copying the data or it will be overwritten.
     - TNumberOfBytes: The function will be provided how many bytes have been actually received.
     */
        typedef SmartPointer<FunctionPointer<TBuffer, const TNumberOfBytes>> TNotification;

    private:
    /**
     *  The execution body of the thread. The thread waits for a mutex, which is released only once when the notification callback is set. If the mutex is taken, the thread will start receiving data from the server.
     */
        void ExecutionBody( void );

    private:
        /**
         *  Thread object executed separately.
         */
        std::thread m_thread;
    
        /**
         *  Notification callback to be called.
         */
        TNotification m_notification;
        
        /**
         *  Boolean variable to stop the thread.
         */
        bool m_started;
        
        /**
         *  This mutex is unlocked only if there is a legitimate callback function.
         */
        std::mutex m_allowingMutex;
    
        std::mutex m_deathMutex;
    
        SmartPointer<char> m_myselfBeforeDeath;

    public:
    /**
     *  Initializes the class with a ready socket and address, and starts the reception thread.
        @param socketId Handle to the socket.
        @param clientAddress IPAddress of the connected client.
     */
        ThreadedTCPConnectedClient( const TSocketId socketId, const IPAddress & clientAddress );
    
    /**
     *  Copies the TCP connection information and starts a new thread.
     */
        /*ThreadedTCPConnectedClient( const TCPConnectedClient & client );*/
    
        ThreadedTCPConnectedClient( ThreadedTCPConnectedClient && client ) = delete;
    
    /**
     *  Stops the execution of the thread.
     */
        ~ThreadedTCPConnectedClient( void );

    /**
     *  Sets the notification callback function. If the mutex was already released, it is taken back. If the new callback is legit, it is released again.
     *
     *  @param notification Notification callback function.
     */
        void
        SetNotificationCallback( TNotification && notification );
    
    /**
     *  Stops the thread at the earliest time possible.
     */
        void
        StopThread( const SmartPointer<ThreadedTCPConnectedClient> & myself );
};

#endif /* THREADEDTCPCONNECTEDCLIENT_H_ */
