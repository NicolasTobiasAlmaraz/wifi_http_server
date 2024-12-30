// MobileApp.cpp
#include "MobileApp.h"
#include "http_endpoints_types.h"
#include <ArduinoJson.h>

// Constructor initializes the app with default SSID, password, and network configurations
MobileApp::MobileApp(const char* ssid, const char* password, IPAddress ip) {
  strcpy(m_ssid, ssid);
  strcpy(m_password , password);
  m_ip_host = ip;
  m_ip_gw = ip;
  m_subnet = IPAddress(255, 255, 255, 0);
  m_server = NetworkServer(80);
  set_led_cb = nullptr;
  get_config_cb = nullptr;
}

// Set a new SSID
void MobileApp::setSSID(const char* ssid) {
  strcpy(m_ssid, ssid);
}

// Get the current SSID
const char* MobileApp::getSSID() const {
  return m_ssid;
}

// Set a new password
void MobileApp::setPassword(const char* password) {
  strcpy(m_password , password);
}

// Get the current password
const char* MobileApp::getPassword() const {
  return m_password;
}


// Set the callback for handling
void MobileApp::set_callback_set_led   (  response_set_led_t    (*callback_func) (request_set_led_t) ) {
  set_led_cb = callback_func;
}

// Set the callback for handling
void MobileApp::set_callback_get_config(   response_get_config_t(*callback_func)(void)    ) {
  get_config_cb = callback_func;
}

// Start the Wi-Fi access point and the server
void MobileApp::begin() {
  WiFi.softAP(m_ssid, m_password); // Start the access point
  WiFi.softAPConfig(m_ip_host, m_ip_gw, m_subnet); // Configure the access point with IP settings
  Serial.print("Init Wi-Fi Ok");
  m_server.begin(); // Start the HTTP server
  Serial.print("HTTP Server Ok");
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP()); // Print the access point IP address

  Serial.print("Init App Ok");
}

// Handle incoming client requests
void MobileApp::handleClient() {
  String request_line;
  http_method_t http_method;
  String headers;
  String body;
  int body_len = 0;
  int body_counter = 0;
  http_parser_state_t state;
  int len;
  char last_char;
  m_client = m_server.available(); // Check if a client is connected

  //Si hay un cliente, me fijo que quiere
  if (m_client) {
    Serial.println("New Client.");
    String current_section = "";
    state = REQUEST_LINE;
    request_line = "";
    headers = "";
    body = "";

    //Mientras esté conectado
    while (m_client.connected()) {

      //Me fijo si tiene info para leer
      if (!m_client.available())
        continue;

      //Voy acumulando letras de cada sección
      char c = m_client.read();
      Serial.write(c);
      current_section += c;

      //Analizo info
      switch (state) {
        default:
          state = REQUEST_LINE;
          break;
        case REQUEST_LINE:
          //Guardo request line
          if (c == '\n') {
            request_line = current_section;
            current_section = "";
            state = HEADERS;
            if ( request_line.indexOf("GET") >= 0 )
              http_method = GET;
            if ( request_line.indexOf("POST") >= 0 )
              http_method = POST;
            //Por ahora solo implemento GET y POST
          }
          break;

        case HEADERS:
          //La seccion de headers termina con \n\n
          len = current_section.length();
          last_char = current_section.charAt(len - 1); //Chequear si estoy poniendo bien el indice
          if ( (c == '\n') && (last_char == '\n') ) {
            headers = current_section;
            current_section = "";
            if (http_method == POST) {
              state = BODY;
              body_len = get_body_len(headers);
              body_counter = 0;
            }
            else {
              state = ANALYZE;
              body = "";
            }
          }
          break;

        case BODY:
          body_counter++;
          if (body_counter == body_len) {
            state = ANALYZE;
            body = current_section;
            current_section = "";
          }
          break;

        case ANALYZE:
          http_request_manager(request_line, headers, body);
          break;
      }
    }
    m_client.stop(); // Disconnect the client
    Serial.println("Client disconnected.");
  }
}

int MobileApp::get_body_len(String headers) {
  String content_length_key = "Content-Length:";
  int start_index = headers.indexOf(content_length_key);

  if (start_index == -1) {
    // Si no se encuentra el encabezado, devolvemos -1
    return -1;
  }

  // Avanzamos después de "Content-Length:"
  start_index += content_length_key.length();
  int end_index = headers.indexOf('\n', start_index);

  // Extraemos y limpiamos el valor
  String content_length_value = headers.substring(start_index, end_index);
  content_length_value.trim();

  // Convertimos el valor a entero
  return content_length_value.toInt();
}

void MobileApp::http_request_manager(String request_line, String req_headers, String req_body) {
  http_method_t http_method;

  //Identifico metodo
  if ( request_line.indexOf("GET") >= 0 )
    http_method = GET;
  if ( request_line.indexOf("POST") >= 0 )
    http_method = POST;

  //Recupero el endpoint
  String endpoint_path = get_endpoint(request_line);
  
  int stat_code;
  String resp_headers;
  String resp_body;
  StaticJsonDocument<200> doc;
  
  if (http_method == GET && endpoint_path.indexOf("/get_config") == 0 ) {
    response_get_config_t response = get_config_cb();
    
    stat_code = response.status_code;
    
    //Todo String struct2string(void* struct, endpoint_type_t type);
    String resp_body = "{\n"
              "  \"frequency\": 50,\n"
              "  \"pressure\": 10,\n"
              "  \"power\": 100\n"
              "}";
    
    resp_headers = "Content-Type: application/json\n";
    resp_headers += "Content-Length: 53";
  }
  else if (http_method == POST && endpoint_path.indexOf("/set_led") == 0) {
    int body_len = get_body_len(req_body);
    DeserializationError error = deserializeJson(doc, req_body);
    request_set_led_t struct_request;
    if(error)
      return;  
    struct_request.pin = doc["led"];
    struct_request.state = doc["state"];
    
    response_set_led_t response = set_led_cb(struct_request);
    stat_code = response.status_code;
    resp_headers = "Connection: Keep-Alive";
    resp_body = "";
  }
  else {
    // Send a 404 response for unknown endpoints
    m_client.println("HTTP/1.1 404 Not Found");
    m_client.println("Connection: close");
    m_client.println();
    return;
  }
  send_http_response(stat_code, resp_headers, resp_body);
}

String MobileApp::get_endpoint(String request_line) {
  int start = request_line.indexOf(" ") + 1;
  int end = request_line.indexOf(" ", start);
  return request_line.substring(start, end);
}

void MobileApp::send_http_response(int stat_code, const String& headers, const String& body) {
    // Formatear la línea de estado HTTP
    String status_line = "HTTP/1.1 ";
    switch (stat_code) {
        case 200:
            status_line += "200 OK";
            break;
        case 404:
            status_line += "404 Not Found";
            break;
        case 500:
            status_line += "500 Internal Server Error";
            break;
        // Puedes agregar más códigos de estado según sea necesario
        default:
            status_line += String(stat_code) + " Unknown Status";
            break;
    }
    // Imprimir la línea de estado
    m_client.println(status_line);
    
    // Imprimir los headers si no son nulos
    if (!headers.isEmpty()) {
        m_client.println(headers);
    }
    
    // Finalizar los headers con una línea vacía
    m_client.println();
    
    // Imprimir el cuerpo si no es nulo
    if (!body.isEmpty()) {
        m_client.println(body);
    }
}
