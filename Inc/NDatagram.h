//==============================================================================
#ifndef NDATAGRAM_H
    #define NDATAGRAM_H
    #include <stdint.h>
    #include "Property.h"
	#include "PROSA.h"
    //#include "PST2.h"
    #include "DRV_CPU.h"

namespace ndatagram_space{

    //--------------------------------------------------------------------------
    class NDatagram {
    public:
        static const unsigned short CCITT_POLY          = 0x1021;
        static const unsigned short CCITT_SEED          = 0xFFFF;
        static const uint8_t RETRIES_MAX       			= 10;

        private:
          
            //struct {
                uint8_t dest_addr;
                uint8_t srce_addr;
                uint8_t len;
                uint8_t command;
                uint8_t* payload;
                uint16_t  crc;
            //};
			uint8_t retries;
            uint16_t size;
            uint8_t payload_capacity;
            uint8_t header_size;
            
            
            //---------------------------------------
            // fun��es de manipula��o das propriedades
            uint8_t GetDestination();
            void SetDestination(uint8_t);
            uint8_t GetSource();
            void SetSource(uint8_t);
            uint8_t GetLength();
            void SetLength(uint8_t);
            uint8_t GetCommand();
            void SetCommand(uint8_t);
            void SetPayloadCapacity(uint8_t);
            uint8_t GetPayloadCapacity();
            uint16_t GetSize();
            bool GetValid();
            void SetCrc(uint16_t);
            uint16_t GetCrc();
            void SetRetries(uint8_t);
            uint8_t GetRetries();

            //--------------------------------------------
            // fun��es auxiliares
			void Twist();
			void Untwist();
            bool UpdateSize();
            void ShiftRight(uint8_t);
            void ShiftLeft(uint8_t);
            uint16_t CRC16(uint16_t, uint8_t*, uint32_t);
            
        public:
            //--------------------------------------------
            // M�TODOS do componente NDatagram
            NDatagram();
            void Refresh();                             // zera os campos
            void Flush();                               // esvazia o payload 
            uint16_t ExtractHeader(uint8_t*);           // copia header a um buffer externo
            uint16_t ExtractPayload(uint8_t*);          // copia o payload "    "     "
            uint16_t FillHeader(uint8_t*);              // inverso de ExtractHeader
            uint16_t FillPayload(uint8_t*, uint8_t);    // inverso de Extract Payload
            bool Append(float);                         // insere um float no payload (atualiza len)
            bool Append(uint32_t);                      // insere um int no payload (atualiza len)
            bool Append(uint16_t);                      //    "    " short  "          "
            bool Append(uint8_t);                       //    "    "  char  "          "
            bool Append(uint8_t*, uint8_t);             //    "    "  bloco de bytes   "          "
            bool Extract(float*);                       // inverso de Append(float)
            bool Extract(uint32_t*);                    // inverso de Append(uint32_t)
            bool Extract(uint16_t*);                    //   "     "    "   (uint16_t)
            bool Extract(uint8_t*);                     //   "     "    "   (uint8_t)
            uint8_t Extract();                          //   "     "    "   (uint8_t)
            bool Extract(uint8_t*, uint8_t);            //   "     "  bloco de bytes   "          "
            uint16_t Export(uint8_t*);                  // copia o datagrama inteiro en um buffer externo
            uint16_t Import(uint8_t*);                  // inverso de Export(uint8_t*)
            void CopyTo(NDatagram*);                    // copia esse datagrama em outro
            void CopyFrom(NDatagram*);                  // copia outro datagrama nesse
            bool Compare(NDatagram*);                   // compara esse datagrama com um datagram externo
            void ExchangeHeader(NDatagram*);            // copia o header de outro datagram "trocando os endere�os"
            void SwapAddresses();                       // troca os endere�os "desse datagrama"
			uint8_t Encript(uint8_t);                 	// criptografa o datagrama
			uint8_t Decript();                 			// decriptografa o datagrama
			bool IsEncripted();							// checa se est� criptografado
			uint8_t Decription();						// decriptografa retornando o c�digo ksc
            //uint8_t PayloadItem(uint32_t);
            uint16_t BytesRemaining();                  // (len + sizeof(crc)) para recep��o
            uint16_t UpdateCrc();
            bool Validate();                            // refresh crc

            //-------------------------------------------
            // EVENTOS do componente NDatagram

            //-------------------------------------------
            // PROPRIEDADES do componente NDatagram             < DEFAULT VALUES >
            bool AutoUpdateCrc;                                     	    // true
            property<NDatagram, bool, propRead> Valid;             	        // false  condi��o do datagrama (CRC)
            property<NDatagram, uint8_t, propReadWrite> Destination;   	    // 0x00   endere�o de destino
            property<NDatagram, uint8_t, propReadWrite> Source;        	    // 0x00   endere�o de origem
            property<NDatagram, uint8_t, propRead> Length;         	        // 0      tamanho do payload atual       
            property<NDatagram, uint8_t, propReadWrite> Command;       	    // 0x00   comando 
            property<NDatagram, uint16_t, propReadWrite> Crc;          	    // 0x0000 c�digo de redund�ncia c�clica
            property<NDatagram, uint8_t, propReadWrite> PayloadCapacity;    // 164    bytes "alocados" para payload
            property<NDatagram, uint16_t, propRead> Size;                   // 6      tamanho total do datagrama
			property<NDatagram, uint8_t, propReadWrite> Retries;   		    // 0      n�mero de tentativas de envio

    };
    //--------------------------------------------------------------------------
}
using namespace ndatagram_space;
#endif
//==============================================================================
