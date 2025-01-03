#ifndef MOBILE_APP_H
#define MOBILE_APP_H

#include <WiFi.h>
#include <NetworkClient.h>
#include "http_endpoints_types.h"

class MobileApp {
  public:
    typedef enum {AP_CONNECTED, AP_CONNECTING, AP_CONNECT_ERROR} ap_status_t;
  
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
    void begin();             //Init web server & wi-fi module
    void handleClient();      //Loop for handle clients
    
    // Method to get ap connected
    ap_status_t ap_status();  
    
  private:
    //Typedefs
    typedef enum {NO_CLIENT, PARSING_REQUEST}general_state_t; //general state machine 
    typedef enum {GET, POST} http_method_t;                                     // HTTP
    typedef enum {REQUEST_LINE, HEADERS, BODY} http_request_state_t;   //Estados

    //Config server
    IPAddress m_ip_host = IPAddress(192, 168, 0, 1);  // IP Servidor
    IPAddress m_ip_gw = IPAddress(192, 168, 0, 1);    // IP Gateway
    IPAddress m_subnet = IPAddress(255, 255, 255, 0); // Mascara Subred

    //Var server
    char m_ssid[50];         // Wi-Fi SSID
    char m_password[50];     // Wi-Fi password
    NetworkServer m_server;   // HTTP server instance
    NetworkClient m_client;   // HTTP Client instance

    //Requests Callbacks
    response_set_led_t (*set_led_cb)(request_set_led_t); //Metodo POST: Request con body - Response con status code
    response_get_config_t (*get_config_cb)(void);        //Metodo GET:  Request sin body - Response con status code y body

    //Methods to parse HTTP request
    bool http_request_parser();
    void http_request_manager();
    String get_endpoint(String request_line);                                             //Toma el path del request line
    int get_body_len(String headers);                                                     //Toma el header Content-Length

    //Request parts
    String request_line;
    http_method_t http_method;
    String req_headers;
    String req_body;
    String endpoint_path;

    //Method to send responses
    void send_http_response(int stat_code, const String& headers, const String& body);    //Envia response al cliente
    
    //Methods to handle responses
    void handler_endpoint_set_led();
    void handler_endpoint_get_config();
    void handler_not_found_endpoint();

    void update_wifi_cred();  //Updates the wifi credentials (SSID & password)
    void connect_as_STA_mode(String ssid, String psw);  //Updates the wifi credentials (SSID & password)
    
};

#endif
