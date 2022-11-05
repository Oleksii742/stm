#ifndef __ESP8266DRV07_h
#define __ESP8266DRV07_h
#include "DRV_CFG.h"


extern char ESP8266_BUF[ESP8266_BUF_SIZE];
extern char CMD_temp[CMD_temp_SIZE];

extern char*    READ_BUFFER;
extern uint16_t READ_BUF_SIZE;
extern uint16_t WIFI_READ_COUNT;
extern uint16_t WIFI_BUF_SIZE;
extern uint8_t  WIFI_MODE;

extern char Self_IP[17];

extern uint8_t ERR[15];
//extern uint16_t ESP8266_BUF_READ_COUNT;
extern const char NTP[];
extern int RSSI_Power;
extern uint8_t DMA_TC;
extern char *WIFI_WEB_ID;
extern char WIFI_ID[5];

extern uint16_t PING_TIME;

typedef struct
{
  char WIFI_mode;
  char WIFI_dhcp;
  char WIFI_cip;
  char WIFI_ap_name;
  char WIFI_ap_ip;
  char WIFI_mux;
  char WIFI_server;
  char WIFI_auto_connect;
  char WIFI_START_CONNECTING;
  char WIFI_CONNECTED_TO_SSID;
  char WIFI_CONNECTED_TO_TCP;
  char WIFI_BROADCAST_PORT_START;

} WIFI_ERR;

WIFI_ERR WIFI_status;

typedef struct
{
	  int	tm_sec;
	  int	tm_min;
	  int	tm_hour;
	  int	tm_mday;
	  int	tm_mon;
	  int	tm_year;
	  int	tm_wday;
	  int	tm_yday;
	  int	tm_isdst;
}TypeDef;

typedef struct
{
 char Network[32];
 char Password[32];
 char AP_NAME[32];
 char AP_PASS[32];
 char HOSTNAME[32];
 char ADDR[8];
 char BAUD[8];
 char TIME[8];
 char ANGLE_CONST[4];

 char LOGIN[32];
 char PASSWORD[32];

 //char UNIX[32];

 uint32_t RW_KEY;

}ALL_Seting;

ALL_Seting Seting;

enum WIFI_DHCP_Mode
{
 DHCP_AP_ON = 1,
 DHCP_AP_OFF = 2,

 DHCP_STA_ON = 3,
 DHCP_STA_OFF = 4,

 DHCP_APSTA_ON = 5,
 DHCP_APSTA_OFF = 6
};

enum WIFI_Mode
{
 MODE_AP = 1,
 MODE_STA = 2,
 MODE_APSTA = 3
};

enum WIFI_MUX
{
 WIFI_MUX_ON = 1,
 WIFI_MUX_OFF = 2
};

enum WIFI_CIP
{
 WIFI_CIP_ON = 1,
 WIFI_CIP_OFF = 2
};

void USART_DMA_ACTIVE();

void WIFI_RESET_HARD(int time);
void WIFI_ENABLE();
void send_to_USART_WIFI(const char* dat, uint16_t size);
uint8_t ESP8266_DMA_send(const char* temp,int len,const char* answer,const char* from, uint16_t count);
char ESP8266_send(char* temp, int len, char* answer, char* from, uint32_t count);
uint8_t WIFI_BROADCAST_Config();
uint8_t WIFI_BROADCAST_PORT_START(char* Link, char* PORT);
uint8_t WIFI_SEND_TO_BROADCAST(const char* IP, const char* port, const char* dat, uint16_t size, const char* Answer, char *Link);
uint8_t READ_BROADCAST();

uint8_t ESP_AT(void);
uint8_t ESP_WIFI_MODE_SET(uint8_t dat, uint32_t count);
uint8_t WIFI_AP_init(uint8_t MODE, uint8_t DHCP, uint8_t CIP, uint8_t MUX, const char* SERVER_PORT, uint16_t timing);
uint8_t WIFI_STA_init(uint8_t MODE, uint8_t DHCP, uint8_t CIP, uint8_t MUX, const char* SERVER_PORT, uint16_t timing);
uint8_t WIFI_APSTA_init(uint8_t MODE, uint8_t DHCP, uint8_t CIP, uint8_t MUX, const char* SERVER_PORT, uint16_t timing);
uint8_t DHCP_ONOFF(uint8_t DHCP,uint32_t count);
uint8_t WIFI_CIP(uint8_t mode, uint32_t count);
uint8_t ESP_MUX(uint8_t data, uint32_t count);
uint8_t ESP_SERVER_ON(const char* port, uint32_t count);
int8_t  WIFI_CONNECT_CUR(const char* SSID, const char* PASS, uint16_t timing);
uint8_t WIFI_CONNECT_DEF(const char* SSID, const char* PASS, uint16_t timing);
uint8_t ESP_STA_IP(const char* IP, const char* GATEWAY, const char* MASK,uint16_t timing);
int8_t  CONNECT_TO_TCP(const char* ADDR, const char* PORT, const char* ID, uint16_t timing);
uint8_t WIFI_SEND_string(const char* dat, const char* Answer, char *Link);
uint8_t WIFI_SEND_TO_TCP(const char* IP, const char* port, const char* dat, const char* Answer, char *Link);
uint8_t CONNECT_TO_UDP(const char* ADDR, const char* PORT, const char* ID, const char* PORT_2, uint16_t timing);//++

uint8_t WIFI_SEND_TO_UDP(const char* IP, const char* port, const char* dat, const char* Answer, char *Link);
void WIFI_PARSING();
void WIFI_MODE_SELECT();





int8_t  GET_TIME();
uint8_t WIFI_Setting_TCP();
uint8_t ESP_AP_NAME(char data[],char temp[],uint32_t count);
uint8_t ESP_AP_IP(char temp[],uint32_t count);
uint16_t PING(char addr[]);
int8_t  Auto_Connect();

void WIFI_AP_READ();

char* READ_ALL_NETWORK_SSID(char* RETURN);

void BUF_clear(char* BUF, char DAT, uint16_t size);
void ESP_TEMP_clear();
void send_to_USART_WIFI(const char* dat, uint16_t size);
void WIFI_READ_ALL_SSID(char* ANSWER, uint32_t count);
uint8_t CIPCLOSE(char temp[]);
void Send_Broadcast();
uint8_t ESP8266_HOSTNAME(char* Name);

int8_t READ_WIFI_CONNECTION(char* data, uint16_t timing);

uint8_t ESP_FACTORY_RESET();
void Program_ESP8266();
uint8_t ESP_READ_CONFIG_FROM_SERVER(uint32_t count);

uint8_t FLASH_WriteSettings(uint16_t PAGE);
uint8_t FLASH_ReadSettings(uint16_t PAGE);


#endif
