//
//  ClientHistoryManager.h
//  S2Sim
//
//  Created by Alper Sinan Akyurek on 10/14/14.
//  Copyright (c) 2014 Alper Sinan Akyurek. All rights reserved.
//

#ifndef __S2Sim__ClientHistoryManager__
#define __S2Sim__ClientHistoryManager__

#include <fstream>
#include <mutex>
#include <time.h>

class ClientHistoryManager
{
    private:
        std::mutex m_fileMutex;
        std::ofstream m_fileStream;
    
    private:
        ClientHistoryManager( void );
    
    public:
        ~ClientHistoryManager( void );
    
        void
        AddSyncConnect( const std::string & name );
    
        void
        AddAsyncConnect( const std::string & name );
    
        void
        AddDisconnect( const std::string & name );
    
        static ClientHistoryManager&
        GetClientHistoryManager( void );
};

#endif /* defined(__S2Sim__ClientHistoryManager__) */
