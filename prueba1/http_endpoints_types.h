#ifndef HTTP_ENDPOINTS_TYPES
#define HTTP_ENDPOINTS_TYPES


// Constantes endpoint set led
typedef enum {LED_CHANGED, LED_ERROR} endpoint_set_led_status_code_t;    

typedef struct {
  int pin;
  bool state;
  } request_set_led_t;
  
typedef struct {
  endpoint_set_led_status_code_t status_code;
} response_set_led_t;

    
// Constantes endpoint get config
typedef enum {CONFIG_OK} endpoint_get_config_status_code_t;    

typedef struct {
  endpoint_get_config_status_code_t status_code;
  float frequency;
  float pressure;
  float power;
} response_get_config_t;

#endif
