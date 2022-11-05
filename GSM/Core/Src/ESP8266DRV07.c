#include "string.h"
#include "ESP8266DRV07.h"
#include "stdlib.h"
#include "main.h"
#include "stdio.h"
#include "time.h"
#include "DRV_CFG.h"
//#include "ESP8266WEB.h"
//#include "Mod_Bus.h"
//#include "MAX7219.h"

#define OK  "OK"


char     ESP8266_BUF      [ESP8266_BUF_SIZE] = {0};

char*    READ_BUFFER = 0;
uint16_t WIFI_READ_COUNT   = 0;
uint16_t READ_BUF_SIZE     = 0;
uint16_t WIFI_BUF_SIZE     = 0;
uint8_t WIFI_MODE = 0;

char     Self_IP          [17] = {0};
char     CMD_temp         [CMD_temp_SIZE] = {0};
char     CMD_temp_temp    [CMD_temp_temp_SIZE] = {0};
uint8_t  ERR              [15] = {0};

uint8_t                   DMA_TC = 0;
int8_t                    GLOBAL_RETURN = 0;
int                       RSSI_Power = 0;
char WIFI_ID[5]   =       {'\0'};
char *WIFI_WEB_ID =       WIFI_ID;

TypeDef tm;


const char AT[] =                     "AT\r\n";
const char CIFSR[] =                  "AT+CIFSR\r\n";
const char RESTORE[] =                "AT+RESTORE\r\n";
const char STA[] = 					  "AT+CWMODE_CUR=1\r\n";
const char AP[] = 					  "AT+CWMODE_CUR=2\r\n";
const char AP_STA[] = 				  "AT+CWMODE_CUR=3\r\n";
const char CIP_ON[] = 				  "AT+CIPMODE=1\r\n";
const char CIP_OFF[] = 				  "AT+CIPMODE=0\r\n";
const char MUX_ON[] = 				  "AT+CIPMUX=1\r\n";
const char MUX_OFF[] = 				  "AT+CIPMUX=0\r\n";
const char STAON[] = 				  "AT+CWDHCP_CUR=1,1\r\n";
const char APON[] = 				  "AT+CWDHCP_CUR=0,1\r\n";
const char APSTAON[] = 				  "AT+CWDHCP_CUR=2,1\r\n";
const char STAOFF[] = 				  "AT+CWDHCP_CUR=1,0\r\n";
const char APOFF[] = 				  "AT+CWDHCP_CUR=0,0\r\n";
const char APSTAOFF[] = 			  "AT+CWDHCP_CUR=2,0\r\n";
const char APNAME[] =                 "AT+CWSAP_CUR=";
const char NTP[] = 					  "AT+CIPSNTPCFG=1,2,\"0.ua.pool.ntp.org\",\"1.ua.pool.ntp.org\",\"2.ua.pool.ntp.org\"\r\n";
const char ASK_CWJAP_CUR[] = 		  "AT+CWJAP_CUR?\r\n";
const char CLOSE[] =				  "AT+CIPCLOSE=";


void ESP8266_USART_CALLBACK()
{
	ESP8266_BUF[WIFI_READ_COUNT] = LL_USART_ReceiveData8(ESP8266_USART);

   if(WIFI_READ_COUNT < ESP8266_BUF_SIZE)
    {
     WIFI_READ_COUNT++;
    }
}

void USART_DMA_ACTIVE()
{
  LL_USART_EnableIT_RXNE(USART1);
  LL_USART_EnableDMAReq_TX(USART1);
  LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_4);
}

void USART_DMA_TC_CALLBACK()
{
 if(ESP8266_DMA_ACTIVE_TC_FLAG(ESP8266_DMA))
  {
	ESP8266_DMA_CLEAR_TC_FLAG(ESP8266_DMA);
	DMA_TC = SET;
  }
}

uint8_t ESP8266_DMA_send(const char* temp,int len,const char* answer,const char* from, uint16_t count)//++++
{
	uint32_t i;

	LL_DMA_DisableChannel(ESP8266_DMA, ESP8266_DMA_CHANEL);
	LL_DMA_ConfigAddresses(ESP8266_DMA, ESP8266_DMA_CHANEL, (uint32_t)temp, LL_USART_DMA_GetRegAddr(ESP8266_USART), LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
	LL_DMA_SetDataLength(ESP8266_DMA, ESP8266_DMA_CHANEL, len);
	LL_DMA_EnableChannel(ESP8266_DMA, ESP8266_DMA_CHANEL);

	while(DMA_TC != 1){};

    for(i=0; i < count; i+=5)
     {
      HAL_Delay(5);

      if(strstr(from,answer) != 0)
       {
    	i = count;
    	GLOBAL_RETURN = 0;
       }
     }

    if(strstr(from,answer) == 0)
     {
    	GLOBAL_RETURN = 1;
     }

      DMA_TC = 0;

	  return GLOBAL_RETURN;
}

void BUF_clear(char* BUF, char DAT, uint16_t size)
{
  memset(BUF,DAT,size);
  WIFI_READ_COUNT = 0;
}


/////////////////////////////////////////////////////ESP_SYS////////////////////////////////////////////

#ifdef ENABLE
void WIFI_ENABLE()
{
	HAL_GPIO_WritePin(ESP8266_ENABLE_PORT, ESP8266_ENABLE_PIN, GPIO_PIN_SET);
	HAL_Delay(500);
}
#endif

#ifdef RESET
void WIFI_RESET_HARD(int time)
{
	uint8_t i = 0;

	HAL_GPIO_WritePin(ESP8266_RESET_PORT, ESP8266_RESET_PIN, GPIO_PIN_SET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(ESP8266_RESET_PORT, ESP8266_RESET_PIN, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(ESP8266_RESET_PORT, ESP8266_RESET_PIN, GPIO_PIN_SET);

	while(i != 1)
	 {
	  HAL_Delay(500);
	  BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);

	  if(ESP_AT() == 0)
	   {
		i = 1;
	   }
	 }
}
#endif

uint8_t ESP_AT()
{
  GLOBAL_RETURN = ESP8266_DMA_send(AT,4,OK,ESP8266_BUF,2000);

  BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);

  return GLOBAL_RETURN;
}

uint8_t ESP_WIFI_MODE_SET(uint8_t dat, uint32_t count)//++
{
	char* CMD = 0;

    switch(dat)
	 {
	  case 1:
	   CMD = (char*)AP;//memcpy(CMD_temp,AP,strlen(AP));
	  break;

	  case 2:
	   CMD = (char*)STA;//memcpy(CMD_temp,STA,strlen(STA));
	  break;

	  case 3:
	   CMD = (char*)AP_STA;//memcpy(CMD_temp,AP_STA,strlen(AP_STA));
	  break;
	 }

    GLOBAL_RETURN = ESP8266_DMA_send(CMD,strlen(CMD),OK, ESP8266_BUF,count);

    BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);

	return GLOBAL_RETURN;
}

