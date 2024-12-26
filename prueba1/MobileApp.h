#ifndef MOBILE_APP_H
#define MOBILE_APP_H

#include <WiFi.h>
#include <NetworkClient.h>
#include <WiFiAP.h>

class MobileApp {
private:
    char m_ssid[100];                           // Wi-Fi SSID
    char m_password[100];                       // Wi-Fi password
    const IPAddress m_ip_host(192, 168, 0, 1);  // IP Servidor
    const IPAddress m_ip_gw(192, 168, 0, 1);    // IP Gateway
    const IPAddress m_subnet(255, 255, 255, 0); // Mascara Subred
    NetworkServer m_server;                     // HTTP server instance
    
    void (*toggle_led_cb)(WiFiClient&) = nullptr; // Callback endpoint "toggle_led"
    void (*get_config_cb)(WiFiClient&) = nullptr; // Callback endpoint "get_config"0
    
    void m_parser_http_request(const char* request_line, const char* headers, const char* body); // Capta solicitudes y se fija si corresponde a algun endpoint
    void m_send_http_response(const char* response_line, const char* headers, const char* body); // Responde con la respuesta indicada

public:
    
    // Constructor
    MobileApp(const char* defaultSsid, const char* defaultPassword, IPAddress ip, IPAddress gw, IPAddress sn);

    // Setters and getters for SSID
    void setSSID(const char* newSsid);
    const char* getSSID() const;

    // Setters and getters for Password
    void setPassword(const char* newPassword);
    const char* getPassword() const;

    // Setters for HTTP request callbacks
    void setRowaIdentificationCallback(void (*callback)(WiFiClient&));
    void setUpdateConfigCallback(void (*callback)(WiFiClient&));

    // Methods to start the server and handle requests
    void begin();
    void handleClient();
};

#endif*/
