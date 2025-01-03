#define LED1 2  //Led

#include "MobileApp.h"
#include "http_endpoints_types.h"

//Defino credenciales WiFi e IP Fija
const char ssid[] = "ejemplo_wifi";      //SSID
const char psw[] = "123456789";           //PSW
IPAddress ip = IPAddress(192,168,0,1);  //IP
MobileApp app(ssid, psw, ip);

//Defino los callbacks
response_set_led_t       handle_set_led     (request_set_led_t);  //Callback
response_get_config_t    handle_get_config  (void);               //Callback

//Setup
void setup() {
    //Inicializo led
    pinMode(LED1, OUTPUT);
    
    //Inicializo UART
    Serial.begin(115200);
    Serial.println("Prueba Concepto Web Server");

    //Seteo Frecuencia a 80M a ver q hace
    setCpuFrequencyMhz(80);
    
    //Seteo Callbacks
    app.set_callback_set_led(handle_set_led);
    app.set_callback_get_config(handle_get_config);
    app.set_callback_wifi_off(handle_wifi_off);
    app.set_callback_change_ssid(handle_change_ssid);
    app.set_callback_wifi_connect(handle_wifi_connect);
    app.set_callback_wifi_connect_status(handle_wifi_connect_status);

    // Inicializo comunicacion con celular
    app.begin();
}

//Loop
void loop() {
    //Atiendo clientes -> Non blocling function
    app.handleClient();
}

// Callback:
// El celular pidió cambiar el estado de un led
response_set_led_t handle_set_led(request_set_led_t request) {
    int led = request.pin;
    bool state = request.state;

    //Procesamiento ...
    Serial.println("Endpoint: set led");
    Serial.println("Pin: "+String(led));
    Serial.println("State: "+String(state));
    digitalWrite(led, state);
    //...
    
    response_set_led_t response;
    response.status_code = LED_CHANGED;
    return response;
}

// Callback:
// El celular me pidió que le diga la configuración cargada
response_get_config_t handle_get_config(void) {
    
    //Procesamiento ...
    Serial.println("Endpoint: get config");
    //...
    
    response_get_config_t response;
    response.status_code = CONFIG_OK;
    response.frequency = 50;
    response.pressure = 7.2;
    response.power = 100;
    return response;
}

void handle_wifi_off(void) {
  Serial.println("Red Wifi Apagada");
}

void handle_change_ssid( req_change_ssid_t request ) {
  Serial.println("SSID cambiada");
  Serial.println(request.new_ssid);
}
 
void handle_wifi_connect( req_wifi_connect_t request ) {
  Serial.println("Conectando a red Wifi:");
  Serial.println("SSID: " + request.ssid);
  Serial.println("Pass: " + request.password);
  Serial.println("...");
}

void handle_wifi_connect_status(void) {
  MobileApp::ap_status_t state = MobileApp.ap_status()
  switch(state) {
    case AP_CONNECTED:
      Serial.print("Se conectó correctamente");
      break;
    
    case AP_CONNECTING:
      Serial.print("Aún no se conectó");
      break;
      
    case AP_CONNECT_ERROR:
      Serial.print("Error en la conexion");
      break;
  }
}