uint8_t DHCP_ONOFF(uint8_t DHCP,uint32_t count)//++
{
	char* CMD = 0;

    switch(DHCP)
    {
     case 1:
      CMD = (char*)APON;//memcpy(CMD_temp,APON,strlen(APON));
     break;

     case 2:
      CMD = (char*)APOFF;//memcpy(CMD_temp,APOFF,strlen(APOFF));
     break;

     case 3:
      CMD = (char*)STAON;//memcpy(CMD_temp,STAON,strlen(STAON));
     break;

     case 4:
      CMD = (char*)STAOFF;//memcpy(CMD_temp,STAOFF,strlen(STAOFF));
     break;

     case 5:
      CMD = (char*)APSTAON;//memcpy(CMD_temp,APSTAON,strlen(APSTAON));
     break;

     case 6:
      CMD = (char*)APSTAOFF;//memcpy(CMD_temp,APSTAOFF,strlen(APSTAOFF));
     break;
    }

    GLOBAL_RETURN = ESP8266_DMA_send(CMD,strlen(CMD),OK,ESP8266_BUF,count);

    BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);

	return GLOBAL_RETURN;
}

uint8_t WIFI_CIP(uint8_t mode, uint32_t count)//++
{
	char* CMD = 0;

	switch(mode)
	{
	 case 1:
	  CMD = (char*)CIP_ON;
	 break;

	 case 2:
	  CMD = (char*)CIP_OFF;
	 break;
	}

	GLOBAL_RETURN = ESP8266_DMA_send(CMD,strlen(CMD),OK, ESP8266_BUF,count);

    BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);

	return GLOBAL_RETURN;
}

uint8_t ESP_MUX(uint8_t data, uint32_t count)//++
{
	char* CMD = 0;

	switch(data)
	{
	 case 1:
	  CMD = (char*)MUX_ON;//memcpy(CMD,MUX_ON,strlen(MUX_ON));
	 break;

	 case 2:
	  CMD = (char*)MUX_OFF;//memcpy(CMD,MUX_OFF,strlen(MUX_OFF));
	 break;
	}

	GLOBAL_RETURN = ESP8266_DMA_send(CMD,strlen(CMD),OK,ESP8266_BUF,count);


    BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);

	return GLOBAL_RETURN;
}

uint8_t ESP_STA_IP(const char* IP, const char* GATEWAY, const char* MASK,uint16_t timing)//++
{
	strcat(CMD_temp,"AT+CIPSTA_CUR=");
	strcat(CMD_temp,"\"");
	strcat(CMD_temp,IP);
	strcat(CMD_temp,"\",\"");
	strcat(CMD_temp,GATEWAY);
	strcat(CMD_temp,"\",\"");
	strcat(CMD_temp,MASK);
	strcat(CMD_temp,"\"");
	strcat(CMD_temp,"\r\n");

	if(ESP8266_send(CMD_temp,strlen(CMD_temp),"OK",READ_BUFFER,timing) == 0)
	 {
      GLOBAL_RETURN = 0;
     }
    else
     {
      GLOBAL_RETURN = 1;
     }

    BUF_clear(READ_BUFFER,'*',READ_BUF_SIZE);
    BUF_clear(CMD_temp,0,CMD_temp_SIZE);

    return GLOBAL_RETURN;
}

uint8_t ESP_SERVER_ON(const char* port, uint32_t count)//++
{
	char CMD[50] = {0};

	strcat(CMD,"AT+CIPSERVER=1,");
	strcat(CMD,port);
	strcat(CMD,"\r\n");

	GLOBAL_RETURN = ESP8266_DMA_send(CMD,strlen(CMD),OK,ESP8266_BUF,count);

    BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);

    return GLOBAL_RETURN;
}

uint8_t CIPCLOSE(char temp[])
{
	memcpy(CMD_temp,CLOSE,12);
	strcat(CMD_temp + strlen(CLOSE),temp);
	strcat(CMD_temp,"\r\n");

	GLOBAL_RETURN = ESP8266_DMA_send(CMD_temp,strlen(CMD_temp),OK,ESP8266_BUF,2000);

    BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);
    BUF_clear(CMD_temp,0,CMD_temp_SIZE);

	return GLOBAL_RETURN;
}

uint8_t WIFI_STA_init(uint8_t MODE, uint8_t DHCP, uint8_t CIP, uint8_t MUX, const char* SERVER_PORT, uint16_t timing)
{
	uint8_t x[5];
	memset(x,'*',5);

	WIFI_RESET_HARD(1000);
    x[0] = ESP_WIFI_MODE_SET(MODE,timing);
    x[1] = DHCP_ONOFF(DHCP,timing);
    x[2] = WIFI_CIP(CIP,timing);
    x[3] = ESP_MUX(MUX,timing);
    x[4] = ESP_SERVER_ON(SERVER_PORT,timing);

    return 0;
}

uint8_t WIFI_APSTA_init(uint8_t MODE, uint8_t DHCP, uint8_t CIP, uint8_t MUX, const char* SERVER_PORT, uint16_t timing)//++
{
    WIFI_status.WIFI_mode = ESP_WIFI_MODE_SET(MODE,timing);         //WIFI MODE
    ESP8266_HOSTNAME(Seting.HOSTNAME);                              //WIFI HOSTNAME

	while(WIFI_status.WIFI_mode == 1)
	 {
	  ERR[3] = 1;
	 }

	////////////////////////////////////////////////////////
    WIFI_status.WIFI_dhcp = DHCP_ONOFF(DHCP,timing);                //WIFI DHCP

	while(WIFI_status.WIFI_dhcp == 1)
	 {
	   ERR[4] = 1;
	 }

	////////////////////////////////////////////////////////
    WIFI_status.WIFI_cip = WIFI_CIP(CIP,timing);//WIFI

	while(WIFI_status.WIFI_cip == 1)
	 {
	   ERR[5] = 1;
	 }

	////////////////////////////////////////////////////////
    WIFI_status.WIFI_ap_name = ESP_AP_NAME(Seting.AP_NAME,Seting.AP_PASS,timing);

	while(WIFI_status.WIFI_ap_name == 1)
	 {
	   ERR[6] = 1;
	 }

	////////////////////////////////////////////////////////
    WIFI_status.WIFI_ap_ip = ESP_AP_IP("1.2.3.4",timing);

	while(WIFI_status.WIFI_ap_ip == 1)
	 {
	   ERR[7] = 1;
	 }

	////////////////////////////////////////////////////////
    WIFI_status.WIFI_mux = ESP_MUX(MUX,timing);

    while(WIFI_status.WIFI_mux == 1)
	 {
	   ERR[7] = 1;
	 }

	////////////////////////////////////////////////////////
    WIFI_status.WIFI_server = ESP_SERVER_ON(SERVER_PORT,timing);

    while(WIFI_status.WIFI_server == 1)
	 {
	   ERR[8] = 1;
	 }

	////////////////////////////////////////////////////////
    //WIFI_status.WIFI_auto_connect = WIFI_AUTOCONNECT("1");
   //WIFI_status.WIFI_START_CONNECTING = WIFI_CONNECT_CUR(Seting.Network,Seting.Password,10000);
    //delay_ms(1000);

    return 0;
}

