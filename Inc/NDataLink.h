//==============================================================================
#ifndef NDATALINK_H
    #define NDATALINK_H

    #include "NComponent.h"
    #include "NComFifo.h"

namespace ndatalink_space {

    //-----------------------------------
    // tipos especiais para uso nas propriedades
    enum dlBusPrivileges { dlMaster, dlSlave, dlMonitor};
    enum dlFlushables { dlInFifo, dlOutFifo, dlBothFifos};
    
    //-----------------------------------
    // defini��o da classe NDataLink
    class NDataLink: private NComponent{
		#define KSC (uint8_t)SysTick->VAL
        public:
            static const uint32_t FIFO_UNDERFLOW          	= 1;
            static const uint32_t FIFO_OVERFLOW           	= 3;
            
        private:
            //-------------------------
            // vari�veis das propriedades
			//unsigned int 	MaxPayloadLength;			// tamanho maximo do campo Payload
			NComFifo*		OutFifo;					// fila de mensagens a enviar
			NDatagram* 		InData;						// montagem do datagrama recebido
			NDatagram* 		OutData;					// montagem do datagrama a ser enviado
            NDatagram* 		tmpInData;					// usado pelo "controlador de protocolo"
            NDatagram* 		periodicData;				// usado pelo "controlador de protocolo"
			uint8_t*		OutBuffer;					// buffer para o pacote a ser enviado
			uint32_t        OutSize;					// comprimento do pacote a ser enviado

			uint32_t		outbuffer_size;				// dimensao de OutBuffer
			uint16_t		lower_threshold;			// limite inferior de preenchimento da fifo
			uint16_t		upper_threshold;			// limite superior de preenchimento da fifo

            //-------------------------
            // vari�veis de configura��o (internas)
            uint8_t 		local_addr;				    // endereco "local"
            uint8_t			service_addr;				// endereco de "manutencao"
            uint8_t			broadcast_addr;			    // endereco "publico" (apenas ouve)
			dlBusPrivileges privilege;				    // privilegio de acesso ao barramento
			uint32_t		time_dispatch;				// tempo entre chamadas a DispatchData()
            uint32_t		time_reload;				// tempo entre chamadas a Reload()
			uint32_t		unanswered;					// contador de pacotes "nao respondidos"
			uint32_t		off_threshold;				// limiar de "desconexao"

            //-------------------------
            // variaveis funcionais
			uint16_t header_size;						// numero de bytes no "cabecalho" do pacote
            uint16_t bytesToRead;
            NComponent* Protocol;
			bool flagEncripted;							// datagrama recebido estava encriptado
            uint8_t last_source_address;          		// ultimo endereco fonte quando em modo "slave"
			
			//-------------------------
            // controle do time-frame
            bool flagReloaded;                          // indica a recarga do comando periodico
            bool flagTimingOut;                         // contagem do timeout em andamento
            bool flagRequestTiming;
            uint32_t dispatch_counter;              	// contador do timeout de DispatchData();
            uint32_t reload_counter;                	// contador do timeout de Reload()

			//-------------------------
            // controle do timeout por "silencio"
            int32_t timeout_packet;						// tempo "programado" por SetTimeout
            int32_t timeout_counter;                    // contador do timeout de montagem
            uint32_t packet_retries;                    // numero de tentativas de re-envio
            int32_t retries_counter;                    // contador de tentativas de re-envio

		//-------------------------------------------
        // fun��es auxiliares
        private:
			bool DispatchData();
            bool ProcessData();

            //---------------------------------------
            // fun��es de manipula��o das propriedades
            void SetLocalAddress(uint8_t);
            uint8_t GetLocalAddress();
            void SetServiceAddress(uint8_t);
            uint8_t GetServiceAddress();
            void SetBroadcastAddress(uint8_t);
            uint8_t GetBroadcastAddress();
            void SetBusPrivilege(dlBusPrivileges);
            dlBusPrivileges GetBusPrivilege();
            void SetTimeDispatch(uint32_t);
            uint32_t GetTimeDispatch();
            void SetTimeReload(uint32_t);
            uint32_t GetTimeReload();
            void SetTimeout(uint32_t);
            uint32_t GetTimeout();
            void SetRetries(uint32_t);
            uint32_t GetRetries();
            void SetOffThreshold(uint32_t);
            uint32_t GetOffThreshold();

