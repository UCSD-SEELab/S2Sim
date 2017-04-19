//
//  SmartPointer.hpp
//  S2Sim
//
//  Created by Alper Sinan Akyurek on 9/27/14.
//  Copyright (c) 2014 Alper Sinan Akyurek. All rights reserved.
//

#ifndef S2Sim_SmartPointer_hpp
#define S2Sim_SmartPointer_hpp

#include "LogPrint.h"

#include <mutex>

template <typename Type>
class SmartPointer
{
    public:
        typedef size_t TArraySize;
    
    private:
        template<typename Type2>
        friend class SmartPointer;
    
        typedef size_t TReferenceNumber;
        typedef Type TObject;
        
        template <typename T, size_t N>
        static constexpr bool IsArray( T(&)[N] )
        {
            return ( true );
        }
        
        template <typename T>
        static constexpr bool IsArray( const T & )
        {
            return ( false );
        }
        
        template <bool SelectFirstCondition, typename First, typename Second>
        struct TypeSelection
        {
            typedef First TResult;
        };
        
        template <typename First, typename Second>
        struct TypeSelection<false, First, Second>
        {
            typedef Second TResult;
        };
        
        enum TypeProperties
        {
            IsTypeArray = IsArray( *( ( Type* )( nullptr ) ) )
        };
        
        typedef typename TypeSelection<IsTypeArray, decltype( ( ( Type* )( nullptr ) )[0] ), Type>::TResult TSubscriptType;
        typedef typename TypeSelection<IsTypeArray, decltype( &( ( ( Type* )( nullptr ) )[0] ) ), Type*>::TResult TPointer;
    
    
        class DeleterObject
        {
            public:
                virtual ~DeleterObject( void ){}
                virtual void DeleteObject( void ) = 0;
        };
    
        class DeleterImplementation : public DeleterObject
        {
            private:
                Type* m_baseObject;
            
            public:
                void DeleteObject( void )
                {
                    delete m_baseObject;
                }
            
                DeleterImplementation( const Type* object ) : m_baseObject( object ){}
                DeleterImplementation( const void* object ) : m_baseObject( ( Type* )object ){}
        };
    
        class DeleterArrayImplementation : public DeleterObject
        {
            private:
                Type* m_baseObject;
                
            public:
                void DeleteObject( void )
                {
                    delete[] m_baseObject;
                }
                
                DeleterArrayImplementation( const Type* object ) : m_baseObject( object ){}
                DeleterArrayImplementation( const void* object ) : m_baseObject( ( Type* )object ){}
        };
    
    private:
        TReferenceNumber* m_referenceNumber;
        void* m_object;
        TArraySize m_arraySize;
        DeleterObject* m_deleterObject;
        bool m_isMoved;
    
    private:
        void
        DeleteMemory( void )
        {
            LOG_FUNCTION_START();
            this->m_deleterObject->DeleteObject();
            delete this->m_referenceNumber;
            delete this->m_deleterObject;
            this->m_referenceNumber = nullptr;
            this->m_deleterObject = nullptr;
            this->m_object = nullptr;
            this->m_arraySize = 0;
            LOG_FUNCTION_END();
        }
        
        void
        DecreaseReference( void )
        {
            LOG_FUNCTION_START();
            if ( this->m_referenceNumber != nullptr )
            {
                --( *( this->m_referenceNumber ) );
                if ( *( this->m_referenceNumber ) == 0 )
                {
                    this->DeleteMemory();
                }
            }
            LOG_FUNCTION_END();
        }
        
        void
        IncreaseReference( void )
        {
            LOG_FUNCTION_START();
            ++( *( this->m_referenceNumber ) );
            LOG_FUNCTION_END();
        }
        
        void
        AllocateObject( void )
        {
            LOG_FUNCTION_START();
            this->m_object = new Type();
            if ( this->m_object == nullptr )
            {
                LOG_FUNCTION_END();
                throw ( std::bad_alloc() );
            }
            this->m_referenceNumber = new TReferenceNumber( 1 );
            this->m_deleterObject = new DeleterImplementation( this->m_object );
            if ( this->m_referenceNumber == nullptr )
            {
                LOG_FUNCTION_END();
                throw ( std::bad_alloc() );
            }
            LOG_FUNCTION_END();
        }
    
