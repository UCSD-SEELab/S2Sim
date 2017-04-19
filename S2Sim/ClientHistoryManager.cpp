//
//  ClientHistoryManager.cpp
//  S2Sim
//
//  Created by Alper Sinan Akyurek on 10/14/14.
//  Copyright (c) 2014 Alper Sinan Akyurek. All rights reserved.
//

#include "ClientHistoryManager.h"

ClientHistoryManager&
ClientHistoryManager::GetClientHistoryManager( void )
{
    static ClientHistoryManager clientHistoryManager;
    return ( clientHistoryManager );
}

ClientHistoryManager::ClientHistoryManager( void )
{
    this->m_fileMutex.lock();
    this->m_fileStream.open( "clientHistory.txt", std::ofstream::out | std::ofstream::app );
    this->m_fileMutex.unlock();
}

ClientHistoryManager::~ClientHistoryManager( void )
{
    this->m_fileMutex.lock();
    this->m_fileStream.close();
    this->m_fileMutex.unlock();
}

void
ClientHistoryManager::AddSyncConnect( const std::string & name )
{
    this->m_fileMutex.lock();
    this->m_fileStream << "+S," << name << "," << time( nullptr ) << std::endl;
    this->m_fileMutex.unlock();
}

void
ClientHistoryManager::AddAsyncConnect( const std::string & name )
{
    this->m_fileMutex.lock();
    this->m_fileStream << "+A," << name << "," << time( nullptr ) << std::endl;
    this->m_fileMutex.unlock();
}

void
ClientHistoryManager::AddDisconnect( const std::string & name )
{
    this->m_fileMutex.lock();
    this->m_fileStream << "-," << name << "," << time( nullptr ) << std::endl;
    this->m_fileMutex.unlock();
}