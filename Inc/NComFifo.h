//==============================================================================
#ifndef NComFifo_H
    #define NComFifo_H

    #include "NFifo.h"
    #include "NDatagram.h"

//------------------------------------------------------------------------------
namespace ncomfifo_space{

    //-----------------------------------------------------
    class NComFifo : public NFifo{

        private:
            //--------------------------------------------
            PARAMS params;
            uint8_t* local_buffer;
            uint16_t local_buffer_size;
            //uint8_t HeaderSize;

        public:

            //--------------------------------------------
            // M�TODOS do componente NDatagram
            NComFifo(void);
            NComFifo(uint32_t, uint32_t);
            ~NComFifo(void);
            bool Put(NDatagram*);
            bool Get(NDatagram*);
            uint32_t Counter();
            void GetControl(PARAMS*);
            void Flush();

    };
    //-----------------------------------------------------
}
    using namespace ncomfifo_space;
#endif
//==============================================================================

