#include <WiFi.h>
#include <ArduinoJson.h>

#include "MobileApp.h"
#include "http_endpoints_types.h"

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
  //Estado de la maquina de estados
  static general_state_t state = NO_CLIENT;

  switch (state) {

    case NO_CLIENT:
      // Check if a client is connected
      m_client = m_server.available();
      if (m_client) {
        state = PARSING_REQUEST;
      }
      break;

    case PARSING_REQUEST:
      //Si se desconecta, voy al estado de desconexion
      if (!m_client.connected()) {
        state = NO_CLIENT;
        m_client.stop(); // Disconnect the client
        Serial.println("Client disconnected.");
        break;
      }

      //Si hay info nueva, voy parseando hasta que haya tomar accion
      if ( m_client.available() ) {
        bool new_request = http_request_parser();
        if (new_request)
          http_request_manager();
        break;
      }
  }
}

bool MobileApp::http_request_parser() {
  static http_request_state_t state = REQUEST_LINE;
  static int body_len = 0;
  static String current_section = "";
  static String current_line = "";

  //Flag de salida, indica si finalizo request
  bool end_of_request = false;

  //Leo caracter
  char c = m_client.read();
  Serial.print(c);
  current_section += c;
  current_line += c;

  //Analizo info
  switch (state) {
    default:
      state = REQUEST_LINE;
      break;

    case REQUEST_LINE:
      //Espero a que termine el request line
      if (c == '\n') {
        request_line = current_section;
        
        //Me fijo si es metodo GET
        if ( request_line.indexOf("GET") >= 0 ) {
          http_method = GET;
          state = HEADERS;
        }

        //Me fijo si es metodo POST
        else if ( request_line.indexOf("POST") >= 0 ) {
          http_method = POST;
          state = HEADERS;
        }

        //Recupero el endpoint
        endpoint_path = get_endpoint(request_line);

        //Limpio memoria
        current_section = "";
        current_line = "";
      }
      break;

    case HEADERS:
      //Me fijo si terminó la linea
      if (c != '\n') {
        break;
      }

      //Me fijo si la linea es CRLF \r\n
      if ((c == '\n') && (current_line != "\r\n")) {
        current_line = "";
        break;
      }

      //Ya llegó el \r\n
      req_headers = current_section;
      current_section = "";
      Serial.println(req_headers);
      
      //Si era metodo POST, voy al body
      if (http_method == POST) {
        state = BODY;
        body_len = get_body_len(req_headers);
      }

      //Si era metodo GET ya se puede ejecutar
      else {
        state = REQUEST_LINE;
        req_body = "";
        end_of_request = true;
      }
      break;

    case BODY:
      if (current_section.length() == body_len) {
        req_body = current_section;
        current_section = "";
        end_of_request = true;
        state = REQUEST_LINE;
      }
      break;
  }
  return end_of_request;
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
void MobileApp::http_request_manager() {
  if (http_method == GET && endpoint_path.indexOf("/get_config") == 0 ) {
    handler_endpoint_get_config();
  }
  else if (http_method == POST && endpoint_path.indexOf("/set_led") == 0) {
    handler_endpoint_set_led();
  }
  else {
    handler_not_found_endpoint();
    return;
  }
}

//Handler Endpoint Set Led
void MobileApp::handler_endpoint_set_led() {
  int stat_code;
  String resp_headers;
  String resp_body;
  StaticJsonDocument<200> doc;

  //Parseo Json
  DeserializationError error = deserializeJson(doc, req_body);
  if (error != DeserializationError::Ok) {
    Serial.println("Error en el json");
    Serial.println(error.c_str());
    return;
  }
  request_set_led_t struct_request;
  struct_request.pin = doc["led"].as<int>();
  struct_request.state = doc["state"].as<int>();

  //Ejecuto callback usuario
  response_set_led_t response = set_led_cb(struct_request);

  //Pongo un body
  resp_body = "CAMBIAR ESTADO LED";

  //Cargo status code
  stat_code = response.status_code;

  //Pongo headers
  resp_headers += "Connection: keep-alive\r\n";
  resp_headers += "Content-Type: text/plain\r\n";
  resp_headers += "Access-Control-Allow-Origin: *\r\n";
  resp_headers += "Content-Length: " + String(resp_body.length());

  //Envio respuesta
  send_http_response(stat_code, resp_headers, resp_body);
}

//Handler Endpoint Get Config
void MobileApp::handler_endpoint_get_config() {
  int stat_code;
  String resp_headers;
  String resp_body;
  StaticJsonDocument<200> doc;

  response_get_config_t response = get_config_cb();

  stat_code = response.status_code;

  //Creo un json doc
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["frequency"] = response.frequency;
  jsonDoc["pressure"] = response.pressure;
  jsonDoc["power"] = response.power;
  serializeJson(jsonDoc,resp_body);
  
  resp_headers = "Content-Type: application/json\n";
  resp_headers += "Content-Length: " + String(resp_body.length());
  
  send_http_response(stat_code, resp_headers, resp_body);
}

//Handler Not Found request
void MobileApp::handler_not_found_endpoint() {
  // Send a 404 response for unknown endpoints
  m_client.println("HTTP/1.1 404 Not Found");
  m_client.println("Connection: close");
  m_client.println();
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
