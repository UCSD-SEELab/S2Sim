//
//  FunctionPointer.hpp
//  S2Sim
//
//  Created by Alper Sinan Akyurek on 10/4/14.
//  Copyright (c) 2014 Alper Sinan Akyurek. All rights reserved.
//

#ifndef S2Sim_FunctionPointer_hpp
#define S2Sim_FunctionPointer_hpp

#include "SmartPointer.hpp"

template <class ...TArgs>
void
Scream( TArgs... args )
{
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

template <class ...TArgs>
class FunctionPointer
{
    public:
        virtual void
        operator () ( TArgs... args )
        {
            ::Scream();
        }
};

template <>
class FunctionPointer<>
{
    public:
        virtual void
        operator () ( void )
        {
            ::Scream();
        }
};

template <class ...TArgs>
class CFunctionPointer : public FunctionPointer<TArgs...>
{
    private:
        typedef void ( *TFunctionPointer )( TArgs... );
        
    private:
        TFunctionPointer m_function;
        
    public:    
        void
        operator () ( TArgs... args )
        {
            ( this->m_function )( args... );
        }
    
        CFunctionPointer( TFunctionPointer function ) : m_function( function )
        {
            LOG_FUNCTION_START();
            LOG_FUNCTION_END();
        }
};

template <>
class CFunctionPointer<> : public FunctionPointer<>
{
    private:
        typedef void ( *TFunctionPointer )( void );
        
    private:
        TFunctionPointer m_function;
        
    public:
        void
        operator () ( void )
        {
            ( this->m_function )();
        }
        
        CFunctionPointer( TFunctionPointer function ) : m_function( function )
        {
            LOG_FUNCTION_START();
            LOG_FUNCTION_END();
        }
};

template <typename T, class ...TArgs>
class CPPFunctionPointer : public FunctionPointer<TArgs...>
{
    private:
        typedef SmartPointer<T> TObjectPointer;
        typedef void ( T::*TFunctionPointer )( TArgs... );
        
    private:
        SmartPointer<T> m_object;
        TFunctionPointer m_function;
        
    public:
        void
        operator () ( TArgs... args )
        {
            LOG_FUNCTION_START();
            ( ( *( this->m_object ) ).*this->m_function )( args... );
            LOG_FUNCTION_END();
        }
    
        CPPFunctionPointer( const TObjectPointer & object, TFunctionPointer function ) : m_object( object ), m_function( function )
        {
            LOG_FUNCTION_START();
            LOG_FUNCTION_END();
        }
    
        CPPFunctionPointer( const CPPFunctionPointer<T, TArgs...> & copy ) : m_object( copy.m_object ), m_function( copy.m_function )
        {
            LOG_FUNCTION_START();
            LOG_FUNCTION_END();
        }
    
        CPPFunctionPointer( CPPFunctionPointer<T, TArgs...> && move ) : m_object( std::move( move.m_object ) ), m_function( move.m_function )
        {
            LOG_FUNCTION_START();
            move.m_function = nullptr;
            LOG_FUNCTION_END();
        }
    
        ~CPPFunctionPointer( void )
        {
            LOG_FUNCTION_START();
            LOG_FUNCTION_END();
        }
};

template <typename T, class ...TArgs>
auto
CreateFunctionPointer( const SmartPointer<T> & object, void ( T::*function )( TArgs... ) ) -> SmartPointer<FunctionPointer<TArgs...>>
{
    LOG_FUNCTION_START();
    typedef CPPFunctionPointer<T, TArgs...> TFunctionPointer;
    typedef SmartPointer<FunctionPointer<TArgs...>> TReturnPointer;
    SmartPointer<TFunctionPointer> functionObject = CreateObject<TFunctionPointer>( object, function ) ;
    TReturnPointer returnObject = functionObject;
    LOG_FUNCTION_END();
    return ( returnObject );
}

template <typename T, class ...TArgs>
auto
CreateFunctionPointer( SmartPointer<T> && object, void ( T::*function )( TArgs... ) ) -> SmartPointer<FunctionPointer<TArgs...>>
{
    LOG_FUNCTION_START();
    typedef CPPFunctionPointer<T, TArgs...> TFunctionPointer;
    typedef SmartPointer<FunctionPointer<TArgs...>> TReturnPointer;
    TReturnPointer returnObject = CreateObject<TFunctionPointer>( TFunctionPointer( std::move( object ), function ) );
    LOG_FUNCTION_END();
    return ( returnObject );
}

template <class ...TArgs>
auto
CreateFunctionPointer( void ( *function )( TArgs... ) ) -> SmartPointer<CFunctionPointer<TArgs...>>
{
    LOG_FUNCTION_START();
    typedef CFunctionPointer<TArgs...> TFunctionPointer;
    typedef SmartPointer<FunctionPointer<TArgs...>> TReturnPointer;
    TReturnPointer returnObject = CreateObject<TFunctionPointer>( TFunctionPointer( function ) );
    LOG_FUNCTION_END();
    return ( returnObject );
}

template <class ...TArgs>
auto
CreateFunctionPointer( void ( *function )( TArgs&&... ) ) -> SmartPointer<CFunctionPointer<TArgs&&...>>
{
    LOG_FUNCTION_START();
    typedef CFunctionPointer<TArgs&&...> TFunctionPointer;
    typedef SmartPointer<FunctionPointer<TArgs&&...>> TReturnPointer;
    TReturnPointer returnObject = CreateObject<TFunctionPointer>( std::move( TFunctionPointer( function ) ) );
    LOG_FUNCTION_END();
    return ( returnObject );
}

#endif
