/**
 * @file DemandNegotiation.cpp
 * Implements the DemandNegotiation class.
 *  @date Oct 31, 2013
 *  @author: Alper Sinan Akyurek
 */

#include "DemandNegotiation.h"

namespace TerraSwarm
{
    namespace Synchronous
    {
        DemandNegotiation::DemandNegotiation( void )
        {
        }

        DemandNegotiation::~DemandNegotiation( void )
        {
        }

        SmartPointer<DemandNegotiation>
        DemandNegotiation::GetNewDemandNegotiation( const MessageHeader::TSenderId senderId,
                                                    const MessageHeader::TReceiverId receiverId,
                                                    const TNumberOfDataPoints numberOfDataPoints,
                                                    TDataPoint* dataPoints )
        {
            TDataSize totalDataSize = MessageHeader::MessageHeaderSize +
                                      NumberOfDataPointsSize +
                                      DataPointSize * numberOfDataPoints +
                                      MessageEnder::EndOfMessageSize;
            auto newMemory = CreateObjectArray<char>( totalDataSize );
            ( ( MessageHeader* )newMemory )->PrepareOutgoingMessage( senderId, receiverId, MessageType, MessageId, totalDataSize );
            ( ( TNumberOfDataPointsAccessor* )newMemory )->Write( numberOfDataPoints );
            memcpy( ( newMemory + DataStartIndex ), dataPoints, DataPointSize * numberOfDataPoints );
            ( ( MessageEnder* )newMemory )->SetEndOfMessageField();
            return ( newMemory );
        }

        DemandNegotiation::TCheckResult
        DemandNegotiation::CheckMessage( void ) const
        {
            if ( ( ( MessageHeader* )this )->GetMessageType() == MessageType &&
                 ( ( MessageHeader* )this )->GetMessageId() == MessageId )
            {
                return ( Success );
            }
            return ( Fail );
        }

        DemandNegotiation::TNumberOfDataPoints
        DemandNegotiation::GetNumberOfDataPoints( void ) const
        {
            TNumberOfDataPoints value;
            ( ( TNumberOfDataPointsAccessor* )this )->Read( value );
            return ( value );
        }

        SmartPointer<DemandNegotiation::TDataPoint>
        DemandNegotiation::GetDataPoints( void ) const
        {
            TNumberOfDataPoints numberOfDataPoints = this->GetNumberOfDataPoints();
            auto dataPoints = CreateObjectArray<TDataPoint>( numberOfDataPoints );
            memcpy( dataPoints, ( ( char* )this ) + DataStartIndex, numberOfDataPoints * DataPointSize );
            return ( dataPoints );
        }
    } /* namespace Synchronous */
} /* namespace TerraSwarm */
