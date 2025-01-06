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
const String url_get_config = "http://192.168.0.1/get_config";

//Led indicador
const int LED_PIN = 2;

//Web server
WebServer server(80);

//Estado maquina de estados
typedef enum {
  DESCONECTADO,
  CONECTANDO,
  CONECTADO_STA_AP,
  CONECTADO,
} estado_t;

estado_t estado = DESCONECTADO;

IPAddress ap_ip_local(192, 168, 0, 1);
IPAddress ap_ip_gw(192, 168, 0, 1);
IPAddress ap_subnet(255, 255, 255, 0);

bool flag_aviso_conexion = false;

// Configuración inicial de Wi-Fi en modo AP
void setup_wifi_ap() {
  // Configuro STA/AP
  WiFi.mode(WIFI_AP_STA);
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
    server.send(200, "application/json", "{\"message\":\"Intentando conectar al Wi-Fi\"}");
    Serial.println("Intentando conectar al master Wi-Fi:");
    estado = CONECTANDO;
  } else {
    server.send(400, "application/json", "{\"error\":\"Faltan parámetros 'ssid' o 'password'\"}");
    Serial.println("Faltan parámetros 'ssid' o 'password'");
    estado = DESCONECTADO;
  }
}

const char* wl_status_to_string(wl_status_t status) {
  switch (status) {
    case WL_NO_SHIELD: return "WL_NO_SHIELD";
    case WL_IDLE_STATUS: return "WL_IDLE_STATUS";
    case WL_NO_SSID_AVAIL: return "WL_NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED: return "WL_SCAN_COMPLETED";
    case WL_CONNECTED: return "WL_CONNECTED";
    case WL_CONNECT_FAILED: return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST: return "WL_CONNECTION_LOST";
    case WL_DISCONNECTED: return "WL_DISCONNECTED";
  }
}

//Defino callbacks
void handle_connect_wifi();
void handle_wifi_status();

void setup() {
  //Inicializo UART
  Serial.begin(115200);
  Serial.println("Prueba 2 STA/AP con HTTP bidireccional");
  
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

  //Estado inicial
  estado = DESCONECTADO;
}

void loop() {
  // Manejar las solicitudes HTTP
  server.handleClient();

  //Maquinas de estados
  maquina_estados();
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
    case CONECTADO:
      wifi_status = "CONECTADO APAGANDO WIFI";
      flag_aviso_conexion = true;
      break;
    case CONECTADO_STA_AP:
      wifi_status = "CONECTADO";
      flag_aviso_conexion = true;
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
  StaticJsonDocument<200> doc;
  HTTPClient http;
      
  switch (estado) {
    case DESCONECTADO:
      break;
    case CONECTANDO:
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
        Serial.println(wl_status_to_string(WiFi.status()));
      }

      //Me fijo si conectó bien
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Conexión exitosa");
        Serial.print("IP de STA: ");
        Serial.println(WiFi.localIP());
        estado = CONECTADO_STA_AP;
        flag_aviso_conexion = false;
      }
      break;

    case CONECTADO_STA_AP:
      if(flag_aviso_conexion) {
        estado = CONECTADO;
        delay(500);
        server.stop();
        WiFi.mode(WIFI_STA);
        delay(500);
      }
      break;
    
    case CONECTADO:
      if (WiFi.status() != WL_CONNECTED) {
        WiFi.disconnect();
        delay(100);
        setup();
        break;
      }
    
      static bool led_status = true;
      led_status = !led_status; // Alterna el estado del LED

      // Configurar HTTP POST
      http.begin("http://192.168.0.1:80/set_led");
      http.addHeader("Content-Type", "application/json");

      // Crear el JSON
      doc["led"] = 2;
      doc["state"] = led_status;
      String json_payload;
      serializeJson(doc, json_payload);

      // Enviar la solicitud POST
      httpCode = http.POST(json_payload);

      // Verificar respuesta
      Serial.printf("POST enviado. Código de respuesta: %d\n", httpCode);  
      http.end();
      sleep(1); // Pausa entre envíos
      break;
  }
}
