//
//  ClientNumberManager.h
//  S2Sim
//
//  Created by Alper Sinan Akyurek on 10/14/14.
//  Copyright (c) 2014 Alper Sinan Akyurek. All rights reserved.
//

#ifndef __S2Sim__ClientNumberManager__
#define __S2Sim__ClientNumberManager__

#include <mutex>
#include "MessageHeader.h"

class ClientNumberManager
{
    public:
        typedef TerraSwarm::MessageHeader::TId TClientNumber;
    
    private:
        ClientNumberManager( void );
    
    public:
        ~ClientNumberManager( void );
    
        void
        AddSynchronousClient( void );
    
        void
        AddAsynchronousClient( void );
    
        void
        RemoveSynchronousClient( void );
    
        void
        RemoveAsynchronousClient( void );
    
        static ClientNumberManager&
        GetClientNumberManager( void );
    
        TClientNumber
        GetSynchronousClientNumber( void ) const;
    
        TClientNumber
        GetAsynchronousClientNumber ( void ) const;
    
        TClientNumber
        GetClientNumber( void ) const;
    
    private:
        std::mutex m_clientNumberMutex;
    
        TClientNumber m_synchronousClientNumber;
    
        TClientNumber m_asynchronousClientNumber;
};

#endif /* defined(__S2Sim__ClientNumberManager__) */
