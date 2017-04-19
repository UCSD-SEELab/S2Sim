/**
 * @file MessageHeader.cpp
 * Implements the MessageHeader class.
 *  @date Oct 13, 2013
 *  @author: Alper Sinan Akyurek
 */

#include "MessageHeader.h"

namespace TerraSwarm
{

    MessageHeader::MessageHeader( void )
    {
    }

    SmartPointer<MessageHeader>
    MessageHeader::GetNewMessageHeader( void )
    {
        return ( CreateObjectArray<char>( MessageHeaderSize ) );
    }

    void
    MessageHeader::PrepareOutgoingMessage( const TSenderId senderId,
                                           const TReceiverId receiverId,
                                           const TMessageType messageType,
                                           const TMessageId messageId,
                                           const TDataSize dataSize )
    {
        this->Access<TStartOfMessageAccessor>()->Write( ( TStartOfMessage )StartOfMessageDefaultValue );
        this->Access<TSenderIdAccessor>()->Write( senderId );
        this->Access<TReceiverIdAccessor>()->Write( receiverId );
        this->Access<TSequenceNumberAccessor>()->Write( GetNextSequenceNumber( senderId, receiverId ) );
        this->Access<TMessageTypeAccessor>()->Write( messageType );
        this->Access<TMessageIdAccessor>()->Write( messageId );
        this->Access<TDataSizeAccessor>()->Write( dataSize );
    }

    MessageHeader::TSequenceNumber
    MessageHeader::GetNextSequenceNumber( const TSenderId senderId, const TReceiverId receiverId )
    {
        typedef std::pair<TSenderId,TReceiverId> TIdPair;
        typedef std::map<TIdPair, TSequenceNumber> TIdPairMap;
        static TIdPairMap idPairMap;
        TSequenceNumber sequenceNumber = idPairMap[std::make_pair( senderId, receiverId )];
        ++idPairMap[std::make_pair( senderId, receiverId )];
        return ( sequenceNumber );
    }

} /* namespace TerraSwarm */