        template <class ... TArgs>
        void
        AllocateObject( TArgs&&... args )
        {
            LOG_FUNCTION_START();
            this->m_object = new Type( args... );
            if ( this->m_object == nullptr )
            {
                LOG_FUNCTION_END();
                throw ( std::bad_alloc() );
            }
            this->m_referenceNumber = new TReferenceNumber( 1 );
            this->m_deleterObject = new DeleterImplementation( this->m_object );
            if ( this->m_referenceNumber == nullptr )
            {
                LOG_FUNCTION_END();
                throw ( std::bad_alloc() );
            }
            LOG_FUNCTION_END();
        }
    
        void
        AllocateObject( const TObject & object )
        {
            LOG_FUNCTION_START();
            this->m_object = new Type( object );
            if ( this->m_object == nullptr )
            {
                LOG_FUNCTION_END();
                throw ( std::bad_alloc() );
            }
            this->m_referenceNumber = new TReferenceNumber( 1 );
            this->m_deleterObject = new DeleterImplementation( this->m_object );
            if ( this->m_referenceNumber == nullptr )
            {
                LOG_FUNCTION_END();
                throw ( std::bad_alloc() );
            }
            LOG_FUNCTION_END();
        }
    
        void
        AllocateObject( TObject && object )
        {
            LOG_FUNCTION_START();
            this->m_object = new Type( std::move( object ) );
            if ( this->m_object == nullptr )
            {
                LOG_FUNCTION_END();
                throw ( std::bad_alloc() );
            }
            this->m_referenceNumber = new TReferenceNumber( 1 );
            this->m_deleterObject = new DeleterImplementation( this->m_object );
            if ( this->m_referenceNumber == nullptr )
            {
                LOG_FUNCTION_END();
                throw ( std::bad_alloc() );
            }
            LOG_FUNCTION_END();
        }
    
        void
        AllocateObjectArray( const TArraySize arraySize )
        {
            LOG_FUNCTION_START();
            this->m_arraySize = arraySize;
            this->m_object = new Type[arraySize];
            if ( this->m_object == nullptr )
            {
                LOG_FUNCTION_END();
                throw ( std::bad_alloc() );
            }
            this->m_referenceNumber = new TReferenceNumber( 1 );
            this->m_deleterObject = new DeleterArrayImplementation( this->m_object );
            if ( this->m_referenceNumber == nullptr )
            {
                LOG_FUNCTION_END();
                throw ( std::bad_alloc() );
            }
            LOG_FUNCTION_END();
        }
    
        void
        NullifyMovedObject( void )
        {
            LOG_FUNCTION_START();
            this->m_referenceNumber = nullptr;
            this->m_object = nullptr;
            this->m_arraySize = 0;
            this->m_deleterObject = nullptr;
            LOG_FUNCTION_END();
        }
    
    public:
        SmartPointer( void ) : m_referenceNumber( nullptr ),
                               m_object( nullptr ),
                               m_arraySize( 0 ),
                               m_deleterObject( nullptr ),
                               m_isMoved( false )
        {
            LOG_FUNCTION_START();
            LOG_FUNCTION_END();
        }
    
        SmartPointer( const SmartPointer<Type> & copy ) : m_referenceNumber( copy.m_referenceNumber ),
                                                          m_object( copy.m_object ),
                                                          m_arraySize( copy.m_arraySize ),
                                                          m_deleterObject( ( DeleterObject* )copy.m_deleterObject ),
                                                          m_isMoved( copy.m_isMoved )
        {
            LOG_FUNCTION_START();
            this->IncreaseReference();
            LOG_FUNCTION_END();
        }
    
        template <typename Type2>
        SmartPointer( const SmartPointer<Type2> & copy ) : m_referenceNumber( copy.m_referenceNumber ),
                                                           m_object( copy.m_object ),
                                                           m_arraySize( copy.m_arraySize ),
                                                           m_deleterObject( ( DeleterObject* )copy.m_deleterObject ),
                                                           m_isMoved( copy.m_isMoved )
        {
            LOG_FUNCTION_START();
            this->IncreaseReference();
            LOG_FUNCTION_END();
        }
    
