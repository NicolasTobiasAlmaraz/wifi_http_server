// main.ino
#include "MobileApp.h"

//Defino credenciales WiFi e IP Fija
const char *ssid = "ejemplo_wifi";  //SSID
const char *psw = "12345678";       //PSW

//Creo Handler para App movil
MobileApp app(ssid, psw, IP_fija);

//Defino los callbacks
void handle_toggle_led(WiFiClient& client);   //Callback
void handle_get_config(WiFiClient& client);   //Callback

//Setup
void setup() {
    //Inicializo Led
    pinMode(LED_BUILTIN, OUTPUT);
    
    //Inicializo UART
    Serial.begin(115200);
    Serial.print("Prueba Concepto Web Server");
    
    //Seteo Callbacks
    app.set_toggle_led_cb(handle_toggle_led);
    app.set_get_config_cb(handle_get_config);

    // Inicializo Red Wi-Fi y Web Server
    app.begin();
    
    Serial.print("Init Ok");
}

//Loop
void loop() {
    //Atiendo Clientes
    app.handleClient();
}

void handle_toggle_led(WiFiClient& client) {
    static bool led=false;
    if(led)
      led = false;
    else
      led = true;
    digitalWrite(LED_BUILTIN, led);
    Serial.print("Endpoint: toggle led");
    
}

void handle_get_config(WiFiClient& client) {
    Serial.print("Endpoint: get config");
}