uint8_t WIFI_AP_init(uint8_t MODE, uint8_t DHCP, uint8_t CIP, uint8_t MUX, const char* SERVER_PORT, uint16_t timing)//++
{
    WIFI_status.WIFI_mode = ESP_WIFI_MODE_SET(MODE,timing);
    ESP8266_HOSTNAME(Seting.HOSTNAME);
    WIFI_status.WIFI_dhcp = DHCP_ONOFF(DHCP,timing);
    WIFI_status.WIFI_cip = WIFI_CIP(CIP,timing);
    WIFI_status.WIFI_ap_name = ESP_AP_NAME(Seting.AP_NAME,Seting.AP_PASS,timing);
    WIFI_status.WIFI_mux = ESP_MUX(MUX,timing);
    WIFI_status.WIFI_server = ESP_SERVER_ON(SERVER_PORT,timing);

    return 0;
}

uint8_t ESP_AP_NAME(char data[],char temp[],uint32_t count)
{
	char CMD[60] = {0};

	strcat(CMD,APNAME);
	strcat(CMD,"\"");
	strcat(CMD,data);
	strcat(CMD,"\",\"");
	strcat(CMD,temp);
	strcat(CMD,"\",");
	strcat(CMD,"10,4\r\n");

	GLOBAL_RETURN = ESP8266_DMA_send(CMD,strlen(CMD),OK,ESP8266_BUF,count);

    BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);

	return GLOBAL_RETURN;
}

uint8_t ESP_AP_IP(char temp[],uint32_t count)//!!
{
	    char AP_name[] = "AT+CIPAP_CUR=";
		strcat(CMD_temp,AP_name);
		strcat(CMD_temp,"\"");
		strcat(CMD_temp,temp);
		strcat(CMD_temp,"\"");
		strcat(CMD_temp,"\r\n");

		if(ESP8266_DMA_send(CMD_temp,strlen(CMD_temp),OK,ESP8266_BUF,count)==0)
		 {
			GLOBAL_RETURN = 0;
	     }
	    else
	     {
	    	GLOBAL_RETURN = 1;
	     }

	    BUF_clear(ESP8266_BUF,'*',1024);
	    BUF_clear(CMD_temp,0,50);

		return GLOBAL_RETURN;
}

uint8_t ESP_FACTORY_RESET()
{
	uint8_t i = 0;

	GLOBAL_RETURN = ESP8266_DMA_send(RESTORE,strlen(RESTORE),OK, ESP8266_BUF,2000);

	while(i != 1)
	 {
      HAL_Delay(500);
      BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);

      if(ESP_AT() == 0)
       {
    	i = 1;
       }
	 }

	BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);

	return GLOBAL_RETURN;
}

/////////////////////////////////////////////////////END_ESP_SYS////////////////////////////////////////


/////////////////////////////////////////////////////ESP_UDP////////////////////////////////////////////

uint8_t CONNECT_TO_UDP(const char* ADDR, const char* PORT, const char* ID, const char* PORT_2, uint16_t timing)//!!
{
	strcat(CMD_temp,"AT+CIPSTART=");
	strcat(CMD_temp,ID);
	strcat(CMD_temp,",\"");
	strcat(CMD_temp,"UDP");//UDP
	strcat(CMD_temp,"\"");
	strcat(CMD_temp,",\"");
	strcat(CMD_temp,ADDR);
	strcat(CMD_temp,"\",");
	strcat(CMD_temp,PORT);
	strcat(CMD_temp,",");
	strcat(CMD_temp,PORT_2);
	strcat(CMD_temp,",");
	strcat(CMD_temp,"0");
	strcat(CMD_temp,"\r\n");

	  if(ESP8266_send(CMD_temp,strlen(CMD_temp),OK, ESP8266_BUF,timing) == 0)
       {
		  WIFI_status.WIFI_CONNECTED_TO_TCP = 1;
		  GLOBAL_RETURN = 0;
       }
      else
       {
    	  WIFI_status.WIFI_CONNECTED_TO_TCP = 0;
    	  GLOBAL_RETURN = 1;
       }

	    BUF_clear(ESP8266_BUF,'*',1024);
	    BUF_clear(CMD_temp,0,50);

	return GLOBAL_RETURN;
}

uint8_t WIFI_SEND_TO_UDP(const char* IP, const char* port, const char* dat, const char* Answer, char *Link)//!!
{
	if(WIFI_status.WIFI_CONNECTED_TO_SSID == 1)
	 {
	     char ito_temp[4] = {0};

	     BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);

	     memset(ito_temp,0,4);
	     itoa(sizeof(dat),ito_temp,10);
	     memcpy(CMD_temp,"AT+CIPSEND=",11);
	     memcpy(CMD_temp+11,Link,1);
	  	 memcpy(CMD_temp+12,",",1);
	  	 memcpy(CMD_temp+13,ito_temp,strlen(ito_temp));
	  	 strcpy(CMD_temp+15,"\r\n");

	     ESP8266_DMA_send(CMD_temp,strlen(CMD_temp),">",ESP8266_BUF,2000);
	     HAL_Delay(2);
	     BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);
	     HAL_Delay(2);
	     ESP8266_DMA_send((char*)dat,strlen(dat),"SEND OK",ESP8266_BUF,2000);
	 }

    BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);
    BUF_clear(CMD_temp,0,CMD_temp_SIZE);

     return 0;
}