        SmartPointer( SmartPointer<Type> && movedObject )
        {
            LOG_FUNCTION_START();
            this->m_referenceNumber = movedObject.m_referenceNumber;
            this->m_object = movedObject.m_object;
            this->m_arraySize = movedObject.m_arraySize;
            this->m_deleterObject = ( DeleterObject* )movedObject.m_deleterObject;
            this->m_isMoved = movedObject.m_isMoved;
            movedObject.NullifyMovedObject();
            movedObject.m_isMoved = true;
            LOG_FUNCTION_END();
        }
    
        template <typename Type2>
        SmartPointer( SmartPointer<Type2> && movedObject )
        {
            LOG_FUNCTION_START();
            this->m_referenceNumber = movedObject.m_referenceNumber;
            this->m_object = movedObject.m_object;
            this->m_arraySize = movedObject.m_arraySize;
            this->m_deleterObject = ( DeleterObject* )movedObject.m_deleterObject;
            this->m_isMoved = movedObject.m_isMoved;
            movedObject.NullifyMovedObject();
            movedObject.m_isMoved = true;
            LOG_FUNCTION_END();
        }
    
        SmartPointer( Type* object ) : m_arraySize( 0 )
        {
            LOG_FUNCTION_START();
            this->m_object = object;
            if ( this->m_object == nullptr )
            {
                LOG_FUNCTION_END();
                throw ( std::bad_alloc() );
            }
            this->m_referenceNumber = new TReferenceNumber( 1 );
            this->m_deleterObject = new DeleterImplementation( this->m_object );
            this->m_isMoved = false;
            if ( this->m_referenceNumber == nullptr )
            {
                LOG_FUNCTION_END();
                throw ( std::bad_alloc() );
            }
            LOG_FUNCTION_END();
        }
    
    
        template <class ...TArgs>
        SmartPointer( TArgs... args )
        {
            LOG_FUNCTION_START();
            throw ( 0 );
            LOG_FUNCTION_END();
        }
    
        ~SmartPointer( void )
        {
            LOG_FUNCTION_START();
            if ( !this->m_isMoved )
            {
                this->DecreaseReference();
            }
            LOG_FUNCTION_END();
        }
    
        SmartPointer<Type> &
        operator = ( const SmartPointer<Type> & copy )
        {
            LOG_FUNCTION_START();
            if ( this == &( copy ) )
            {
                LOG_FUNCTION_END();
                return ( *this );
            }
            if ( this->m_object == copy.m_object )
            {
                LOG_FUNCTION_END();
                return ( *this );
            }
            this->DecreaseReference();
            this->m_object = copy.m_object;
            this->m_referenceNumber = copy.m_referenceNumber;
            this->m_arraySize = copy.m_arraySize;
            this->m_deleterObject = copy.m_deleterObject;
            this->m_isMoved = copy.m_isMoved;
            this->IncreaseReference();
            LOG_FUNCTION_END();
            return ( *this );
        }
    
        template <typename Type2>
        SmartPointer<Type> &
        operator = ( const SmartPointer<Type2> & copy )
        {
            LOG_FUNCTION_START();
            if ( ( void* )this == ( void* )&( copy ) )
            {
                LOG_FUNCTION_END();
                return ( *this );
            }
            if ( this->m_object == copy.m_object )
            {
                LOG_FUNCTION_END();
                return ( *this );
            }
            this->DecreaseReference();
            this->m_object = copy.m_object;
            this->m_referenceNumber = copy.m_referenceNumber;
            this->m_arraySize = copy.m_arraySize;
            this->m_deleterObject = ( DeleterObject* )copy.m_deleterObject;
            this->m_isMoved = copy.m_isMoved;
            this->IncreaseReference();
            LOG_FUNCTION_END();
            return ( *this );
        }
    
        SmartPointer<Type> &
        operator = ( SmartPointer<Type> && move )
        {
            LOG_FUNCTION_START();
            if ( this == &( move ) )
            {
                LOG_FUNCTION_END();
                return ( *this );
            }
            if ( this->m_object == move.m_object )
            {
                LOG_FUNCTION_END();
                return ( *this );
            }
            this->DecreaseReference();
            this->m_object = move.m_object;
            this->m_referenceNumber = move.m_referenceNumber;
            this->m_arraySize = move.m_arraySize;
            this->m_deleterObject = move.m_deleterObject;
            this->m_isMoved = move.m_isMoved;

            move.NullifyMovedObject();
            move.m_isMoved = true;
            LOG_FUNCTION_END();
            return ( *this );
        }
    
