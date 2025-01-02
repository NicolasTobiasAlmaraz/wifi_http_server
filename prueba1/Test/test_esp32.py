import requests

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
        
        print("\n-------------------------------------------\n")

        print(f"Respuesta del servidor (código {response.status_code}):")
        print(response.text)
        return response

    except requests.exceptions.RequestException as e:
        print(f"Error al enviar la solicitud: {e}")
        return None

## Configuración
esp32_ip = "192.168.0.1"  # Cambia esto por la IP de tu ESP32
path = "/set_led"             # Cambia esto por el path adecuado
method = "POST"            # Cambia a "GET" si es necesario.
headers = {
    "Content-Type": "application/json",
}
body = {
    "led": "2",
    "state": "0",
} if method.upper() == "POST" else None

# Enviar solicitud
send_http_request(esp32_ip, path, method=method, headers=headers, body=body)
