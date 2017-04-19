/**
 * @file RegulationClientRegisterRequest.h
 * Defines the RegulationClientRegisterRequest class.
 *  @date Sep 24, 2014
 *  @author: Alper Sinan Akyurek
 */

#ifndef __S2Sim__RegulationClientRegisterRequest__
#define __S2Sim__RegulationClientRegisterRequest__

#include "MessageHeader.h"
#include "MessageEnder.h"

#include "SmartPointer.hpp"

namespace TerraSwarm
{
    /**
     *  Regulation Client Register Request message sent from the client to participate in regulation.
     */
    class RegulationClientRegisterRequest
    {
    private:
        /**
         *  Message header values.
         */
        enum HeaderValues
        {
            MessageType = 0x0001,
            MessageId = 0x000A
        };
        
    public:
        /**
         *  Defines the message check result type.
         */
        typedef bool TCheckResult;
        
        /**
         *  Defines the values for TCheckResult.
         */
        enum CheckResultValues
        {
            Success = ( TCheckResult )true, /**< Message is of correct type and id **/
            Fail = ( TCheckResult )false /**< Message has incorrect type or id **/
        };
        
    private:
        /**
         *  No use. Private constructor to force usage of the static creation method.
         */
        RegulationClientRegisterRequest( void );
        
    public:
        /**
         *  Deallocates the memory for the message.
         */
        ~RegulationClientRegisterRequest( void );
        
        /**
         *  Creates a new RegulationClientRegisterRequest message and allocates memory for it. @warning Deallocation is the responsibility of the user.
         *
         *  @param senderId      Id of the sender.
         *  @param receiverId    Id of the receiver.
         *
         *  @return Returns a new allocated message.
         */
        static SmartPointer<RegulationClientRegisterRequest>
        GetNewRegulationClientRegisterRequest( const MessageHeader::TSenderId senderId,
                                               const MessageHeader::TReceiverId receiverId );
        
        /**
         *  Checks whether the current memory contains a SystemTimePrompt message.
         *
         *  @return Result of the check.
         */
        TCheckResult
        CheckMessage( void ) const;
        
        /**
         *  Returns the size of the message.
         *
         *  @return Size of the current message.
         */
        TDataSize
        GetSize() const;
    };
} /* namespace TerraSwarm */

#endif /* defined(__S2Sim__RegulationClientRegisterRequest__) */