        template <typename Type2>
        SmartPointer<Type> &
        operator = ( SmartPointer<Type2> && move )
        {
            LOG_FUNCTION_START();
            if ( this == &( move ) )
            {
                LOG_FUNCTION_END();
                return ( *this );
            }
            if ( this->m_object == move.m_object )
            {
                LOG_FUNCTION_END();
                return ( *this );
            }
            this->DecreaseReference();
            this->m_object = move.m_object;
            this->m_referenceNumber = move.m_referenceNumber;
            this->m_arraySize = move.m_arraySize;
            this->m_deleterObject = move.m_deleterObject;
            this->m_isMoved = move.m_isMoved;
            
            move.NullifyMovedObject();
            move.m_isMoved = true;
            LOG_FUNCTION_END();
            return ( *this );
        }
    
        template <typename Type2>
        bool
        operator == ( const SmartPointer<Type2> & rhs ) const
        {
            LOG_FUNCTION_START();
            if ( this->m_object == rhs.m_object )
            {
                LOG_FUNCTION_END();
                return ( true );
            }
            LOG_FUNCTION_END();
            return ( false );
        }
    
        template <typename Type2>
        bool
        operator == ( Type2* rhs ) const
        {
            LOG_FUNCTION_START();
            if ( this->m_object == rhs )
            {
                LOG_FUNCTION_END();
                return ( true );
            }
            LOG_FUNCTION_END();
            return ( false );
        }
    
        template <typename Type2>
        bool
        operator != ( const SmartPointer<Type2> & rhs ) const
        {
            LOG_FUNCTION_START();
            if ( this->m_object != rhs.m_object )
            {
                LOG_FUNCTION_END();
                return ( true );
            }
            LOG_FUNCTION_END();
            return ( false );
        }
    
        template <typename Type2>
        bool
        operator != ( Type2* rhs ) const
        {
            LOG_FUNCTION_START();
            if ( this->m_object != rhs )
            {
                LOG_FUNCTION_END();
                return ( true );
            }
            LOG_FUNCTION_END();
            return ( false );
        }
    
        template <typename Type2>
        bool
        operator < ( const SmartPointer<Type2> & rhs ) const
        {
            LOG_FUNCTION_START();
            if ( this->m_object < rhs.m_object )
            {
                LOG_FUNCTION_END();
                return ( true );
            }
            LOG_FUNCTION_END();
            return ( false );
        }
    
        template <typename Type2>
        bool
        operator > ( const SmartPointer<Type2> & rhs ) const
        {
            LOG_FUNCTION_START();
            LOG_FUNCTION_END();
            return ( rhs < ( *this ) );
        }
    
        template <typename Type2>
        bool
        operator <= ( const SmartPointer<Type2> & rhs ) const
        {
            LOG_FUNCTION_START();
            LOG_FUNCTION_END();
            return ( ( ( *this ) < rhs ) || ( ( *this ) == rhs ) );
        }
        
        template <typename Type2>
        bool
        operator >= ( const SmartPointer<Type2> & rhs ) const
        {
            LOG_FUNCTION_START();
            LOG_FUNCTION_END();
            return ( ( rhs < ( *this ) ) || ( ( *this ) == rhs ) );
        }
    
        template <class ...TArgs>
        void
        operator () ( const TArgs&... args )
        {
            LOG_FUNCTION_START();
            ( *( ( Type* )this->m_object ) )( args... );
            LOG_FUNCTION_END();
        }
    
        template <class ...TArgs>
        void
        operator () ( TArgs&&... args )
        {
            LOG_FUNCTION_START();
            ( *( ( Type* )this->m_object ) )( args... );
            LOG_FUNCTION_END();
        }
    
        void
        operator () ( void )
        {
            LOG_FUNCTION_START();
            ( *( ( Type* )this->m_object ) )();
            LOG_FUNCTION_END();
        }
    
        static
        SmartPointer<Type>
        CreateObject( void )
        {
            LOG_FUNCTION_START();
            SmartPointer<Type> newPointer;
            newPointer.AllocateObject();
            LOG_FUNCTION_END();
            return ( newPointer );
        }
    
    
        template <class ...TArgs>
        static
        SmartPointer<Type>
        CreateObject( TArgs&&... args )
        {
            LOG_FUNCTION_START();
            SmartPointer<Type> newPointer;
            newPointer.AllocateObject( args... );
            LOG_FUNCTION_END();
            return ( newPointer );
        }
    
