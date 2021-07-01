#include "COM.h"

CommandType R_Cmd,T_Cmd;

byte MY_ADDRESS_P0[] = {0x78,0x78,0x78,0x78,0x78};   
byte MY_ADDRESS_P1[] = {0xF1,0xB3,0xB4,0xB5,0xB6};     
byte MY_ADDRESS_P2[] = {0xCD}; //  fULL: 0xCD,0xB3,0xB4,0xB5,0xB6
byte MY_ADDRESS_P3[] = {0xA3};
byte MY_ADDRESS_P4[] = {0x0F};
byte MY_ADDRESS_P5[] = {0x05};

byte Devices_Last_Index[DeviceTypeNum] = {0,0,0,0,0,0,0};

void NRF_Init(void){
	  Mirf.cePin = NRF_CE_PIN;     //Defined in Device_Cfg.h
    Mirf.csnPin = NRF_CSN_PIN;   //Defined in Device_Cfg.h
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();
    Mirf.configRegister(RF_SETUP,0x26);//250kbps,0dB
    Mirf.writeRegister(RX_ADDR_P0,MY_ADDRESS_P0,5);
    Mirf.setRADDR((byte *)MY_ADDRESS_P1);//P1 set
    Mirf.writeRegister(RX_ADDR_P2,MY_ADDRESS_P2,1);
    Mirf.writeRegister(RX_ADDR_P3,MY_ADDRESS_P3,1);
    Mirf.writeRegister(RX_ADDR_P4,MY_ADDRESS_P4,1);
    Mirf.writeRegister(RX_ADDR_P5,MY_ADDRESS_P5,1);
    Mirf.configRegister(EN_RXADDR,0x3F);
    Mirf.configRegister(RX_PW_P2, sizeof(CommandType));
    Mirf.configRegister(RX_PW_P3, sizeof(CommandType));
    Mirf.configRegister(RX_PW_P4, sizeof(CommandType));
    Mirf.configRegister(RX_PW_P5, sizeof(CommandType));
    Mirf.payload = sizeof(CommandType);
    Mirf.channel = NRF_CHANNEL;
    Mirf.config();
    
    /*Commands Init*/
    R_Cmd.ThisDeviceId = DEFAULT_DEVICEID;
    R_Cmd.Command = DEFAULT_COMMAND;
    R_Cmd.Operation_Index = DEFAULT_INDEX;
    T_Cmd.ThisDeviceId = DEFAULT_DEVICEID;
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