        //-------------------------------------------
        public:
            NDataLink();
            ~NDataLink(void);
			void Open();
			void Close();

        //------------------------------------------------
        // M�TODOS do componente NDataLink
        public:
            void BindReceptionAssistent(NComponent*);
			bool Write(NDatagram*);
            bool Put(NDatagram*);
            bool Get(NDatagram*);
            bool Flush(dlFlushables);
			void ForceRetry();
            
   			//bool BuildDatagram(unsigned char*, unsigned char);
            bool ProcessPacket(unsigned char*, unsigned char);
            bool ProcessDatagram(NDatagram*);
        
        //------------------------------------------------
        // EVENTOS do componente NDataLink
        public:
            void (*OnPacketToSend)(uint8_t*, uint8_t);	// requisita envio de dados
			void (*OnPacketSent)(void);					// datagrama enviado
			void (*OnReload)(NDatagram*);				// recarrega de datagrama peri�dico (em Master)
			void (*OnSilence)();						// falta de resposta (apenas em MASTER)
            void (*OnBrokenPacket)(uint8_t*, uint8_t); 	// datagrama recebido parcialmente
			void (*OnInvalidPacket)();			    	// datagrama recebido est� corrompido
            void (*OnUnicastReceived)(NDatagram*);		// datagrama recebido no endere�o de rede
            void (*OnServiceReceived)(NDatagram*);		// datagrama recebido no endere�o de servi�o
            void (*OnBroadcastReceived)(NDatagram*);		// datagrama recebido no endere�o de difus�o
            void (*OnRoutingReceived)(NDatagram*);        // datagrama recebido num endere�o desconhecido
			
            void (*OnAnyDatagram)(NDatagram*);		// datagrama recebido (todos)
            void (*OnNewDatagram)(NDatagram*);		// datagrama recebido (com filtro de endere�amento desativado)
            void (*OnDispatch)(NDatagram*);        	// datagrama est� prestes a ser enviado
			void (*OnDisconnected)();				// desconex�o por falta de resposta (apenas em MASTER)

			//-------------------------------------------
			// PROPRIEDADES do componente NDataLink                         < DEFAULT VALUES >
			bool AssistedReception;                                         	// false : indica se ha um "empacotador" presente
			bool AssistedTransmission;                                      	// false : indica se ha um "despachante" presente
            bool AddressesEnabled;                                          	// true: (AssistedReception deve ser "false")
            bool RoutingEnabled;                                            	// false
			bool EncriptionEnabled;                                         	// false : se true, dados "enviados" devem ser criptografados
			bool AutoDecriptionEnabled;											// false : se true, os datagramas recebidos criptografados s�o,
																				// automaticamente, decriptografados e ,em modo Slave, a
																				// resposta � automaticamente criptografada.

			property<NDataLink, uint8_t, propReadWrite> LocalAddress;    		// 00h : endereco especifico
			property<NDataLink, uint8_t, propReadWrite> ServiceAddress;  		// FEh : endereco de servico (manutencao)
			property<NDataLink, uint8_t, propReadWrite> BroadcastAddress;		// 01h : endereco publico (difusao)
			property<NDataLink, dlBusPrivileges, propReadWrite> BusPrivilege;	// dlSlave : privilegio de acesso (master ou slave)
			property<NDataLink, uint32_t, propReadWrite> TimeDispatch;			// 20 (ms) : minimo intervalo entre envio
            property<NDataLink, uint32_t, propReadWrite> TimeReload;			// 60 (ms): intervalo entre envio de comandos periodicos
			property<NDataLink, uint32_t, propReadWrite> Timeout;				// 15 (ms) : janela de tempo a para resposta (modo dlMaster)
			property<NDataLink, uint32_t, propReadWrite> Retries;	            // 0 : numero de re-envios em caso de "n�o resposta"
			property<NDataLink, uint32_t, propReadWrite> OffThreshold;     		// 0 : tolerancia a "nao resposta" antes de desconectar
			
		//-------------------------------------------
        // recebe mensagens da fila do sistema
        public:
            virtual void Notify(NMESSAGE*);
    };
    //------------------------------------
}
    using  namespace ndatalink_space;
#endif
//==============================================================================
