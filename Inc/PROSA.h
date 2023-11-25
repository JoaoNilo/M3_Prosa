//==============================================================================
#ifndef PROSA_H
    #define PROSA_H

//------------------------------------------------------------------------------
// ALERTA: DEFINE TAMANHO DOS BUFFERS DE MENSAGEM (ADEQUAR � APLICA��O)
#define PROSA_PAYLOAD_CAPACITY           ((uint8_t) 32)

//------------------------------------------------------------------------------
#define PROSA_DEST_OFFSET                ((uint8_t) 0x00)
#define PROSA_SRCE_OFFSET                ((uint8_t) 0x01)
#define PROSA_LENG_OFFSET                ((uint8_t) 0x02)
#define PROSA_COMM_OFFSET                ((uint8_t) 0x03)
#define PROSA_PAYL_OFFSET                ((uint8_t) 0x04)
#define PROSA_SIZE_HEADER                ((uint8_t) 4)
#define PROSA_SIZE_COMM                  ((uint8_t) 1)
#define PROSA_SIZE_CRC                   ((uint8_t) 2)

//------------------------------------------------------------------------------
// tabela de endere�amento
//------------------------------------------------------------------------------
#define PROSA_ADDR_NULL              0x00    //
#define PROSA_ADDR_DEVICE1           0x10    //
#define PROSA_ADDR_DEVICE2           0x11    //
#define PROSA_ADDR_DEVICE3           0x12    //
#define PROSA_ADDR_DEVICE4           0x13    //
#define PROSA_ADDR_DEVICE5           0x14    //
#define PROSA_ADDR_DEVICE6           0x15    //
#define PROSA_ADDR_DEVICE7           0x16    //
#define PROSA_ADDR_DEVICE8           0x17    //
#define PROSA_ADDR_IHM1              0x61    //
#define PROSA_ADDR_IHM2              0x62    //
#define PROSA_ADDR_MOLESA            0x90    //
#define PROSA_ADDR_SERVICE           0xFE    //
#define PROSA_ADDR_BROADCAST         0xFF    //

//------------------------------------------------------------------------------
// constantes utilizadas no PROTOCOLO PROSA
#define PROSA_OK                    'O'
#define PROSA_FAILED                'F'

//------------------------------------------------------------------------------
// PROSA protocol - basic command set
//------------------------------------------------------------------------------
#define PROSA_CMD_SECURE_DATA  	    0x00    //
#define PROSA_CMD_VERSION      	    0x01    //
#define PROSA_CMD_ECHO              0x02    //
#define PROSA_CMD_PING    	        0x03    //

//------------------------------------------------------------------------------
// PROSA protocol - extended basic command set
//------------------------------------------------------------------------------
#define	PROSA_CMD_GETSTATUS			0x50	//
#define	PROSA_CMD_SETDATA			0x52	//
#define	PROSA_CMD_SETSERVO			0x54	//

//------------------------------------------------------------------------------


#define PROSA_HEADER_TIMEOUT     	((uint32_t)3) 			// 5  //3
#define PROSA_TIMEOUT     			((uint32_t)20) 			// 20 //16
#define PROSA_TIMEOUT_MIN  			((uint32_t)10) 			// 10 //8

#define PROSA_TIMEDISPATCH_MIN		PROSA_TIMEOUT
#define PROSA_TIMEDISPATCH		    ((uint32_t)50) 			// 25
#define PROSA_TIMEDISPATCH_MAX		((uint32_t)200)

#define PROSA_TIMERELOAD_MIN		PROSA_TIMEDISPATCH_MIN * 2
#define PROSA_TIMERELOAD		    PROSA_TIMEDISPATCH * 2
#define PROSA_TIMERELOAD_MAX		((uint32_t)1000)

#define PROSA_HEADER_SIZE 			PROSA_SIZE_HEADER
#define PROSA_RETRIES_MAX			((uint32_t)10)


#endif
//==============================================================================
