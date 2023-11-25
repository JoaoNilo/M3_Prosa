//==============================================================================
#include "NSerialCommand.h"
#include "NSerialProtocol.h"

//------------------------------------------------------------------------------
// construtora da classe base NSerialCommand
NSerialCommand::NSerialCommand(NComponent* protocol){
    //------------------------------------
    OnProcess = NULL; OnPostProcess = NULL;

    //------------------------------------
    // tratamento da propriedade Identifier
    Identifier.setOwner(this);
    Identifier.set(&NSerialCommand::SetIdentifier);
    Identifier.get(&NSerialCommand::GetIdentifier);

    //------------------------------------
    status = fsmCommandIdle;
    Enabled = true;
    Responsive = true;

    //------------------------------------
    // registra o comando no protocolo
    //protocol->IncludeCommand(Handle);
    ((NSerialProtocol*)protocol)->IncludeCommand(Handle);

}

//------------------------------------------------------------------------------
// destrutora da classe base NSerialCommand
NSerialCommand::~NSerialCommand(){ }

//------------------------------------------------------------------------------
// atribui novo identificado de comando (Command Id)
void NSerialCommand::SetIdentifier(uint8_t newId){
    NCommand::ID = newId;
}

//------------------------------------------------------------------------------
// retorna o identificador do comando (Command Id)
uint8_t NSerialCommand::GetIdentifier(){ return(NCommand::ID);}

//------------------------------------------------------------------------------
// notifica mensagens para a classe NSerialCommand
void NSerialCommand::Notify(NMESSAGE* msg){
    if(Enabled){
        if(msg->message == NM_POSTPROCESS){
            if(status == fsmPendingPostProcess){
                if(OnPostProcess!=NULL){ OnPostProcess();}
                status =  fsmCommandIdle;
            }
        } else {
            switch(msg->message){
                case NM_UNICAST_PROCESS: msg->message = NM_UNICAST_RESPONSE; break;
                case NM_SERVICE_PROCESS: msg->message = NM_SERVICE_RESPONSE; break;
                case NM_BROADCAST_PROCESS: msg->message = NM_NULL; break;
                case NM_ROUTING_PROCESS: msg->message = NM_ROUTING_RESPONSE; break;
            }
            NDatagram* dt = (NDatagram*)msg->data1;
            if(OnProcess!=NULL){ OnProcess(dt);}

            // checa se deve responder
            if(Responsive){
                msg->data1 = (uint32_t)dt;
                status = fsmPendingPostProcess;
            } else {
                msg->message = NM_NULL; msg->data1 = 0;
            }
        }
    } else { msg->message = NM_NULL; msg->data1 = 0;}
}

//==============================================================================
