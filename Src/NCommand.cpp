//============================================================================//
#include "NCommand.h"

//------------------------------------------------------------------------------
// construtora da classe base NCommand
NCommand::NCommand(){
    Handle = (HANDLE)this; ID32 = 0;
}

//------------------------------------------------------------------------------
// destrutora da classe base NCommand
NCommand::~NCommand(){}

//------------------------------------------------------------------------------
// notifica mensagens para a classe base NCommand
void NCommand::Notify(NMESSAGE* pmsg){}

//------------------------------------------------------------------------------
// chamada pelo vetor de interrupcao "por hardware"
void NCommand::InterruptCallBack(NMESSAGE* msg){}

//==============================================================================
