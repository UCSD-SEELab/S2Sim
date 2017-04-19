//
//  ClientNumberManager.cpp
//  S2Sim
//
//  Created by Alper Sinan Akyurek on 10/14/14.
//  Copyright (c) 2014 Alper Sinan Akyurek. All rights reserved.
//

#include "ClientNumberManager.h"

ClientNumberManager::ClientNumberManager( void ) : m_synchronousClientNumber( 0 ), m_asynchronousClientNumber( 0 )
{
}

ClientNumberManager::~ClientNumberManager( void )
{
}

void
ClientNumberManager::AddSynchronousClient( void )
{
    std::lock_guard<std::mutex> lockGuard( this->m_clientNumberMutex );
    ++this->m_synchronousClientNumber;
}

void
ClientNumberManager::AddAsynchronousClient( void )
{
    std::lock_guard<std::mutex> lockGuard( this->m_clientNumberMutex );
    ++this->m_asynchronousClientNumber;
}

void
ClientNumberManager::RemoveSynchronousClient( void )
{
    std::lock_guard<std::mutex> lockGuard( this->m_clientNumberMutex );
    --this->m_synchronousClientNumber;
}

void
ClientNumberManager::RemoveAsynchronousClient( void )
{
    std::lock_guard<std::mutex> lockGuard( this->m_clientNumberMutex );
    --this->m_asynchronousClientNumber;
}

ClientNumberManager&
ClientNumberManager::GetClientNumberManager( void )
{
    static ClientNumberManager clientNumberManager;
    return ( clientNumberManager );
}

ClientNumberManager::TClientNumber
ClientNumberManager::GetSynchronousClientNumber( void ) const
{
    return ( this->m_synchronousClientNumber );
}

ClientNumberManager::TClientNumber
ClientNumberManager::GetAsynchronousClientNumber ( void ) const
{
    return ( this->m_asynchronousClientNumber );
}

ClientNumberManager::TClientNumber
ClientNumberManager::GetClientNumber( void ) const
{
    return ( this->m_synchronousClientNumber + this->m_asynchronousClientNumber );
}