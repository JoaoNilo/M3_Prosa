//==============================================================================
// 2023-05-17: OnSilence event corrections. It was not being triggered.
//			   Code clean-up and "type" standardization.
//------------------------------------------------------------------------------
#include "NSerialProtocol.h"

//------------------------------------------------------------------------------
// inicializa o componente NDataLink
NDataLink::NDataLink(){

	// anula os ponteiros aos eventos
	OnUnicastReceived = NULL; OnPacketSent = NULL;
	OnBrokenPacket = NULL; OnInvalidPacket = NULL;
	OnSilence = NULL; OnServiceReceived = NULL;
	OnBroadcastReceived = NULL; OnReload = NULL;
    OnNewDatagram = NULL; Protocol = NULL;
    OnDispatch = NULL; OnRoutingReceived = NULL;
	OnDisconnected = NULL; OnAnyDatagram = NULL;
    OnPacketToSend = NULL;

	// inicializa a capacidade maxima do payload
	outbuffer_size = PROSA_PAYLOAD_CAPACITY;

	// instancia buffers e objetos
	InData = new NDatagram();
    InData->AutoUpdateCrc = false;
	OutData = new NDatagram();
    OutData->AutoUpdateCrc = false;
    tmpInData = new NDatagram();
    periodicData = new NDatagram();
    uint16_t linesize = outbuffer_size + PROSA_SIZE_HEADER + PROSA_SIZE_CRC;
	OutFifo = new NComFifo(linesize, FIFO_OVERFLOW);

	// dimensiona os buffers
	OutBuffer = new unsigned char[outbuffer_size];

	// tratamento da propriedade LocalAddress
    LocalAddress.setOwner(this);
    LocalAddress.set(&NDataLink::SetLocalAddress);
    LocalAddress.get(&NDataLink::GetLocalAddress);

	// tratamento da propriedade ServiceAddress
    ServiceAddress.setOwner(this);
    ServiceAddress.set(&NDataLink::SetServiceAddress);
    ServiceAddress.get(&NDataLink::GetServiceAddress);

	// tratamento da propriedade BroadcastAddress
    BroadcastAddress.setOwner(this);
    BroadcastAddress.set(&NDataLink::SetBroadcastAddress);
    BroadcastAddress.get(&NDataLink::GetBroadcastAddress);

	// tratamento da propriedade BusPrivilege
    BusPrivilege.setOwner(this);
    BusPrivilege.set(&NDataLink::SetBusPrivilege);
    BusPrivilege.get(&NDataLink::GetBusPrivilege);

    // tratamento da propriedade BusPrivilege
    BusPrivilege.setOwner(this);
    BusPrivilege.set(&NDataLink::SetBusPrivilege);
    BusPrivilege.get(&NDataLink::GetBusPrivilege);

    // tratamento da propriedade TimeDispatch
    TimeDispatch.setOwner(this);
    TimeDispatch.set(&NDataLink::SetTimeDispatch);
    TimeDispatch.get(&NDataLink::GetTimeDispatch);

    // tratamento da propriedade TimeReload
    TimeReload.setOwner(this);
    TimeReload.set(&NDataLink::SetTimeReload);
    TimeReload.get(&NDataLink::GetTimeReload);
    
    // tratamento da propriedade Timeout
    Timeout.setOwner(this);
    Timeout.set(&NDataLink::SetTimeout);
    Timeout.get(&NDataLink::GetTimeout);

    // tratamento da propriedade Retries
    Retries.setOwner(this);
    Retries.set(&NDataLink::SetRetries);
    Retries.get(&NDataLink::GetRetries);

    // tratamento da propriedade OffThreshold
    OffThreshold.setOwner(this);
    OffThreshold.set(&NDataLink::SetOffThreshold);
    OffThreshold.get(&NDataLink::GetOffThreshold);
	
	// inicializa vari�veis de NDataLink
    AssistedTransmission = false;
    AssistedReception = false;
    AddressesEnabled = true;
    RoutingEnabled = false;
	EncriptionEnabled = false;
	AutoDecriptionEnabled = false;
	
	header_size = PROSA_SIZE_HEADER;
	lower_threshold = FIFO_UNDERFLOW;
	upper_threshold = FIFO_OVERFLOW-1;
	local_addr = PROSA_ADDR_NULL;
	service_addr = PROSA_ADDR_NULL;
	broadcast_addr = PROSA_ADDR_NULL;
	
	time_dispatch = PROSA_TIMEDISPATCH;
	timeout_packet = PROSA_TIMEOUT;								// tempo total de espera pelo pacote
	time_reload = PROSA_TIMERELOAD;
    flagTimingOut = false;
    flagRequestTiming = false;

    dispatch_counter = time_dispatch;
    reload_counter = time_reload;
    flagReloaded = false;
    
    packet_retries = 0;
    retries_counter = packet_retries;
    unanswered = 0;
	off_threshold = 0;
    last_source_address = PROSA_ADDR_NULL;

	privilege = dlSlave;
}

