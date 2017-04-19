/**
 * @file LogPrint.cpp
 * Defines logging related parameters.
 *  @date Oct 28, 2013
 *  @author: Alper Sinan Akyurek
 */

#include "LogPrint.h"

#include <fstream>

void LogManager::IncreaseCallLevel( void )
{
    if ( this->m_threadMap.find( std::this_thread::get_id() ) == this->m_threadMap.end() )
    {
        this->m_threadMap[std::this_thread::get_id()] = 1;
        this->m_threadAliveMap[std::this_thread::get_id()] = true;
    }
    else
    {
        ++( this->m_threadMap[std::this_thread::get_id()] );
    }
}

void LogManager::DecreaseCallLevel( void )
{
    --( this->m_threadMap[std::this_thread::get_id()] );
}

LogManager::TCallLevel LogManager::GetCallLevel( void )
{
    if ( this->m_threadMap.find( std::this_thread::get_id() ) == this->m_threadMap.end() )
    {
        this->m_threadMap[std::this_thread::get_id()] = 0;
        this->m_threadAliveMap[std::this_thread::get_id()] = true;
    }
    LogManager::TCallLevel returnLevel = this->m_threadMap[std::this_thread::get_id()];
    
    if ( this->m_threadAliveMap[std::this_thread::get_id()] == false )
    {
        if ( returnLevel == 0 )
        {
            this->m_threadMap.erase( std::this_thread::get_id() );
            this->m_threadAliveMap.erase( std::this_thread::get_id() );
        }
    }
    
    return ( returnLevel );
}

void LogManager::ThreadIsDying( void )
{
    LogPrint( "Thread has said its last words..." );
}

std::string LogManager::GetIndentString( void )
{
    std::string indentString( this->m_dotLevel * this->GetCallLevel(), '.' );
    std::cout << std::this_thread::get_id();
    return ( indentString );
}

void LogManagerThreadCallback( void )
{
    while ( 1 )
    {
        LogManager::GetLogManager().CheckConfigurationFile();
        std::this_thread::sleep_for( std::chrono::seconds( 5 ) );
    }
}

LogManager::LogManager( void ) : m_logLevel( LogLevel::Warnings ),
    m_logEntered( false ),
    m_warningEntered( false ),
    m_errorEntered( false ),
    m_functionPrint( true ),
    m_dotLevel( 4 )
{
    std::thread logPropertyCheckThread( &LogManagerThreadCallback );
    logPropertyCheckThread.detach();
}


void LogManager::CheckConfigurationFile( void )
{
    const std::size_t lineBufferSize = 1000;
    char lineBuffer[lineBufferSize];
    std::ifstream configurationFile;
    configurationFile.open( "logConfig.conf", std::ios::in );
    while ( configurationFile )
    {
        configurationFile.getline( lineBuffer, lineBufferSize );
        
        std::string lineString = lineBuffer;
        std::string::size_type equalPosition = lineString.find( "=" );
        std::string keyword = lineString.substr( 1, equalPosition - 1 );
        std::string value = lineString.substr( equalPosition + 1, lineString.length() );
        
        if ( keyword == "LogLevel" )
        {
            LogLevel newLogLevel = ( LogLevel )std::stoi( value );
            if ( newLogLevel != this->m_logLevel )
            {
                this->m_logMutex.lock();
                this->m_logLevel = newLogLevel;
                this->m_logMutex.unlock();
            }
        }
        else if ( keyword == "PrintFunction" )
        {
            bool newFunctionPrintValue = std::stoi( value );
            if ( this->m_functionPrint != newFunctionPrintValue )
            {
                this->m_logMutex.lock();
                this->m_functionPrint = newFunctionPrintValue;
                this->m_logMutex.unlock();
            }
        }
        else if ( keyword == "DotLevel" )
        {
            TDotLevel newDotLevel = std::stoul( value );
            if ( newDotLevel != this->m_dotLevel )
            {
                this->m_logMutex.lock();
                this->m_dotLevel = newDotLevel;
                this->m_logMutex.unlock();
            }
        }
    }
    configurationFile.close();
}
