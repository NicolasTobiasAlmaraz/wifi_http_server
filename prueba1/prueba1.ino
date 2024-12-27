#define LED_BUILTIN 2 //Led

#include "MobileApp.h"

//Defino credenciales WiFi e IP Fija
const char *ssid = "ejemplo_wifi";      //SSID
const char *psw = "12345678";           //PSW
IPAddress ip = IPAddress(192,168,0,1);  //IP
MobileApp app(ssid, psw, ip);

//Defino los callbacks
void* handle_toggle_led(void*);   //Callback
void* handle_get_config(void*);   //Callback

//Setup
void setup() {
    //Inicializo Led
    pinMode(LED_BUILTIN, OUTPUT);
    
    //Inicializo UART
    Serial.begin(115200);
    Serial.print("Prueba Concepto Web Server");
    
    //Seteo Callbacks
    app.set_callback_set_led(handle_set_led);
    app.set_callback_get_config(handle_get_config);

    // Inicializo Red Wi-Fi y Web Server
    app.begin();
    
    Serial.print("Init Ok");
}

//Loop
void loop() {
    //Atiendo Clientes
    app.handleClient();
}

void* handle_set_led(void*) {
    
    led = true;
    digitalWrite(LED_BUILTIN, led);
    Serial.println("Endpoint: toggle led");
    return 0;
}

void* handle_get_config(void *) {
    Serial.println("Endpoint: get config");
    return 0;
}
