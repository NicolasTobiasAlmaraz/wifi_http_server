#ifndef HTTP_ENDPOINTS_TYPES
#define HTTP_ENDPOINTS_TYPES

//##########################################################
// Constantes endpoint /set_led
//##########################################################

typedef enum {LED_CHANGED=200, LED_ERROR=500} endpoint_set_led_status_code_t;    

typedef struct {
  int pin;
  bool state;
  } request_set_led_t;
  
typedef struct {
  endpoint_set_led_status_code_t status_code;
} response_set_led_t;

    
//##########################################################
// Constantes endpoint /get_config
//##########################################################
typedef enum {CONFIG_OK=200} endpoint_get_config_status_code_t;    

typedef struct {
  endpoint_get_config_status_code_t status_code;
  float frequency;
  float pressure;
  float power;
} response_get_config_t;

//##########################################################
// Constantes endpoint /new_ssid
//##########################################################
typedef struct {
  String new_ssid;
}request_new_ssid_t;

#endif
