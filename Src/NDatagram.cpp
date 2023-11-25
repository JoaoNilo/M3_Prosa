//============================================================================//
#include "NDatagram.h"

static const uint16_t CRC16_Table[256] = {
 0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
 0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
 0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
 0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
 0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
 0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
 0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
 0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
 0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
 0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
 0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
 0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
 0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
 0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
 0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
 0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
 0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
 0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
 0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
 0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
 0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
 0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
 0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
 0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
 0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
 0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
 0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
 0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
 0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
 0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
 0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
 0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

//----------------------------------------------------------------------------//
// inicializa um datagrama
NDatagram::NDatagram(){

    // tratamento da propriedade Destination
    Destination.setOwner(this);
    Destination.set(&NDatagram::SetDestination);
    Destination.get(&NDatagram::GetDestination);

    // tratamento da propriedade Source
    Source.setOwner(this);
    Source.set(&NDatagram::SetSource);
    Source.get(&NDatagram::GetSource);

    // tratamento da propriedade Length
    Length.setOwner(this);
    Length.set(&NDatagram::SetLength);
    Length.get(&NDatagram::GetLength);

    // tratamento da propriedade Command
    Command.setOwner(this);
    Command.set(&NDatagram::SetCommand);
    Command.get(&NDatagram::GetCommand);

    // tratamento da propriedade Crc
    Crc.setOwner(this);
    Crc.set(&NDatagram::SetCrc);
    Crc.get(&NDatagram::GetCrc);

    // tratamento da propriedade PayloadCapacity
    PayloadCapacity.setOwner(this);
    PayloadCapacity.set(&NDatagram::SetPayloadCapacity);
    PayloadCapacity.get(&NDatagram::GetPayloadCapacity);

    // tratamento da propriedade Size
    Size.setOwner(this);
    Size.get(&NDatagram::GetSize);

    // tratamento da propriedade Retries
    Retries.setOwner(this);
    Retries.set(&NDatagram::SetRetries);
    Retries.get(&NDatagram::GetRetries);
	
    // tratamento da propriedade Valid
    Valid.setOwner(this);
    Valid.get(&NDatagram::GetValid);

    // atribui a condi��o inicial da classe
    header_size = PROSA_SIZE_HEADER;
    payload_capacity = PROSA_PAYLOAD_CAPACITY;
    payload = new uint8_t[payload_capacity];
    size = header_size + sizeof(this->crc);
	retries = 0;
    AutoUpdateCrc = true;
    Refresh();
}

//------------------------------------------------------------------------------
//                              PROPRIEDADES
//------------------------------------------------------------------------------
// atribui novo endere�o de destino
void NDatagram::SetDestination(uint8_t ndest){
    dest_addr = ndest; if(AutoUpdateCrc){ this->crc = UpdateCrc();}
}

//------------------------------------------------------------------------------
// retorna endere�o de destino
uint8_t NDatagram::GetDestination(){ return dest_addr;}

//------------------------------------------------------------------------------
// atribui novo endere�o fonte
void NDatagram::SetSource(uint8_t nsrce){
    srce_addr = nsrce; if(AutoUpdateCrc){ this->crc = UpdateCrc();}
}

//------------------------------------------------------------------------------
// retorna endere�o fonte
uint8_t NDatagram::GetSource(){ return srce_addr;}

//------------------------------------------------------------------------------
// atribui novo comprimento de length
void NDatagram::SetLength(uint8_t nlength){
    this->len = nlength; UpdateSize();
}

//------------------------------------------------------------------------------
// retorna o comprimento do length
uint8_t NDatagram::GetLength(){ return len;}

//------------------------------------------------------------------------------
// atribui novo comando
void NDatagram::SetCommand(uint8_t ncommand){
    command = ncommand; if(AutoUpdateCrc){ this->crc = UpdateCrc();}
}

//------------------------------------------------------------------------------
// retorna o comando
uint8_t NDatagram::GetCommand(){ return command;}

//------------------------------------------------------------------------------
// atribui novo c�digo de redund�ncia c�clica
void NDatagram::SetCrc(uint16_t newcrc){ this->crc = newcrc;}

//------------------------------------------------------------------------------
// retorna o c�digo de redund�ncia c�clica
uint16_t NDatagram::GetCrc(){ return(this->crc);}

//------------------------------------------------------------------------------
// redimensiona a capacidade de "tamanho m�ximo" de payload
void NDatagram::SetPayloadCapacity(uint8_t npayloadcapacity){
    payload_capacity = npayloadcapacity;
    payload = new uint8_t[payload_capacity];
}

//------------------------------------------------------------------------------
// retorna o "tamanho m�ximo" de payload
uint8_t NDatagram::GetPayloadCapacity(){ return payload_capacity;}

//------------------------------------------------------------------------------
// atribui novo n�mero de tentativas de envio
void NDatagram::SetRetries(uint8_t new_retries){
    if(new_retries > RETRIES_MAX){ new_retries = RETRIES_MAX;}
	retries = new_retries;
}

//------------------------------------------------------------------------------
// retorna o n�mero de tentativas de envio
uint8_t NDatagram::GetRetries(){ return retries;}

//------------------------------------------------------------------------------
// retorna o "tamanho total" do datagrama (header + payload + crc)
uint16_t NDatagram::GetSize(){ return size;}

//------------------------------------------------------------------------------
// retorna o estado de valida��o do datagrama baseado na compara��o de crc
bool NDatagram::GetValid(){ return(this->Validate());}

//------------------------------------------------------------------------------
void NDatagram::Twist(){
	int sz1 = this->len; int sz2; uint8_t swp;
	for(int c=1;c<sz1;c++){*(payload+c)^=*payload;} sz2=sz1>>1; sz1--;
	for(int c=0;c<sz2;c++){
		swp=*(payload+c); *(payload+c)=*(payload+sz1-c); *(payload+sz1-c)=swp;
	}
}

//------------------------------------------------------------------------------
void NDatagram::Untwist(){
	int sz1 = this->len; int sz2; uint8_t swp;
	sz2=sz1>>1; sz1--;
	for(int c=0;c<sz2;c++){
		swp=*(payload+c); *(payload+c)=*(payload+sz1-c); *(payload+sz1-c)=swp;
	}; sz1++;
	for(int c=1;c<sz1;c++){*(payload+c)^=*payload;}
}

//------------------------------------------------------------------------------
// atualiza o tamanho do datagrama e atualiza o crc (se necess�rio)
bool NDatagram::UpdateSize(){
    size = header_size + this->len + sizeof(this->crc);
    if(AutoUpdateCrc){ this->crc = UpdateCrc();}
    //State = fsmDatagramHeader;
    return(true);
}

//------------------------------------------------------------------------------
// desloca o payload desse datagrama N vezes para a direita
void NDatagram::ShiftRight(uint8_t N){
    for(int j=0; j<N; j++){
        for(int i=len; i>0; i--){
            *(payload+i) = *(payload + (i-1));
        }
    }
}

//------------------------------------------------------------------------------
// desloca o payload desse datagrama N vezes para a esquerda
void NDatagram::ShiftLeft(uint8_t N){
    for(int j=0; j<N; j++){
        for(int i=0; i<len; i++){
            *(payload + i) = *(payload+(i+1));
        }
    }
}

//------------------------------------------------------------------------------
//                              M�TODOS
//------------------------------------------------------------------------------
// zera o datagrama
void NDatagram::Refresh(){
    dest_addr = 0; srce_addr = 0;
    len = 0; command = 0; crc = 0;
    size = header_size + sizeof(this->crc);// State = fsmDatagramHeader;
}

//------------------------------------------------------------------------------
// esvazia o payload do datagrama
void NDatagram::Flush(){
    len = 0;
    size = header_size + sizeof(this->crc);// State = fsmDatagramHeader;
    if(AutoUpdateCrc){ this->crc = UpdateCrc();}
}

//------------------------------------------------------------------------------
// insere um bloco de dados externo no header desse datagrama
// retorna: n�mero de bytes "ainda aguardados" para completar esse datagrama
uint16_t NDatagram::FillHeader(uint8_t* srcData){
	uint8_t result = sizeof(this->crc);
    if(srcData[PROSA_LENG_OFFSET] > payload_capacity){ return(0);}
    uint8_t* header = &this->dest_addr;
    for(uint32_t c=0; c<header_size; c++){ *(header+c) = *(srcData+c);}
    size = header_size + result;
	this->len = srcData[PROSA_LENG_OFFSET]; result += this->len;
    if(AutoUpdateCrc){ this->crc = UpdateCrc();}
	retries = 0;
    return(result);
}

//------------------------------------------------------------------------------
// insere um bloco de dados externo no payload desse datagrama
uint16_t NDatagram::FillPayload(uint8_t* srcData, uint8_t srcSize){
    if(srcSize > payload_capacity){ srcSize = payload_capacity;}
    for(uint32_t c=0; c<srcSize; c++){ *(payload+c) = *(srcData+c);}
    this->len = srcSize;
    UpdateSize();
    return(srcSize);
}

//------------------------------------------------------------------------------
// ATEN��O: m�todos "Append" e "Extract" trabalham em modo LIFO, ou seja, o
// �ltimo par�metro inserido deve ser o primeiro a ser extra�do.
//------------------------------------------------------------------------------
// insere um "float" no payload desse datagrama
bool NDatagram::Append(float FloatVar){
    bool result = false;
    uint32_t varSize = sizeof(float);
    if((this->len + varSize) < payload_capacity){
        this->len += varSize;
        ShiftRight(varSize);
        *(uint32_t*)(payload) = *(uint32_t*)&FloatVar;
        result = UpdateSize();
    }
    return(result);
}

//------------------------------------------------------------------------------
// insere um "int" no payload desse datagrama
bool NDatagram::Append(uint32_t IntVar){
    bool result = false;
    uint32_t varSize = sizeof(uint32_t);
    if((this->len + varSize) < payload_capacity){
        this->len += varSize;
        ShiftRight(varSize);
        *(uint32_t*)(payload) = IntVar;
        result = UpdateSize();
    }
    return(result);
}

//------------------------------------------------------------------------------
// insere um "short" no payload desse datagrama
bool NDatagram::Append(uint16_t ShortVar){
    bool result = false;
    uint32_t varSize = sizeof(uint16_t);
    if((this->len + varSize) < payload_capacity){
        this->len += varSize;
        ShiftRight(varSize);
        *(uint16_t*)(payload) = ShortVar;
        result = UpdateSize();
    }
    return(result);
}

//------------------------------------------------------------------------------
// insere um "byte" no payload desse datagrama
bool NDatagram::Append(uint8_t ByteVar){
    bool result = false;
    uint32_t varSize = sizeof(uint8_t);
    if((this->len + varSize) < payload_capacity){
        this->len += varSize;
        ShiftRight(varSize);
        *(payload) = ByteVar;
        result = UpdateSize();
    }
    return(result);
}

//------------------------------------------------------------------------------
// insere um "bloco de bytes" no payload desse datagrama
bool NDatagram::Append(uint8_t* ByteBlock, uint8_t BlockSize){
    bool result = false;
    uint32_t varSize = BlockSize;
    if((this->len + varSize) <= payload_capacity){
        this->len += varSize;
        for(uint32_t i=0; i < varSize; i++){
            *(payload+i) = *(ByteBlock+i);
        }
    }
    result = UpdateSize();
    return(result);
}

//------------------------------------------------------------------------------
// ATEN��O: m�todos "Append" e "Extract" trabalham em modo LIFO, ou seja, o
// �ltimo par�metro inserido deve ser o primeiro a ser extra�do.
//------------------------------------------------------------------------------
// extrai um "float" do payload desse datagrama
bool NDatagram::Extract(float* FloatVar){
    bool result = false;
    uint32_t varSize = sizeof(float);
    if(this->len >= varSize){
        *FloatVar = *(float*)payload;
        ShiftLeft(varSize);
        this->len -= varSize;
        result = UpdateSize();
    }
    return(result);
}

//------------------------------------------------------------------------------
// extrai um "int" do payload desse datagrama
bool NDatagram::Extract(uint32_t* IntVar){
    bool result = false;
    uint32_t varSize = sizeof(uint32_t);
    if(this->len >= varSize){
        *IntVar = *(uint32_t*)payload;
        ShiftLeft(varSize);
        this->len -= varSize;
        result = UpdateSize();
    }
    return(result);
}

//------------------------------------------------------------------------------
// extrai um "short" do payload desse datagrama
bool NDatagram::Extract(uint16_t* ShortVar){
    bool result = false;
    uint32_t varSize = sizeof(uint16_t);
    if(this->len >= varSize){
        *ShortVar = *(uint16_t*)payload;
        ShiftLeft(varSize);
        this->len -= varSize;
        result = UpdateSize();
    }
    return(result);
}

//------------------------------------------------------------------------------
// extrai um "byte" do payload desse datagrama
bool NDatagram::Extract(uint8_t* ByteVar){
    bool result = false;
    uint32_t varSize = sizeof(uint8_t);
    if(this->len >= varSize){
        *ByteVar = *payload;
        ShiftLeft(varSize);
        this->len -= varSize;
        result = UpdateSize();
    }
    return(result);
}

//------------------------------------------------------------------------------
// extrai um "byte" do payload desse datagrama
// ATEN��O: assegurar-se de que haja pelo menos 1 byte no payload ANTES de
//  chamar esse m�todo, pois sempre retornar� um valor (v�lido ou n�o)
uint8_t NDatagram::Extract(){
    uint8_t result = 0x00;
    if(this->len >= 1){
        result = *payload;
        ShiftLeft(1);
        this->len -= 1;
        UpdateSize();
    }
    return(result);
}

//------------------------------------------------------------------------------
// insere um "bloco de bytes" no payload desse datagrama
bool NDatagram::Extract(uint8_t* ByteBlock, uint8_t BlockSize){
    bool result = false;
    uint32_t varSize = BlockSize;
    if(this->len >= varSize){
        for(uint32_t i=0; i < varSize; i++){
            *(ByteBlock+i) = *(payload+i);
        }
		if((this->len-varSize) > 0){ ShiftLeft(varSize);}
		this->len -= varSize;
	    result = UpdateSize();
    }
    return(result);
}

//------------------------------------------------------------------------------
// extrai o header desse datagrama num bloco de dados externo
uint16_t NDatagram::ExtractHeader(uint8_t* destHeader){
    uint8_t* header = &this->dest_addr;
    for(uint32_t c=0; c<header_size; c++){ *(destHeader+c) = *(header+c);}
    return(header_size);
}

//------------------------------------------------------------------------------
// extrai o payload desse datagrama num bloco de dados externo
uint16_t NDatagram::ExtractPayload(uint8_t* destData){
    uint16_t result = this->len;
    if(result > 0){
        for(uint32_t c=0; c<this->len; c++){ *(destData+c) = *(payload+c);}
    }
    return(result);
}

//------------------------------------------------------------------------------
// exporta esse datagrama (PST2) convertendo-o num buffer linear externo
uint16_t NDatagram::Export(uint8_t* dts){
    uint8_t i=0L, j=0L;
	uint32_t localPacket_size = 0;

    localPacket_size = header_size + this->len;
    while(i<header_size) {*(dts+i)= *((uint8_t*)(&this->dest_addr)+i); i++;};
    while(i<localPacket_size){ *(dts+i)= *(this->payload+j); i++; j++;};
    dts[i++] = this->crc;
    dts[i++] = this->crc>>8L;
    localPacket_size += sizeof(this->crc);
	return(localPacket_size);
}

//------------------------------------------------------------------------------
// importa esse datagrama (PST2) de um buffer linear externo
uint16_t NDatagram::Import(uint8_t* src){
    uint8_t i=0L, j=0L;
  	uint32_t localPacket_size = 0;
    if(src != NULL){
        localPacket_size = header_size + src[PROSA_LENG_OFFSET];
        while(i<header_size) {*((uint8_t*)(&this->dest_addr)+i) = *(src+i); i++;};
        while(i<localPacket_size){ *(this->payload+j) = *(src+i); i++; j++;};
        this->crc = ((uint16_t)src[i+1] << 8) | (uint16_t)(src[i]);
        size = localPacket_size += sizeof(this->crc);
    }
	return(size);
}

//------------------------------------------------------------------------------
// copia esse datagrama em um outro
void NDatagram::CopyTo(NDatagram* destDAT){
    destDAT->FillHeader(&this->dest_addr);
    destDAT->FillPayload(payload, this->len);
	destDAT->Retries = this->retries;
}

//------------------------------------------------------------------------------
// copia um datagrama externo nesse datagrama
void NDatagram::CopyFrom(NDatagram* srcDAT){
    size = srcDAT->ExtractHeader(&this->dest_addr);
    size += srcDAT->ExtractPayload(payload);
    size += sizeof(this->crc);
    this->crc = srcDAT->Crc;
	this->retries = srcDAT->Retries;
}

//------------------------------------------------------------------------------
// compara esse datagrama com um datagrama externo "trgDAT"
bool NDatagram::Compare(NDatagram* trgDAT){
    bool result = false;
    if(this->size == trgDAT->Size){
        if(this->Length == trgDAT->Length){
            this->UpdateCrc(); trgDAT->UpdateCrc();
            if(this->Crc == trgDAT->Crc){ result = true;}
        }
    }
    return(result);
}

//------------------------------------------------------------------------------
// prepara o "header de resposta" baseado no datagrama "recebido"
void NDatagram::ExchangeHeader(NDatagram* inDAT){
    this->dest_addr = inDAT->srce_addr;
    this->srce_addr = inDAT->dest_addr;
    this->command = inDAT->command;
    this->len = 0; UpdateSize();
}

//------------------------------------------------------------------------------
// inverte os endere�os, criando um "header de resposta"
void NDatagram::SwapAddresses(){
    uint8_t temp_dest = this->dest_addr;
    this->dest_addr = this->srce_addr;
    this->srce_addr = temp_dest; //State = fsmDatagramHeader;
    if(AutoUpdateCrc){ this->crc = UpdateCrc();}
}

//------------------------------------------------------------------------------
// criptografa o datagrama (KSC: Kripto Security Code)
uint8_t NDatagram::Encript(uint8_t KSC){
	Append(this->command); Append(KSC);
	this->command = PROSA_CMD_SECURE_DATA;
	Twist(); this->crc = UpdateCrc();
	return(this->Length);
}

//------------------------------------------------------------------------------
// decriptografa o datagrama
uint8_t NDatagram::Decript(){
  	uint8_t ksc;
  	Untwist();
	Extract(&ksc); Extract(&this->command);
	this->crc = UpdateCrc();
  	return(this->Length);
}

//--------------------------------------------------------------------------
// checa se o datagrama recebido est� criptografado
bool NDatagram::IsEncripted() {
	bool result = false;
	if(Validate()){ if(this->command==PROSA_CMD_SECURE_DATA){ result = true;}}
	return(result);
}

//------------------------------------------------------------------------------
// decriptografa o datagrama
uint8_t NDatagram::Decription(){
  	uint8_t ksc;
  	Untwist();
	Extract(&ksc); Extract(&this->command);
	this->crc = UpdateCrc();
  	return(ksc);
}

/*
//------------------------------------------------------------------------------
// cyclic redundancy check (16 bits)
// CCITT V.41 - polynomial 1021 - seed 0xFFFF
uint16_t NDatagram::UpdateCrc(){
	uint8_t bit;
	uint16_t c, length;
    uint16_t bit_mask, xor_mask;
    uint8_t* ptOffset;
    uint16_t work_crc = CCITT_SEED;

    length = header_size + this->len;

    for(c=0;c<length;c++){
		bit_mask = 0x80;
		//----------------------------------------------------------------------
		if(c<header_size) ptOffset = &this->dest_addr+c;
        else ptOffset = this->payload+(c-header_size);
		//----------------------------------------------------------------------
		for (bit=0; bit<8; bit++){
            if(work_crc & 0x8000) xor_mask=1;
			else xor_mask=0;
            work_crc <<= 1;
            //------------------------------------------------------------------
            if(*ptOffset & bit_mask) work_crc += 1;
            //------------------------------------------------------------------
			if(xor_mask) work_crc ^= CCITT_POLY;
            bit_mask >>= 1;
        }
    }

    for (c=0; c<16; c++){
		if(work_crc & 0x8000) xor_mask=1;
        else xor_mask=0;
        work_crc <<=1;
        if(xor_mask) work_crc ^= CCITT_POLY;
	}
    return(work_crc);
}
*/

//------------------------------------------------------------------------------
// Seed for CRC calculation, Pointer to byte array to perform CRC on, Number of bytes to CRC
uint16_t NDatagram::CRC16( uint16_t crc_seed, uint8_t* c_ptr, uint32_t len){
	uint8_t *c = c_ptr;
	uint16_t crc = crc_seed;
	while(len-->0){ crc = (crc << 8) ^ CRC16_Table[((crc >> 8) ^ *c++)];}
	return (crc);
}

//------------------------------------------------------------------------------
// cyclic redundancy check (16 bits)
// CCITT V.41 - polynomial 1021 - seed 0xFFFF
uint16_t NDatagram::UpdateCrc(){
    uint16_t work_crc = 0x1D0F; //CCITT_SEED;
    work_crc = CRC16(work_crc, &this->dest_addr, header_size);
    if(this->len>0){ work_crc = CRC16(work_crc, this->payload, this->len);}

    /*uint16_t work_crc = CPU_CRC_SEED16_CCITT;
    work_crc = CPU_Crc16(&this->dest_addr, (uint16_t)header_size, CPU_CRC_POLY16, work_crc, false);
    if(this->len>0){
        work_crc = CPU_Crc16(this->payload, (uint16_t)this->len, CPU_CRC_POLY16, work_crc, false);
    }*/

    return(work_crc);
}

//------------------------------------------------------------------------------
// (Size - HeaderSize) : auxilia na recep��o e montagem do datagrama
uint16_t NDatagram::BytesRemaining(){
    return(this->len + sizeof(this->crc));
}

//------------------------------------------------------------------------------
// checa a integridade do datagrama
bool NDatagram::Validate(){
    if(this->crc == UpdateCrc()){ return(true);}
    else { return(false);}
}

//==============================================================================
