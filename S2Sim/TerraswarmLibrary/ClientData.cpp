/**
 * @file ClientData.cpp
 * Implements the Async and Synchronous ClientData classes and messages.
 *  Created on: Oct 13, 2013
 *      Author: Alper
 */

#include "ClientData.h"

namespace TerraSwarm
{
    namespace Asynchronous
    {

        ClientData::ClientData( void )
        {
        }

        ClientData::~ClientData( void )
        {
        }

        SmartPointer<ClientData>
        ClientData::GetNewClientData( const MessageHeader::TSenderId senderId,
                                      const MessageHeader::TReceiverId receiverId,
                                      const TStartTime startTime,
                                      const TTimeResolution timeResolution,
                                      const TNumberOfDataPoints numberOfDataPoints,
                                      TDataPoint* dataPoints )
        {
            TDataSize totalDataSize = MessageHeader::MessageHeaderSize +
                                      StartTimeSize +
                                      TimeResolutionSize +
                                      NumberOfDataPointsSize +
                                      DataPointSize * numberOfDataPoints +
                                      MessageEnder::EndOfMessageSize;
            auto newMemory = CreateObjectArray<char>( totalDataSize );
            ( ( MessageHeader* )newMemory )->PrepareOutgoingMessage( senderId, receiverId, MessageType, MessageId, totalDataSize );
            ( ( TStartTimeAccessor* )newMemory )->Write( startTime );
            ( ( TTimeResolutionAccessor* )newMemory )->Write( timeResolution );
            ( ( TNumberOfDataPointsAccessor* )newMemory )->Write( numberOfDataPoints );
            memcpy( ( newMemory + DataStartIndex ), dataPoints, DataPointSize * numberOfDataPoints );
            ( ( MessageEnder* )newMemory )->SetEndOfMessageField();
            return ( newMemory );
        }

        ClientData::TCheckResult
        ClientData::CheckMessage( void ) const
        {
            if ( ( ( MessageHeader* )this )->GetMessageType() == MessageType &&
                 ( ( MessageHeader* )this )->GetMessageId() == MessageId )
            {
                return ( Success );
            }
            return ( Fail );
        }

        ClientData::TStartTime
        ClientData::GetStartTime( void ) const
        {
            TStartTime value;
            ( ( TStartTimeAccessor* )this )->Read( value );
            return ( value );
        }

        ClientData::TTimeResolution
        ClientData::GetTimeResolution( void ) const
        {
            TTimeResolution value;
            ( ( TTimeResolutionAccessor* )this )->Read( value );
            return ( value );
        }

        ClientData::TNumberOfDataPoints
        ClientData::GetNumberOfDataPoints( void ) const
        {
            TNumberOfDataPoints value;
            ( ( TNumberOfDataPointsAccessor* )this )->Read( value );
            return ( value );
        }

        SmartPointer<ClientData::TDataPoint>
        ClientData::GetDataPoints( void ) const
        {
            TNumberOfDataPoints numberOfDataPoints = this->GetNumberOfDataPoints();
            auto dataPoints = CreateObjectArray<TDataPoint>( numberOfDataPoints );
            memcpy( dataPoints, ( ( char* )this ) + DataStartIndex, numberOfDataPoints * DataPointSize );
            return ( dataPoints );
        }

    } /* namespace Asynchronous */

    namespace Synchronous
    {

        ClientData::ClientData( void )
        {
        }

        ClientData::~ClientData( void )
        {
        }

        SmartPointer<ClientData>
        ClientData::GetNewClientData( const MessageHeader::TSenderId senderId,
                                      const MessageHeader::TReceiverId receiverId,
                                      TDataPoint dataPoint )
        {
            TDataSize totalDataSize = MessageHeader::MessageHeaderSize +
                                      DataPointSize +
                                      MessageEnder::EndOfMessageSize;
            auto newMemory = CreateObjectArray<char>( totalDataSize );
            ( ( MessageHeader* )newMemory )->PrepareOutgoingMessage( senderId, receiverId, MessageType, MessageId, totalDataSize );
            ( ( TDataPointAccessor* )newMemory )->Write( dataPoint );
            ( ( MessageEnder* )newMemory )->SetEndOfMessageField();
            return ( newMemory );
        }

        ClientData::TCheckResult
        ClientData::CheckMessage( void ) const
        {
            if ( ( ( MessageHeader* )this )->GetMessageType() == MessageType &&
                 ( ( MessageHeader* )this )->GetMessageId() == MessageId )
            {
                return ( Success );
            }
            return ( Fail );
        }

        ClientData::TDataPoint
        ClientData::GetDataPoint( void ) const
        {
            TDataPoint value;
            ( ( TDataPointAccessor* )this )->Read( value );
            return ( value );
        }
        
        ClientExtendedData::ClientExtendedData( void )
        {
        }
        
        ClientExtendedData::~ClientExtendedData( void )
        {
        }
        
        SmartPointer<ClientExtendedData>
        ClientExtendedData::GetNewClientData( const MessageHeader::TSenderId senderId,
                                              const MessageHeader::TReceiverId receiverId,
                                              const TNumberOfDataPoints numberOfDataPoints,
                                              TDataPoint* dataPoints )
        {
            TDataSize totalDataSize = MessageHeader::MessageHeaderSize +
            DataPointSize +
            MessageEnder::EndOfMessageSize;
            auto newMemory = CreateObjectArray<char>( totalDataSize );
            ( ( MessageHeader* )newMemory )->PrepareOutgoingMessage( senderId, receiverId, MessageType, MessageId, totalDataSize );
            ( ( TNumberOfDataPointsAccessor* )newMemory )->Write( numberOfDataPoints );
            memcpy( ( newMemory + DataStartIndex ), dataPoints, numberOfDataPoints * DataPointSize );
            ( ( MessageEnder* )newMemory )->SetEndOfMessageField();
            return ( newMemory );
        }
        
        ClientExtendedData::TCheckResult
        ClientExtendedData::CheckMessage( void ) const
        {
            if ( ( ( MessageHeader* )this )->GetMessageType() == MessageType &&
                ( ( MessageHeader* )this )->GetMessageId() == MessageId )
            {
                return ( Success );
            }
            return ( Fail );
        }
        
        ClientExtendedData::TNumberOfDataPoints
        ClientExtendedData::GetNumberOfDataPoints( void ) const
        {
            TNumberOfDataPoints value;
            ( ( TNumberOfDataPointsAccessor* )this )->Read( value );
            return ( value );
        }
        
        SmartPointer<ClientExtendedData::TDataPoint>
        ClientExtendedData::GetDataPoints( void ) const
        {
            TNumberOfDataPoints numberOfDataPoints = this->GetNumberOfDataPoints();
            auto dataPoints = CreateObjectArray<TDataPoint>( numberOfDataPoints );
            memcpy( dataPoints, ( ( ( char* )this) + DataStartIndex ), numberOfDataPoints * DataPointSize );
            return ( dataPoints );
        }

    } /* namespace Synchronous */

} /* namespace TerraSwarm */
