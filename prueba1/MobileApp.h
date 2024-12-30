#ifndef MOBILE_APP_H
#define MOBILE_APP_H

#include <WiFi.h>
#include <NetworkClient.h>
//#include <WiFiAP.h>

class MobileApp {
private:
    //Constantes
    typedef enum {GET, POST} http_method_t;                                   //Metodos HTTP
    typedef enum {REQUEST_LINE, HEADERS, BODY, ANALYZE} http_parser_state_t;  //Estados

    IPAddress m_ip_host = IPAddress(192, 168, 0, 1);  // IP Servidor
    IPAddress m_ip_gw = IPAddress(192, 168, 0, 1);    // IP Gateway
    IPAddress m_subnet = IPAddress(255, 255, 255, 0); // Mascara Subred

    //Variables red
    char m_ssid[100];     // Wi-Fi SSID
    char m_password[100]; // Wi-Fi password
    NetworkServer m_server; // HTTP server instance
    
    //Callbacks endpoints
    //Chequear sintaxis
    response_set_led_t (set_led_cb)(request_set_led_t) = nullptr; //Metodo POST: Request con body - Response con status code
    response_get_config_t (get_config_cb)(void) = nullptr;        //Metodo GET:  Request sin body - Response con status code y body

    //Metodos
    void http_request_manager(String request_line, String headers, String body, NetworkClient client);
    String get_endpoint(String request_line);
    int get_body_len(String headers);
    
public:
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
    
    // Constructor
    MobileApp(const char* ssid, const char* password, IPAddress ip);

    // Setters and getters for SSID
    void setSSID(const char* newSsid);
    const char* getSSID() const;

    // Setters and getters for Password
    void setPassword(const char* newPassword);
    const char* getPassword() const;

    // Setters for HTTP request callbacks
    void set_callback_get_config(  response_get_config_t (*callback) (void*)             );
    void set_callback_set_led   (  response_set_led_t    (*callback) (request_set_led_t) );

    // Methods to start the server and handle requests
    void begin();
    void handleClient();
};

#endif
