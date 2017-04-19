/**
 * @file RegulationClientRegisterRequest.cpp
 * Implements the RegulationClientRegisterRequest class.
 *  @date Sep 24, 2014
 *  @author: Alper Sinan Akyurek
 */

#include "RegulationClientRegisterRequest.h"

namespace TerraSwarm
{
    RegulationClientRegisterRequest::RegulationClientRegisterRequest( void )
    {
    }
    
    RegulationClientRegisterRequest::~RegulationClientRegisterRequest( void )
    {
    }
    
    SmartPointer<RegulationClientRegisterRequest>
    RegulationClientRegisterRequest::GetNewRegulationClientRegisterRequest( const MessageHeader::TSenderId senderId,
                                                                            const MessageHeader::TReceiverId receiverId )
    {
        TDataSize dataSize = 0;
        TDataSize totalDataSize = MessageHeader::MessageHeaderSize + dataSize + MessageEnder::EndOfMessageSize;
        auto newMemory = CreateObjectArray<char>( totalDataSize );
        ( ( MessageHeader* )newMemory )->PrepareOutgoingMessage( senderId, receiverId, MessageType, MessageId, dataSize );
        ( ( MessageEnder* )newMemory )->SetEndOfMessageField();
        return ( newMemory );
    }
    
    RegulationClientRegisterRequest::TCheckResult
    RegulationClientRegisterRequest::CheckMessage( void ) const
    {
        if ( ( ( MessageHeader* )this )->GetMessageType() == MessageType &&
            ( ( MessageHeader* )this )->GetMessageId() == MessageId )
        {
            return ( Success );
        }
        return ( Fail );
    }
    
    TDataSize
    RegulationClientRegisterRequest::GetSize( void ) const
    {
        return ( MessageHeader::MessageHeaderSize + MessageEnder::EndOfMessageSize );
    }
} /* namespace TerraSwarm */