uint8_t WIFI_Setting_UDP()
{
	WIFI_APSTA_init(MODE_APSTA, DHCP_APSTA_ON, WIFI_CIP_OFF, WIFI_MUX_ON, "80", 5000);

	if((WIFI_status.WIFI_mode + WIFI_status.WIFI_dhcp + WIFI_status.WIFI_cip + WIFI_status.WIFI_ap_name + WIFI_status.WIFI_ap_ip + WIFI_status.WIFI_mux + WIFI_status.WIFI_server) == 0)
	 {
		GLOBAL_RETURN = 0;
	 }
	else
	 {
		GLOBAL_RETURN = 1;
	 }

	return GLOBAL_RETURN;
}

/////////////////////////////////////////////////////END_ESP_UDP///////////////////////////////////////


/////////////////////////////////////////////////////ESP_TCP///////////////////////////////////////////

uint8_t WIFI_SEND_TO_TCP(const char* IP, const char* port, const char* dat, const char* Answer, char *Link)//!!
{
	if(WIFI_status.WIFI_CONNECTED_TO_SSID == 1)
	 {
	  CONNECT_TO_TCP(IP,port,Link,2000);

	  if(WIFI_status.WIFI_CONNECTED_TO_TCP == 1)
	   {
	     char ito_temp[4] = {0};

	     BUF_clear(ESP8266_BUF,'*',1024);

	     memset(ito_temp,0,4);
	     itoa(strlen(dat),ito_temp,10);
	     memcpy(CMD_temp,"AT+CIPSEND=",11);
	     memcpy(CMD_temp+11,Link,1);
	  	 memcpy(CMD_temp+12,",",1);
	  	 memcpy(CMD_temp+13,ito_temp,strlen(ito_temp));
	  	 strcpy(CMD_temp+14,"\r\n");

	     ESP8266_DMA_send(CMD_temp,strlen(CMD_temp),">",ESP8266_BUF,2000);

	     BUF_clear(ESP8266_BUF,'*',1024);

	     ESP8266_DMA_send((char*)dat,strlen(dat),"SEND OK",ESP8266_BUF,2000);

	     CIPCLOSE(Link);
	   }

	 }

    BUF_clear(ESP8266_BUF,'*',1024);
    BUF_clear(CMD_temp,0,50);
     return 0;
}

int8_t CONNECT_TO_TCP(const char* ADDR, const char* PORT, const char* ID, uint16_t timing)//!!
{
	strcat(CMD_temp,"AT+CIPSTART=");
	strcat(CMD_temp,ID);
	strcat(CMD_temp,",\"");
	strcat(CMD_temp,"TCP");//UDP
	strcat(CMD_temp,"\"");
	strcat(CMD_temp,",\"");
	strcat(CMD_temp,ADDR);
	strcat(CMD_temp,"\",");
	strcat(CMD_temp,PORT);
	strcat(CMD_temp,",");
	strcat(CMD_temp,"0");
	strcat(CMD_temp,"\r\n");

	  if(ESP8266_DMA_send(CMD_temp,strlen(CMD_temp),OK,ESP8266_BUF,5000) == 0)
       {
		  WIFI_status.WIFI_CONNECTED_TO_TCP = 1;
		  GLOBAL_RETURN = 0;
       }
      else
       {
    	  WIFI_status.WIFI_CONNECTED_TO_TCP = 0;
    	  GLOBAL_RETURN = 1;
       }

	    BUF_clear(ESP8266_BUF,'*',1024);
	    BUF_clear(CMD_temp,0,50);

	return GLOBAL_RETURN;
}

uint8_t WIFI_Setting_TCP()
{
	WIFI_APSTA_init(MODE_APSTA, DHCP_APSTA_ON, WIFI_CIP_OFF, WIFI_MUX_ON, "80", 5000);

	if((WIFI_status.WIFI_mode + WIFI_status.WIFI_dhcp + WIFI_status.WIFI_cip + WIFI_status.WIFI_ap_name + WIFI_status.WIFI_ap_ip + WIFI_status.WIFI_mux + WIFI_status.WIFI_server) == 0)
	 {
		GLOBAL_RETURN = 0;
	 }
	else
	 {
		GLOBAL_RETURN = 1;
	 }

	return GLOBAL_RETURN;
}

/////////////////////////////////////////////////////END_ESP_TCP///////////////////////////////////////


/////////////////////////////////////////////////////ESP_WIFI//////////////////////////////////////////

int8_t WIFI_CONNECT_CUR(const char* SSID, const char* PASS, uint16_t timing)//!!
{
  if(WIFI_status.WIFI_CONNECTED_TO_SSID == 0)
   {
	  //BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);

      //char* temp;
      //uint8_t i = 0;
	  strcat(CMD_temp,"AT+CWJAP_CUR=");
	  strcat(CMD_temp,"\"");
	  strcat(CMD_temp,SSID);
	  strcat(CMD_temp,"\",\"");
	  strcat(CMD_temp,PASS);
	  strcat(CMD_temp,"\"");
	  strcat(CMD_temp,"\r\n");


	  if(ESP8266_DMA_send(CMD_temp,strlen(CMD_temp),OK,(char*)ESP8266_BUF,timing) == 0)
       {
	    GLOBAL_RETURN = 0;

	    BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);

	    WIFI_status.WIFI_CONNECTED_TO_SSID = 1;
	    READ_WIFI_CONNECTION(Seting.Network,2000);

	    ESP8266_DMA_send((char*)CIFSR,strlen(CIFSR),OK,(char*)ESP8266_BUF,2000);

	    char* temp_start = strstr((char*)ESP8266_BUF,"\"")+1;
	    char* temp_end   = strstr(temp_start,"\"");
	    memcpy(Self_IP,temp_start,temp_end - temp_start);
     }
	else
	 {
	  GLOBAL_RETURN = 1;
	  WIFI_status.WIFI_CONNECTED_TO_SSID = 0;
	 }

  }

  BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);
  BUF_clear(CMD_temp,0,CMD_temp_SIZE);

	return GLOBAL_RETURN;
}

