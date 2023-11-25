//==============================================================================
#ifndef NSERIALCOMMAND_H
    #define NSERIALCOMMAND_H
    
    #include "NComponent.h"
    #include "NDatagram.h"
    #include "NSerialProtocolDefs.h"
    
    

namespace nserialcommand_space{

    //-------------------------------------------------
    // definicao da classe NSerialCommand
    class NSerialCommand:public NCommand {

        //-------------------------------------------------
        enum fsmCommandStates { fsmCommandIdle, fsmPendingProcess, fsmPendingDataProcess,
                            fsmPendingPostProcess, fsmPendingDataPostProcess,
                            fsmPendingResponse, fsmCommandAcomplished};
        
        private:
            fsmCommandStates status;

            //---------------------------------------
            // funcoes de manipulacao das propriedades
            void SetIdentifier(uint8_t);
            uint8_t GetIdentifier();

        public:

            //-------------------------------------------
            // METODOS do componente NSerialCommand             < APLICABILIDADE >
            NSerialCommand(NComponent*);
            ~NSerialCommand();
            virtual void Notify(NMESSAGE*);

            //-------------------------------------------
            // EVENTOS do componente NSerialCommand             < OCORRENCIA >
            void (*OnProcess)(NDatagram*);                      // processamento anterior a resposta
            void (*OnPostProcess)();                            // processamento posterior a resposta

            //-------------------------------------------
            // PROPRIEDADES do componente NSerialCommand        <DEFAULT VALUES>
            property<NSerialCommand, uint8_t, propReadWrite> Identifier;   // 0x00
            bool Enabled;                                               // true
            bool Responsive;                                            // true

    };
    //-------------------------------------------------
};
    using namespace nserialcommand_space;
#endif
//==============================================================================