        static
        SmartPointer<Type>
        CreateObject( const TObject & object )
        {
            LOG_FUNCTION_START();
            SmartPointer<Type> newPointer;
            newPointer.AllocateObject( object );
            LOG_FUNCTION_END();
            return ( newPointer );
        }
    
        static
        SmartPointer<Type>
        CreateObject( TObject && object )
        {
            LOG_FUNCTION_START();
            SmartPointer<Type> newPointer;
            newPointer.AllocateObject( std::move( object ) );
            LOG_FUNCTION_END();
            return ( newPointer );
        }
    
        static
        SmartPointer<Type>
        CreateObjectArray( const TArraySize arraySize )
        {
            LOG_FUNCTION_START();
            SmartPointer<Type> newPointer;
            newPointer.AllocateObjectArray( arraySize );
            LOG_FUNCTION_END();
            return ( newPointer );
        }
        
        Type &
        operator * ( void )
        {
            LOG_FUNCTION_START();
            LOG_FUNCTION_END();
            return ( *( ( Type* )this->m_object ) );
        }
        
        const Type &
        operator * ( void ) const
        {
            LOG_FUNCTION_START();
            LOG_FUNCTION_END();
            return ( *( ( Type* )this->m_object ) );
        }
    
        TPointer
        operator ->( void )
        {
            LOG_FUNCTION_START();
            LOG_FUNCTION_END();
            return ( ( ( Type* )this->m_object ) );
        }
    
        const TPointer
        operator ->( void ) const
        {
            LOG_FUNCTION_START();
            LOG_FUNCTION_END();
            return ( ( ( Type* )this->m_object ) );
        }
    
        TSubscriptType &
        operator [] ( const size_t index )
        {
            LOG_FUNCTION_START();
            LOG_FUNCTION_END();
            if ( index >= this->m_arraySize )
            {
                class ERROR_OutOfArrayBounds{} error;
                throw ( error );
            }
            return( *( ( Type* )( &( **this ) ) + index ) );
        }
        
        const TSubscriptType &
        operator [] ( const size_t index ) const
        {
            LOG_FUNCTION_START();
            LOG_FUNCTION_END();
            if ( index >= this->m_arraySize )
            {
                class ERROR_OutOfArrayBounds{} error;
                throw ( error );
            }
            return( *( ( TPointer )( &( **this ) ) + index ) );
        }
    
        template <typename Type2>
        operator Type2*( void ) const
        {
            return ( ( Type2* )this->m_object );
        }
    
        Type*
        operator + ( const size_t offset )
        {
            return ( ( ( Type* )this->m_object ) + offset );
        }
    
        const Type*
        operator + ( const size_t offset ) const
        {
            return ( ( ( Type* )this->m_object ) + offset );
        }
    
        Type*
        operator - ( const size_t offset )
        {
            return ( ( ( Type* )this->m_object ) - offset );
        }
        
        const Type*
        operator - ( const size_t offset ) const
        {
            return ( ( ( Type* )this->m_object ) - offset );
        }
    
        bool
        IsNull( void ) const
        {
            LOG_FUNCTION_START();
            LOG_FUNCTION_END();
            return ( this->m_object == nullptr );
        }
    
        void
        NullifyPointer( void )
        {
            LOG_FUNCTION_START();
            this->DecreaseReference();
            this->NullifyMovedObject();
            LOG_FUNCTION_END();
        }
};

template <typename MainType>
SmartPointer<MainType>
CreateObject( void )
{
    return ( SmartPointer<MainType>::CreateObject() );
}

template <typename MainType, class ...TArgs>
SmartPointer<MainType>
CreateObject( TArgs&&... args )
{
    return ( SmartPointer<MainType>::CreateObject( args... ) );
}

template <typename MainType>
SmartPointer<MainType>
CreateObjectArray( const typename SmartPointer<MainType>::TArraySize arraySize )
{
    return ( SmartPointer<MainType>::CreateObjectArray( arraySize ) );
}

template <typename MainType>
SmartPointer<MainType>
CreateObject( const MainType & object )
{
    return ( SmartPointer<MainType>::CreateObject( object ) );
}

template <typename MainType>
SmartPointer<MainType>
CreateObject( MainType && object )
{
    return ( SmartPointer<MainType>::CreateObject( std::move( object ) ) );
}

#endif
