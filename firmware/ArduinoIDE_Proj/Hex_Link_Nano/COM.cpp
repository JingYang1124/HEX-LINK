#include "Universal_Headers.h"
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#include "COM.h"

CommandType R_Cmd,T_Cmd;

byte TARGET_ADDRESS[6][5] = {{0x78,0x78,0x78,0x78,0x78},  
{0xF1,0xB3,0xB4,0xB5,0xB6},     
{0xCD,0xB3,0xB4,0xB5,0xB6}, 
{0xA3,0xB3,0xB4,0xB5,0xB6},
{0x0F,0xB3,0xB4,0xB5,0xB6},
{0x05,0xB3,0xB4,0xB5,0xB6}};


void NRF_Init(void){
    
  
	  Mirf.cePin = NRF_CE_PIN;     //Defined in Device_Cfg.h
    Mirf.csnPin = NRF_CSN_PIN;   //Defined in Device_Cfg.h
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();   
    Mirf.configRegister(RF_SETUP,0x26);//250kbps,0dB
    Mirf.setTADDR(TARGET_ADDRESS[DeviceId]);
    Mirf.payload = sizeof(CommandType);
    Mirf.channel = NRF_CHANNEL;
    Mirf.config();
    
    /*Commands Init*/
    R_Cmd.ThisDeviceId = DeviceId;
    R_Cmd.Command = DEFAULT_COMMAND;
    R_Cmd.Operation_Index = DEFAULT_INDEX;
    T_Cmd.ThisDeviceId = DeviceId;
    T_Cmd.Command = DEFAULT_COMMAND;
    T_Cmd.Operation_Index = DEFAULT_INDEX;
}


void NRF_Read(void)
{
    byte data[Mirf.payload];
    if(Mirf.dataReady())    //Wait until data is ready
    {
        Mirf.getData(data);    //Obtain the data
        R_Cmd.ThisDeviceId = data[0];
        R_Cmd.Command = data[1];
        R_Cmd.Operation_Index = data[2];
    }
}

void NRF_Send(CommandType * Tdata)
{
    Mirf.send((byte *)Tdata);
    while(Mirf.isSending()) {}//Wait until sending is finished
}
