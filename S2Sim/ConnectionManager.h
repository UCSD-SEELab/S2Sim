/**
 * @file ConnectionManager.h
 * This file defines the ConnectionManager class.
 *  @date Oct 13, 2013
 *  @author: Alper Sinan Akyurek
 */

#ifndef CONNECTIONMANAGER_H_
#define CONNECTIONMANAGER_H_

#include "ClientManager.h"
#include "ThreadedTCPServer.h"
#include "ThreadedTCPConnectedClient.h"
#include "LogPrint.h"

#include <map>
#include <algorithm>
#include <fstream>
#include <vector>

class ClientManager;

/**
 *  @brief Wrapper callback method for reception notification.
 *
 *  This method is called to notify that a data has been received. This method relays the information directly to ConnectionManager.
 *
 *  @param client Client information of the sender.
 *  @param buffer Buffer containing the message.
 *  @param size   Size of the message.
 */
void
ConnectionReceiveHandler( ThreadedTCPConnectedClient* client, void* buffer, size_t size );

/**
 *  @brief Wrapper callback method for connection notification.
 *
 *  This method is called to notify that a new connection is established. This method relays the notification directly to the ConnectionManager.
 *
 *  @param acceptedClient Information about the newly accepted client.
 */
void
ConnectionNotificationHandler( TCPConnectedClient* acceptedClient );

/**
 *  @brief Manages connections to all clients.
 *
 *  This class manages the connections to all clients. It implements a TCP server and answers any connection attempts. It forwards received data to the correct ClientManager instance for processing.
 */
class ConnectionManager
{
    /**
     *  Friend method for singleton implementation.
     *
     *  @return Returns the reference to the ConnectionManager.
     */
    friend class SmartPointer<ConnectionManager>;

    private:
    /**
     *  Instance of the TCP server.
     */
        ThreadedTCPServer m_server;

    private:
    /**
     *  Private constructor for singleton implementation.
     */
        ConnectionManager( void );

    public:
        void
        Initialize( void );
    
    /**
     *  @brief Incoming connection handler.
     *
     *  This method handles a new connection from a new client. A new ClientManager is created and the information is added to the lists for book-keeping.
     *
     *  @param newClient TCP Information of the new connection.
     */
        void
        IncomingConnection( const SmartPointer<ThreadedTCPConnectedClient> & newClient  );
};

SmartPointer<ConnectionManager> &
GetConnectionManager( void );

#endif /* CONNECTIONMANAGER_H_ */
