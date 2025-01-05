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
char master_ssid_char[20];
char master_password_char[20];
const String url = "http://192.168.0.1/get_config"; // Dirección del servidor

//Led indicador
const int LED_PIN = 2;
#define WIFI_DEBUG_LEVEL WIFI_LOG_VERBOSE

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

IPAddress ap_ip_local(192, 168, 0, 1);
IPAddress ap_ip_gw(192, 168, 0, 1);
IPAddress ap_subnet(255, 255, 255, 0);

// Configuración inicial de Wi-Fi en modo AP
void setup_wifi_ap() {
  WiFi.softAP(ssid_ap, password_ap);
  WiFi.softAPConfig(ap_ip_local, ap_ip_gw, ap_subnet);
  Serial.println("AP configurado");
  Serial.print("IP de AP: ");
  Serial.println(WiFi.softAPIP());
}

// Manejo del POST para "/connect_wifi"
void handle_connect_wifi() {
  String ssid;
  String password;

  Serial.println("Llega endpoint /connect_wifi:");
  String body = server.arg("plain");
  Serial.println("Cuerpo de la solicitud:");
  Serial.println(body);

  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, body);

  if (error) {
    Serial.print("Error al analizar JSON: ");
    Serial.println(error.c_str());
    server.send(400, "application/json", "{\"error\":\"JSON inválido\"}");
    return;
  }

  ssid = doc["ssid"].as<String>();
  password = doc["password"].as<String>();
  Serial.println(ssid);
  Serial.println(password);
  master_ssid = ssid;
  master_password = password;
    
  if (master_ssid.length() && master_password.length()) {
    server.send(200, "application/json", "{\"message\":\"Intentando conectar al Wi-Fi...\"}");
    Serial.println("Intentando conectar al master Wi-Fi:");
    estado = CONECTANDO;
  } else {
    server.send(400, "application/json", "{\"error\":\"Faltan parámetros 'ssid' o 'password'\"}");
    Serial.println("Faltan parámetros 'ssid' o 'password'");
    estado = DESCONECTADO;
  }
}

// Manejo del GET para "/wifi_connect_status"
void handle_wifi_status() {
  String wifi_status;

  switch (estado) {
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

  switch (estado) {
    case DESCONECTADO:
      break;
    case CONECTANDO:
      // Cambio de IP del AP antes de conectar como STA
      //WiFi.softAPConfig(ap_ip_aux, ap_ip_gw_aux, ap_subnet);
      //Serial.println("IP del AP cambiada a evitar conflictos.");
      //Serial.print("IP de AP: ");
      //Serial.println(WiFi.softAPIP());
      
      // Cerrar conexiones activas de clientes HTTP
      //server.stop();
      
      //todo probar esto
      WiFi.disconnect(false);
      delay(500);
      /////////
      
      // Configuro STA/AP
      //WiFi.mode(WIFI_AP_STA);

      // Inicio como STA
      Serial.println("Conectando...");
      
      strcpy(master_ssid_char,master_ssid.c_str());
      strcpy(master_password_char, master_password.c_str());
      WiFi.begin(master_ssid.c_str(), master_password.c_str());

      tries = 0;
      while (WiFi.status() != WL_CONNECTED && tries < 10) { // Máximo 10 intentos
        delay(1000);
        Serial.print(".");
        tries++;
        WiFi.printDiag(Serial);    
        Serial.println(WiFi.status());
      }

      // Reiniciar servidor
      //server.begin();
      //Serial.println("Servidor HTTP reiniciado.");

      //Me fijo si conectó bien
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Conexión exitosa");
        estado = VERIFICANDO;
      } else {
        Serial.println("No se pudo conectar, volviendo a DESCONECTADO");
        WiFi.disconnect(); // Desconecta si falla
        estado = DESCONECTADO;
      }
      break;

    case VERIFICANDO:
      Serial.println("Verificando modelo...");
      // Realizar la solicitud HTTP GET al servidor externo
      http.begin(url);
      httpCode = http.GET();

      //Si recibo un error salgo
      if (httpCode != 200) {
        estado = DESCONECTADO;
        WiFi.disconnect();
        Serial.println("Error al obtener configuración.");
        http.end();
        break;
      }

      //Si la solicitud fue exitosa me fijo si esta bien config
      payload = http.getString();
      Serial.println("---------------");
      Serial.println("Llega este payload:");
      Serial.println(payload);
      Serial.println("---------------");

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
        digitalWrite(LED_PIN, HIGH);
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

  // Configuro STA/AP
  WiFi.mode(WIFI_AP_STA);

  // Inicializamos el AP
  setup_wifi_ap();

  // Configurar las rutas
  server.on("/connect_wifi", HTTP_POST, handle_connect_wifi);
  server.on("/connect_wifi_status", HTTP_GET, handle_wifi_status);

  // Iniciar el servidor
  server.begin();
  Serial.println("Servidor HTTP iniciado");

  //Config led
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // Manejar las solicitudes HTTP
  server.handleClient();

  //Maquinas de estados
  maquina_estados();
}

/*
#include <WiFi.h>

// Configuración del Punto de Acceso (AP)
const char* ap_ssid = "ESP32_AP";
const char* ap_password = "12345678";

// Credenciales de la red a la que se conectará como Estación (STA)
const char* sta_ssid = "ESP32_MASTER";
const char* sta_password = "master1234";

void setup() {
  // Inicializa la comunicación serial para depuración
  Serial.begin(115200);

  // Configura el ESP32 para operar en modo AP+STA
  WiFi.mode(WIFI_AP_STA);

  // Inicia el Punto de Acceso (AP)
  WiFi.softAP(ap_ssid, ap_password);
  Serial.println("Punto de Acceso iniciado:");
  Serial.print("SSID: ");
  Serial.println(ap_ssid);
  Serial.print("IP del AP: ");
  Serial.println(WiFi.softAPIP());

  // Espera un momento antes de intentar la conexión como STA
  delay(5000);

  // Conecta a la red externa como Estación (STA)
  WiFi.begin(sta_ssid, sta_password);
  Serial.println("Conectando a la red externa...");

  // Espera hasta que la conexión se establezca
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  // Una vez conectado, muestra la IP asignada
  Serial.println("\nConexión a la red externa establecida.");
  Serial.print("IP del STA: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Aquí puedes agregar el código que desees ejecutar continuamente
}*/