uint8_t WIFI_CONNECT_DEF(const char* SSID, const char* PASS, uint16_t timing)//!!
{
	if(WIFI_status.WIFI_CONNECTED_TO_SSID == 0)
	   {
		WIFI_READ_ALL_SSID(ESP8266_BUF,5000);

		if(strstr((char*)ESP8266_BUF,Seting.Network) != 0)
		 {
		  BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);

	      char* temp;
	      uint8_t i = 0;
		  strcat(CMD_temp,"AT+CWJAP_DEF=");
		  strcat(CMD_temp,"\"");
		  strcat(CMD_temp,SSID);
		  strcat(CMD_temp,"\",\"");
		  strcat(CMD_temp,PASS);
		  strcat(CMD_temp,"\"");
		  strcat(CMD_temp,"\r\n");


		  if(ESP8266_DMA_send(CMD_temp,strlen(CMD_temp),OK,(char*)ESP8266_BUF,timing) == 0)
	       {
		    GLOBAL_RETURN = 0;

		    BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);

		    WIFI_status.WIFI_CONNECTED_TO_SSID = 1;
		    READ_WIFI_CONNECTION(Seting.Network,2000);

		    ESP8266_DMA_send((char*)CIFSR,strlen(CIFSR),OK,(char*)ESP8266_BUF,2000);

		    temp = strstr((char*)ESP8266_BUF,"STAIP,\"");

		   while(temp[i+7] != '"')
		    {
		  	Self_IP[i] = temp[i+7];
		  	i++;
		    }
	     }
		else
		 {
		  GLOBAL_RETURN = 1;
		  WIFI_status.WIFI_CONNECTED_TO_SSID = 0;
		 }
	   }
	  }

    BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);
    BUF_clear(CMD_temp,0,CMD_temp_SIZE);

		return GLOBAL_RETURN;
}

uint8_t WIFI_AUTOCONNECT(char* dat)//????????
{
  char data[20] = {"AT+CWAUTOCONN="};
  strcat(data,dat);
  strcat(data,"\r\n");

  if(ESP8266_send(data,strlen(data),OK,ESP8266_BUF,100) == 0)
   {
    GLOBAL_RETURN = 0;
   }
  else
   {
	GLOBAL_RETURN = 1;
   }

  return GLOBAL_RETURN;
}

int8_t READ_WIFI_CONNECTION(char* data, uint16_t timing)//!!
{

  char temp[4] = {0};

  if(ESP8266_DMA_send((char*)ASK_CWJAP_CUR,strlen(ASK_CWJAP_CUR),"OK",(char*)ESP8266_BUF,timing) == 0)
   {
	if(strstr((char*)ESP8266_BUF,"No AP") != 0)
	 {
	  GLOBAL_RETURN = 1;
	  goto END;
	 }

	if(strstr((char*)ESP8266_BUF,data) != 0)
	 {
	  GLOBAL_RETURN = 0;

	  char* START = strstr((char*)ESP8266_BUF,"-");
	  char* END   = strstr(START,"\r");

	  memcpy(temp,START,END-START);
      RSSI_Power = atoi(temp);

	  goto END;
	 }
   }


 END:
 BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);
 BUF_clear(CMD_temp,0,CMD_temp_SIZE);

 return GLOBAL_RETURN;
}

int8_t Auto_Connect()//!!
{

  if(WIFI_CONNECT_CUR(Seting.Network,Seting.Password,10000) > -1)///////////////////////////////////////
   {
    if(WIFI_status.WIFI_CONNECTED_TO_SSID == 1)
     {
    	GLOBAL_RETURN = 0;
    	goto END;
     }

    if(WIFI_status.WIFI_CONNECTED_TO_SSID == 0)
     {
    	GLOBAL_RETURN = 1;
     }
   }

   END:

 return GLOBAL_RETURN;
}

uint8_t READ_WIFI_STATUS()
{
  ESP8266_send("AT+CIPSTATUS\r\n",14,"OK",ESP8266_BUF,2000);

  char* tem;
  uint8_t i = 0;
  char Status[2] = {0};

  tem = strstr(ESP8266_BUF,"STATUS:");

  if(tem != 0)
   {
   while(tem[i+7] != '\r')
	{
	 Status[i] = tem[i+7];
	 i++;
	}
   }
  else
   {
	    BUF_clear(ESP8266_BUF,'*',1024);
	return -1;
   }

    if(strcmp(Status,"2") == 0)//������� ESP8266 ���������� � ����� �������, � �� ������������ IP �����;
	 {
        BUF_clear(ESP8266_BUF,'*',1024);
	  GLOBAL_RETURN = 2;
	 }

	if(strcmp(Status,"3") == 0)//� ������� ESP8266 ���� ��������� TCP ��� UDP ����������;
	 {
	    BUF_clear(ESP8266_BUF,'*',1024);
	  GLOBAL_RETURN = 3;
	 }

	if(strcmp(Status,"4") == 0)//TCP ��� UDP �������� ������� ESP8266 ���������;
	 {
	    BUF_clear(ESP8266_BUF,'*',1024);
	  GLOBAL_RETURN = 4;
	 }

	if(strcmp(Status,"5") == 0)//������� ESP8266 �� ���������� � ����� �������;
	 {
	    BUF_clear(ESP8266_BUF,'*',1024);
	  GLOBAL_RETURN = 5;
	 }

	return GLOBAL_RETURN;
}

char* READ_ALL_NETWORK_SSID(char* RETURN)
{
	BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);

	WIFI_READ_ALL_SSID(RETURN,5000);

	uint8_t SSID_COUNT = 0;

	char* SSID_TEMP[10] = {0};

	SSID_TEMP[0] = strstr((char*)RETURN,"(");
	SSID_COUNT++;

	if(SSID_TEMP[0] != 0)
	 {
	  for(uint8_t i = 1; i < 10; i++)
	   {
		SSID_TEMP[i] = strstr(SSID_TEMP[i-1]+1,"(");

		if(SSID_TEMP[i] != 0)
		 {
		  SSID_COUNT++;
		 }
	   }

	  char SSID_RSSI[SSID_COUNT][80];
	  memset(SSID_RSSI,0,SSID_COUNT*80);

	  for(uint8_t i = 0; i < SSID_COUNT; i++)
	   {
		  char ECN[2] = {0};
		  char BGN[2] = {0};

		  char* START_SSID = strstr(SSID_TEMP[i],"\"")+1;
		  char* END_SSID   = strstr(START_SSID,"\"");
		  char* START_RSSI = strstr(END_SSID,"-");
		  char* END_RSSI   = strstr(START_RSSI,",");
		  char* MAC_START  = strstr(END_RSSI,"\"")+1;
		  char* MAC_END    = strstr(MAC_START,"\"");
		  char* ECN_START  = strstr(SSID_TEMP[i],"(")+1;
		  char* ECN_END    = strstr(ECN_START,",");
		  char* BGN_START  = strstr(MAC_END,")")- 3;

		  memcpy(BGN,BGN_START,1);

		  strcat(SSID_RSSI[i],"*#");
		  memcpy(SSID_RSSI[i]+strlen(SSID_RSSI[i]) ,START_SSID,END_SSID - START_SSID);
		  strcat(SSID_RSSI[i],";");
		  memcpy(SSID_RSSI[i]+strlen(SSID_RSSI[i]) ,START_RSSI,END_RSSI - START_RSSI);
		  strcat(SSID_RSSI[i],";");
		  memcpy(SSID_RSSI[i]+strlen(SSID_RSSI[i]) ,MAC_START,MAC_END - MAC_START);
		  strcat(SSID_RSSI[i],";");

		  memcpy(ECN,ECN_START,ECN_END - ECN_START);

		  switch(atoi(ECN))
		   {
			case 0:
			 strcat(SSID_RSSI[i],"OPEN");
			break;

			case 1:
			 strcat(SSID_RSSI[i],"WEP");
			break;

			case 2:
			 strcat(SSID_RSSI[i],"WPA_PSK");
			break;

			case 3:
			 strcat(SSID_RSSI[i],"WPA2_PSK");
			break;

			case 4:
			 strcat(SSID_RSSI[i],"WPA_WPA2_PSK");
			break;

			case 5:
			 strcat(SSID_RSSI[i],"WPA2_Enterprise");
			break;
		   }

		  strcat(SSID_RSSI[i],";");

		  if(atoi(BGN) & 0x01)
		   {
			strcat(SSID_RSSI[i],"b");
		   }

		  if(atoi(BGN) & 0x02)
		   {
			strcat(SSID_RSSI[i],"g");
		   }

		  if(atoi(BGN) & 0x04)
		   {
			strcat(SSID_RSSI[i],"n");
		   }


		   strcat(SSID_RSSI[i],"#*");
	   }

	  memset(RETURN,0,strlen(RETURN));

	  for(uint8_t i = 0; i < SSID_COUNT; i++)
	   {
	    memcpy(RETURN,SSID_RSSI[i],strlen(SSID_RSSI[i]));
	   }
	 }
	return RETURN;
}

