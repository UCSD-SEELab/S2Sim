/**
 * @file ThreadedTCPClient.h
 * Defines the ThreadedTCPClient class.
 *  @date Jan 22, 2014
 *  @author: Alper Sinan Akyurek
 */

#ifndef THREADEDTCPCLIENT_H_
#define THREADEDTCPCLIENT_H_

#include <mutex>
#include <thread>
#include "FunctionPointer.hpp"

#include "TCPClient.h"

/**
 *  This is a TCP client class that receives data in a separate thread in the background. In order to use the class, a notification callback needs to be set that will be called when a data is received.
 */
class ThreadedTCPClient : public TCPClient
{
    public:
    /**
     *  Defines the callback type. The function should have three inputs:
        - ThreadedTCPClient*: The function will be notified who the sender is.
        - TBuffer: The function will be provided the data received in a buffer on the stack. The function is responsible of copying the data or it will be overwritten.
        - TNumberOfBytes: The function will be provided how many bytes have been actually received.
     */
        typedef SmartPointer<FunctionPointer<TBuffer, TNumberOfBytes>> TNotification;
        // typedef void ( *TNotification )( ThreadedTCPClient *, TBuffer data, const TNumberOfBytes size );

    private:
    /**
     *  The execution body of the thread. The thread waits for a semaphore, which is released only once when the notification callback is set. If the semaphore is taken, the thread will start receiving data from the server.
     *
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

    public:
    /**
     *  Starts the thread and initializes the variables.
     */
        ThreadedTCPClient( void );
    
    /**
     *  Copies only the TCP Client part and starts a new thread.
     */
        ThreadedTCPClient( const TCPClient & client ) = delete;
    
    /**
     *  Moves only the TCP Client part and starts a new thread.
     */
        ThreadedTCPClient( TCPClient && client );
    
    /**
     *  Stops execution.
     */
        ~ThreadedTCPClient( void ) throw();
    
        ThreadedTCPClient( const ThreadedTCPClient & ) = delete;
    
    /**
     *  Moves the instance to the newly created instance.
     */
        ThreadedTCPClient( ThreadedTCPClient && ) = delete;
    
        ThreadedTCPClient&
        operator = ( const ThreadedTCPClient & ) = delete;
    
        ThreadedTCPClient&
        operator = ( ThreadedTCPClient && ) = delete;

    /**
     *  Sets the notification callback function. If the semaphore was already released, it is taken back. If the new callback is legit, it is released again.
     *
     *  @param notification Notification callback function.
     */
        void
        SetNotificationCallback( TNotification & notification );
};

#endif /* THREADEDTCPCLIENT_H_ */
