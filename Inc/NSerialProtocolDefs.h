//==============================================================================
#ifndef NSERIALPROTOCOLDEFS_H
    #define NSERIALPROTOCOLDEFS_H
    #include <stdint.h>
    #include "NCommand.h"

    //-------------------------------------------------
    // definições do PST2 (RT004-GEPE/08)

    //-------------------------------------------------
    // mensagens de baixa prioridade (definidas pela aplicação)
    //const uint32_t NM_APPLMSG_FIRST = 0x10000000;
    const uint32_t NM_UNICAST_PROCESS = 0x10000001;
    //#warning "Defined system message: NM_UNICAST_PROCESS = 0x10000001"
    const uint32_t NM_SERVICE_PROCESS = 0x10000002;
    //#warning "Defined system message: NM_SERVICE_PROCESS = 0x10000002"
    const uint32_t NM_BROADCAST_PROCESS = 0x10000003;
    //#warning "Defined system message: NM_BROADCAST_PROCESS = 0x10000003"
    const uint32_t NM_UNICAST_POSTPROCESS = 0x10000004;
    //#warning "Defined system message: NM_UNICAST_POSTPROCESS = 0x10000004"
    const uint32_t NM_SERVICE_POSTPROCESS = 0x10000005;
    //#warning "Defined system message: NM_SERVICE_POSTPROCESS = 0x10000005"
    const uint32_t NM_UNICAST_RESPONSE = 0x10000006;
    //#warning "Defined system message: NM_UNICAST_RESPONSE = 0x10000006"
    const uint32_t NM_SERVICE_RESPONSE = 0x10000007;
    //#warning "Defined system message: NM_SERVICE_RESPONSE = 0x10000007"
    const uint32_t NM_POSTPROCESS = 0x10000008;

    //#warning "Defined system message: NM_ROUTING_PROCESS = 0x10000009"
    const uint32_t NM_ROUTING_PROCESS  = 0x10000009;
    //#warning "Defined system message: NM_ROUTING_RESPONSE = 0x1000000A"
    const uint32_t NM_ROUTING_RESPONSE  = 0x1000000A;

    //#warning "Defined system message: NM_UNICAST_PROCESS = 0x10000008"
    const uint32_t NM_SERIAL_ONINTERPRET = 0x10000021;
    //#warning "Defined system message: NM_SERIAL_ONINTERPRET = 0x10000021"
    const uint32_t NM_SERIAL_ONPOSTINTERPRET = 0x10000022;
    //#warning "Defined system message: NM_SERIAL_ONPOSTINTERPRET = 0x10000022"
    const uint32_t NM_SERIAL_ONSLAVESILENCE = 0x10000023;
    //#warning "Defined system message: NM_SERIAL_ONSLAVESILENCE = 0x10000023"
    //const uint32_t NM_APPLMSG_FIRST = 0x10000000;
    
    const uint32_t NM_DATAGRAM_PROCESS = 0x1000000C;
    //#warning "Defined system message: NM_BROADCAST_PROCESS = 0x1000000C"
    
    //const uint32_t NM_APPLMSG_LAST  = 0x10000100;
    //-------------------------------------------------

#endif
//==============================================================================
