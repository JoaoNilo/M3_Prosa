//==============================================================================
#ifndef NSERIALPROTOCOL_H
    #define NSERIALPROTOCOL_H

    #include "NSerialCommand.h"
    #include "NDataLink.h"

//------------------------------------------------------------------------------
namespace nserialprotocol_space{


    class NSerialProtocol: public NComponent{

        private:
            NMESSAGE* msgProtocol;
            unsigned int commands_number;
            HANDLE* protCommands;
            NCommand* Command;
            NDataLink* dataLink;
            NDatagram* tmpData;

        public:
            bool Enabled;
            uint8_t maxPayload;
            uint8_t* localPayload;

        private:
            unsigned char Interpret(NMESSAGE*, NDatagram*);
            void Dismiss(NDatagram*);

			//---------------------------------------
            // funcoes de manipulacao das propriedades
            NDataLink* GetDataLink();
            void SetDataLink(NDataLink*);

        //-------------------------------------------
        // METODOS do componente NSerialProtocol
        public:
            NSerialProtocol(NDataLink*);
            ~NSerialProtocol();
            unsigned int IncludeCommand(HANDLE);
            virtual void Notify(NMESSAGE*);
            bool CheckId(NDatagram*);

        //-------------------------------------------
        // EVENTOS do componente NSerialProtocol

        //-------------------------------------------
        // PROPRIEDADES do componente NSerialProtocol    < DEFAULT VALUES >
		property<NSerialProtocol, NDataLink*, propReadWrite> DataLink; // ?
    };
}
//------------------------------------------------------------------------------
using namespace nserialprotocol_space;
#endif

//==============================================================================
