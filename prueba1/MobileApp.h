#ifndef MOBILE_APP_H
#define MOBILE_APP_H

#include <WiFi.h>
#include <NetworkClient.h>
//#include <WiFiAP.h>
#include "http_endpoints_types.h"

class MobileApp {
  public:
    // Constructor
    MobileApp(const char* ssid, const char* password, IPAddress ip);

    // Setters and getters for SSID
    void setSSID(const char* newSsid);
    const char* getSSID() const;

    // Setters and getters for Password
    void setPassword(const char* newPassword);
    const char* getPassword() const;

    // Setters for HTTP request callbacks
    void set_callback_get_config(  response_get_config_t (*callback_func) (void)             );
    void set_callback_set_led   (  response_set_led_t    (*callback_func) (request_set_led_t) );

    // Methods to start the server and handle requests
    void begin();
    void handleClient();

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
    NetworkClient m_client;   // Cliente HTTP

    //Callbacks endpoints
    response_set_led_t (*set_led_cb)(request_set_led_t); //Metodo POST: Request con body - Response con status code
    response_get_config_t (*get_config_cb)(void);        //Metodo GET:  Request sin body - Response con status code y body

    //Metodos
    void http_request_manager(String request_line, String req_headers, String req_body);
    String get_endpoint(String request_line);
    int get_body_len(String headers);
    void send_http_response(int stat_code, const String& headers, const String& body);
};

#endif
