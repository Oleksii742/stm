#include "main.h"

#define ENABLE
#define RESET

#define SELF_ID                                    123
#define REVISION "V-0.1"
#define Setings_page							   64

#define ESP8266_BUF_SIZE                           300
#define CMD_temp_SIZE                              50
#define CMD_temp_temp_SIZE                         50

#define WIFI_BROADCAST_LINK                        "4"
#define WIFI_BROADCAST_PORT                        "65535"

#define ESP8266_USART                              USART1
#define ESP8266_DMA								   DMA1
#define ESP8266_DMA_CHANEL						   LL_DMA_CHANNEL_4
#define ESP8266_DMA_ACTIVE_TC_FLAG				   LL_DMA_IsActiveFlag_TC4
#define ESP8266_DMA_CLEAR_TC_FLAG			       LL_DMA_ClearFlag_TC4

#ifdef ENABLE
#define ESP8266_ENABLE_PORT						   GPIOA
#define ESP8266_ENABLE_PIN						   GPIO_PIN_0
#endif

#ifdef RESET
#define ESP8266_RESET_PORT                         GPIOB
#define ESP8266_RESET_PIN                          GPIO_PIN_15
#endif

