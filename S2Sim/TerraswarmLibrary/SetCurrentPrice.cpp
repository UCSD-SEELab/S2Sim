/**
 * @file SetCurrentPrice.cpp
 * Implements the SetCurrentPrice class.
 *  @date Oct 31, 2013
 *  @author: Alper Sinan Akyurek
 */

#include "SetCurrentPrice.h"

namespace TerraSwarm
{
    namespace Synchronous
    {
        SetCurrentPrice::SetCurrentPrice( void )
        {
        }

        SetCurrentPrice::~SetCurrentPrice( void )
        {
        }

        SmartPointer<SetCurrentPrice>
        SetCurrentPrice::GetNewSetCurrentPrice( const MessageHeader::TSenderId senderId,
                                                const MessageHeader::TReceiverId receiverId,
                                                const TInterval intervalBegin,
                                                const TNumberOfPricePoints numberOfPricePoints,
                                                TPrice* pricePoints )
        {
            TDataSize dataSize = IntervalBeginSize + NumberOfPricePointsSize + numberOfPricePoints * PriceSize;
            TDataSize totalDataSize = MessageHeader::MessageHeaderSize + dataSize + MessageEnder::EndOfMessageSize;
            auto newMemory = CreateObjectArray<char>( totalDataSize );
            ( ( MessageHeader* )newMemory )->PrepareOutgoingMessage( senderId, receiverId, MessageType, MessageId, dataSize );
            ( ( TIntervalBeginAccessor* )newMemory )->Write( intervalBegin );
            ( ( TNumberOfPricePointsAccessor* )newMemory )->Write( numberOfPricePoints );
            memcpy( ( newMemory + PricePointsIndex ), pricePoints, PriceSize * numberOfPricePoints );
            ( ( MessageEnder* )newMemory )->SetEndOfMessageField();
            return ( newMemory );
        }

        SetCurrentPrice::TCheckResult
        SetCurrentPrice::CheckMessage( void ) const
        {
            if ( ( ( MessageHeader* )this )->GetMessageType() == MessageType &&
                 ( ( MessageHeader* )this )->GetMessageId() == MessageId )
            {
                return ( Success );
            }
            return ( Fail );
        }

        SmartPointer<SetCurrentPrice::TPrice>
        SetCurrentPrice::GetPrice( void ) const
        {
            TNumberOfPricePoints numberOfPricePoints = this->GetNumberOfPricePoints();
            auto values = CreateObjectArray<TPrice>( numberOfPricePoints );
            memcpy( values, ( this + PricePointsIndex ), PriceSize * numberOfPricePoints );
            return ( values );
        }

        SetCurrentPrice::TInterval
        SetCurrentPrice::GetIntervalBegin( void ) const
        {
            TInterval value;
            ( ( TIntervalBeginAccessor* )this )->Read( value );
            return ( value );
        }

        SetCurrentPrice::TNumberOfPricePoints
        SetCurrentPrice::GetNumberOfPricePoints( void ) const
        {
            TNumberOfPricePoints value;
            ( ( TNumberOfPricePointsAccessor* )this )->Read( value );
            return ( value );
        }
        
        TDataSize
        SetCurrentPrice::GetSize( void ) const
        {
            return ( MessageHeader::MessageHeaderSize + IntervalBeginSize + NumberOfPricePointsSize + this->GetNumberOfPricePoints() * PriceSize + MessageEnder::EndOfMessageSize );
        }
        
        SetCurrentPriceAndRegulation::SetCurrentPriceAndRegulation( void )
        {
        }
        
        SetCurrentPriceAndRegulation::~SetCurrentPriceAndRegulation( void )
        {
        }
        
        SmartPointer<SetCurrentPriceAndRegulation>
        SetCurrentPriceAndRegulation::GetNewSetCurrentPriceAndRegulation( const MessageHeader::TSenderId senderId,
                                                                          const MessageHeader::TReceiverId receiverId,
                                                                          const TInterval intervalBegin,
                                                                          const TNumberOfPricePoints numberOfPricePoints,
                                                                          TPrice* pricePoints,
                                                                          TPrice* upRegulationPricePoints,
                                                                          TPrice* downRegulationPricePoints )
        {
            TDataSize dataSize = IntervalBeginSize + NumberOfPricePointsSize + numberOfPricePoints * PriceSize;
            TDataSize totalDataSize = MessageHeader::MessageHeaderSize + dataSize + MessageEnder::EndOfMessageSize;
            auto newMemory = CreateObjectArray<char>( totalDataSize );
            ( ( MessageHeader* )newMemory )->PrepareOutgoingMessage( senderId, receiverId, MessageType, MessageId, dataSize );
            ( ( TIntervalBeginAccessor* )newMemory )->Write( intervalBegin );
            ( ( TNumberOfPricePointsAccessor* )newMemory )->Write( numberOfPricePoints );
            char* currentMemory = newMemory + PricePointsIndex;
            for ( auto index = 0; index < numberOfPricePoints; ++index )
            {
                memcpy( currentMemory, pricePoints, PriceSize );
                currentMemory += PriceSize;
                memcpy( currentMemory, upRegulationPricePoints, PriceSize );
                currentMemory += PriceSize;
                memcpy( currentMemory, downRegulationPricePoints, PriceSize * numberOfPricePoints );
                currentMemory += PriceSize;
            }
            ( ( MessageEnder* )newMemory )->SetEndOfMessageField();
            return ( newMemory );
        }
        
        SetCurrentPriceAndRegulation::TCheckResult
        SetCurrentPriceAndRegulation::CheckMessage( void ) const
        {
            if ( ( ( MessageHeader* )this )->GetMessageType() == MessageType &&
                ( ( MessageHeader* )this )->GetMessageId() == MessageId )
            {
                return ( Success );
            }
            return ( Fail );
        }
        
        SmartPointer<SetCurrentPriceAndRegulation::TPrice>
        SetCurrentPriceAndRegulation::GetPrice( void ) const
        {
            TNumberOfPricePoints numberOfPricePoints = 3 * this->GetNumberOfPricePoints();
            auto values = CreateObjectArray<TPrice>( numberOfPricePoints );
            memcpy( values, ( this + PricePointsIndex ), PriceSize * numberOfPricePoints );
            return ( values );
        }
        
        SetCurrentPriceAndRegulation::TInterval
        SetCurrentPriceAndRegulation::GetIntervalBegin( void ) const
        {
            TInterval value;
            ( ( TIntervalBeginAccessor* )this )->Read( value );
            return ( value );
        }
        
        SetCurrentPriceAndRegulation::TNumberOfPricePoints
        SetCurrentPriceAndRegulation::GetNumberOfPricePoints( void ) const
        {
            TNumberOfPricePoints value;
            ( ( TNumberOfPricePointsAccessor* )this )->Read( value );
            return ( value );
        }
        
        TDataSize
        SetCurrentPriceAndRegulation::GetSize( void ) const
        {
            return ( MessageHeader::MessageHeaderSize + IntervalBeginSize + 3 * NumberOfPricePointsSize + this->GetNumberOfPricePoints() * PriceSize + MessageEnder::EndOfMessageSize );
        }
    } /* namespace Synchronous */
} /* namespace TerraSwarm */
