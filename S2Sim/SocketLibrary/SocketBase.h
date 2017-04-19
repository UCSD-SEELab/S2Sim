/**
 * @file SocketBase.h
 * Defines the SocketBase template class.
 *  @date Jun 21, 2013
 *  @author: Alper Sinan Akyurek
 */

#ifndef SOCKETBASE_H_
#define SOCKETBASE_H_

#include "LogPrint.h"

#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#elif defined( __linux__ ) || defined ( __APPLE__ )
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>
#endif

/**
 * This class is an abstraction over the OS socket and defines a base class for socket opening and closing utilities.
 
    @tparam SocketType Type of the socket for UDP, TCP or other types.
 */
template <int SocketType>
class SocketBase
{
    private:
#if defined(_WIN32) || defined(_WIN64)
        class WindowsStarter
        {
            public:
                WindowsStarter( void )
                {
                    WSAData wsaData;
                    WSAStartup( MAKEWORD( 2, 0 ), &wsaData );
                }
                ~WindowsStarter( void )
                {
                    WSACleanup();
                }
        };
#endif
    
    public:
    /**
     *  Defines the type for number of bytes.
     */
        typedef size_t TNumberOfBytes;
    
    /**
     *  Defines the type for a buffer pointer.
     */
        typedef void* TBuffer;

    protected:
    /**
     *  Defines the type of a Socket handle.
     */
        typedef int SOCKET;
    
    /**
     *  Redefines the socket handle type for multiplatform purposes.
     */
        typedef SOCKET TSocketId;

    /**
     *  Special values for the TSocketId.
     */
        enum SocketIdValues
        {
            InvalidSocketId = ( TSocketId )( -1 ) /**< Defines an invalid socket handle */
        };

    protected:
    /**
     *  Constructor that receives a handle as input. It does not open a new socket.
     *
     *  @param socketId Handle of a socket.
     */
        SocketBase( const TSocketId socketId ) : m_socketId( socketId ){}
    
    /**
     *  Copies the handle of another socket.
     *
     *  @param copy Socket to be copied.
     */
        SocketBase( const SocketBase<SocketType> & copy ) = delete;
    
    /**
     *  Moves the handle of another socket.
     *
     *  @param movedObject Socket to be moved.
     */
        SocketBase( SocketBase<SocketType> && movedObject ) : m_socketId( movedObject.m_socketId )
        {
            movedObject.m_socketId = InvalidSocketId;
        }
    
        SocketBase<SocketType> &
        operator = ( const SocketBase<SocketType> & rhs ) = delete;
    
        SocketBase<SocketType> &
        operator = ( SocketBase<SocketType> && rhs ) = delete;
    
    /**
     *  Sets the handle to the invalid value.
     */
        SocketBase( void );
    
    /**
     *  Closes the socket handle.
     */
        ~SocketBase( void );

    protected:
    /**
     *  Handle to the socket used for OS utilities.
     */
        TSocketId m_socketId;

    private:
    /**
     *  Opens a new socket and sets its reusable options.
     */
        void
        OpenSocket( void );

    /**
     *  Closes the socket handle.
     */
        void
        CloseSocket( void );
    
    public:
        void
        ForceCloseSocket( void )
        {
            this->CloseSocket();
        }
};

template <int SocketType>
SocketBase<SocketType>::SocketBase( void ) : m_socketId( InvalidSocketId )
{
#if defined(_WIN32) || defined(_WIN64)
    static WindowsStarter windowsStarterInstance;
#endif
    this->OpenSocket();
}

template <int SocketType>
SocketBase<SocketType>::~SocketBase( void )
{
    this->CloseSocket();
}

template <int SocketType>
void
SocketBase<SocketType>::OpenSocket( void )
{
    LOG_FUNCTION_START();
    this->m_socketId = socket( AF_INET, SocketType, 0 );
    if ( this->m_socketId <= 0 )
    {
        ErrorPrint( "Invalid Socket" );
    }

#if defined( __linux__ ) || defined ( __APPLE__ )
    int value = 1;
    if ( 0 != setsockopt( this->m_socketId, SOL_SOCKET, SO_REUSEADDR, &value, sizeof( value ) ) )
    {
        ErrorPrint( "Socket Option Not Set! Error: ", errno );
    }
#endif
#if defined ( __APPLE__ )
    if ( 0 != setsockopt( this->m_socketId, SOL_SOCKET, SO_REUSEPORT, &value, sizeof( value ) ) )
    {
        ErrorPrint( "Socket Option Not Set! Error: ", errno );
    }
#endif
    LOG_FUNCTION_END();
}

template <int SocketType>
void
SocketBase<SocketType>::CloseSocket( void )
{
    LOG_FUNCTION_START();
    if ( this->m_socketId != InvalidSocketId )
    {
#if defined(_WIN32) || defined(_WIN64)
        closesocket( this->m_socketId );
#elif defined( __linux__ ) || defined ( __APPLE__ )
        LogPrint( "Socket Closed" );
        close( this->m_socketId );
#endif
    }
    LOG_FUNCTION_END();
}

#endif /* SOCKETBASE_H_ */