void WIFI_READ_ALL_SSID(char* ANSWER, uint32_t count)//!!
{
   ESP8266_DMA_send("AT+CWLAP\r\n",12,OK,ANSWER,count);
}

uint8_t ESP8266_HOSTNAME(char* Name)
{
  char CMD[50] = {0};

  strcat(CMD,"AT+CWHOSTNAME=");
  strcat(CMD,"\"");
  strcat(CMD,Name);
  strcat(CMD,"\"");
  strcat(CMD,"\r\n");

  GLOBAL_RETURN = ESP8266_DMA_send(CMD,strlen(CMD),OK,ESP8266_BUF,2000);

  BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);

  return GLOBAL_RETURN;
}



/////////////////////////////////////////////////////END_ESP_WIFI///////////////////////////////////////


/////////////////////////////////////////////////////ESP_BROADCAST//////////////////////////////////////

uint8_t WIFI_BROADCAST_Config()
 {
  WIFI_status.WIFI_mode = ESP_WIFI_MODE_SET(MODE_STA,1000); // MODE
  WIFI_status.WIFI_cip = WIFI_CIP(WIFI_CIP_OFF,1000);//WIFI_CIP
  WIFI_status.WIFI_mux = ESP_MUX(WIFI_MUX_ON,1000); //MUX
  WIFI_status.WIFI_server = ESP_SERVER_ON(WIFI_BROADCAST_PORT,1000); //SERVER_ON

  if(WIFI_status.WIFI_mode + WIFI_status.WIFI_cip + WIFI_status.WIFI_mux + WIFI_status.WIFI_server == 0)
   {
	GLOBAL_RETURN = 0;
   }
  else
   {
	GLOBAL_RETURN = 1;
   }

  return GLOBAL_RETURN;
 }

uint8_t WIFI_BROADCAST_PORT_START(char* Link, char* PORT)
     {
      strcat(CMD_temp,"AT+CIPSTART=");
      strcat(CMD_temp,Link);
      strcat(CMD_temp,",\"UDP\",\"0\",0,");
      strcat(CMD_temp,PORT);
      strcat(CMD_temp,",2\r\n");

      if(ESP8266_DMA_send(CMD_temp,strlen(CMD_temp),OK,ESP8266_BUF,1000) == 0)
       {
    	  WIFI_status.WIFI_BROADCAST_PORT_START = 0;
    	  GLOBAL_RETURN = 0;
       }
      else
       {
    	  WIFI_status.WIFI_BROADCAST_PORT_START = 1;
    	  GLOBAL_RETURN = 1;
       }

      BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);
      BUF_clear(CMD_temp,0,CMD_temp_SIZE);

      return GLOBAL_RETURN;
     }

uint8_t WIFI_SEND_TO_BROADCAST(const char* IP, const char* port, const char* dat, uint16_t size, const char* Answer, char *Link)//!!
{
	if(WIFI_status.WIFI_CONNECTED_TO_SSID == 1)
	 {
	     char ito_temp[4] = {0};

	     BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);

	     memset(ito_temp,0,4);
	     itoa(size,ito_temp,10);

	     strcat(CMD_temp,"AT+CIPSEND=");
	     strcat(CMD_temp+11,Link);
		 strcat(CMD_temp+12,",");
		 strcat(CMD_temp,ito_temp);
		 strcat(CMD_temp,"\r\n");

	     ESP8266_DMA_send(CMD_temp,strlen(CMD_temp),">",ESP8266_BUF,2000);
	     //HAL_Delay(2);
	     BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);
	     //HAL_Delay(2);
	     ESP8266_DMA_send((char*)dat,size,"SEND OK",ESP8266_BUF,2000);
	 }

    BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);
    BUF_clear(CMD_temp,0,CMD_temp_SIZE);

     return 0;
}

uint8_t READ_BROADCAST()
{
	  char *temp = strstr(ESP8266_BUF,"+IPD");

	  if(strstr(ESP8266_BUF,"+IPD") != 0)
	   {
	    WIFI_ID[0] = temp[5];
	   }

	  else
	   {
		  goto FINISH;
	   }

	  Mod_Bus(strstr(ESP8266_BUF,":")+1);



	  FINISH:
	  BUF_clear(ESP8266_BUF, 0, ESP8266_BUF_SIZE);
	  return 0;
}

