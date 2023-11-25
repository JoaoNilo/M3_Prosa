//==============================================================================
#ifndef NCOMMAND_H
    #define NCOMMAND_H

    #include "GenericTypeDefs.h"
namespace ncommand_space{

    //-----------------------------------
    // definicao da classe NCommand
    class NCommand{

        private:
        protected:

        public:
            HANDLE  Handle;
            unsigned int Tag;
            union {
                unsigned int ID32;
                unsigned short ID16;
                unsigned char ID;
                unsigned char IDH;
            };

        public:
            NCommand();
            ~NCommand();
            virtual void Notify(NMESSAGE*);
            virtual void InterruptCallBack(NMESSAGE*);
    };
};
    using namespace ncommand_space;
#endif
//==============================================================================
