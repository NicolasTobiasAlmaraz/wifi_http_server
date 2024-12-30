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
Código Fuente: [sources_prueba_1](link)

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
Código Fuente: [sources_prueba_2](link)

### 🎯Objetivo
El objetivo de esta prueba concepto es probar la administración la conexión Wi-Fi de los ESP32 gobernadas a través de solicitudes HTTP.
Particularmente se intentará probar los endpoints:
- POST "/wifi_off": Apaga red Wi-Fi
- POST "/change_ssid": Cambia nombre de red Wi-Fi
- POST "/wifi_connect": Le das la orden de que se conecte a otra red Wi-Fi

### 🖥️Plataforma
Arduino IDE