void WIFI_PARSING()
 {
	 if(WIFI_READ_COUNT >= 10)
	  {
		HAL_Delay(10);

		if(WIFI_MODE == 0)
		 {

		  if(strstr(ESP8266_BUF+5,"DISCONNECT") != 0)
		   {
			 WIFI_status.WIFI_CONNECTED_TO_SSID = 0;
			 BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);
			 LL_TIM_EnableCounter(TIM4);
			// HAL_GPIO_WritePin(WIFI_OK_GPIO_Port, WIFI_OK_Pin, GPIO_PIN_RESET);
		   }

		  if(strstr(ESP8266_BUF+5,"CONNECTED") != 0)
		   {
			 WIFI_status.WIFI_CONNECTED_TO_SSID = 1;

			 READ_WIFI_CONNECTION(Seting.Network,2000);

			 ESP8266_DMA_send("AT+CIFSR\r\n",10,"OK",(char*)ESP8266_BUF,2000);

			 memset(Self_IP,0,17);
			 char* temp_start = strstr((char*)ESP8266_BUF,"\"")+1;
			 char* temp_end   = strstr(temp_start,"\"");
			 memcpy(Self_IP,temp_start,temp_end - temp_start);
			 BUF_clear(ESP8266_BUF,0,ESP8266_BUF_SIZE);
			 LL_TIM_DisableCounter(TIM4);
			// HAL_GPIO_WritePin(WIFI_OK_GPIO_Port, WIFI_OK_Pin, GPIO_PIN_SET);
		   }

		  if(WIFI_status.WIFI_CONNECTED_TO_SSID == 1)
		   {
			READ_BROADCAST();
		   }
		 }

		else
		 {
			WIFI_AP_READ();
		 }
	  }
 }

void WIFI_MODE_SELECT()
{
	/*     if(HAL_GPIO_ReadPin(WIFI_BUTTON_GPIO_Port, WIFI_BUTTON_Pin) == 0)
		  {
		   MAX7219_Send_CMD(0x0C,0x01);//ON
		   LL_TIM_EnableCounter(TIM3);
		   LED_ON_OFF_count = LED_ON_OFF_TIME;

	       HAL_Delay(3000);

	       if(WIFI_MODE == 0)
	        {
	        if(HAL_GPIO_ReadPin(WIFI_BUTTON_GPIO_Port, WIFI_BUTTON_Pin) == 0)
	       	  {
	    	   HAL_Delay(200);

	    	   LL_TIM_DisableCounter(TIM4);
	    	   LL_TIM_SetAutoReload(TIM4, 100);
	    	   LL_TIM_SetCounter(TIM4, 0);
	    	   LL_TIM_ClearFlag_UPDATE(TIM4);
	    	   LL_TIM_EnableIT_UPDATE(TIM4);
	    	   LL_TIM_EnableCounter(TIM4);

	    	   HAL_Delay(1000);

	    	   WIFI_RESET_HARD(500);

	    	     if(ESP_AT() == 0)
	    	      {
	    	    	 ESP_FACTORY_RESET();
	    	         WIFI_AP_init(MODE_APSTA, DHCP_APSTA_ON, WIFI_CIP_OFF, WIFI_MUX_ON, "80", 5000);
	    	         WIFI_MODE = 1;

	    	      }
	       	  }
		    }
	      else
	      {
	       if(HAL_GPIO_ReadPin(WIFI_BUTTON_GPIO_Port, WIFI_BUTTON_Pin) == 0)
	    	{
	    	 HAL_Delay(600);

	         if(HAL_GPIO_ReadPin(WIFI_BUTTON_GPIO_Port, WIFI_BUTTON_Pin) == 0)
	          {
	           HAL_Delay(200);

	    	   LL_TIM_DisableCounter(TIM4);
	    	   LL_TIM_SetAutoReload(TIM4, 500);
	    	   LL_TIM_SetCounter(TIM4, 0);
	    	   LL_TIM_ClearFlag_UPDATE(TIM4);
	    	   LL_TIM_EnableIT_UPDATE(TIM4);
	    	   LL_TIM_EnableCounter(TIM4);

	    	   HAL_Delay(1000);

	           WIFI_RESET_HARD(500);

	          	     if(ESP_AT() == 0)
	          	      {
	          	    	if(WIFI_BROADCAST_Config() == 0)
	          	    	 {
	          	    	          if(Auto_Connect() == 0)
	          	    	           {
	          	    	            WIFI_BROADCAST_PORT_START(WIFI_BROADCAST_LINK,WIFI_BROADCAST_PORT);
	          	    				LL_TIM_DisableCounter(TIM4);
	          	    				HAL_GPIO_WritePin(WIFI_OK_GPIO_Port, WIFI_OK_Pin, GPIO_PIN_SET);
	          	    	           }
	          	    	         }
	          	      }
	          }
	    	}
	        WIFI_MODE = 0;
	       }
		  }
*/}

/////////////////////////////////////////////////////END_ESP_BROADCAST//////////////////////////////////



/////////////////////////////////////////////////////OTHER//////////////////////////////////////////////

/*uint8_t WIFI_SEND_string(const char* dat, const char* Answer, char *Link)//!!
{
	char ito_temp[4] = {0};

	ESP_TEMP_clear();

	memset(ito_temp,0,4);
	itoa(strlen(dat),ito_temp,10);
	strcat(CMD_temp,"AT+CIPSEND=");
	strcat(CMD_temp,Link);
	strcat(CMD_temp,",");
	strcat(CMD_temp,ito_temp);
	strcat(CMD_temp,"\r\n");

	ESP8266_DMA_send(CMD_temp,strlen(CMD_temp),">",ESP8266_BUF,2000);

	ESP_BUF_clear();
	HAL_Delay(2);

	  if(ESP8266_DMA_send((char*)dat,strlen(dat),"SEND OK",ESP8266_BUF,2000) == 0)
	   {
         GLOBAL_RETURN = 0;
	   }
	  else
	   {
		  GLOBAL_RETURN = 1;
	   }

	  ESP_BUF_clear();
	  ESP_TEMP_clear();

    return GLOBAL_RETURN;
}*/