//------------------------------------------------------------------------------
// destroi o componente NDataLink
NDataLink::~NDataLink(void){
	delete(OutBuffer);
	delete(OutFifo);
	delete(InData);
	delete(OutData);
}

//------------------------------------------------------------------------------
// abre o "link" para a comunicacao
void NDataLink::Open(){
  	if(off_threshold > 0){
        unanswered = off_threshold;
    }
}

//------------------------------------------------------------------------------
// fecha o "link" para a comunicacao
void NDataLink::Close(){}

//------------------------------------------------------------------------------
// associa um gerenciador de protocolo
void NDataLink::BindReceptionAssistent(NComponent* protController){
    Protocol = protController;
    AssistedReception = true; AddressesEnabled = true;
}

//------------------------------------------------------------------------------
// atribui um novo endereco de rede para o componente NDataLink
void NDataLink::SetLocalAddress(unsigned char l_add){
	if((l_add == service_addr)||(l_add == broadcast_addr)) return;
	local_addr = l_add;
}

//------------------------------------------------------------------------------
// retorna o endereco de rede atual do componente NDataLink
unsigned char NDataLink::GetLocalAddress(){ return(local_addr);}

//------------------------------------------------------------------------------
// atribui um novo endere�o de "manutencao" para o componente NDataLink
void NDataLink::SetServiceAddress(unsigned char s_add){
	if((s_add == local_addr)||(s_add == broadcast_addr)) return;
	service_addr = s_add;
}

//------------------------------------------------------------------------------
// retorna o endereco de "manutencao" atual do componente NDataLink
unsigned char NDataLink::GetServiceAddress(){ return(service_addr);}

//------------------------------------------------------------------------------
// atribui um novo endere�o de "broadcast" para o componente NDataLink
void NDataLink::SetBroadcastAddress(unsigned char b_add){
	if((b_add == local_addr)||(b_add == service_addr)) return;
	broadcast_addr = b_add;
}

//------------------------------------------------------------------------------
// retorna o endereco de "broadcast" atual do componente NDataLink
unsigned char NDataLink::GetBroadcastAddress(){	return(broadcast_addr);}

//------------------------------------------------------------------------------
// atribui novo privilegio de acesso ao meio
void NDataLink::SetBusPrivilege(dlBusPrivileges new_privilege){
	privilege = new_privilege;
}

//------------------------------------------------------------------------------
// retorna o privilegio de acesso ao meio atual
dlBusPrivileges NDataLink::GetBusPrivilege(){ return(privilege);}

//------------------------------------------------------------------------------
// atribui novo intervalo de "envio de pacotes" (modo dlMaster)
void NDataLink::SetTimeDispatch(uint32_t new_timedispatch){
    // checa se o tempo desejado e valido
    if(new_timedispatch < PROSA_TIMEDISPATCH_MIN){
        new_timedispatch = PROSA_TIMEDISPATCH_MIN;
    } else if(new_timedispatch > PROSA_TIMEDISPATCH_MAX){
        new_timedispatch = PROSA_TIMEDISPATCH_MAX;
    }
    
    // atualiza o tempo de envio de pacotes
	time_dispatch = new_timedispatch;
	dispatch_counter = time_dispatch;
    
    // checa se timeout ficou maior que o intervalo
    if((unsigned int)timeout_packet >= new_timedispatch){
        timeout_packet = new_timedispatch - 2;
    }
}

