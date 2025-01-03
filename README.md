# Servidor Web HTTP vía Wi-Fi
El objetivo de este repositorio es estudiar el protocolo HTTP para montar un servidor web en un ESP32.

En este repositorio encontrarán una introducción teórica al protoclo HTTP y dos pruebas concepto en el microcontrolador.

## 🧑‍💻Autor
Nicolás Almaraz

## 🌐Protocolo HTTP
Introducción teórica al protocolo HTTP: [Estudio protocolo HTTP](https://github.com/NicolasTobiasAlmaraz/wifi_http_server/blob/main/investigacion/README.md)

---
## 🧪Prueba Concepto 1
### 📂Firmware
Código Fuente: [sources_prueba_1](https://github.com/NicolasTobiasAlmaraz/wifi_http_server/tree/main/prueba1)

### 🎯Objetivo
El objetivo de la prueba 1 es diseñar una clase en C++ para el IDE de Arduino que adminisitre la comunicación entre el sistema embebido y una interfaz Wi-Fi con el Web Server dentro.

Se tendrán dos endpoints:
- Endpoint POST "/set_led": setea el estado del led indicado.
- Endpoint GET "/get_config": Retorna la configuración cargada.

### 🖥️Plataforma
Arduino IDE

---

## 🧪Prueba Concepto 2
### 📂Firmware
Código Fuente: [sources_prueba_2](https://github.com/NicolasTobiasAlmaraz/wifi_http_server/tree/main/prueba2)

### 🎯Objetivo
El objetivo de esta prueba concepto es probar el funcionamiento del ESP32 como STA/AP, para eso temdrá el siguiente web server:
- POST "/connect_wifi": Se conecta a una red Wi-Fi
- GET "/wifi_connect_status": Verifica si ya se pudo conectar como STA a otra red

El objetivo es que se conecte a un AP brindado por el cliente. Por otro lado, tendrá un endpoint al cual le consultarán si pudo conectarse o no.

### 🖥️Plataforma
Arduino IDE