int8_t GET_TIME()
{
	char tem[5] = {0};
	char TIME[] = "AT+CIPSNTPTIME?\r\n";

	ESP_BUF_clear();

	ESP8266_DMA_send((char*)NTP,strlen(NTP),OK,ESP8266_BUF,1000);
	ESP_BUF_clear();
	HAL_Delay(500);
	ESP8266_DMA_send(TIME,strlen(TIME),OK,ESP8266_BUF,2000);
	HAL_Delay(500);

	if(strstr(ESP8266_BUF,"1970") != 0)
	 {
		 HAL_Delay(1000);
		 ESP8266_DMA_send((char*)NTP,strlen(NTP),OK,ESP8266_BUF,1000);
		 ESP_BUF_clear();
		 HAL_Delay(200);
		 ESP8266_DMA_send(TIME,strlen(TIME),OK,ESP8266_BUF,2000);


	    if(strstr(ESP8266_BUF,"1970") != 0)
	     {
	      HAL_Delay(1000);
	      ESP8266_DMA_send((char*)NTP,strlen(NTP),"OK",ESP8266_BUF,1000);
	      ESP_BUF_clear();
		  HAL_Delay(200);
		  ESP8266_DMA_send(TIME,strlen(TIME),"OK",ESP8266_BUF,2000);
	     }

	    if(strstr(ESP8266_BUF,"1970") != 0)
	     {
	      HAL_Delay(1000);
	      ESP8266_DMA_send((char*)NTP,strlen(NTP),"OK",ESP8266_BUF,1000);
	      ESP_BUF_clear();
		  HAL_Delay(200);
		  ESP8266_DMA_send(TIME,strlen(TIME),"OK",ESP8266_BUF,2000);
	     }

	    if(strstr(ESP8266_BUF,"1970") != 0)
	     {
	      HAL_Delay(1000);
	      ESP8266_DMA_send((char*)NTP,strlen(NTP),"OK",ESP8266_BUF,1000);
	      ESP_BUF_clear();
		  HAL_Delay(200);
		  ESP8266_DMA_send(TIME,strlen(TIME),"OK",ESP8266_BUF,2000);
	     }
	 }

	if(strstr(ESP8266_BUF,"1970") == 0)
	 {
	   uint8_t i = 0;
	   char* temp = strstr(ESP8266_BUF,"CIPSNTPTIME:");

       while(temp[i+16] != ' ')//' '
        {
    	 tem[i] = temp[i+16];
    	 i++;

    	 if(i > 5)
    	  {
           goto END;
    	  }
        }
         if((strstr(tem,"Jan") != 0)) tm.tm_mon = 1;
         if((strstr(tem,"Feb") != 0)) tm.tm_mon = 2;
         if((strstr(tem,"Mar") != 0)) tm.tm_mon = 3;
         if((strstr(tem,"Apr") != 0)) tm.tm_mon = 4;
         if((strstr(tem,"May") != 0)) tm.tm_mon = 5;
         if((strstr(tem,"Jun") != 0)) tm.tm_mon = 6;
         if((strstr(tem,"Jul") != 0)) tm.tm_mon = 7;
         if((strstr(tem,"Aug") != 0)) tm.tm_mon = 8;
         if((strstr(tem,"Sep") != 0)) tm.tm_mon = 9;
         if((strstr(tem,"Oct") != 0)) tm.tm_mon = 10;
         if((strstr(tem,"Nov") != 0)) tm.tm_mon = 11;
         if((strstr(tem,"Dec") != 0)) tm.tm_mon = 12;

         i = 0;
         memset(tem,'\0',5);

       while(temp[i+20] != ' ')
	    {
		 tem[i] = temp[i+20];
		 i++;

    	 if(i > 5)
    	  {
    		 goto END;
    	  }
		}

         tm.tm_wday = atoi(tem);
         i = 0;
         memset(tem,'\0',5);

        while(temp[i+23] != ':')
		 {
		  tem[i] = temp[i+23];
		  i++;

		 if(i > 5)
		  {
			 goto END;
		  }
		 }

         tm.tm_hour = atoi(tem);
		 i = 0;
		 memset(tem,'\0',5);

	   while(temp[i+26] != ':')
		{
		 tem[i] = temp[i+26];
		 i++;

    	 if(i > 5)
    	  {
    		 goto END;
    	  }
		}

		tm.tm_min = atoi(tem);
		i = 0;
		memset(tem,'\0',5);

	   while(temp[i+29] != ' ')
		{
		 tem[i] = temp[i+29];
		 i++;

    	 if(i > 5)
    	  {
    		 goto END;
    	  }
		}

		tm.tm_sec = atoi(tem);
		i = 0;
		memset(tem,'\0',5);

	   while(temp[i+32] != '\n')
		{
		 tem[i] = temp[i+32];
		 i++;

    	 if(i > 5)
    	  {
    		 goto END;
    	  }
		}

		tm.tm_year = (atoi(tem));
		i = 0;
		memset(tem,'\0',5);

		ESP_BUF_clear();

		return 0;

	}

	END:
	return -1;
}

uint16_t PING(char addr[])//!!
{
 uint8_t i = 0;
 char PING_time[4] = {0};

 char dat[64]= {'\0'};
 strcat(dat,"AT+PING=");
 strcat(dat,"\"");
 strcat(dat,addr);
 strcat(dat,"\"");
 strcat(dat,"\r\n");

 ESP_BUF_clear();

 if(ESP8266_DMA_send(dat,sizeof(dat),OK,ESP8266_BUF,3000) == 0)
  {
	 GLOBAL_RETURN = 0;

	 char* tem;
	 tem = strstr(ESP8266_BUF+10,"+");

	  while(tem[i+1] != '\r')
	   {
		PING_time[i] = tem[i+1];
		i++;
	   }

      PING_TIME = atoi(PING_time);
  }
 else
  {
	 GLOBAL_RETURN = 1;
  }

 ESP_BUF_clear();
 return GLOBAL_RETURN;
}

uint8_t FLASH_WriteSettings(uint16_t PAGE)
{
	Seting.RW_KEY += 1;

    int Page_ADDR = 0x08000000 + (PAGE*1024);
	uint32_t  PAGE_ERR = 0;
	uint32_t*  source_addr = (void *)&Seting;
	uint32_t*  dest_addr = (uint32_t*)Page_ADDR;
	uint16_t  SETING_WORDS = (sizeof(Seting)/4);

	FLASH_EraseInitTypeDef EraseInitStruct;

	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.NbPages = 1;
	EraseInitStruct.PageAddress = (1024*PAGE) + 0x08000000;

	HAL_FLASH_Unlock();
	HAL_FLASHEx_Erase(&EraseInitStruct, &PAGE_ERR);

	for(uint16_t i = 0; i < SETING_WORDS; i++)
	 {
	  HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)dest_addr, (uint32_t)*source_addr);
	  source_addr++;
	  dest_addr++;
	 }
	return HAL_FLASH_Lock();
}

uint8_t FLASH_ReadSettings(uint16_t PAGE)
{
	    int Page_ADDR = 0x08000000 + (PAGE*1024);

	    uint32_t *source_addr = (uint32_t *)Page_ADDR;
	    uint32_t *dest_addr = (void *)&Seting;

	    for (uint16_t i=0; i<sizeof(Seting); i+=4)
	    {
	      *dest_addr = *(__IO uint32_t*)source_addr;
	      source_addr++;
	      dest_addr++;
	    }
	    return 0;
}