//------------------------------------------------------------------------------
// retorna o intervalo de "envio de pacotes" atual
uint32_t NDataLink::GetTimeDispatch(){	return(time_dispatch);}

//------------------------------------------------------------------------------
// atribui novo intervalo de "envio de pacotes" (modo dlMaster)
void NDataLink::SetTimeReload(uint32_t new_timereload){
    if(new_timereload > PROSA_TIMERELOAD_MAX){
        new_timereload = PROSA_TIMERELOAD_MAX;
    } else if(new_timereload > 0){
        if(new_timereload <= time_dispatch){
            new_timereload = time_dispatch+1;
        }
    }
	time_reload = new_timereload;
    reload_counter = time_reload;
}

//------------------------------------------------------------------------------
// retorna o intervalo de "envio de pacotes" atual
uint32_t NDataLink::GetTimeReload(){ return(time_reload);}

//------------------------------------------------------------------------------
// atribui nova "janela de tempo" para a resposta (modo dlMaster)
void NDataLink::SetTimeout(uint32_t new_timeout){
  	if(new_timeout < PROSA_TIMEOUT_MIN){ new_timeout = PROSA_TIMEOUT_MIN;}
	timeout_packet = new_timeout;
}

//------------------------------------------------------------------------------
// retorna "janela de tempo" para a resposta atual
uint32_t NDataLink::GetTimeout(){ return(timeout_packet);}

//------------------------------------------------------------------------------
// atribui novo numero de re-envio automatico de pacotes nao respondidos (modo dlMaster)
void NDataLink::SetRetries(uint32_t new_retries){
	if(new_retries > PROSA_RETRIES_MAX){ new_retries = PROSA_RETRIES_MAX;}
	packet_retries = new_retries;
    retries_counter = 0;
}

//------------------------------------------------------------------------------
// retorna o numero de re-envio automatico de pacotes nao respondidos
uint32_t NDataLink::GetRetries(){ return(packet_retries);}

//------------------------------------------------------------------------------
// atribui nova tolerancia a pacotes nao respondidos (modo dlMaster)
void NDataLink::SetOffThreshold(uint32_t ot){
	if(ot>255){ ot = 255;}
  	off_threshold = ot;
}

//------------------------------------------------------------------------------
// retorna o atual limiar de tolerancia a pacotes nao respondidos
uint32_t NDataLink::GetOffThreshold(){ return(off_threshold);}

//------------------------------------------------------------------------------
bool NDataLink::ProcessPacket(uint8_t* indata, uint8_t insize){
    InData->Import(indata);
    return(ProcessData());
}

//------------------------------------------------------------------------------
bool NDataLink::ProcessDatagram(NDatagram* iDt){
    InData->CopyFrom(iDt);
    return(ProcessData());
}

