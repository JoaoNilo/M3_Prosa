//==============================================================================
#include "NComFifo.h"

//------------------------------------------------------------------------------
// contrutora da classe NComFifo
NComFifo::NComFifo(uint32_t MaxPacketSize, uint32_t Packets):
          NFifo(MaxPacketSize, Packets){
    //-------------------------------------
    // inicializa par�metros
    //HeaderSize = 4L;
    local_buffer_size = MaxPacketSize;
    local_buffer = new uint8_t[local_buffer_size];

    //-------------------------------------
    // atualiza par�metros da classe descendente
    NFifo::LockOnOverflow = true;
    NFifo::GetControl(&params);
}

//------------------------------------------------------------------------------
// destrutora da classe NComFifo
NComFifo::~NComFifo(void){}

//------------------------------------------------------------------------------
// retorna os par�metros de controle da NComFifo
void NComFifo::GetControl(PARAMS* fifo_struct){
    *fifo_struct = params;
    return;
}

//------------------------------------------------------------------------------
// inclui novo datagrama no buffer circular
bool NComFifo::Put(NDatagram* inDatagram){
    bool result = false;
    uint16_t packet_size = inDatagram->Export(local_buffer);
    if(local_buffer_size>=packet_size){
        result = NFifo::Put(local_buffer, packet_size);
        NFifo::GetControl(&params);
    }
    return(result);
}

//------------------------------------------------------------------------------
// extrai o datagrama mais antigo do buffer circular
bool NComFifo::Get(NDatagram* opkt){
    bool result = false;
    if(params.counter>0L){
        //-------------------------------------
        // retira dados no buffer intermedi�rio
        uint32_t packet_size = NFifo::Get(local_buffer);
        // carrega no datagrama externo
        opkt->Import(local_buffer);
        if(opkt->Valid){ result = true;}

        //-------------------------------------
        // atualiza par�metros da classe descendente
        NFifo::GetControl(&params);
    }
    return(result);
}

//------------------------------------------------------------------------------
// retorna o n�mero de pacotes na fila
uint32_t NComFifo::Counter(){
    NFifo::GetControl(&params);
    return(params.counter);
}

//------------------------------------------------------------------------------
// inclui uma nova amostra no buffer circular
void NComFifo::Flush(){
    //-------------------------------------
    // chama m�todo da classe ancestral
    NFifo::Flush();
    //-------------------------------------
    // atualiza par�metros da classe descendente
    NFifo::GetControl(&params);
}

//==============================================================================
