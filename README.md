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

Para probar a cada uno de estos endpoints se provee un cliente para utilizarse desde un script de Python.

A modo de práctica conceptual, se implementó un parser HTTTP básico para los métodos GET y POST y el header Content-Length sobre el handler TCP. 

### 🖥️Plataforma
Arduino IDE

### 🎥Funcionamiento
[![Prueba Concepto 1](https://img.youtube.com/vi/bc_M6ueIpgA/0.jpg)](https://www.youtube.com/watch?v=bc_M6ueIpgA)

---

## 🧪Prueba Concepto 2
### 📂Firmware
Código Fuente: [sources_prueba_2](https://github.com/NicolasTobiasAlmaraz/wifi_http_server/tree/main/prueba2)

### 🎯Objetivo
El objetivo de esta prueba concepto es probar el funcionamiento del ESP32 como STA/AP.

Para eso, se tendrán dos ESP32:
- ESP32 master: Es un ESP32 con el código de la ```Prueba Concepto 1`` 
- ESP32 slave: Es un ESP32 funcionando como STA/AP

El ESP32 Slave genera una red Wi-Fi y recibe solicitudes HTTP hechas por un script de Python. En el web server del ESP32 Slave se tienen los siguientes endpoints:
- POST "/connect_wifi": Se le indican las credenciales Wi-Fi del ESP32 Master
- GET "/wifi_connect_status": Verifica si ya se pudo conectar como STA a la red del ESP32 Master

La prueba será entonces hacer que el ESP32 Slave se conecte al ESP32 Master. En ese momento el ESP32 Slave se queda esperando la solicitud /wifi_connect_status para avisarle al cliente (script Python) que se conectó exitosamente.

Luego, para verificar la efectividad de la conexión, el ESP32 Slave enviará solicitudes POST /set_led, de tal modo que el led del ESP32 master cambie de estado.

### 🖥️Plataforma
Arduino IDE

### 🎥Funcionamiento
[![Prueba Concepto 2](https://img.youtube.com/vi/96kvuOUdJv8/0.jpg)](https://www.youtube.com/watch?v=96kvuOUdJv8)