//------------------------------------------------------------------------------
bool NDataLink::ProcessData(){
	bool result = false;
    NMESSAGE msg;
    
    if(InData->Valid){
        // zera o contador de re-envio
        retries_counter = 0;
        // p�ra a temporizacao de timeout
        flagTimingOut = false;
        // invalida a contagem de desconexao
        if(off_threshold>0){ unanswered = off_threshold;}
        
        // segue o jogo
        if(tmpInData != NULL){ tmpInData->CopyFrom(InData);}
        
        // checa se e criptografado
        if(EncriptionEnabled || AutoDecriptionEnabled){
            if(tmpInData->IsEncripted()){
                flagEncripted = true; tmpInData->Decript();
            }
        }
        
        // registra ultimo endereco remetente valido
        last_source_address = tmpInData->Source;
          
        // notifica recebimeno "irrestrito"
        if(OnAnyDatagram != NULL){ OnAnyDatagram(tmpInData);}

        if(!AssistedReception){
            // se nao houver um interpretador associado, chama evento
            if(AddressesEnabled){
                if((local_addr>PROSA_ADDR_NULL)&&(tmpInData->Destination==local_addr)){
                    if(OnUnicastReceived != NULL){ OnUnicastReceived(tmpInData);}
                } else if((service_addr>PROSA_ADDR_NULL)&&(tmpInData->Destination==service_addr)){
                    if(OnServiceReceived != NULL){ OnServiceReceived(tmpInData);}
                } else if((broadcast_addr>PROSA_ADDR_NULL)&&(tmpInData->Destination==broadcast_addr)){
                    if(OnBroadcastReceived != NULL){ OnBroadcastReceived(tmpInData);}
                } else if((RoutingEnabled)&&(OnRoutingReceived != NULL)){
                    OnRoutingReceived(tmpInData);
                }
            } else { if(OnNewDatagram != NULL){ OnNewDatagram(tmpInData);}}
        } else {
            if(AddressesEnabled){
                // se houver um interpretador associado, despacha mensagem
                // ATENCAO: Habilitar enderecamento (AddressesEnabled = true)
                if((local_addr>PROSA_ADDR_NULL)&&(tmpInData->Destination==local_addr)){
                    msg.message = NM_UNICAST_PROCESS;
                } else if((service_addr>PROSA_ADDR_NULL)&&(tmpInData->Destination==service_addr)){
                    msg.message = NM_SERVICE_PROCESS;
                } else if((broadcast_addr>PROSA_ADDR_NULL)&&(tmpInData->Destination==broadcast_addr)){
                    msg.message = NM_BROADCAST_PROCESS;
                } else if(RoutingEnabled){ msg.message = NM_ROUTING_PROCESS;}
            }
            // finaliza a preparacao da mensagem para notificacao
            msg.data1 = (unsigned int)Handle;
            if(Protocol != NULL){ Protocol->Notify(&msg);}
        }
        result = true;
    } else { if(OnInvalidPacket != NULL){ OnInvalidPacket();}}
    
    return(result);
}

//------------------------------------------------------------------------------
// despacha o datagrama "mais antigo" da fila de envio (OutFifo) pela serial
bool NDataLink::DispatchData(){
	bool result = false;
    // checa se ha re-envio pendente
    if(retries_counter>0){
        retries_counter--;
        if(OnPacketToSend != NULL){ OnPacketToSend(OutBuffer, OutSize);}
        result = true;
    } else {
        // checa se ha novo datagrama assincrono pendente
        if(OutFifo->Counter()>0){
            if(OutFifo->Get(OutData)){
                if(EncriptionEnabled){OutData->Encript(SSR_GetKSCode());}
                OutSize = OutData->Export(OutBuffer);
                if(OnPacketToSend != NULL){ OnPacketToSend(OutBuffer, OutSize);}
				// Retries do datagrama prevalece sobre Retries do DataLink
				if(OutData->Retries > 0){
					retries_counter = OutData->Retries; OutData->Retries = 0;
				} else if(packet_retries > 0){ retries_counter = packet_retries;}
                result = true;
            }
        // checa se ha novo datagrama periodico pendente
        } else if(flagReloaded){
            flagReloaded = false;
            if(EncriptionEnabled){periodicData->Encript(SSR_GetKSCode());}
            OutSize = periodicData->Export(OutBuffer);
            if(OnPacketToSend != NULL){ OnPacketToSend(OutBuffer, OutSize);}
            result = true;
        }
    }
	return(result);
}

//------------------------------------------------------------------------------
// escreve um datagrama direto na USART (N�O UTILIZAR!)
bool NDataLink::Write(NDatagram* ExtData){
	OutSize = ExtData->Export(OutBuffer);
    if(OnPacketToSend != NULL){ OnPacketToSend(OutBuffer, OutSize);}
	return(true);
}

