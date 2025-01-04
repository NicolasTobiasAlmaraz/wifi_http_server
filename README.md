# Servidor Web HTTP vía Wi-Fi
El objetivo de este repositorio es estudiar el protocolo HTTP para montar un servidor web en un ESP32.

En este repositorio encontrarán una introducción teórica al protocolo HTTP y dos pruebas concepto en el microcontrolador.

## 🧑‍💻Autor
Nicolás Almaraz

## 🌐Protocolo HTTP
Introducción teórica al protocolo HTTP: [Estudio protocolo HTTP](https://github.com/NicolasTobiasAlmaraz/wifi_http_server/blob/main/investigacion/README.md)

---
## 🧪Prueba Concepto 1
### 📂Firmware
Código Fuente: [sources_prueba_1](https://github.com/NicolasTobiasAlmaraz/wifi_http_server/tree/main/prueba1)

### 🎯Objetivo
El objetivo de la prueba 1 es diseñar una clase en C++ para el IDE de Arduino que administre la comunicación entre el sistema embebido y una interfaz Wi-Fi con el Web Server dentro.

Se tendrán los endpoints:
- Endpoint POST "/set_led": configura el estado del led indicado.
- Endpoint GET "/get_config": Retorna la configuración cargada.
- Endpoint POST "/new_ssid": Actualiza el nombre de la red Wi-Fi.

Para probar a cada uno de estos endpoints se provee un cliente para utilizarse desde un script de python

### 🖥️Plataforma
Arduino IDE

---

## 🧪Prueba Concepto 2
### 📂Firmware
Código Fuente: [sources_prueba_2](https://github.com/NicolasTobiasAlmaraz/wifi_http_server/tree/main/prueba2)

### 🎯Objetivo
El objetivo de esta prueba concepto es probar el funcionamiento del ESP32 como STA/AP, para eso tendrá el siguiente web server:
- POST "/connect_wifi": Se conecta a una red Wi-Fi
- GET "/wifi_connect_status": Verifica si ya se pudo conectar como STA a otra red y además si la red es compatible

Para esta prueba, se combina con la Prueba Concepto 1:
- ESP32_A: tiene el firmware de la prueba concepto 1
- ESP32_B: tiene el firmware de la prueba concepto 2
- 
La prueba consiste en lo siguiente:
- Mediante un cliente se le indica al ESP32_A las credenciales del ESP32_B
- El ESP32_A usa el endpoint /get_config y verifica que los parámetros sean compatibles
  - Si los parámetros son compatibles se queda conectado
  - Si los parámetros no son compatibles se desconecta
 
En todo momento puede consultarse el estado de la conexión con el endpoint /wifi_connect_status:
- Desconectado
- Conectando
- Verificando
- Conectado

### 🖥️Plataforma
Arduino IDE
