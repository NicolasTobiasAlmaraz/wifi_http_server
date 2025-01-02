#include <WiFi.h>
#include <ArduinoJson.h>

#include "MobileApp.h"
#include "http_endpoints_types.h"

#define LEDA 25 //Led
#define LEDB 26 //Led
#define LEDC 33 //Led

// Constructor initializes the app with default SSID, password, and network configurations
MobileApp::MobileApp(const char* ssid, const char* password, IPAddress ip) {
  Serial.println("Construtor init");
  strcpy(m_ssid, ssid);
  strcpy(m_password , password);
  m_ip_host = ip;
  m_ip_gw = ip;
  m_subnet = IPAddress(255, 255, 255, 0);
  Serial.println("Instancia network server");
  m_server = NetworkServer(80);
  set_led_cb = nullptr;
  get_config_cb = nullptr;
}

// Start the Wi-Fi access point and the server
void MobileApp::begin() {
  Serial.println("Comenzando...");
  WiFi.softAP(m_ssid, m_password);
  WiFi.softAPConfig(m_ip_host, m_ip_gw, m_subnet); // Configure the access point with IP settings
  m_server.begin(); // Start the HTTP server
}

// Handle incoming client requests
void MobileApp::handleClient() {
  String request_line;
  http_method_t http_method;
  String headers;
  String body;
  int body_len = 0;
  //int body_counter = 0;
  http_parser_state_t state;
  int len;
  char last_char;
  m_client = m_server.available(); // Check if a client is connected

  //Si hay un cliente, me fijo que quiere
  if (m_client) {
    String current_section = "";
    String current_line = "";
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
      current_section += c;
      current_line += c;
      Serial.print(c);

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
            current_line = "";
            state = HEADERS;
            if ( request_line.indexOf("GET") >= 0 ) {
              http_method = GET;
            }
            if ( request_line.indexOf("POST") >= 0 ) {
              http_method = POST;
            }
            //Por ahora solo implemento GET y POST
          }
          break;

        case HEADERS:
          //Me fijo si terminó la linea
          if(c != '\n') {
            break;
          }

          //Si termino la linea, me fijo además que sea \r\n
          if((c == '\n')&&(current_line != "\r\n")) {
              current_line = "";
              break;
          }
          
          //Ya llegó el \r\n
          headers = current_section;
          current_section = "";
          
          if (http_method == POST) {
              state = BODY;
              body_len = get_body_len(headers);
          }
          else {
              state = ANALYZE;
              body = "";
          }
          break;

        case BODY:
          if (current_section.length() == body_len-1) {
            body = current_section;
            current_section = "";
            state = ANALYZE;
          }
          break;

        case ANALYZE:
          body += c;
          http_request_manager(request_line, headers, body);
          break;
      }
    }
    m_client.stop(); // Disconnect the client
    Serial.println("Client disconnected.");
  }
}

//Averigua cuanto vale el header "Content-Length: "
int MobileApp::get_body_len(String headers) {
  String content_length_key = "Content-Length:";
  int start_index = headers.indexOf(content_length_key);

  if (start_index == -1) {
    // Si no se encuentra el encabezado, devolvemos -1
    return -1;
  }

  // Avanzamos después de "Content-Length: "
  start_index += content_length_key.length();

  int end_index = headers.indexOf('\n', start_index);

  // Extraemos y limpiamos el valor
  String content_length_value = headers.substring(start_index, end_index);
  content_length_value.trim();

  // Convertimos el valor a entero
  return content_length_value.toInt();
}

//Identifica la solicitud que llegó y toma acción para enviar el response
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

  //todo: Para implementar más prolijo, puede ser útil el uso de switch case, usando otra función que codifique a cada uno de los endpoints
  
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
    resp_headers += "Content-Length: "+String(resp_body.length());
  }
  else if (http_method == POST && endpoint_path.indexOf("/set_led") == 0) {
    DeserializationError error = deserializeJson(doc, req_body);
    if (error!=DeserializationError::Ok) {
      Serial.println("Error en el json");
      Serial.println(error.c_str());
      return;
    }

    request_set_led_t struct_request;
    struct_request.pin = doc["led"].as<int>();
    struct_request.state = doc["state"].as<int>();
    
    response_set_led_t response = set_led_cb(struct_request);
    
    resp_body = "LED CAMBIADO";
    
    stat_code = response.status_code;
    resp_headers += "Connection: keep-alive\r\n";
    resp_headers += "Content-Type: text/plain\r\n";
    resp_headers += "Access-Control-Allow-Origin: *\r\n";
    resp_headers += "Content-Length: " + String(resp_body.length()) + "\r\n";
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

//Toma el path del request line
String MobileApp::get_endpoint(String request_line) {
  int start = request_line.indexOf(" ") + 1;
  int end = request_line.indexOf(" ", start);
  return request_line.substring(start, end);
}

//Envia el response
void MobileApp::send_http_response(int stat_code, const String& headers, const String& body) {
  Serial.println("----Inicio response ---:");
  
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
    default:
      status_line += String(stat_code) + " Unknown Status";
      break;
  }
  // Imprimir la línea de estado
  m_client.println(status_line);
  Serial.println(status_line);
  
  // Imprimir los headers si no son nulos
  if (!headers.isEmpty()) {
    m_client.println(headers);
    Serial.println(headers);
  } else
    return;

  // Finalizar los headers con una línea vacía
  m_client.println();
  Serial.println();
  
  // Imprimir el cuerpo si no es nulo
  if (!body.isEmpty()) {
    m_client.println(body);
    Serial.println(body);
  }
  Serial.println("------Fin response------");
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

void MobileApp::update_wifi_cred() {

}

void MobileApp::connect_as_STA_mode(String ssid, String psw) {

}