//------------------------------------------------------------------------------
// insere um datagrama na fila de envio (OutFifo)
bool NDataLink::Put(NDatagram* ExtData){
	bool result = false;
    if(ExtData->Valid){
        if(privilege == dlMaster){
            //checa se ha vaga na fifo
            result = OutFifo->Put(ExtData);
        } else if(privilege == dlSlave){
		  	if((EncriptionEnabled)||(AutoDecriptionEnabled && flagEncripted)){
				ExtData->Encript(SSR_GetKSCode());
			}
			OutSize = ExtData->Export(OutBuffer);
            if(OnPacketToSend != NULL){ OnPacketToSend(OutBuffer, OutSize);}
			result = true;
        }
    }
	return(result);
}

//------------------------------------------------------------------------------
// retira o datagrama "mais antigo" da fila de recepcao (InFifo)
bool NDataLink::Get(NDatagram* ExtData){
	bool result = false;
    // tmpInData recebe o dado "recem-montado" por BuildDatagram
    tmpInData->CopyTo(ExtData); // InData pode ja estar "sobrescrito"
	return(result);
}

//------------------------------------------------------------------------------
// limpa mensagens pendentes na filas de entrada, saida ou ambas
bool NDataLink::Flush(dlFlushables nFlushable){
    bool result = false;
    switch(nFlushable){
        case dlInFifo: result = true; break;
        case dlOutFifo: OutFifo->Flush(); result = true; break;
        case dlBothFifos:
		default:
			OutFifo->Flush(); result = true; break;
    }
    return(result);
}

//------------------------------------------------------------------------------
// forcaa o reenvio do ultimo datagrama
void NDataLink::ForceRetry(){
	// restaura o datagrama 
	OutSize = OutData->Export(OutBuffer);
	// incrementa o contador de re-envio
	retries_counter = 1;
	// libera a temporizacao de timeout
	flagRequestTiming = true; 
    timeout_counter = timeout_packet;
}

//------------------------------------------------------------------------------
void NDataLink::Notify(NMESSAGE* pmsg){

	NComponent::Notify(pmsg);

	switch(pmsg->message){

        case NM_TIMETICK:
			pmsg->message = NM_NULL;
			if(privilege == dlMaster){
				//-----------------------------------------
				// controla o temporizador de "OnReload()" do modo MASTER
				if(time_reload > 0){
					if(reload_counter>0){ reload_counter--;}
					else {
						reload_counter = time_reload;

						//--------------------------------
						// checa se tem pacote na fila de saida
						if(OutFifo->Counter() < lower_threshold){
							if((OnReload != NULL)&& !flagReloaded){
								OnReload(periodicData);
								if(periodicData->Valid){ flagReloaded = true;}
							}
						}
					}
				}
                
                //-----------------------------------------
                // controla o temporizador de "Dispatch()" do modo MASTER
                if(dispatch_counter > 0){ dispatch_counter--;}
                else {
                    dispatch_counter = time_dispatch;
                    flagRequestTiming = DispatchData();
                    if(flagRequestTiming){
                    	timeout_counter = timeout_packet;
                    	flagTimingOut = true; 	// <<<<<<<<<<<  TESTE DO DIA 16/05/2023
                    }
                }
			}

			//----------------------------------------
			// controla o temporizador de "resposta" (Timeout)
            if(flagTimingOut){
                if(timeout_counter==0){
					flagTimingOut = false;
                    // checa se deve evitar OnSilence
                    if(retries_counter == 0){
                        if(AssistedTransmission){
                            pmsg->message = NM_SERIAL_ONSLAVESILENCE;
                            pmsg->data1 = (unsigned int)Handle;
                        } else {
                            if(OnSilence != NULL){ OnSilence();}
                        }
						// checa a condicao de desconexao
						if(off_threshold>0){
							if(unanswered>0){ unanswered--;}
							else {
								Close();
								if(OnDisconnected != NULL){ OnDisconnected();}
							}
						}
                    }
                } else {timeout_counter--;}
            }
			//----------------------------------------
            break;

		default:
			pmsg->message = NM_NULL;
			break;
	}
}

//==============================================================================
