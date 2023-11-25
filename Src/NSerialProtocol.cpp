//==============================================================================
#include "NSerialProtocol.h"
#include <string.h>

//------------------------------------------------------------------------------
NSerialProtocol::NSerialProtocol(NDataLink* DLink){

    //---------------------------------------
    dataLink = DLink;
    dataLink->BindReceptionAssistent((NComponent*)this->Handle);
    tmpData = new NDatagram();
	
    // tratamento da propriedade DataLink
    DataLink.setOwner(this);
    DataLink.set(&NSerialProtocol::SetDataLink);
    DataLink.get(&NSerialProtocol::GetDataLink);

    //---------------------------------------
    // zera contador de objetos "instanciados"
    protCommands = NULL; commands_number = 0L;
    msgProtocol = new NMESSAGE;
    Enabled = true;
    //maxPayload = PST2_PAYLOAD_CAPACITY_MAX;
    localPayload = new uint8_t[PROSA_PAYLOAD_CAPACITY];
}

//------------------------------------------------------------------------------
NSerialProtocol::~NSerialProtocol(){}

//------------------------------------------------------------------------------
// associa esse interpretador de protocolo ao novo "DLink"
void NSerialProtocol::SetDataLink(NDataLink* DLink){
    //---------------------------------------
    dataLink = DLink;
     dataLink->BindReceptionAssistent((NComponent*)this->Handle);
}

//------------------------------------------------------------------------------
// devolve o "DLink" associado a esse interpretador de protocolo
NDataLink* NSerialProtocol::GetDataLink(){ return(dataLink);}

//------------------------------------------------------------------------------
// registra um novo comando no protocolo
unsigned int NSerialProtocol::IncludeCommand(HANDLE newcommand){
    uint32_t bkpSize = commands_number * sizeof(HANDLE);
    uint32_t bkpItems = commands_number + 1;

    HANDLE* bkpCommands = new HANDLE[bkpItems];
    if(bkpSize>0){
        memcpy((uint8_t*)bkpCommands, (uint8_t*)protCommands, bkpSize);
    }
    bkpCommands[commands_number++] = newcommand;
    delete[] protCommands;
    protCommands = bkpCommands;
    return(commands_number);
}

//------------------------------------------------------------------------------
// checa se um ID est� registrado no interpretador NSerialProtocol
bool NSerialProtocol::CheckId(NDatagram* iDt){
    bool result = false;
	// busca um manipulador de comando associado
    for(uint32_t n=0; n < commands_number; n++){
        Command = (NCommand*)protCommands[n];
        if(Command->ID == iDt->Command){ result = true;}
    }
    return(result);
}

//------------------------------------------------------------------------------
// interpreta o datagrama recebido do "NDataLink" (camada 2)
unsigned char NSerialProtocol::Interpret(NMESSAGE* msg, NDatagram* iDt){
    unsigned int n=0L;

    //maxPayload = iDt->ExtractPayload(localPayload);
    iDt->ExtractPayload(localPayload);
    msgProtocol->message = msg->message;
    msgProtocol->data1 = (uint32_t) iDt;
    msgProtocol->data2 = 0;

	// busca um manipulador de comando associado
    for(uint32_t n=0; n<commands_number; n++){
        Command = (NCommand*)protCommands[n];
        if(Command->ID == iDt->Command){
            Command->Notify(msgProtocol);
            if(msgProtocol->message != NM_NULL){
                switch(msgProtocol->message){
                    case NM_UNICAST_RESPONSE:
                    case NM_SERVICE_RESPONSE:
                        if(dataLink->BusPrivilege == dlSlave){
                            dataLink->Put((NDatagram*)msgProtocol->data1);
                        }
                        break;
                }
                break;
            }
        }
    }
    return(n);
}

//------------------------------------------------------------------------------
// fim da transmiss�o da resposta pelo "NDataLink" (camada 2)
void NSerialProtocol::Dismiss(NDatagram* oDt){

    msgProtocol->message = NM_POSTPROCESS;
    msgProtocol->data1 = 0;
    msgProtocol->data2 = 0;
    
    // notifica p�s-processamento para todos os comandos
    for(uint32_t n=0; n<commands_number; n++){
        Command = (NCommand*)protCommands[n];
        if(Command->ID == oDt->Command){
            Command->Notify(msgProtocol);
        }
    }
}

//------------------------------------------------------------------------------
// recebe notifica��o de "timetick" do sistema (1ms)
void NSerialProtocol::Notify(NMESSAGE* msg){

    if(Enabled){
        switch(msg->message){
            case NM_UNICAST_PROCESS:
            case NM_SERVICE_PROCESS:
            case NM_BROADCAST_PROCESS:
                dataLink->Get(tmpData);
                Interpret(msg, tmpData);
                msg->message = NM_NULL;
                break;
            case NM_SERIAL_ONPOSTINTERPRET:
                msg->message = NM_NULL;
                Dismiss(tmpData);
                break;
            case NM_SERIAL_ONSLAVESILENCE:
                msg->message = NM_NULL;
                break;
            case NM_TIMETICK:
                    msg->message = NM_NULL;
                    break;
                default: msg->message = NM_NULL; break;
        }
    } else msg->message = NM_NULL;
}

//==============================================================================
