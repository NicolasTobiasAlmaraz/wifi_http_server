import requests
import time

def send_http_request(ip, path, method="POST", headers=None, body=None):
    """
    Envía una solicitud HTTP al servidor.

    :param ip: Dirección IP del servidor ESP32 (ej. "192.168.4.1").
    :param path: Path de la solicitud HTTP (ej. "/api/data").
    :param method: Método HTTP a utilizar ("GET" o "POST").
    :param headers: Diccionario con los headers HTTP.
    :param body: Diccionario o string con el cuerpo de la solicitud (para POST).
    :return: Respuesta del servidor.
    """
    url = f"http://{ip}{path}"

    try:
        print(f"Enviando solicitud {method} a: {url}")
        print(f"Headers: {headers}")
        if method.upper() == "POST":
            print(f"Body: {body}")
            response = requests.post(url, headers=headers, json=body, timeout=5)
        elif method.upper() == "GET":
            response = requests.get(url, headers=headers, timeout=5)
        else:
            print("Método HTTP no soportado. Use 'GET' o 'POST'.")
            return None
        
        print("\n\n")

        print(f"Respuesta del servidor (código {response.status_code}):")
        print(response.text)
        return response.status_code

    except requests.exceptions.RequestException as e:
        print(f"Error al enviar la solicitud: {e}")
        return None

def send_wifi_connect(ssid,psw):
    ## Configuración
    esp32_ip = "192.168.0.1"  
    path = "/connect_wifi"             
    method = "POST"           
    headers = {
        "Content-Type": "application/json",
    }
    body = {
        "ssid": ssid,
        "password": psw,
    } if method.upper() == "POST" else None    
    
    # Enviar solicitud
    send_http_request(esp32_ip, path, method=method, headers=headers, body=body)

def send_wifi_connect_status():
    ## Configuración
    esp32_ip = "192.168.0.1"  
    path = "/connect_wifi_status"             
    method = "GET"           
    headers = {
        "Content-Type": "application/json",
    }
    headers = None
    
    # Enviar solicitud
    response = send_http_request(esp32_ip, path, method=method, headers=headers, body=None)
    return response

#Envio credenciales
print("Inicio Prueba")
send_wifi_connect_status()
print("-------------------------------")

send_wifi_connect("ESP32_MASTER","master1234")
print("-------------------------------")
time.sleep(5)

#Chequeo estado cada 1 seg
for i in range(0,5):
    code = send_wifi_connect_status()
    print("-------------------------------")
    if(code==200):
        break
    time.sleep(1)

print("Fin prueba, quedó conectado!")
