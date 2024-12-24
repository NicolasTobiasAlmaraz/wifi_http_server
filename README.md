# Servidor Web HTTP vía Wi-Fi
El objetivo de este repositorio es estudiar el protocolo HTTP para montar un servidor Web en un ESP32

## 🌐Protocolo HTTP
Introducción teórica al protocolo HTTP: [Estudio protocolo HTTP](link)

## 🧪Pruebas Concepto
### 🛠️Prueba 1
#### 📂Firmware
Código Fuente: [sources_prueba_1](link)

#### 🎯Objetivo
El objetivo de la prueba 1 es diseñar una clase en C++ para el IDE de Arduino que adminisitre la comunicación entre el sistema embebido y una interfaz Wi-Fi con el Web Server dentro.

Se tendrá un endpoint para togglear un led y otro para solicitar una medición.

#### 🖥️Plataforma
🛠️ Arduino IDE

### 🛠️Prueba 2
#### 📂Firmware
Código Fuente: [sources_prueba_2](link)

#### 🎯Objetivo
El objetivo de la prueba 2 es diseñar una el conjunto de módulos de Firmware para ESP-IDF en C que administren la red Wi-Fi y servidor HTTP.

El objetivo es comunicar a dos ESP32 entre sí. Para ello el ESP32 máster generará una red Wi-Fi y el ESP32 slve simplemente se conectará. Cada ESP32 tendrá su correspondiente servidor HTTP

#### 🖥️Plataforma
Espressif IDE

## 🧑‍💻Autor
Nicolás Almaraz
