#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

//Credenciales slave
const char *ssid_ap = "ESP32_SLAVE";
const char *password_ap = "slave1234";

//Credenciales master
String master_ssid = "";
String master_password = "";
const String url = "http://192.168.0.1/get_config"; // Dirección del servidor

//Led indicador
const int LED_PIN = 2; // Usado para indicar la conexión

//Web server
WebServer server(80);

//Estado maquina de estados
typedef enum {
  DESCONECTADO,
  CONECTANDO,
  VERIFICANDO,
  CONECTADO,
} estado_t;

estado_t estado = DESCONECTADO;

// Configuración inicial de Wi-Fi en modo AP
void setup_wifi_ap() {
  WiFi.softAP(ssid_ap, password_ap);
  IPAddress ap_ip_local(192,168,1,1);
  IPAddress ap_ip_gw(192,168,1,0);
  IPAddress ap_subnet(255,255,255,0);
  WiFi.softAPConfig(ap_ip_local, ap_ip_gw, ap_subnet);
  Serial.println("AP configurado");
}

// Manejo del POST para "/connect_wifi"
void handle_connect_wifi() {
  Serial.println("Llega endpoint /connect_wifi:");
  if (server.hasArg("ssid") && server.hasArg("password")) {
    master_ssid = server.arg("ssid");
    master_password = server.arg("password");
    server.send(200, "text/plain", "Intentando conectar al master Wi-Fi...");
    Serial.println("Intentando conectar al master Wi-Fi:");
    Serial.println(master_ssid);
    Serial.println(master_password);
    estado = CONECTANDO;
  } else {
    server.send(400, "text/plain", "Faltan parámetros 'ssid' y 'password'");
    Serial.println("Falta SSID o Password");
  }
}

// Manejo del GET para "/wifi_connect_status"
void handle_wifi_status() {
  String wifi_status;
  switch(estado) {
    default:
    case DESCONECTADO:
      wifi_status = "DESCONECTADO";
      break;
    case CONECTANDO:
      wifi_status = "CONECTANDO";
      break;
    case VERIFICANDO:
      wifi_status = "VERIFICANDO";
      break;
    case CONECTADO:
      wifi_status = "CONECTADO";
      break;
  }
  Serial.println("Llega solicitud status:");
  Serial.println(wifi_status);
  server.send(200, "text/plain", wifi_status);
}

void maquina_estados() {
  int tries = 0;
  int httpCode;
  float frecuencia;
  DeserializationError error;
  static String payload = "";
  static HTTPClient http;
  StaticJsonDocument<200> doc;
  
  switch(estado) {
    case DESCONECTADO:
      break;
    case CONECTANDO:
      //Inicio como STA
      Serial.println("Conectando...");
      WiFi.begin(master_ssid.c_str(), master_password.c_str());
  
      // Esperar hasta que se conecte
      while (WiFi.status() != WL_CONNECTED && tries < 30) {
        delay(1000);
        Serial.print(".");
        tries++;
      }

      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Conexión exitosa");
        estado = VERIFICANDO;
      } else {
        Serial.println("No se pudo conectar");
        estado = DESCONECTADO;
      }
      break;
    
    case VERIFICANDO:
      Serial.println("Verificando modelo...");
      // Realizar la solicitud HTTP GET al servidor externo
      http.begin(url);
      httpCode = http.GET();

      //Si recibo un error salgo
      if(httpCode!=200) {
        estado = DESCONECTADO;
        WiFi.disconnect();
        Serial.println("Error al obtener configuración.");
        http.end();
        break;
      }

      //Si la solicitud fue exitosa me fijo si esta bien config
      payload = http.getString();
      error = deserializeJson(doc, payload);
      if (error != DeserializationError::Ok) {
        Serial.println("Error en el json");
        Serial.println(error.c_str());
        return;
      }
      frecuencia = doc["frecuencia"].as<float>();

      if (frecuencia == 50) {
        estado = CONECTADO;
        Serial.println("Modelo valido, frecuencia=50");
        WiFi.softAPdisconnect(true); 
        Serial.println("AP desactivado, operando solo como STA.");
      } else {
        estado = DESCONECTADO;
        WiFi.disconnect();
        Serial.println("Modelo no compatible. Frecuencia no es 50.");
      }
      http.end();
      break;
      
    case CONECTADO:
      break;
  }
}

void setup() {
  Serial.begin(115200);

  // Inicializamos el AP
  setup_wifi_ap();

  // Configurar las rutas
  server.on("/connect_wifi", HTTP_POST, handle_connect_wifi);
  server.on("/wifi_connect_status", HTTP_GET, handle_wifi_status);

  // Iniciar el servidor
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  // Manejar las solicitudes HTTP
  server.handleClient();

  //Maquinas de estados
  maquina_estados();
}
