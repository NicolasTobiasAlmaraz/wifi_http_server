# **Protocolo HTTP**

[**Overview de HTTP**](#overview-de-http)
- [Características Generales](#características-generales)
- [Introducción Conceptual](#introducción-conceptual)
- [Proxies](#proxies)
  - [Analogía](#analogía)
  - [Funciones generales](#funciones-generales)
- [Cliente: el user-agent](#cliente:-el-user-agent)
- [El web server](#el-web-server)
- [Características de HTTP](#características-de-http)
  -[Simplicidad](#simplicidad)
  - [Extensible](#extensible)
  - [Sin estado pero con sesión](#sin-estado-pero-con-sesión)
  - [HTTP y las conexiones](#http-y-las-conexiones)
- [Qué se puede controlar con HTTP](#qué-se-puede-controlar-con-http)
  - [Memoria Caché](#memoria-caché)
  - [Relajar las restricciones de origen](#relajar-las-restricciones-de-origen)
  - [Autenticación](#autenticación)
  - [Proxy y tunneling](#proxy-y-tunneling)
  - [Sesiones](#sesiones)
- [Flujo HTTP](#flujo-http)
- [Mensajes HTTP](#mensajes-http)
  - [Request](#request)
  - [Response](#response)
- [APIs basadas en HTTP](#apis-basadas-en-http)
  - [API REST](#api-rest)
  - [Principios REST](#principios-rest)
    - [1 \- Interfaz uniforme](#1---interfaz-uniforme)
    - [2 \- Capacidad de almacenamiento en caché](#2---capacidad-de-almacenamiento-en-caché)
    - [3 \- Sin estado](#3---sin-estado)
    - [4 \- Desacoplamiento cliente-servidor](#4---desacoplamiento-cliente-servidor)
    - [5 \- Arquitectura de sistema en capas](#5---arquitectura-de-sistema-en-capas)
    - [6 \- Código bajo demanda (opcional)](#6---código-bajo-demanda-\(opcional\))
  - [Funcionamiento](#funcionamiento)

[**URIs**](#uris)
- [URLs](#urls)
- [URNs](#urns)
- [Sintaxis de las URL](#sintaxis-de-las-url)
  - [Scheme](#scheme)
  - [Authority](#authority)
  - [Path](#path)
  - [Query](#query)
  - [Fragment](#fragment)

[**Sesión HTTP típica**](#sesión-http-típica)
- [Definición de sesión](#definición-de-sesión)
- [Establecimiento de la conexión y mensajes HTTP](#establecimiento-de-la-conexión-y-mensajes-http)
- [Mensajes HTTP](#mensajes-http-1)
- [Estructura de la solicitud del cliente (Request)](#estructura-de-la-solicitud-del-cliente-\(request\))
  - [Request Line](#request-line)
  - [Headers (opcionales)](#headers-\(opcionales\))
  - [Línea vacía (\\r\\n)](#línea-vacía-\(\\r\\n\))
  - [Body (opcional)](#body-\(opcional\))
  - [Ejemplos](#ejemplos)
    - [Ejemplo 1](#ejemplo-1)
      - [Request Line](#request-line)
      - [Headers](#headers)
      - [Body](#body)
    - [Ejemplo 2](#ejemplo-2)
      - [Request Line](#request-line-1)
      - [Headers](#headers-1)
      - [Body](#body-1)
    - [Resumiendo](#resumiendo)
- [Estructura de la respuesta del servidor (Response)](#estructura-de-la-respuesta-del-servidor-\(response\))
  - [Status Line](#status-line)
  - [Headers (opcionales)](#headers-\(opcionales\)-1)
  - [Línea vacía (\\r\\n)](#línea-vacía-\(\\r\\n\)-1)
  - [Body](#body)
  - [Ejemplos](#ejemplos-1)
    - [Ejemplo 1](#ejemplo-1-1)
    - [Ejemplo 2](#ejemplo-2-1)
    - [Ejemplo 3](#ejemplo-3)

[**MIME Types**](#mime-types)
- [Tipos Discretos](#tipos-discretos)
- [Tipos Múltiples](#tipos-múltiples)
- [Los tipos más importantes para desarrolladores web](#los-tipos-más-importantes-para-desarrolladores-web)
- [La importancia de setear bien el MIME type](#la-importancia-de-setear-bien-el-mime-type)
- [MIME Sniffing](#mime-sniffing)
- [Ejemplo](#ejemplo)
- [Otros métodos para indicar el tipo de documento](#otros-métodos-para-indicar-el-tipo-de-documento)

[**Resumen Práctico de Headers**](#resumen-práctico-de-headers)
- [Headers de uso exclusivo en solicitudes (Request Headers)](#headers-de-uso-exclusivo-en-solicitudes-\(request-headers\))
- [Headers de uso exclusivo en respuestas (Response Headers)](#headers-de-uso-exclusivo-en-respuestas-\(response-headers\))
- [Headers usados en ambos contextos (Request y Response)](#headers-usados-en-ambos-contextos-\(request-y-response\))
- [Contexto mixto: detalles importantes](#contexto-mixto:-detalles-importantes)

[**Resumen Práctico de Métodos**](#resumen-práctico-de-métodos)
- [Idempotencia en los Métodos HTTP](#idempotencia-en-los-métodos-http)
- [Principales Métodos HTTP y su Propósito](#principales-métodos-http-y-su-propósito)
- [Métodos Cacheables](#métodos-cacheables)
  - [Beneficios del almacenamiento en caché](#beneficios-del-almacenamiento-en-caché)
  - [Limitaciones del almacenamiento en caché](#limitaciones-del-almacenamiento-en-caché)
- [Ejemplos](#ejemplos-2)

[**Resumen Práctico de Status Codes**](#resumen-práctico-de-status-codes)
- [1xx \- Informativos](#1xx---informativos)
- [2xx \- Éxito](#2xx---éxito)
- [3xx \- Redirección](#3xx---redirección)
- [4xx \- Errores del Cliente](#4xx---errores-del-cliente)
- [5xx \- Errores del Servidor](#5xx---errores-del-servidor)

[**Administración de la conexión en HTTP/1.x**](#administración-de-la-conexión-en-http/1.x)
- [Conexiones cortas](#conexiones-cortas)
- [Conexiones persistentes](#conexiones-persistentes)
- [Conexiones pipelining](#conexiones-pipelining)

[**Características adicionales de HTTP**](#características-adicionales-de-http)
- [HTTP Caching](#http-caching)
  - [¿Cómo funciona el caching?](#¿cómo-funciona-el-caching?)
    - [Encabezados de respuesta del servidor](#encabezados-de-respuesta-del-servidor)
    - [Revalidación](#revalidación)
  - [Ventajas](#ventajas)
  - [Ejemplo de encabezados de caché](#ejemplo-de-encabezados-de-caché)
- [HTTP Authentication](#http-authentication)
  - [Tipos comunes de autenticación](#tipos-comunes-de-autenticación)
    - [Basic Authentication](#basic-authentication)
      - [Ejemplo](#ejemplo)
    - [Bearer Token](#bearer-token)
      - [Ejemplo](#ejemplo-1)
    - [Digest Authentication](#digest-authentication)
      - [Flujo típico](#flujo-típico)
      - [Ventajas](#ventajas-1)
      - [Ejemplo](#ejemplo-2)
- [HTTP Cookies](#http-cookies)
  - [Características](#características)
  - [Encabezados principales](#encabezados-principales)
    - [Set-Cookie](#set-cookie)
    - [Ejemplo](#ejemplo-3)
  - [Cookie](#cookie)
    - [Ejemplo](#ejemplo-4)
    - [Ventajas](#ventajas-2)
- [Proxies y Tunneling](#proxies-y-tunneling)
  - [Proxies](#proxies)
    - [Proxy directo](#proxy-directo)
      - [Ejemplo](#ejemplo-5)
    - [Proxy inverso](#proxy-inverso)
  - [Tunneling](#tunneling)
    - [Ejemplo de solicitud de túnel](#ejemplo-de-solicitud-de-túnel)
    - [Ventajas](#ventajas-3)
- [Compresión](#compresión)
  - [¿Cómo funciona la compresión en HTTP?](#¿cómo-funciona-la-compresión-en-http?)
  - [Ejemplo de solicitud con compresión](#ejemplo-de-solicitud-con-compresión)
  - [Ventajas de la compresión](#ventajas-de-la-compresión)
  - [Limitaciones de la compresión](#limitaciones-de-la-compresión)
- [Negociación en HTTP](#negociación-en-http)
  - [Tipos de negociación en HTTP](#tipos-de-negociación-en-http)
  - [Ejemplo de negociación de contenido](#ejemplo-de-negociación-de-contenido)
  - [Ejemplo de negociación condicional](#ejemplo-de-negociación-condicional)
  - [Relación entre Compresión y Negociación](#relación-entre-compresión-y-negociación)

[**Evolución de HTTP**](#evolución-de-http)
- [1989-1990: Creación de la World Wide Web](#1989-1990:-creación-de-la-world-wide-web)
- [1991: Primer documento HTTP](#1991:-primer-documento-http)
- [1996: HTTP/1.0 (RFC 1945\)](#1996:-http/1.0-\(rfc-1945\))
- [1997: HTTP/1.1 (RFC 2068 y RFC 2616 en 1999\)](#1997:-http/1.1-\(rfc-2068-y-rfc-2616-en-1999\))
- [2005-2006: Creciente adopción de HTTPS como estándar](#2005-2006:-creciente-adopción-de-https-como-estándar)
- [2015: HTTP/2 (RFC 7540\)](#2015:-http/2-\(rfc-7540\))
- [2018: Desarrollo de HTTP/3](#2018:-desarrollo-de-http/3)
- [2020: Lanzamiento oficial de HTTP/3 (RFC 9114 en 2022\)](#2020:-lanzamiento-oficial-de-http/3-\(rfc-9114-en-2022\))
- [Resumen general de versiones](#resumen-general-de-versiones)

[**Bibliografía**](#bibliografía)

# **Protocolo HTTP**

En este documento se exploran los fundamentos del protocolo HTTP, con un enfoque particular en su implementación dentro de un servidor web para un sistema embebido. Por eso se hará foco en HTTP 1.x.

Aunque HTTP es un estándar ampliamente utilizado en Internet, las condiciones en las que opera en dispositivos con recursos de hardware limitados, como los microcontroladores, presentan desafíos únicos. Mientras que en un entorno de servidores tradicionales los recursos disponibles permiten una amplia gama de características y optimizaciones, en un sistema embebido las limitaciones de memoria, procesamiento y ancho de banda requieren una aproximación más austera y optimizada.

A lo largo de este informe, se hará un recorrido por los aspectos esenciales del protocolo HTTP, centrándose en aquellos elementos que son clave para el desarrollo de aplicaciones en sistemas embebidos. Se dará especial atención a las características más simples y necesarias para la comunicación básica, omitiendo funciones avanzadas que, aunque son parte del estándar en servidores tradicionales, no son viables dentro del entorno de los microcontroladores debido a las restricciones de recursos.

El objetivo principal de este estudio es proporcionar una comprensión clara de los principios básicos de HTTP y cómo se pueden adaptar e implementar dentro de los sistemas embebidos, garantizando un rendimiento adecuado sin comprometer las capacidades del dispositivo. Este enfoque permitirá que futuros desarrolladores que trabajen con sistemas embebidos puedan comprender y aplicar el protocolo HTTP de manera eficiente, considerando las limitaciones inherentes a su hardware.

# **Overview de HTTP**

## **Características Generales**

* Es un protocolo para la transferencia de documentos hipermedia, por ejemplo HTML.  
* Comunicación entre buscadores y servidores  
* Comunicación M2M  
* Acceso a servidor mediante APIs  
* Topología cliente servidor, en donde el cliente inicia la comunicación mediante un request y se queda esperando el response del servidor  
* Es un protocolo sin estado (sin memoria), es decir, el servidor no recuerda qué solicitudes le hicieron previamente. Sin embargo, más adelante se añadió el uso de Cookies que le pone ciertos matices a este punto.

## **Introducción Conceptual**

HTTP es un protocolo en donde se tiene un servidor quien está atento a las solicitudes que tengan los clientes. Este es el clásico esquema cliente servidor.

Por lo tanto, los clientes inician la comunicación haciendo una solicitud o request y el servidor envía su respuesta o response.

Por ejemplo, desde un buscador se intenta acceder a una página web.  
Entonces, el proceso sería:

- El buscador envía una solicitud HTTP al servidor (Request)  
- El servidor retorna la página web solicitada (Response)

Es decir, nunca el servidor inicia la comunicación, siempre es el cliente.  
![][image1]  
En cuanto a la capa de red, el protocolo HTTP corre sobre *TCP* o *TCP con encriptado TLS*. Sin embargo, teóricamente podría implementarse con cualquier protocolo de transporte que sea confiable.  
![][image2]

## **Proxies**

El protocolo HTTP tiene un esquema cliente servidor. Sin embargo, muchas veces no es tan simple como esto, sino que hay componentes intermedios llamados de forma colectiva “proxies”. Estos pueden tener diferentes funcionalidades, por ejemplo, gateways o memoria caché.  
![][image3]  
Los servidores proxies (o simplemente proxies), son programas intermedios entre el cliente  y el servidor. Estos facilitan la conexión a la World Wide Web. Estos proxies interceptan el request del cliente y retorna los responses. puede reenviar peticiones o no (por ejemplo los caché) y puede modificarlas (por ejemplo, cambiando sus encabezados).

Físicamente pueden estar tanto de manera local en el cliente o bien en cualquier punto intermedio entre el cliente y el servidor. Hay dos tipos de proxies principales:

- Proxy directo: toma los request de los clientes y los redirige hacia un servidor en internet protegiendo la identidad del cliente.  
- Proxy invertido: toma los request del cliente y los redirige a servidores internos (que no están en internet, sino en una red interna), de esta manera el cliente no sabe la identidad de los servidores con los que habla.


| Característica | Forward Proxy | Reverse Proxy |
| ----- | ----- | ----- |
| **Usuarios principales** | Clientes (usuarios finales) | Servidores (backend) |
| **Dirección del tráfico** | Cliente → Proxy → Servidor | Cliente → Proxy → Servidores internos |
| **Propósito principal** | Proteger y gestionar las solicitudes de clientes | Proteger y gestionar los servidores backend |
| **Oculta** | La identidad del cliente | La identidad de los servidores |

#### Analogía

- Proxy directo: Es como si enviaras una carta a alguien a través de un amigo (el proxy). El destinatario (servidor) no sabe quién eres realmente, pero tú sabes dónde enviar la carta. Generalmente, estos se utilizan también para filtrar qué tipos de clientes pueden acceder a ciertas solicitudes.

- Proxy inverso: Es como enviar una carta a una oficina central (el reverse proxy) de una gran empresa. Tú no sabes quién dentro de la empresa responderá, pero la oficina central se encarga de entregar la carta a la persona correcta y luego responderte.

#### Funciones generales

- Caching: el cache puede ser público o privado, como el caché del buscador  
- Filtrado: Como un antivirus  
- Carga balanceada: Para permitir a múltiples servidores atender a diferentes request.  
- Autenticación: Para control de acceso a ciertos recursos  
- Logging: Permitir el almacenamiento de historiales de búsqueda

Sin embargo, el uso de proxies tiene sentido para cuando se tiene el  servidor conectado a internet, esto no será nuestro caso.

## **Cliente: el user-agent**

El user-agent es cualquier herramienta con la que interactúa directamente el usuario. Su principal formato son los buscadores. Sin embargo, podría ser un programa que utilizan ingenieros para debug.

Los buscadores web y páginas web son páginas de hipertexto y es una característica principal del user-agent. Ya que de una página me pueden redirigir a otra cuando hago clic en alguna parte. Esto en definitiva lo que hace es enviar otra solicitud HTTP a un servidor.

## **El web server**

El web server es una pieza de software que es consultada por los clientes. Ahora bien este web server puede estar asociado a más de un servidor físico, incluso puede estar asociado a otro tipo de softwares como bases de datos, caches o servidores e-commerce.

Un servidor no necesariamente es una única máquina individual. Sin embargo, dentro de una misma máquina puede haber múltiples web servers. Eso se puede hacer mediante HTTP/1.1 que permite identificar diferentes puertos para una misma dirección IP.

## **Características de HTTP**

### **Simplicidad**

Cada solicitud y respuesta es de fácil lectura e interpretación por humanos

### **Extensible**

Tiene headers (o encabezados) que permiten extender el protocolo añadiendo normas en cada solicitud que deben respetar tanto el cliente como el servidor.

### **Sin estado pero con sesión**

Es sin estado, ya que no recuerda cuáles fueron las peticiones anteriores. Sin embargo, con el uso de Cookies se permiten crear sesiones. De esta manera, cuando un cliente hace un request, el servidor envia el response pero además envía una Cookie. El concepto es que ahora el usuario tiene una cookie (memoria). Generalmente estas cookies se almacenan en el navegador y sirven para guardar personalizaciones, números de ID, etc.

### **HTTP y las conexiones**

Una conexión es controlada en la capa de transporte y por lo tanto, no entra en el foco de HTTP. Es decir, HTTP necesita una capa de transporte que sea fiable, es decir, que no pierda mensajes y que en caso de hacerlo que lo reporte. HTTP. Por eso HTTP está basado en el protocolo TCP. Sin embargo, el hecho de que use TCP no implica que necesite usar la característica de TCP de connection-based (conexión permanente).

Antes de que el cliente y el servidor hagan el intercambio de mensajes request/response, se tiene que establecer la conexión TCP. El comportamiento por default es abrir una conexión TCP separada para cada par request/response HTTP. Esto es menos eficiente que compartir una única conexión TCP cuando hay múltiples solicitudes seguidas.

Para eso es que se utiliza el llamado pipelining. Esto está estandarizado en HTTP/1.1 (aunque es difícil de implementar) mediante la cabecera “connection”. Esto permite usar la misma conexión TCP para varias solicitudes / respuestas.

HTTP/2 permite multiplexar mensajes sobre una sola conexión, lo que mejora la performance. Esto permite que varias solicitudes y respuestas se envíen al mismo tiempo sin que se choquen entre sí.

## **Qué se puede controlar con HTTP**

Para tener control sobre la Web y hacer que los sistemas funcionen más eficientemente HTTP permite controlar ciertos parámetros del sistema. Por eso existen los llamados métodos de autenticación y memoria caché

### **Memoria Caché**

Los caché son fracciones de la solicitud que pueden almacenarse y no es necesario que todo el tiempo lo estén consultando. Por ejemplo, el css de una página web raramente cambie, por lo tanto, no tiene sentido solicitarlo todo el tiempo. HTTP permite indicar a los clientes / proxies por cuanto tiempo almacenar ciertos datos en memoria caché y además en el response puede indicarse que deben ignorar el caché antiguo (en consecuencia deben solicitar esa información antigua nuevamente así se actualiza).

### **Relajar las restricciones de origen**

Para prevenir que te espíen y otras invasiones de privacidad, los buscadores web exigen una separación estricta entre páginas web de diferente origen. Es decir, solo las páginas de un mismo origen pueden intercambiar información entre sí. Sin embargo, puede relajarse esta política mediante el uso de cabeceras HTTP especiales y un mecanismo llamado CORS.

### **Autenticación**

Algunas páginas pueden estar protegidas,por lo que, solo usuarios específicos pueden accederlas. La autenticación básica puede ser proveída por HTTP, por ejemplo usando las cabeceras www-authenticate, o utilizando cookies HTTP. 

Estas últimas son una “memoria” que el servidor le envía al cliente luego de hacer alguna solicitud. En dicha cookie se guarda información del usuario que sirve para las próximas autenticaciones y por cuánto tiempo. Por ejemplo, cuando iniciamos sesión en una página web, si cerramos la pestaña y luego la volvemos a abrir, probablemente ya nos guarde como registrado. Eso se debe a que quedó guardado el token de la sesión en una cookie (que al pasar cierto tiempo se vence).

### **Proxy y tunneling**

Como ya se mencionó los proxies permiten garantizar seguridad y filtrar información. Por otro lado, un túnel es una técnica que encapsula datos de un protocolo dentro de otro para permitir que los datos viajen a través de una red que podría no ser adecuada para esos datos originalmente. A través de un túnel, un cliente puede acceder a recursos de una red interna o externa de manera segura, especialmente cuando se atraviesan redes intermedias. Un protocolo que permite utilizar esta técnica de tunneling es SOCKS que puede manejar protocolos como FTP o POP3. Además los túneles sirven para crearle un “blindaje” al canal de comunicación. Por ejemplo con SSH

### **Sesiones**

El uso de cookies HTTP permite vincular las peticiones con el estado del servidor. Esto crea sesiones, a pesar de que el protocolo HTTP básico es un protocolo sin estado. Sin embargo, esto es útil para el uso de sesiones, al igual que en el ejemplo que se dió en autenticación.

## **Flujo HTTP**

Cuando un cliente quiere comunicarse con un servidor, ya sea un proxy o el servidor, ocurren los siguientes pasos:

1. Se abre una conexión TCP, sobre la cual se hará una o varias solicitudes y se recibirá una respuesta. El cliente puede abrir una nueva conexión, reutilizar una existente o abrir varias.  
2. Enviar un mensaje HTTP. Puede ser HTTP/1.x o HTTP/2. Por ejemplo:
```
GET / HTTP/1.1
Host: developer.mozilla.org
Accept-Language: fr
```
3. Leer la respuesta del servidor. Por ejemplo
```
HTTP/1.1 200 OK
Date: Sat, 09 Oct 2010 14:28:02 GMT
Server: Apache
Last-Modified: Tue, 01 Dec 2009 20:18:22 GMT
ETag: "51142bc1-7449-479b075b2891b"
Accept-Ranges: bytes
Content-Length: 29769
Content-Type: text/html

\<\!doctype html\>… (here come the 29769 bytes of the requested web page)
```

4. Cerrar o reutilizar la conexión para próximos requests

Si está activado HTTP con pipelines, pueden ser enviados varios requests sin esperar la respuesta del primero. Sin embargo, es una técnica obsoleta. Para ese tipo de solicitudes se utiliza HTTP/2, cosa que excede para el servidor de un sistema embebido, tiene sentido en aplicaciones Web .

## **Mensajes HTTP**

Los mensajes HTTP/1.1 son legibles para humanos.  
Los HTTP/2 están embebidos en una estructura que permite multiplexarlos. Sin embargo, conceptualmente son iguales.

### **Request**

En las solicitudes (o request) hay que especificar:

- Método HTTP: define el tipo de accionamiento que se pretende sobre el servidor. Usualmente es un verbo como GET o POST. Aunque también puede haber otras opciones como un sustantivo, por ejemplo, OPTIONS o HEAD.  
- Path: es la parte de la URL que le sigue al dominio. Por ejemplo [http://ejemplo.com/path](http://ejemplo.com/path). En este caso el path es justamente “/path”. En caso de haber puerto, no es parte del path  
- Versión del protocolo HTTP: Por ejemplo HTTP/1.1  
- Headers (opcionales), le brinda información extra al servidor  
- Body (opcional), en caso de que la solicitud necesite enviar algún dato, se envía en el body

Nota: En este caso el path sería el directorio “root” del dominio  
![][image4]

### **Response**

En la respuesta van los siguientes elementos:

- Versión del protocolo HTTP  
- Status code: Es un número que representa el código de salida de la solicitud, indican información de cómo salió la petición  
- Mensaje de status: Descripción corta del status code  
- Headers (opcionales)  
- Body (opcional): Contiene los datos solicitados en caso de ser necesarios.

![][image5]

## **APIs basadas en HTTP**

La mayoría de APIs HTTP están basadas en Fetch APIs, las cuales pueden ser usadas para hacer HTTP request desde scripts por ejemplo en JavaScript. Estas reemplazan a las XMLHttpRequest APIs.

Las APIs REST son un caso particular de las API HTTP. En definitiva, las APIs HTTP que cumplen con los principios REST se consideran APIs REST y son las más utilizadas

### **API REST**

Las API REST (Representational State Transfer) proporcionan una forma flexible y ligera de integrar aplicaciones y conectar componentes en arquitecturas de microservicios. Se entiende a un microservicio como a un recurso de la nube que es independiente a toda la solución completa. Este microservicio tiene alta cohesión y bajo acoplamiento.

Cada uno de estos micro-servicios puede ser utilizado mediante APIs, entre otros recursos.

#### Principios REST

Las API REST pueden ser desarrolladas en cualquier lenguaje de programación y admitir una variedad de formatos de datos. El único requisito es que se alineen con estos seis principios de diseño REST, también conocidos como restricciones arquitectónicas:

##### 1 \- Interfaz uniforme

Todas las solicitudes de API para el mismo recurso deben tener el mismo aspecto, independientemente de su procedencia. La API REST debe garantizar que el mismo dato, como el nombre o la dirección de correo electrónico de un usuario, pertenezca a un solo identificador de recursos uniforme (URI). Los recursos no deben ser demasiado grandes, pero deben contener toda la información que el cliente pueda necesitar.

##### 2 \- Capacidad de almacenamiento en caché

Cuando sea posible, los recursos deben poder almacenarse en caché en el lado del cliente o del servidor. Las respuestas del servidor también deben contener información sobre si se permite el almacenamiento en caché para el recurso entregado. El objetivo es mejorar el rendimiento en el lado del cliente y, al mismo tiempo, aumentar la escalabilidad en el lado del servidor.

##### 3 \- Sin estado

Las API REST no tienen estado, lo que significa que cada solicitud debe incluir toda la información necesaria para procesarla. En otras palabras, las API REST no requieren ninguna sesión del lado del servidor. Las aplicaciones de servidor no pueden almacenar ningún dato relacionado con una solicitud de cliente.

##### 4 \- Desacoplamiento cliente-servidor

En el diseño de una API REST, las aplicaciones cliente y servidor deben ser completamente independientes entre sí. La única información que debe conocer la aplicación cliente es el URI del recurso solicitado; no puede interactuar con la aplicación servidor de ninguna otra forma. Del mismo modo, una aplicación servidor no debería modificar la aplicación cliente más allá de pasarle los datos solicitados a través de HTTP.

##### 5 \- Arquitectura de sistema en capas

En las API REST, las llamadas y respuestas pasan por diferentes capas. Como regla general, no asuma que las aplicaciones cliente y servidor se conectan directamente entre sí. Puede haber varios intermediarios diferentes en el circuito de comunicación. Las API REST deben diseñarse de tal manera que ni el cliente ni el servidor puedan saber si se comunica con la aplicación final o con un intermediario.

##### 6 \- Código bajo demanda (opcional)

Las API REST suelen enviar recursos estáticos, pero, en algunos casos, las respuestas también pueden contener código ejecutable (como applets de Java). En estos casos, el código solo debe ejecutarse bajo demanda.

#### Funcionamiento

Las API REST se comunican a través de solicitudes HTTP para realizar funciones estándar de bases de datos, como crear, leer, actualizar y eliminar registros (también conocido como CRUD) dentro de un recurso.

Por ejemplo, una API REST usaría una solicitud GET para recuperar un registro. Una solicitud POST crea un nuevo registro. Una solicitud PUT actualiza un registro y una solicitud DELETE lo elimina. Todos los métodos HTTP se pueden utilizar en las llamadas a la API. Una API REST bien diseñada es similar a un sitio web que se ejecuta en un navegador web con funcionalidad HTTP incorporada.

El estado de un recurso en cualquier instante particular, o marca temporal, se conoce como representación de recursos. Esta información se puede entregar a un cliente en prácticamente cualquier formato, incluyendo JavaScript Object Notation (JSON), HTML, XLT, Python, PHP o texto plano. JSON es popular porque es legible tanto por humanos como por máquinas, y es independiente del lenguaje de programación.

Los encabezados y parámetros de solicitud también son importantes en las llamadas a la API REST, porque incluyen información importante del identificador, como metadatos, autorizaciones, identificadores de recursos uniformes (URI), almacenamiento en caché, cookies y más. Los encabezados de solicitud y los encabezados de respuesta, junto con los códigos de estado HTTP convencionales, se utilizan dentro de las API REST bien diseñadas.

# **URIs**

Los Uniform Resource Identifiers (URI) son utilizados para identificar recursos en la web. Estos son comúnmente usados como etiquetas en las solicitudes HTTP, donde representan la ubicación física de un recurso, por ejemplo, un documento, una foto, un archivo, etc.  
Por otro lado, los URIs también pueden estar asociados a comportamientos, por ejemplo, abrir el mail de un cliente, enviar mensajes de texto o ejecutar un script al hacer clic. A continuación se detallan los URIs más importantes:

- URL: Uniform Resource Locator  
- URN: Uniform Resource Name

## **URLs**

Los URLs, más conocidos como links de una página web, son el tipo de URI más conocido. Por ejemplo:

* [https://developer.mozilla.org](https://developer.mozilla.org)  
* [https://developer.mozilla.org/en-US/docs/Learn\_web\_development/](https://developer.mozilla.org/en-US/docs/Learn_web_development/)   
* [https://developer.mozilla.org/en-US/search?q=URL](https://developer.mozilla.org/en-US/search?q=URL)

Sin embargo, una URL puede estar compuesta por diferentes partes, donde algunas son obligatorias y otras son opcionales. Una URL más compleja se puede ver como:  
[http://www.example.com:80/path/to/myfile.html?key1=value1\&key2=value2\#SomewhereInTheDocument](http://www.example.com:80/path/to/myfile.html?key1=value1&key2=value2#SomewhereInTheDocument) 

## **URNs**

Los URNs son un URI que identifica un recurso por su nombre en un namespace particular.  
Por ejemplo:

* urn:isbn:9780141036144  
* urn:ietf:rfc:7230

El primero corresponde al libro 1984 de George Orwell dentro del ISBN (Número Internacional Normalizado del Libro).

El segundo corresponde la especificación 7230 de la norma IETF: “Hypertext Transfer Protocol (HTTP/1.1): Message Syntax and Routing.”

Estos códigos como se puede ver son internos a un grupo específico.

## **Sintaxis de las URL**

Dentro del URI se deben especificar:

- Scheme  
- Authority  
- Path  
- Query  
- Fragment

Para su sintaxis se pone el siguiente ejemplo:

[http://www.example.com:80/path/to/myfile.html?key1=value1\&key2=value2\#SomewhereInTheDocument](http://www.example.com:80/path/to/myfile.html?key1=value1&key2=value2#SomewhereInTheDocument) 

### **Scheme**

Indica el protocolo utilizado para la transferencia de datos. La sintaxis es:
```
scheme:
```

En general puede ser alguno de los siguientes: blob, data, file, ftp, http, https, javascript, milato, resource, ssh, tel, urn, view-resource, ws/wss  
En nuestro ejemplo:  
![][image6]

### **Authority**

La autoridad está compuesta por:

- Información del usuario (opcional): Información de autenticación de usuario, en general no se pone.  
- Host: Generalmente es el nombre del dominio  
- Port: Número de puerto donde el servidor atiende las solicitudes, dependiendo del protocolo es posible que tenga valores por defecto y no sea necesario indicarlo

La sintaxis, dependiendo cómo se use, será alguna de las siguientes:

```
host host:port user@host user@host:port
```

En nuestro ejemplo:  
![][image7]![][image8]

### **Path**

El path es la parte de la URL que indica la ubicación del recurso dentro del servidor. En las versiones más antiguas de los servidores, era literalmente la ubicación dentro del servidor físico. Sin embargo, hoy en día es algo simbólico pero muy útil para organizar la información

```
/path/to/myfile.html
```

En nuestro ejemplo:

![][image9]

### **Query**

Son parámetros extra en donde se le indica al servidor algún tipo de información extra. Este campo es opcional. Estos parámetros se indican en el formato “key/value”. En general se utilizan para identificar más fácilmente el recurso solicitado. Sintaxis:

```
?key1=value1\&key2=value2 ?: Inicia sesión query llave=valor &: separa los pares llave/valor
```

En nuestro ejemplo:  
![][image10]

### **Fragment**

El fragment es un campo que indica si hay que recuperar una parte o sección específica del recurso solicitado. En general pueden ser, anclajes a alguna sección de una página web por ejemplo. Aunque puede ser también una marca del minuto del video al que hay que ir.  
Sintaxis:

```
\#fragment
```

En nuestro ejemplo:  
![][image11]

# **Sesión HTTP típica**

## **Definición de sesión**

Se define sesión al proceso que ocurre entre la conexión entre el cliente y el servidor y su desconexión.

Para el caso de HTTP, la sesión consiste en las siguientes tres etapas:

1. El cliente establece el canal TCP  
2. El cliente envía un request.  
3. El servidor envía un response.

Dado que un mismo canal TCP puede reutilizarse, es posible que se reitere múltiples veces los pasos 2 y 3\.

## **Establecimiento de la conexión y mensajes HTTP**

El cliente inicia la conexión estableciendo un enlace TCP en la capa de transporte.  
Por default, el puerto TCP que utiliza HTTP es el puerto 80, aunque se puede utilizar cualquiera.  
La URL de una página web tiene tanto el dominio como el puerto. Sin embargo, en HTTP si el puerto es puerto 80 puede omitirse la especificación.

> Note: The client-server model does not allow the server to send data to the client without an explicit request for it. However, various Web APIs enable this use case, including the [Push API](https://developer.mozilla.org/en-US/docs/Web/API/Push_API), [Server-sent events](https://developer.mozilla.org/en-US/docs/Web/API/Server-sent_events), and the [WebSockets API](https://developer.mozilla.org/en-US/docs/Web/API/> WebSockets_API).

## **Mensajes HTTP**

Una vez que se estableció la conexión TCP, el “user-agent” está habilitado a hacer las peticiones que desee.

Para intercambiar información entre el cliente y el servidor hay dos tipos de mensajes:

- Request: Es la solicitud que hace el cliente, es el mensaje que abre la comunicación  
- Response: Es la respuesta del servidor frente a una solicitud del cliente.

![][image12]  
Tanto el request como el response tienen:

1. Start Line: describe versión de HTTP  
2. Headers: Contienen especificaciones acerca del formato del body o características de la solicitud en particular. Los headers son opcionales, pero es una buena práctica ponerlos.  
3. Línea vacía para indicar la finalización de header (haya o no haya body)  
4. Body: Es el mensaje en sí. En el caso de la solicitud puede estar asociado a métodos POST.

El conjunto de “start line \+ headers” se lo conoce como cabecera de la solicitud. Por eso, en HTTP se tiene la “cabecera y el mensaje” separados por la línea vacía.

## **Estructura de la solicitud del cliente (Request)**

Los request consisten en directivas de texto separadas por “\\r\\n”. La sintaxis es la que se ve en la imagen y además cuenta con los 4 puntos antes descritos  
![][image13]

### **Request Line**

Se indica el método del request seguido del path y versión HTTP  
El path es la ruta desde la raíz del servidor. En este caso sería en el directorio “root/”

### **Headers (opcionales)**

Los headers son un string case-sensitive que brinda información acerca del request. Hay dos tipos:

* Request headers: Le da información al servidor acerca de qué o cómo retornar la información en el response.  
* Representation headers: Brindan información acerca de cómo está especificado el body.

Cada header va en una nueva línea, marcada con “\\r\\n”. Si bien los headers son opcionales, es una buena práctica ponerlos. En la sección “Resumen Práctico de Headers” hay un listado de los headers más frecuentes.

### **Línea vacía (\\r\\n)**

Es la línea que separa la cabecera del body. Esta línea debe estar siempre, haya o no haya body.

### **Body (opcional)**

Es el mensaje que se le pasa al servidor. En general, la longitud y el formato van especificadas a través de los headers. Cabe destacar que el body solo debe incluirse para métodos PATCH, POST y PUT. En general, para este tipo de información se usa el formato .json.

### **Ejemplos**

#### Ejemplo 1

```
GET / HTTP/1.1
Host: developer.mozilla.org
Accept-Language: fr
                    → Espacio vacío para indicar la finalización del header
```

##### Request Line

- GET: Método HTTP, en este caso, una solicitud para obtener un recurso (GET).  
- /: Path del recurso en el servidor. En este caso el directorio raíz del sitio web.  
- HTTP/1.1: Versión del protocolo HTTP usado.

##### Headers

- Host: developer.mozilla.org: Indica el dominio del servidor al que se envía la solicitud.  
- Accept-Language: fr: Indica que el cliente prefiere recibir la respuesta en francés (idioma).

##### Body

- Vacío en este caso porque el método GET no requiere un cuerpo.

#### Ejemplo 2

```
POST /contact\_form.php HTTP/1.1
Host: developer.mozilla.org
Content-Length: 64
Content-Type: application/x-www-form-urlencoded 
                    → Espacio vacío para indicar la finalización del header name=Joe%20User\&request=Send%20me%20one%20of%20your%20catalogue |
```

##### Request Line

- POST: Método HTTP, usado para enviar datos al servidor.  
- /contact\_form.php: Path del recurso en el servidor al que se envían los datos.  
- HTTP/1.1: Versión del protocolo HTTP usado.

##### Headers

- Host: developer.mozilla.org: Indica el dominio del servidor al que se envía la solicitud.  
- Content-Length: 64: Indica la longitud (en bytes) del body de la solicitud.  
- Content-Type: application/x-www-form-urlencoded: Indica el tipo de datos en el cuerpo de la solicitud (en este caso, datos codificados en formato URL).

##### Body

name=Joe%20User\&request=Send%20me%20one%20of%20your%20catalogue  
Contiene los datos enviados al servidor. Aquí hay dos campos:

- name: "Joe User" (los espacios se codifican como %20 en formato URL).  
- request: "Send me one of your catalogue".

#### Resumiendo

| Parte | Ejemplo 1 | Ejemplo 2 |
| ----- | ----- | ----- |
| **Método HTTP** | GET | POST |
| **Path** | root/ | root/contact\_form.php |
| **Versión HTTP** | HTTP/1.1	 | HTTP/1.1	 |
| **Headers** | Host, Accept-Language | Host, Content-Length, Content-Type |
| **Body** | Vacío | Datos del formulario codificados en URL |

Cabe destacar que los headers HTTP están parcialmente estandarizados. Esto significa que existen headers definidos oficialmente por los estándares de HTTP, pero también se permite que los desarrolladores y servidores definan sus propios headers personalizados.

## **Estructura de la respuesta del servidor (Response)**

Una vez que se estableció la sesión y el cliente envía la solicitud HTTP, entonces el servidor debe procesar la información y enviar la respuesta, o response.  
Al igual que la solicitud, tiene cuatro secciones.  
![][image14]

### **Status Line**

En la primera línea se indica versión de HTTP, status code y texto de estado.  
Los códigos de estado más comunes son 200, 404 o 302\. En la sección “Resumen Práctico Status Code” hay un listado de varios códigos de estado. En el texto de estado, es simplemente una ayuda visual para interpretar el código. Cada código tiene asociado un texto.

### **Headers (opcionales)**

Los headers son un string case-sensitive que brinda información acerca del response. Hay dos tipos:

* Response headers: Le da información al cliente acerca de la información que está retornando. Por ejemplo, fecha y hora, información del servidor, etc. Por otro lado, puede recibir información acerca del caché, etc.  
* Representation headers: Brindan información acerca de cómo está especificado el body. Tamaño, lectura por chunks de datos, formato, etc.

Cada header va en una nueva línea, marcada con “\\r\\n”. Si bien los headers son opcionales, es una buena práctica ponerlos. En la sección “Resumen Práctico de Headers” hay un listado de los headers más frecuentes.

### **Línea vacía (\\r\\n)**

Es la línea que separa la cabecera del body. Esta línea debe estar siempre, haya o no haya body.

### **Body**

En el body la mayoría de las veces está el fuerte de la información. Acá es donde estará la información que el cliente solicitó. Puede ser una página web, un archivo, un json, etc.  
En caso de falla, es una buena práctica explicar en el body qué ocurrió.

Cabe destacar que un mismo body puede tener varios formatos, típicamente esto ocurre en solicitudes HTML. Para eso se utilizan los “range requests”.

### **Ejemplos**

#### Ejemplo 1

```
HTTP/1.1 200 OK
Content-Type: text/html; charset=utf-8
Content-Length: 55743
Connection: keep-alive
Cache-Control: s-maxage=300, public, max-age=0
Content-Language: en-US Date: Thu, 06 Dec 2018 17:37:18 GMT
ETag: "2e77ad1dc6ab0b53a2996dfd4653c1c3"
Server: meinheld/0.6.1
Strict-Transport-Security: max-age=63072000
X-Content-Type-Options: nosniff
X-Frame-Options: DENY
X-XSS-Protection: 1; mode=block
Vary: Accept-Encoding,Cookie
Age: 7
                    → Espacio vacío para indicar la finalización del header
<!doctype html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>A simple webpage</title\>
  </head>
  <body>
    <h1>Simple HTML webpage</h1>
    <p\>Hello, world!</p>
  </body>
</html>
```

* HTTP/1.1 200 OK: El código de estado 200 indica que la solicitud fue exitosa y el recurso solicitado (una página HTML) está disponible.  
* Content-Type: text/html; charset=utf-8: Especifica que el contenido de la respuesta es HTML codificado en UTF-8.  
* Content-Length: 55743: Indica el tamaño del contenido en bytes.  
* Connection: keep-alive: Mantiene la conexión abierta para solicitudes adicionales.  
* Cache-Control: s-maxage=300, public, max-age=0: Define políticas de almacenamiento en caché. Aquí se permite almacenamiento público y un tiempo de vida en caché de 300 segundos.  
* ETag: Es un identificador único para el contenido actual del recurso. Ayuda a los navegadores a identificar si el contenido cambió desde la última solicitud.  
* Strict-Transport-Security: Refuerza el uso de HTTPS.  
* X-Content-Type-Options: nosniff: Previene que los navegadores interpreten incorrectamente el tipo de contenido.  
* X-Frame-Options: DENY: Prohíbe que la página sea cargada en un \<iframe\>, para evitar ataques de clickjacking.  
* HTML en el cuerpo: Contiene el contenido de la página solicitada.

#### Ejemplo 2

```
HTTP/1.1 301
Moved Permanently Server: Apache/2.4.37 (Red Hat)
Content-Type: text/html; charset=utf-8
Date: Thu, 06 Dec 2018 17:33:08 GMT
Location: https://developer.mozilla.org/
Keep-Alive: timeout=15, max=98
Accept-Ranges: bytes
Via: Moz-Cache-zlb05
Connection: Keep-Alive
Content-Length: 325
                    → Espacio vacío para indicar la finalización del header
```

* HTTP/1.1 301 Moved Permanently: Indica que el recurso solicitado se ha movido permanentemente a otra URL.  
* Location: https://developer.mozilla.org/: Especifica la nueva URL donde se puede encontrar el recurso.  
* Content-Length: 325: Define la longitud del contenido devuelto, usualmente una página que explica la redirección.  
* Keep-Alive: timeout=15, max=98: Mantiene la conexión abierta durante 15 segundos o un máximo de 98 solicitudes.  
* HTML en el cuerpo: Generalmente contiene una página que redirige automáticamente al usuario o informa sobre el cambio.

#### Ejemplo 3

```
HTTP/1.1 404 Not Found
Content-Type: text/html; charset=utf-8
Content-Length: 38217
Connection: keep-alive
Cache-Control: no-cache, no-store, must-revalidate, max-age=0
Content-Language: en-US Date: Thu, 06 Dec 2018 17:35:13 GMT
Expires: Thu, 06 Dec 2018 17:35:13 GMT
Server: meinheld/0.6.1
Strict-Transport-Security: max-age=63072000
X-Content-Type-Options: nosniff
X-Frame-Options: DENY
X-XSS-Protection: 1; mode=block
Vary: Accept-Encoding,Cookie
X-Cache: Error from cloudfront
                    → Espacio vacío para indicar la finalización del header
```

* HTTP/1.1 404 Not Found: Indica que el recurso solicitado no existe en el servidor.  
* Cache-Control: no-cache, no-store, must-revalidate, max-age=0: Previene que el error sea almacenado en caché.  
* Expires: Establece que el recurso expiró inmediatamente.  
* X-Cache: Error from cloudfront: Muestra que la solicitud pasó por CloudFront, pero no se pudo encontrar el recurso.  
* HTML en el cuerpo: Normalmente contiene una página personalizada para notificar al usuario que el recurso no existe.

# **MIME Types**

Los MIME types (Media Types) son un estándar definido por la IANA (Internet Assigned Numbers Authority) para identificar el formato de los archivos transmitidos en la web. La estructura de un MIME type incluye:

1. Tipo: Define la categoría general del contenido. Ejemplos comunes incluyen:  
- text: Contenido textual como texto plano o documentos estructurados.  
- image: Archivos de imágenes en diversos formatos.  
- audio: Archivos de sonido o música.  
- video: Archivos de video en distintos códecs.  
- application: Datos específicos de aplicaciones o binarios genéricos.

2. Subtipo: Especifica el formato particular dentro del tipo general. Por ejemplo:  
   - text/html  
   - image/png  
   - application/json

Parámetros opcionales: Proporcionan información adicional sobre el contenido. Un ejemplo es el parámetro charset, que indica la codificación del texto:

```
text/html; charset=UTF-8
```

## **Tipos Discretos**

Los tipos discretos son los que tienen un formato bien definido y pueden identificarse claramente. Estos incluyen:

* text: Contenido textual como text/plain, text/css, y text/html.  
* image: Imágenes en formatos como image/jpeg, image/png, y image/gif.  
* audio: Archivos de audio como audio/mpeg y audio/wav.  
* video: Archivos de video como video/mp4 y video/webm.  
* application: Archivos específicos de aplicaciones, como application/json, application/xml, y application/octet-stream.

## **Tipos Múltiples**

Estos tipos permiten representar contenido compuesto por diferentes medios. Los más comunes incluyen:

* multipart/form-data: Usado para enviar datos de formularios con archivos adjuntos.  
* multipart/alternative: Para representar el mismo contenido en diferentes formatos, como texto y HTML.  
* multipart/mixed: Combina diferentes partes de datos, como texto y archivos binarios.

## **Los tipos más importantes para desarrolladores web**

Los desarrolladores web trabajan con una variedad de MIME types. Algunos de los más importantes incluyen:

* Textuales:  
  * text/plain: Texto sin formato.  
  * text/html: Documentos HTML.  
  * text/css: Estilos en CSS.  
  * text/javascript o application/javascript: Scripts en JavaScript.  
* Imágenes:  
  * image/jpeg: Imágenes JPEG.  
  * image/png: Imágenes PNG.  
  * image/svg+xml: Imágenes vectoriales SVG.  
* Audio:  
  * audio/mpeg: Archivos de audio MP3.  
  * audio/ogg: Audio en formato Ogg Vorbis.  
* Video:  
  * video/mp4: Videos en formato MP4.  
  * video/webm: Videos en formato WebM.  
* Aplicaciones:  
  * application/json: Archivos JSON.  
  * application/xml: Archivos XML.  
  * application/octet-stream: Contenido binario genérico.

## **La importancia de setear bien el MIME type**

Configurar correctamente el MIME type es crucial para que los navegadores y clientes interpreten y procesen los datos de manera adecuada. Un MIME type mal configurado puede generar problemas como:

* Interpretación incorrecta del contenido (por ejemplo, tratar un archivo de texto como binario).  
* Vulnerabilidades de seguridad, como ataques de XSS (Cross-Site Scripting) debido a scripts interpretados accidentalmente.  
* Experiencias de usuario inconsistentes, donde el contenido no se renderiza correctamente.

## **MIME Sniffing**

El MIME sniffing es una técnica utilizada por los navegadores para determinar el tipo de contenido basándose en los datos reales del archivo, en lugar de confiar exclusivamente en el MIME type declarado. Aunque puede ser útil en algunos casos, también presenta riesgos de seguridad, como:

* Ejecutar contenido malicioso si el navegador adivina incorrectamente el tipo.  
* Ignorar configuraciones de seguridad del servidor.

Los desarrolladores pueden prevenir el MIME sniffing configurando correctamente el encabezado HTTP X-Content-Type-Options con el valor nosniff.

## **Ejemplo**

Al solicitar un recurso en la web, el tipo de contenido se indica a través de los encabezados HTTP. Esto ayuda a que el servidor entregue una respuesta apropiada. Un ejemplo conceptual sería:  
*Solicitud del cliente*:

```
GET /archivo.html HTTP/1.1
Host: www.ejemplo.com
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,\*/\*;q=0.8
```
El encabezado Accept indica los MIME types que el cliente acepta recibir. En este caso:

- text/html: Tiene mayor prioridad.  
- application/xml: Es aceptable con menor prioridad (q=0.9).  
- \*/\*: Acepta cualquier tipo de contenido restante con prioridad baja (q=0.8).

*Respuesta del servidor:*

```
HTTP/1.1 200 OK
Content-Type: text/html; charset=UTF-8
Content-Length: 1024

<html>
  <head> <title>Ejemplo</title> </head>
  <body> <h1>Hola, Mundo!</h1> </body>
</html>
```

El servidor utiliza el encabezado Content-Type para informar al cliente que el recurso es de tipo text/html con codificación UTF-8.

Este mecanismo asegura que tanto cliente como servidor comprendan y procesen el contenido correctamente.

## **Otros métodos para indicar el tipo de documento**

Además de los MIME types, existen otros métodos para indicar el tipo de contenido:

1. Extensiones de archivo: Aunque no son tan fiables como los MIME types, las extensiones como .html, .css, y .png proporcionan pistas sobre el tipo de contenido.  
2. Encabezados HTTP adicionales:  
   * Content-Disposition: Indica si el contenido debe mostrarse en el navegador o descargarse.  
   * Content-Language: Especifica el idioma del contenido.  
3. Meta etiquetas en HTML: Por ejemplo, una página HTML puede incluir:
```
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8>"
```

4. Esquemas URI y Protocolo: El tipo de contenido también puede inferirse a partir de esquemas como data: en las URLs, que incluyen directamente el MIME type junto con el contenido.

# **Resumen Práctico de Headers**

Hay múltiples categorías de headers, dependiendo su uso, si van en el request o en el response, etc. En función de ello se puede destacar las categorías:

- Request headers: Headers que contienen información acerca del recurso que se le está solicitando al servidor  
- Response headers: Dan información extra a las respuesta, por ejemplo, ubicación o acerca del servidor.  
- Representation headers: Brindan información acerca del body (MIME type, compresiones, etc).  
- Payload headers: Contienen información acerca de la representación de la información  
- Administración de la conexión

Por otro lado, hay categorías más avanzadas que son adecuadas para aplicaciones de internet. Por ejemplo, Caché, Cookies, Autenticación, Proxys, CORS, Descargas, Integrity Digest, Negociación, redirecciones, etc. 

Sin embargo, a continuación se detalla un resumen práctico de los headers más frecuentes en la práctica.

Para más info visitar: [https://developer.mozilla.org/en-US/docs/Web/HTTP](https://developer.mozilla.org/en-US/docs/Web/HTTP) 

## **Headers de uso exclusivo en solicitudes (Request Headers)**

| Header | Descripción | Ejemplo |
| :---- | :---- | :---- |
| ***Host*** | Nombre del host al que se envía la solicitud, obligatorio en HTTP/1.1. | Host: www.example.com |
| ***User-Agent*** | Información sobre el cliente (navegador, aplicación, etc.). | User-Agent: Mozilla/5.0 |
| ***Authorization*** | Credenciales para autenticación. | Authorization: Basic dXNlcjpwYXNz |
| ***Accept*** | Tipos de contenido que el cliente puede procesar. | Accept: application/json |
| ***Accept-Language*** | Preferencias de idioma del cliente. | Accept-Language: en-US |
| ***Referer*** | URL de referencia desde donde se hizo la solicitud. | Referer: https://google.com |
| ***Cookie*** | Cookies almacenadas por el cliente. | Cookie: sessionId=1234 |
| ***If-Modified-Since*** | Solicita el recurso sólo si ha cambiado desde una fecha. | If-Modified-Since: Tue, 20 Jun 2023 |
| ***If-None-Match*** | Solicita el recurso sólo si no coincide con un ETag específico. | If-None-Match: "abc123" |

## **Headers de uso exclusivo en respuestas (Response Headers)**

| Header | Descripción | Ejemplo |
| :---- | :---- | :---- |
| ***Server*** | Información sobre el software del servidor. | Server: Apache/2.4.41 |
| ***WWW-Authenticate*** | Indica el esquema de autenticación requerido por el servidor. | WWW-Authenticate: Basic realm="Admin" |
| ***Retry-After*** | Informa cuánto tiempo debe esperar el cliente antes de intentar nuevamente. | Retry-After: 120 |
| ***ETag*** | Identificador único de la versión del recurso. | ETag: "abc123" |
| ***Set-Cookie*** | Envía cookies al cliente para almacenarlas. | Set-Cookie: sessionId=1234; Path=/ |
| ***Last-Modified*** | Fecha de la última modificación del recurso. | Last-Modified: Tue, 20 Jun 2023 |
| ***Location*** | Indica la URL de redirección para el cliente. | Location: https://newsite.com |
| ***Content-Disposition*** | Sugiere cómo se debe manejar un archivo descargado. | Content-Disposition: attachment |

## **Headers usados en ambos contextos (Request y Response)**

| Header | Descripción | Ejemplo |
| :---- | :---- | :---- |
| ***Content-Type*** | Tipo de contenido del cuerpo de la solicitud o respuesta. | Content-Type: application/json |
| ***Content-Length*** | Longitud del cuerpo en bytes. | Content-Length: 256 |
| ***Cache-Control*** | Reglas para el almacenamiento en caché. | Cache-Control: no-cache |
| ***Connection*** | Controla si la conexión debe mantenerse abierta o cerrarse. | Connection: keep-alive |
| ***Accept-Encoding*** | (Request) Codificaciones aceptadas por el cliente. (Response) Codificación usada por el servidor. | Accept-Encoding: gzip, deflate |
| ***Transfer-Encoding*** | Indica si los datos se transfieren en fragmentos (chunked) o con una codificación específica. | Transfer-Encoding: chunked |

## **Contexto mixto: detalles importantes**

1. Content-Type:  
   * En solicitud: Indica el formato del cuerpo enviado por el cliente (e.g., JSON, form-data).  
   * En respuesta: Indica el formato del cuerpo que el servidor envía al cliente.  
2. Content-Length:  
   * Obligatorio en ambos contextos (excepto si usas Transfer-Encoding: chunked).  
   * Proporciona la longitud del cuerpo, necesaria para delimitar el fin de los datos.  
3. Cache-Control:  
   * En solicitud: El cliente puede especificar cómo manejar recursos en caché (e.g., ignorar caché).  
   * En respuesta: El servidor puede indicar reglas de almacenamiento en caché.

# **Resumen Práctico de Métodos**

## **Idempotencia en los Métodos HTTP**

Un método HTTP es idempotente si realizar la misma solicitud varias veces produce el mismo resultado en el servidor, independientemente de cuántas veces se realice.

Ejemplos de métodos idempotentes:

- GET: Obtener información de un recurso no modifica su estado.  
- PUT: Actualizar un recurso con los mismos datos repetidamente mantiene su estado final igual.  
- DELETE: Eliminar un recurso varias veces no cambia el resultado: el recurso deja de existir.

Método no idempotente:

- POST: Crear un recurso varias veces puede generar múltiples instancias, ya que cada solicitud es única.

## **Principales Métodos HTTP y su Propósito**

| Método | Descripción | Idempotente | Seguridad |
| ----- | ----- | :---: | ----- |
| **GET** | Solicita un recurso. No realiza modificaciones en el servidor. | Sí | Seguro e idempotente. |
| **POST** | Envía datos al servidor para crear un recurso nuevo o procesar información. | No | No seguro ni idempotente. |
| **PUT** | Actualiza o reemplaza un recurso existente. Si no existe, puede crearlo. | Sí | Idempotente, pero no seguro. |
| **DELETE** | Elimina un recurso especificado. | Sí | Idempotente, pero no seguro. |
| **PATCH** | Realiza una actualización parcial en un recurso existente. | No | No seguro ni idempotente. |
| **HEAD** | Igual que GET, pero solo obtiene los encabezados de la respuesta. | Sí | Seguro e idempotente. |
| **OPTIONS** | Permite conocer los métodos soportados por el servidor para un recurso. | Sí | Seguro e idempotente. |

### **Métodos Cacheables**

Por otro lado, un método HTTP es considerado cacheable cuando las respuestas generadas por solicitudes que usan dicho método pueden ser almacenadas en cachés intermedias (como navegadores, proxies o servidores). Esto permite que las solicitudes futuras accedan al recurso almacenado en caché, reduciendo la necesidad de enviar una solicitud completa al servidor.

- Por defecto: Sólo las respuestas de solicitudes GET y HEAD son cacheables sin configuración adicional.  
- Condicionalmente cacheables: Algunos métodos como POST o PATCH pueden ser cacheados si y solo si la respuesta contiene encabezados específicos, como Cache-Control o Content-Location.

| Método | ¿Cacheable por defecto? | Notas |
| ----- | :---: | ----- |
| GET | ✔️ Sí | Los recursos pueden ser cacheados si la respuesta incluye encabezados de control de caché como Cache-Control. |
| HEAD | ✔️ Sí | Cacheable si las respuestas serían equivalentes a las de una solicitud GET. |
| POST | Condicional | Puede ser cacheable si la respuesta incluye encabezados de frescura y Content-Location. |
| PUT | ❌ No | Las solicitudes PUT no suelen ser cacheables. |
| DELETE | ❌ No | Las solicitudes DELETE nunca son cacheables. |
| PATCH | Condicional | Igual que POST, puede ser cacheable en casos excepcionales con encabezados adecuados. |
| OPTIONS | ❌ No | Su propósito es informativo, no se almacena en caché. |
| TRACE | ❌ No | Por razones de seguridad, no es cacheable. |

Nota: La capacidad de cachear una respuesta depende de los encabezados que incluye el servidor, como:

* Cache-Control: Establece directivas explícitas de caché (e.g., no-cache, max-age, etc.).  
* Expires: Define una fecha de vencimiento para los datos.  
* ETag: Ayuda a validar si el recurso almacenado aún es válido.

#### Beneficios del almacenamiento en caché

- Reducción del tiempo de respuesta: Los recursos se sirven desde la caché, evitando el tiempo de procesamiento en el servidor.  
- Ahorro de ancho de banda: No es necesario descargar recursos redundantes.  
- Desempeño mejorado: Reduce la carga en los servidores al minimizar solicitudes repetitivas.

#### Limitaciones del almacenamiento en caché

Las respuestas cacheadas pueden quedar desactualizadas si el recurso cambia en el servidor y no se sincroniza correctamente.  
No todos los métodos ni recursos son apropiados para ser cacheados.

## **Ejemplos**

**GET**: Solicita un recurso sin modificarlo
Request:
```
GET /user/123 HTTP/1.1
Host: example.com
```
Response:
```
HTTP/1.1 200 OK
Content-Type: application/json

{
  "id": 123,
  "name": "John Doe",
  "email": "john.doe@example.com" 
}
```

**POST:**  Envía datos al servidor para crear un nuevo recurso.
Request:
```
POST /users HTTP/1.1
Host: example.com
Content-Type: application/json

{
  "name": "John Doe",
  "email": "john.doe@example.com"
}
```
Response:
```
HTTP/1.1 201
Created Location: /users/123
```

**PUT:** Actualiza un recurso existente o lo crea si no existe
Request
```
PUT /user/123 HTTP/1.1
Host: example.com
Content-Type: application/json

{  
  "name": "John Doe",
  "email": "john.updated@example.com"
}
```
Response
```
HTTP/1.1 200 OK
```

**DELETE:** Elimina un recurso especificado.
Request
```
DELETE /user/123 HTTP/1.1
Host: example.com
```
Response
```
HTTP/1.1 204 No Content
```

**PATCH:**  Realiza una actualización parcial en un recurso existente.

```
PATCH /user/123 HTTP/1.1
Host: example.com
Content-Type: application/json

{
  "email": "john.partial@example.com"
}
```

```
HTTP/1.1 200 OK
```

**HEAD:** Solicita solo los encabezados de un recurso.
```
HEAD /file.zip HTTP/1.1
Host: example.com
```
```
HTTP/1.1 200 OK
Content-Length: 1024
Content-Type: application/zip
```

**OPTIONS** Consulta los métodos soportados por el servidor para un recurso.
```
OPTIONS /user/123 HTTP/1.1
Host: example.com
```

```
HTTP/1.1 204
No Content Allow: GET, POST, PUT, DELETE, OPTIONS
```

**Nota Importante sobre la Seguridad**

Un método seguro no modifica el estado del servidor.

* GET y HEAD son seguros.  
* Métodos como POST, PUT, y DELETE no son seguros, ya que modifican recursos o su estado.

# **Resumen Práctico de Status Codes**

Los códigos de estado HTTP se dividen en 5 clases principales

## **1xx \- Informativos**

Indican que la solicitud ha sido recibida y se está procesando.

| Código | Significado | Descripción |
| ----- | ----- | ----- |
| 100 | **Continue** | El servidor recibió los encabezados; el cliente puede continuar enviando el cuerpo. |
| 101 | **Switching Protocols** | El servidor acepta cambiar el protocolo (por ejemplo, a WebSocket). |

## **2xx \- Éxito**

Indican que la solicitud se completó con éxito.

| Código | Significado | Descripción |
| ----- | ----- | ----- |
| 200 | **OK** | Solicitud exitosa. Los datos solicitados están en el cuerpo de la respuesta. |
| 201 | **Created** | Un recurso fue creado (útil en POST o PUT). |
| 204 | **No Content** | Solicitud exitosa, pero sin contenido en la respuesta. |

## **3xx \- Redirección**

Indican que el cliente debe tomar alguna acción adicional para completar la solicitud.

| Código | Significado | Descripción |
| ----- | ----- | ----- |
| 301 | **Moved Permanently** | El recurso se movió permanentemente a otra URL. |
| 302 | **Found** | El recurso se movió temporalmente a otra URL. |
| 304 | **Not Modified** | El recurso no ha cambiado (útil para almacenamiento en caché). |

## **4xx \- Errores del Cliente**

Indican que el problema está en el lado del cliente.

| Código | Significado | Descripción |
| ----- | ----- | ----- |
| 400 | **Bad Request** | Solicitud mal formada. El servidor no puede procesarla. |
| 401 | **Unauthorized** | Requiere autenticación (sin o con credenciales incorrectas). |
| 403 | **Forbidden** | El cliente no tiene permiso para acceder al recurso. |
| 404 | **Not Found** | El recurso solicitado no existe. |
| 408 | **Request Timeout** | La solicitud tardó demasiado en enviarse. |

## **5xx \- Errores del Servidor**

Indican que el servidor no pudo cumplir con una solicitud válida.

| Código | Significado | Descripción |
| ----- | ----- | ----- |
| 500 | **Internal Server Error** | Error genérico en el servidor. |
| 502 | **Bad Gateway** | El servidor actuó como puerta de enlace y recibió una respuesta inválida. |
| 503 | **Service Unavailable** | El servidor no está disponible (sobrecarga o mantenimiento). |
| 504 | **Gateway Timeout** | Tiempo de espera agotado en un servidor intermedio. |

# **Administración de la conexión en HTTP/1.x**

Una conexión HTTP implica que  previamente se establezca la capa de transporte TCP. Sin embargo, crear este canal de comunicación TCP implica una gran esfuerzo computacional. Por lo tanto, no es eficiente abrir y cerrar todo el tiempo sesiones TCP. Este problema se ve incrementado cuando se tienen varios proxies entre el cliente y el servidor.

Es por eso, que hay diferentes tipos de sesiones:

- Conexión breve: Se abre y se cierra un canal TCP por cada par request-response  
- Conexión persistente: Se abre un canal TCP y se cierra luego de un timeout sin peticiones  
- Conexión con pipelining: Se abre un canal TCP y se envían múltiples solicitudes sin esperar a que llegue la respuesta

![][image15]  
Cabe destacar que HTTP/2 añade modelos extras.  
Otro punto a tener en cuenta es que lo estudiado en esta sección aplica para configuraciones hop-to-hop (sin proxies en el medio). Es decir, que si se tiene una configuración end-to-end (con proxys en el medio) se aplicará alguno de estos modelos. Además entre el cliente y el proxy puede haber un modelo, mientras que entre el proxy y el servidor otro modelo.

Por otro lado, un tema relacionado a este es el de “mecanismos de mejora de protocolos”, en donde el texto enviado respeta el protocolo HTTP, sin embargo, se encripta usando otro protocolo; por ejemplo HTTP/2, TLS/1.0 o WebSocket. Más info acá: [https://developer.mozilla.org/en-US/docs/Web/HTTP/Protocol\_upgrade\_mechanism](https://developer.mozilla.org/en-US/docs/Web/HTTP/Protocol_upgrade_mechanism) 

## **Conexiones cortas**

Son el formato de sesión estándar de HTTP, donde se abre y cierra el canal TCP por cada par request-response. Es decir, se hace un handshake TCP por cada solicitud. Esto hace que no se le pueda sacar el máximo jugo posible al protocolo TCP.

Se recomienda usar este tipo de sesión únicamente cuando el hardware no lo permite o el sistema es viejo.

Este es el modelo default en HTTP/1.0. En HTTP/1.1 este modelo es utilizado solamente cuando se usa la cabecera “Connection: close”. 

## **Conexiones persistentes**

Este tipo de conexiones viene incluida a partir de la versión HTTP/1.1. Esta hace uso del header “Connection: keep-alive”.

En este modelo de sesión, el canal TCP queda abierto un determinado tiempo desde que se recibió la última solicitud.  
Cabe destacar que el header keep-alive no debe usarse en HTTP/2 ni HTTP/3, sólo en HTTP/1.x.

La principal desventaja de este modelo es que mantener el canal TCP abierto también consume recursos. Por eso, este sistema es más propenso a sufrir saturación del sistema por exceso de solicitudes. En este principio se basan los ciber-ataques DoS. En ese sentido es más robusto el modelo de conexiones breves.

- En HTTP/1.1 este es el modo default, no es necesario reiterar en el header  
- En HTTP/1.0 hay que activarlo manualmente

## **Conexiones pipelining**

En este esquema se envían varios request en simultáneo, es decir, no se espera a recibir el response del request anterior para enviar el siguiente.

Cabe destacar que el pipelining es una técnica que trae aparejada algunos inconvenientes en cuanto a la performance y por eso no todos los buscadores la permiten implementar.

Cabe destacar que solamente los métodos idempotentes pueden utilizar pipelining.

Para ahorrar eficiencia en el ancho de banda, en lugar de utilizar pipelining, en HTTP/2 y HTTP/3 se hacen técnicas de multiplexado. Por eso, es una mala práctica utilizar HTTP/1.x con pipelining. Para eso es mejor ir a una versión más reciente de HTTP.

# **Características adicionales de HTTP**

En la mayoría de aplicaciones web tradicionales (donde el objetivo es tener múltiples usuarios que acceden una página de forma simultánea y muy frecuentemente) la solución no se hace con un sistema embebido sino directamente con servidores cuyo hardware es más robusto y está preparado para funcionar de esa manera.

Para estos casos, el protocolo HTTP ofrece diferentes funcionalidades que si bien excede al conocimiento que se necesitan para sistemas embebidos es bueno conocer conceptualmente. Ya que aporta conceptualmente cómo funciona la web permitiendo así futuras implementaciones o simplificaciones.

## **HTTP Caching**

El caching en HTTP permite almacenar respuestas de recursos en caché, reduciendo la carga del servidor y mejorando los tiempos de respuesta para solicitudes futuras. Es especialmente útil para recursos estáticos (como imágenes, hojas de estilo o scripts), aunque también puede aplicarse a datos dinámicos cuando se gestiona adecuadamente.

### **¿Cómo funciona el caching?**

#### Encabezados de respuesta del servidor

- Cache-Control: Define políticas explícitas de almacenamiento. Ejemplos:  
  - max-age=3600: Indica que el recurso puede almacenarse durante 3600 segundos.  
  - no-store: Prohíbe almacenar el recurso en caché.  
  - no-cache: Obliga a revalidar con el servidor antes de usar el caché.

- ETag: Proporciona un identificador único basado en el contenido del recurso. Si cambia, indica que el recurso debe actualizarse.  
- Expires: Especifica una fecha de vencimiento absoluta, aunque suele ser reemplazada por Cache-Control.

#### Revalidación

El cliente puede usar encabezados como If-Modified-Since o If-None-Match para verificar si el recurso en caché aún es válido.

### **Ventajas**

- Eficiencia: Reduce la carga en el servidor al disminuir solicitudes repetidas.  
- Rapidez: Mejora los tiempos de carga para los clientes.  
- Reducción de tráfico: Minimiza el uso de ancho de banda.

### **Ejemplo de encabezados de caché**

*Respuesta del servidor:*  
```
HTTP/1.1 200 OK  
Cache-Control: max-age=86400  
ETag: "abcd1234"
```
*Solicitud posterior del cliente:*  
```
GET /image.png HTTP/1.1  
If-None-Match: "abcd1234"
```
*Respuesta si no cambió:*  
```
HTTP/1.1 304 Not Modified
```
## **HTTP Authentication**

La autenticación HTTP asegura que solo usuarios o clientes autorizados accedan a los recursos protegidos.

### **Tipos comunes de autenticación**

#### Basic Authentication

Las credenciales (usuario:contraseña) se envían codificadas en Base64 dentro del encabezado Authorization. Sin embargo, no es seguro si no se usa HTTPS, ya que las credenciales pueden interceptarse fácilmente.

##### Ejemplo

Authorization: Basic dXNlcjpwYXNzd29yZA==

#### Bearer Token

Se utiliza un token de acceso (generalmente obtenido tras autenticación previa). Es común en APIs que implementan OAuth 2.0.

##### Ejemplo

Authorization: Bearer \<access\_token\>

#### Digest Authentication

Más seguro que Basic, ya que utiliza un hash criptográfico en lugar de enviar las credenciales directamente.

##### Flujo típico

1. El cliente realiza una solicitud sin credenciales.  
2. El servidor responde con un código 401 Unauthorized y un encabezado WWW-Authenticate que indica el tipo de autenticación requerido, es decir, pone un desafío de autenticación (Digest Challenge).  
3. El cliente reintenta la solicitud con las credenciales.  
4. El servidor realiza el mismo cálculo de hash con las credenciales almacenadas y los parámetros proporcionados. Si los hashes coinciden, el servidor concede acceso

##### Ventajas

- Protección de recursos sensibles.  
- Control granular de acceso a diferentes recursos.

##### Ejemplo
```
HTTP/1.1 401 Unauthorized  
WWW-Authenticate: Basic realm="Protected Area"
```
Sin embargo, lo más seguro es usar HTTPS con encriptado TLS/SSL para una seguridad confiable

## **HTTP Cookies**

Las cookies HTTP permiten al servidor almacenar pequeños fragmentos de información en el cliente para mantener el estado entre solicitudes, algo que HTTP por defecto no hace debido a su naturaleza sin estado (stateless).

### **Características**

- Almacenamiento: Una cookie se guarda en el navegador del cliente y se envía automáticamente con cada solicitud posterior al servidor.  
- Propósito:   
  - Mantener sesiones de usuario.  
  - Almacenar preferencias (idioma, configuraciones, etc.).  
  - Realizar seguimiento analítico (como en herramientas de marketing).

### **Encabezados principales**

#### Set-Cookie

Enviado por el servidor para definir una cookie.

##### Ejemplo

Set-Cookie: sessionId=abc123; HttpOnly; Secure; Max-Age=3600

- HttpOnly: Evita que scripts accedan a la cookie.  
- Secure: Solo se envía en conexiones HTTPS.  
- Max-Age: Tiempo de vida de la cookie (en segundos).

#### Cookie

Enviado por el cliente con cada solicitud al servidor.

##### Ejemplo

Cookie: sessionId=abc123

##### Ventajas

- Facilita la autenticación y personalización.  
- Permite implementar estados persistentes en aplicaciones web.

## **Proxies y Tunneling**

Los proxies actúan como intermediarios entre el cliente y el servidor, mientras que el tunneling permite enviar datos cifrados a través de un proxy.

### **Proxies**

#### Proxy directo

- Filtra solicitudes, registra datos o balancea la carga entre múltiples servidores.  
- Se configura mediante la URL del proxy en el cliente.

##### Ejemplo
```
Proxy-Authorization: Basic dXNlcjpwYXNzd29yZA==
```
#### Proxy inverso

El proxy actúa como intermediario del lado del servidor, ocultando su estructura interna y distribuyendo tráfico.

### **Tunneling**

Utilizado para conexiones cifradas (HTTPS) a través de un proxy mediante el método CONNECT.

#### Ejemplo de solicitud de túnel

CONNECT www.example.com:443 HTTP/1.1

#### Ventajas

- Seguridad: Filtra tráfico malicioso o restringe el acceso.  
- Privacidad: Oculta la dirección IP del cliente.  
- Rendimiento: Reduce la carga en servidores mediante balanceo de tráfico.

## **Compresión**

La compresión en HTTP permite reducir el tamaño de los datos que se transfieren entre el cliente y el servidor, acelerando la carga de páginas web y reduciendo el uso de ancho de banda.

### **¿Cómo funciona la compresión en HTTP?**

1. Cliente solicita compresión: En el encabezado de la solicitud HTTP, el cliente puede incluir un campo llamado Accept-Encoding indicando los algoritmos de compresión que soporta (por ejemplo, gzip, br, deflate).  
2. Servidor responde con compresión (si puede): Si el servidor soporta uno de los métodos indicados, comprime el contenido y lo envía junto con un encabezado Content-Encoding para informar al cliente cómo descomprimirlo.

### **Ejemplo de solicitud con compresión**

*Solicitud del cliente:*  
```
GET /index.html HTTP/1.1  
Host: example.com  
Accept-Encoding: gzip, deflate, br
```
*Respuesta del servidor (con compresión gzip):*  
```
HTTP/1.1 200 OK  
Content-Encoding: gzip  
Content-Type: text/html  
Content-Length: 1024

... (contenido comprimido) ...
```
### **Ventajas de la compresión**

* Menor tiempo de carga: Se transfieren menos datos.  
* Menor uso de ancho de banda: Beneficioso en redes lentas o costosas.  
* Mayor eficiencia general: Especialmente útil para recursos grandes como HTML, CSS, JavaScript, y JSON.

### **Limitaciones de la compresión**

* Sobrecarga en servidores y clientes: Comprimir y descomprimir puede aumentar el uso de CPU.  
* No aplicable a todos los recursos: Por ejemplo, imágenes y videos suelen estar comprimidos de antemano (JPEG, MP4).

## **Negociación en HTTP**

La negociación en HTTP es el proceso mediante el cual el cliente y el servidor acuerdan cuál es la mejor representación de un recurso para transferir, basándose en las capacidades del cliente y las preferencias declaradas.

#### Tipos de negociación en HTTP

1. Negociación de contenido: Determina el formato más adecuado del recurso. Por ejemplo, HTML para navegadores o JSON para APIs.  
   * Encabezados clave: Accept, Accept-Language, Accept-Charset.  
2. Negociación de compresión: Selecciona el método de compresión más eficiente, como gzip o br, según el encabezado Accept-Encoding.  
3. Negociación condicional: El cliente pide un recurso solo si cumple ciertas condiciones, como no haber cambiado desde la última vez que se obtuvo.  
   * Encabezados clave: If-Modified-Since, If-None-Match.

### **Ejemplo de negociación de contenido**

*Solicitud del cliente:*  
```
GET /resource HTTP/1.1  
Host: example.com  
Accept: application/json, text/html
```
*Respuesta del servidor (elige JSON):*  
```
HTTP/1.1 200 OK  
Content-Type: application/json

{ "message": "Hello, world\!" }
```
### **Ejemplo de negociación condicional**

*Solicitud del cliente:*  
```
GET /image.jpg HTTP/1.1  
Host: example.com  
If-Modified-Since: Wed, 21 Oct 2023 07:28:00 GMT
```
*Respuesta del servidor (el recurso no cambió):*  
```
HTTP/1.1 304 Not Modified
```
### **Relación entre Compresión y Negociación**

Ambos conceptos trabajan juntos para hacer las transferencias HTTP más rápidas y eficientes:

* La compresión reduce el tamaño de los datos enviados.  
* La negociación asegura que el contenido sea útil y compatible con el cliente.

# **Evolución de HTTP**

## **1989-1990: Creación de la World Wide Web**

**Proponente:** Tim Berners-Lee, mientras trabajaba en el CERN.

**Hitos principales**

- Creación de la **World Wide Web (WWW)**, concebida como un sistema para compartir información a través de documentos interconectados mediante hipervínculos.  
- Desarrollo de tres tecnologías fundamentales:  
1. **HTML (HyperText Markup Language):** Un lenguaje de marcado para estructurar y enlazar documentos.  
2. **HTTP (HyperText Transfer Protocol):** Un protocolo para la transferencia de información en la web.  
3. **URL (Uniform Resource Locator):** Un sistema de direcciones para identificar recursos.  
- **HTTP estaba basado en TCP/IP**, operando en la capa de aplicación del modelo TCP/IP y utilizando TCP en la capa de transporte para garantizar conexiones fiables.

## **1991: Primer documento HTTP**

**HTTP/0.9** Primera versión del protocolo.

- Solo permitía transferir documentos en texto plano.  
- No utilizaba cabeceras, ni definía tipos de contenido.  
- El modelo era simple: un cliente enviaba una solicitud y el servidor devolvía un archivo en respuesta, cerrando la conexión inmediatamente después.

## **1996: HTTP/1.0 (RFC 1945\)**

**Primera versión estandarizada del protocolo.**

**Características principales:**

- Introducción de cabeceras HTTP para describir datos y metadatos.  
- Soporte para diferentes tipos de contenido (cabecera `Content-Type`).  
- Cada solicitud-respuesta cerraba la conexión TCP.  
- Se empezó a usar extensivamente para cargar páginas web enriquecidas con texto e imágenes.

## **1997: HTTP/1.1 (RFC 2068 y RFC 2616 en 1999\)**

**Mejoras significativas respecto a HTTP/1.0:**

- Conexiones persistentes: permitía reutilizar la misma conexión TCP para múltiples solicitudes, mejorando el rendimiento.  
- Pipelining: múltiples solicitudes enviadas en paralelo sin esperar las respuestas previas (aunque no ampliamente adoptado por problemas de bloqueo).  
- Soporte para cacheo con cabeceras como Cache-Control y ETag.  
- Nuevos métodos HTTP: OPTIONS, PUT, DELETE y TRACE.  
- Gestión más robusta de errores y respuestas.

## **2005-2006: Creciente adopción de HTTPS como estándar**

**HTTPS (HTTP sobre TLS/SSL):**

- Garantía de confidencialidad, integridad y autenticación mediante cifrado TLS.  
- Google y otras grandes empresas comenzaron a promover HTTPS como estándar de seguridad.

## **2015: HTTP/2 (RFC 7540\)**

**Objetivo:** Adaptar HTTP al crecimiento de contenido web moderno, optimizando el rendimiento.

**Características principales:**

- Multiplexación: múltiples solicitudes y respuestas simultáneas dentro de una sola conexión.  
- Compresión de cabeceras mediante el protocolo HPACK.  
- Eliminación de pipelining, evitando problemas de bloqueo del servidor.  
- Priorización de solicitudes para optimizar el rendimiento.  
- Mayor uso de TLS, aunque no era obligatorio.

## **2018: Desarrollo de HTTP/3**

**Motivo:** Superar las limitaciones de HTTP/2 relacionadas con la dependencia de TCP.

**Cambio clave:** Uso de QUIC (Quick UDP Internet Connections), un protocolo desarrollado por Google basado en UDP.

## **2020: Lanzamiento oficial de HTTP/3 (RFC 9114 en 2022\)**

**Características principales:**

- Uso de QUIC/UDP para conexiones más rápidas y menor latencia.  
- Reconexión ágil en caso de pérdida de paquetes o cambios en la red.  
- Solución a los problemas de bloqueo de cabecera presentes en HTTP/2.  
- Adaptado para redes modernas, como las móviles e IoT.  
- Adopción por grandes servicios como Google, Facebook y Cloudflare.

## **Resumen general de versiones**

| Versión | Año | Características clave |
| :---: | :---: | ----- |
| HTTP/0.9 | 1991 | Solo texto plano, sin cabeceras, conexión simple. |
| HTTP/1.0 | 1996 | Cabeceras, tipos de contenido, conexiones cerradas tras cada respuesta. |
| HTTP/1.1 | 1997 | Conexiones persistentes, cacheo, métodos avanzados, manejo robusto de errores. |
| HTTP/2 | 2015 | Multiplexación, compresión de cabeceras, eliminación de pipelining. |
| HTTP/3 | 2020 | Basado en QUIC/UDP, baja latencia, reconexión ágil, adaptado para redes modernas. |

# **Bibliografía**

[HTTP - Mozilla Developers](https://developer.mozilla.org/en-US/docs/Web/HTTP) 

[image1]: <data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAhEAAACkCAYAAAApBnz9AAAphklEQVR4Xu2dB3hUZdr+2fK5n7u6rrvu6rrf7v5d28oqqKsoCIiIWEFAUKr03ruUkNB76C2UECAQCJ1A6E0IvYTepPcmTQggPP/cj3tmJ2fIZJjJzDmHuX/XdV9JTn3PnJN57vO8LYcQQgghhPhBDvMCQgghhBBfoIkghBBCiF/QRBBCCCHEL2giCCGEEOIXNBGEEEII8QuaCEIIIYT4BU0EIYQQQvyCJoIQQgghfkETQQghhBC/oIkghBBCiF/QRNiYtLQ0OXjwoNy6dcu8ymd++OEH2b17t/z444/mVYQQQkhA0ESEgMuXL8uUKVM0oBvcvn1bli1bJidOnHDbMiOHDx+WBg0ayKlTp8yrfGbHjh1SokQJuXr1qnmV7bh7966kpKTInj17zKsIIYTYEJqIEHDx4kWpX7++fPfdd65lyDIMGTIkwzIz4Wgihg8fLvPnzzevIoQQYkNoIkIAMhGNGzeW/fv36+8wFYaJgFFA8ES1xfr162XTpk1y7do13Q/rGjVqJKmpqbpu586dul9WoOoCb/PYJyEhIYOJwPmxHEJ57ty549oP5dq8ebOu27dvn2ZLUJWCchhVKtjm3Llzeg5kUQ4dOiQbN26UkydPypYtW1zncS8DjAyOBa5cuaLHO3bsmOuacGwsX7dunXzzzTcycOBAXbdr1y65efOmq3yEEELsBU1ECLhx44a0bt1ag2x8fLz06tVLrl+/Lv3799csw5o1a3R9t27dJCIiQoYNG+YKtsWKFZN27dpJ9+7dpU6dOrJw4ULz4TOAYD19+nRp27at9OjRQ5o1a+YyEadPn5YWLVpI+/btdV3dunVlwYIFLkOAfSIjI3VdYmKilhvL69Wrpz9hdnDs2NhYuXTpkpYH2xYtWlRq164t5cuXlwkTJmgZZs2aJW3atNH1xjXBECxfvlxef/11/QxwTZUqVZKlS5fqtWLbkiVLSvXq1fX38ePH6+dACCHEntBEhAAE46ZNm8qqVau0bUSTJk30zR0mAtmAChUqaLYBQRrtJhBc8faOwIqAarQRWL16tXTt2tV09IwgMwBDcv78ef3bqM5A0EdVQYcOHVyNLPGmj0CPdb1795ZBgwZ5ZDq8mYiGDRvK9u3bpWPHjjJu3Dg1SDBCR44c0eMiYwHw89NPP9WqG5iIwoULu6poxowZIzExMVomVmcQQoizoIkIAXgzR5BGwJw4caL06dNHg3Hfvn3l6NGjUqRIEcmZM6e89tprqlKlSsnevXs92kSgqgOZBXOgd2fr1q0SFRXlqqZwNxH9+vWTyZMnu7Y9fvy4GgEcv1OnTjJ79mzXOgNvJgLGCNUe2HfmzJkyadIkNRGomnn33Xcld+7crmvKkyePmguYCBgKo9oD5oMmghBCnAlNRAhAQEc6/6OPPtJMBMwAgjfMBDIGqBZA+t/cDdPdROAYMAA4jjdgSlC1gKoLBGoEeJgIVAsgyKONhRHAEfhhblC1AmPQqlUrbTPhDrII2AcZDqxDA1G05fBmIlBeXB+u00xWJgK/w2gRQgixPzQRIQLB8rHHHtMqBFRZVK5cWRsR4nc0Mvzyyy/lhRdekBdffFHXIXifPXtWPvjgA3n22Wc1U9GzZ09t2OgNZD2mTZsmzzzzjO4Lc/D1119r0Ma5YFxwPJwHJsCocsB6tFPIlSuXrqtVq5YaBRwPDR1feeUVLQO2QbsHbyYChgcZkUKFCumxIBgbnN+biQDInKC6A/uULVs2oJ4phBBCggtNBCGEEEL8giaCEEIIIX5BE0EIIYQQv6CJIIQQQohf0EQQQgghxC9oIgghhBDiFzQRhJCwAuORfP/99zqAmvvcMb6Cwd7Q/RrHyW4wngvKll3Hxjw8GBUX47xgPBhCshuaCEKILcBQ8BjFFcO+T5061TUBHOZWwRgpWA5hrBEEcoyHYiwzhEHbMMy8N3BMnOfDDz/U8U7uF0w4h8HUvI0c6y8YgwUDwBkT3rmDZXPmzJEzZ86YV2UKJrjDfDnFixeXlStXmlcTEjBBNxHZ5agJIQ8uGLQMo7MmJSXpDK4wDphRFgOUIQhiXhdj9lkMzobByfCGjb87d+4sXbp00VlgMT+LeeRXM/hOwiBmGFo+q23vhVUmAp9FxYoVdSC3+wGmCsfE3D2EZDfZbiLwoOOLAJNAYZhnzNj497//naIoi4TRRt98802dzA1zk9htFFAEdczaisyC+aXDMBHe5lPBxG8YLdWXqgkcH2/yGFIeo7W6nw+pfwztjs8H6y9cuOBaD7OBoeSxHNkADP9umAiUEcshY9ZZZFFgdoyRWVE2nDerKgWYCHwW2Ne9DNgPRguz3CYnJ7vKj+PinBCGvEdmxfhplJ0mggSTbDURmG0SbwT4h162bJlOuOSP0yeEZC8IKpjLZO7cuTo1O+YnQfCzAwiAGBZ927Zt5lUuE4H1GLIdMr+J34+JwBs+JsLDzLk1atTIkE3AEPFVq1bVmXLr1q0rjRs31rltYAjmzZsnLVu2lObNm2s2wNgXxgLfeciiYI4ZDGWP9gcI/ti/Xbt2akwWLFig14DqBbNRcmfs2LE6AR+yK5j4DudEGwkMl9+2bVt56aWXdBh7lAOGA+swYy62r1Spku5TrVo13Q+mCNBEkGCSbSYCqUHMpeBvipAQEhow/womgktISPAp8AYbBFzMF4M3bbyBly5dWvLmzSspKSkuE9G/f3/9G0Lgdud+TITBhg0bPKokYC5gAhB8EZwRtFFlgrYamBkXDTGBUZ2Bso0ePVoDP77zcP6RI0fqXDP4G8Ebk9Uhs4B5YPDd6M1AAMwlg+tFBgPmo2PHjvq5gHtVZ2A7mJUZM2bIwoUL1dBs375ds074XAFNBAkm2WIi8E+HyZYIIc4Bb89IfVsNghxeQNauXat/I1uAid4WLVqU7dUZBvcyETAD/fr108wDAjgyNjARyAKgYaeRuTFMBL73sP3TTz8tf/nLX1xC+w0cA2B/ZD2QifClfO5tImAQYF52796t6zIzEfh8VqxYoZ/X4MGDNSNME0FCRcAmAg97XFycLF682LyKEGJjYCCQlrdDG4nx48dr7wrU/dvNRCADgawEMiAoH0wCZrnF7/jug8G4efOm29F/AttjP1wHqjrQO8IoI7ZH2wb0QnHf15uJgBlANQsMiQFNBLGagE0E0n5IoSHlRwhxFqhLT0xMNC8OOahiQZVFwYIFJX/+/DqVPYKsYSJy584tBQoUUKFxqDv3YyKOHz+uVQuvvvqqZhCKFCmiVTuofsjMRCCg4/go2+eff67lRBYHBgTljoyM1OoXo3zoJYKqC2yPY+J4COxffvmlXpPRGBLlePfddzN0M/VmIlAVsnr1annvvff0PKjqgFHIzESgIWdMTIy88847+nniMyxTpowcPHjQdT5CAiVgE4G3mPbt2+s/HSHEWaAeHcGSEEL8IWATga5G+BJiY0pCnAcyiaizJ4QQfwjYRCA1hno/QogzKVasmHkRIYT4RMAmAiPEoVsUIcSZfPzxx+ZFhBDiEzQRhIQ5NBGEEH+hiSAkzKGJIIT4C00EIWEKGkOjC2XRokXvOeETIYRkBU0EIWEIxiDAeAcYP+C3v/2tzrmAnhqEEHI/0EQQEoZgDoonn3xScuTIofrFL36hIy8SQsj9QBNBSBiC4Zp/85vfuEwE9Mc//lEeeeQRWwpltaN+/etf21YPP/ywpcKkapnx45070nV4vBSvGyH//LgqZTOVbdZVps5fmeWEcYAmgpAwBJNdYdhnw0D88pe/lOjoaB2S2Y7Clxl1f7Kal19+2bzIxawlKdJ+4DiZn7JNFq7dTtlMc1Zukpa9R8neQ8fMt84DmghCwhA0qMQ01UabCMyiyaHrSXbizUQMnzRHxs9Z4RG8KPsoaki8bNzx0zT03qCJICTMYRdPEgy8mYgBcdMlbvZyj8BF2Udt+8fJutSfJn/zBk0EIWEOTQQJBjQRzhZNBCHEJ2giSDCgiXC2aCIIIT5BE0GCAU2Es+UoE3H79m25du2aNuyiKCp7hP8pjEqZFTQRJBjQRDhbjjERV65ckfXr18vSpUtl0aJFFEVlk/A/tWnTJjUT3qCJIMGAJsLZcoSJQP/vrVu3yrRp0yiKCpJ2797tddwAmggSDGginC1HmAhM+rN69WqPLz2KorJP69atU8OeGTQRJBjQRDhbjjER3377rceXHkVR2ac1a9bQRJCQQxPhbNFEUBSlookgVkAT4Ww5wkSgVwZSreYvPYqisk9oXOmENhGYinz+/PmSlJQkGzZs0J4l0K5du2Tu3Lm6HEIbD3x3bNy40bXM0ObNm/XlxB/wGR0/flyOHj1qXpUpp06dknLlykm+fPmkdevWcvPmTfMmYQtNhLPlCBOBf9oDBw7IzJkzPb74KIoKXLNmzcoyKNrBRKCMUVFR0r9/fxkzZoxMnDhRzQLm+GjRooUKyyGYBwTrJUuW6N+1atWSOnXqyOjRo2X58uV+B3JkayZMmKDHvF+Q7enbt6/f534QoYlwthxhIgCMxJEjR2THjh0URWWzTpw44TULAexgIvr16yfx8fGucS0Q0PG7YSKQocgM7Ddp0iSvVTa+QBORvdBEOFuOMRGEEGux2kRgHIt69eppd28zhono1q2bGgnInFm5HxOBbaZPny6lSpWSDz/8UKpXry7Hjh2Tq1evSsOGDSVXrlwa/LCuRo0aasK2bNmi5Tt//rweAy89xYoV0/FtDO5lIg4fPiyNGjWSokWLSoECBTQzlJaWpmVAhsgoQ5UqVfSY3oARRNVJhw4dpHDhwrrfiBEjdN3169dl+PDhruXNmzd3jQ1y5swZadKkiZYB69yNWrChiXC2aCIIIT5htYm4cOGCVK5cWfbt26dBr23btlKzZk3Ztm2by0RgqvKYmBgV2ki4cz8m4saNG2oW0LXcnKHJLBOBgNypUyfXPmifMWrUqAzB2GwiMIgezoMqFxwXI4jCUOCaUIYGDRrIypUrPcqQGdinZ8+eMmjQII9sx6FDh+T111/XqmEzqCqGGUJ5Qg1NhLNFE0EI8QmrTQTepBFw0QAUgfncuXPaPmLhwoXZXp2BoI02FXny5JHf//73MmDAAG2kCTIzEQAmoVmzZpqNQNn27Nnjsd7dRMAMFSlSRH73u9/peaDnnntOt0MZkN14++23dXl0dLSrDJmBTAnOu3PnTvMqLfeyZcvkscce0+PBOBigPNjv8ccf1/OZszjBhCbC2aKJIIT4hNUmAiA1P2TIEA166F3Rq1cvHbo7u02EOzgXUv8LFizQwI79J0+erI07zcdCzxG80eNcI0eO1MyAO2YTcfHiRc2uoPeZt2yDUQZkN4ztcGx0fd+7d6+rHPgcunbtqmU1l80dVL/861//umfV0OLFi6VMmTKuaplgQxPhbNFEEEJ8wg4mAl0ru3Tp4uppkTdvXjUOhokoXry4tkuA3N+0wf2YCBwP1QLGsSIjI/U7zGD//v0a/GvXri0dO3aUs2fPutYhCH/yyScZqkJQtYIMBdo3oJsn9ktJSdGyoLoCf+N6cK7OnTurGUEZYJKMMkREROh5DVA98dRTT+k6ZGkAjofsBdo34JhYN3XqVF2H7EKrVq10GaqBUB2EbA5AbxXjPBUrVpRx48Z5GKBgQRPhbNFEEEJ8wg4mAiC4IWijKgDCWzqCNVL5xjIIf5v38zUw4njIErgfyz1TgN/RfgHr8Mbu3u4BbQ4QpN3f5FFG9zJDRllwrEuXLrmWo+0HzIC5DGiv4F4Go0rHvBy4fxbG54CqEGxvLIdJMYAJcS9bVtUm2QlNhLNFE0EI8Qm7mAi7AtOAGVGRJXHvkUG8Y3cTMXflJomZOEOSVmz0WEfRRBBCfIQmwjv4jkOvkNTUVPMq4oVQmohu/YdLbGKS6+/5KanStf8wGRE/3WNbQ/GzF8tb77wrcdOSPdZRNBGEhBS01i9RooQUKlRIA445DW1naCJIMAiliWjcOkIie/SXBWu26d/Jq7bI1zXry6Axkzy2NUQT4V00EYSECDSEy58/v+TIkUP10EMPaQt+p0ATQYJBKE1Eyw5dJKJ7tMxZsUHGzZgvSSs3SsXqdWT4+Km6fvK85TJg5ATVlOQVugwmIt+7hSUmfroMGZug62YvW+9xbLNgVCbNWSIDR0/UfWIT52rmA+vmLN8gQ+Om6PLRCbMlefUWXT5r6VqZmH6+mek/h4ydrOdMXr1Vpi9arSZmwX/2n5++bMyUOa5yoKpl2LjE/5wnyXWeWUvXycT0Mkyet0zXjUqYpcbJXNZARBNBSIjAeAboo2+YCOiZZ57RrnsQehe0bNlSW9BDmKgJ/zNt2rRRoTV9u3btpH379iq01sfIhBB6D6CfP3oKGMLAR2jpj94MELr+YUTH7t27q3r06KE9ENADAOrdu7f06dNHuyBCGJcAw0xD6M740ksv6XgJAwcO1MGMoMGDB2uXy6FDh6qGDRumoyJC6I6JbAuMEoSBlzBvhTG3RWxsrIwdO1YVFxenQq+A8ePH6zgMEHpUYH4M9KqAEhIStHvllClTVImJidr7AMJIjxhlcsaMGSr0zoAw6uPs2bNlzpw5KkzAha6S0Lx58yQ5Odk1yqUhLMM6YzvsY+yPY0E4Lo5vnA/nRhmM8qBsRjlRZpQdwnXgmiBcH64T1wzh+vE5GJ8LPiMInxc+OwifIz5PY1AtfM74vPHZQ7gPuCcQ7g+Ee4X7hvsH4X7ivuIeQ7jfuPcQngPjmcDzgecEwjOD5wfPEYRnCs8XnjMIzxyeQQjPI4RnE88phGcWzy+eYwjPNJ7vJ554wvU/YFbRUhUkdsYij8Dlrzr3GZxuJLpKv5hx8uobb8mEmQuldPnKEjc9Wcal6+ua9aR6vSZSuVZ9XY6ADhOR6/U35MuKVaVWoxbycfFSUrdJa5m3arPH8d2VkLRUqtZpJDUbNNP9ukQP1X3mfrtZWkV2U/NSs2FzKVelpnQfMEL36T9yvOTJl19qpO9Tt2lr+ahYKekxMEYNwntFP1GTg+3GTZ8v7xb5ML1si9RA1GnSKr3MDfQ8FarVll6DR6mR6J9uHPK8U1Bq1G+q+qzklzIoNvOsiz+iiSAkRGBUQgzy424iMIIghj1GluLgwYP6f4LW/ejKh5EZMQYAqkAwyRS6CWIQIcx1sX37dh3VEPXv6OuPbn2YmRIDMWGQJMxuicZ9GH9g7dq1Oj4BuhSi2+GqVat0fAF0LVyxYoV278MgRGgUiDKiiyLGXoDpwXgDRlB94403XIHVCKrmYOoeSI0gagRQI3i6B073oOkeLN0DpXuQNAKkOTC6B0XDBLkHRCMYugdC9yDoHgDNBsyb+XI3XobpQlnczZa70TJMlrvBcjdXhrFyN1SGqXI3VO5myjBS7ibKME+Qu3FyN024t7jHuNe457j3eAbwLEB4LvB84DnB84LnBs8PniM8T3iu8HzhOcPzhucOzx+EZxHPJJ5NPKN4VvHM4tnFM4xnGc80nu3nn39en3U88xCef/wf4H8iqv9IiZu9zCNw+auOvQZKo1YRaiSq1WuswRtmYczkOdKodYR06jNIMwgIwAjiTdtEqYl44+13ZHBsgi5HIEdAj5/l3dzgrf/TEqVl8tyM5UemoFKNejJt4Wr9Gwbhq0rVtAEnTMTb+VF1Mk+zDh17D5TyVWtpRqFijbrSue9g3ad1VPd0o9FUy9p7yGj5Iv0ajKwE2nd8nX587NMv/XgoK7IYKHvLiC7SrnNvj7IGIpoIQkIEuu5hIKJHH31UHn74YXn22Wf1C9kpsDqDBINQVmf0HT5WKlavm/7G3lyzEbUbt5TipcvJlOTl+qb+zHMvyIsvvexS+659PNpEzFicIgXfL+q1MSaEoI3MR+7X35Scr7yqx5q3aouMTjcXOV/JLc+/mNN1HmQ5DBPxeZny9+wJEj0iTg3BmClJ8lmpr2TCrIW6HI1Ff/+HJ+SFf/7LdbxylWvI7OXr1USUKltJZi5Z63G87BJNBCEhBP308VaJt0+8vTkJmggSDEJpIgbHTtKsQoMWbSUxeYVUrt1AipUuq8YAVQyoFjC3GTCbCBiR4un7GJkEX4RqkdIVKsukpKU//V6+8j0banozETAZqKpAVQrKj7YSWI62FYU++MhlKtxFE2ECM9uhL/bp06cpmwgD6BgzAWbF6V2xcmRdRzmwvCFlM51IHSQ3LnlOzOQOTQQJBqE0EWio+Nobb8nA0fEalKvUbigffFpcg/aMxWu0ncKfn/4/+fNf/qqKiZ8hifNXSt6C78mfnvqzLitVtmK6gVjl6uGRmQaNmSgvvZxb9/l/zz6vVRDIRCBD0X3AcHn+nzld56nfvI3u481EQKi6eLtAIRk+fpprGRplduw9yHW8vz3zD2kR0VlNBk2EGxgGFvVrqHej7CdMUeytu+L576bLud1D5e7l9LfvKxspmynt9EI5mdpf7t65Zb51LmgiSDAIpYmgsl+OMBEYAhaNbMyBi7KP0JjKfRhdMydSB4vQQNhap1P7yJ0f08y3zgVNBAkGNBHOliNMBMaIR+tdc+Ci7COYCGSLMuPoxh4eQYuyl05s7EQTQUIOTYSzRRNBZYtoIpwvmghiBTQRzpZjTAT6D5sDF2UfwURgJsLMoImwv2giiBXQRDhbjjARaBOBAXnMgYuyjzC4jLdplmki7C+aCGIFNBHOliNMBLh8+bKOdoYeGpT9dPToUfMtywBNhP1FE0GsgCbC2XKMiQDXr1+XK1euUDbT1atXzbfKA5oI+4smglgBTYSz5SgTQZwLTYT9RRNBrIAmwtmiiSAhgSbC/qKJIFZAE+Fs0USQkEATYX/RRBAroIlwtmgiSEigibC/aCKIFdBEOFs0ESQk0ETYXzQRxApoIpwtmggSEmgi7C+aCGIFNBHOlqNMxM2bN+XixYty7tw5KoTCcNa3b9823477gibC/qKJ+C87d+6UiIgI7cKc3eB/aciQIbJ27VrzqrCEJsLZcoyJwGBTGNTIPFIiFRphoC+M0+EvNBH2lxNMxK1bt2TJkiXyj3/8Q55++mmpWLGifjdgBtkWLVrIn/70J10Ode3aVddVr15d/3788cdV+L1evXpeh2lft26dlC1bVl9ashtcQ+vWrWX+/PnmVWEJTYSz5QgTwanA7aGTJ0+ab43P0ETYX04wEXPmzNHvkQsXLujfyJThu8UwEd4Cc3x8vEyaNEm/T7KCJiJ00EQ4W44wEZzF0x6CkfMXmgj7y+4mAtWZLVu2lFWrVplXBcVElCxZUrp37y7vvfeeVK1aVc6fP6/r8H00efJkef/99+WTTz6RhQsXyt27d3UdTEejRo2kYMGCmiU5duyY65grVqzQ5QUKFJC//vWvGcraq1cvXQcdOHBAl6EqpUePHrJ69Wr57LPP5IsvvtCMoDeQeYmLi5OVK1fqsXANqampWr6lS5dKUlKS9O3bV9d17NhRDQ3AZ4vqG6MMH330kcydO9enzypQaCKcLZoIymcdPHjQfGt8hibC/rK7iUD2oXLlyvpdgO8EBPUzZ85oADRMxB/+8Aet0oDGjh2bYf/7NRFPPvmkTJ06VbcfNmyYJCcnazCGGUAAxoRzyIQ0a9ZM9uzZo20dsA3KhX1gNOrUqSNpaWmyZs0aKV++vBoD90wEtuvdu7dER0fr/jDqpUuX1rloYEjy5csnpUqV0v1wbOO8mYFzf/755/Lpp5+qoVi2bJnUrVtXrl27puXBNS1evFg/L3yW8+bN0/0GDBighgllgNGoXbu2bhMKaCKcLZoIymcxE/Fgy+4mAkGxZs2a+jZ+9uxZad++veTNm1eDcTAyEe7VGRMnTtT9jaDfoEEDbRw5aNAgzRAg8AIE/5EjR+o6GIVatWqpAejTp49mAoC7iYAJKl68uHTp0kX3gXBemA6c+6uvvpJNmzbpfqdPn5YFCxaoacoMmAiYmt27f/pSx+dUokQJ/RsmokOHDrocZWjbtq2aCHy/ov0IMhi4vg0bNkiNGjW8thnJTmginC1HmAhOBW4PsU3Egy27mwh8DyDYzp49WzMCCISoBli0aFFITUS/fv2kZ8+e2sDTkNGLqVOnTrotliF7gSwAJqhDFQLKCcwmAlUOKJdxLFTXIIDj3BUqVJBdu3a5F80r9zIRyGzgJSwzEwFghtBYFVU3OCf+3335nLIDbyZi3MxFEj93pUfgouyj7qMSJXVP1llqS00EwD8o6vY2b95MWSC8/QWS3rSribhzab1cObFCbp5f47Eu3GR3EwHQPgBpeLQbQABGeh9tEgwTgeoHBF/I3Jsou0wEsg4I1DDVWI8MCZbDSDRt2lQzB6dOndIgXa1aNS1HYmKitjlAkB83bpw88cQTaiIQzKOioqRz584a8N2Pl5WJQGbwqaee0p4mxrXi+I0bN9buo/h9/Pjx0q1bN1f1yr1MBLarUqWKfr8an51RhlDgzUQsXbNFanXoLzOXbaBsqMkLVkvDLkPk0PFT5lvngeUmAuCLAmaCCr281cP6gl1NxOEdc+TN13NK/KjOHuvCTU4wEchApKSk6Bs+qjagbdu2aYofgRp1+cbyadOmZdgXgXX9+vWuRpDeQPufmJgYl3GGqcD+2BfnwrGNc8EcwNCgWgDZBpQNgRw9SRISErSdAfbB8bA9jAwMhNFIEgEd1RhGuSMjI/V/DuceNWpUphlABP/mzZuruTEaSGIZjEeZMmX0WDgn2mSg3KimmDVrlm6Hsk6fPl22b9+umRJUZ8A0GWVAdsS43mDjzUSA+SvXy8Bx06Vt9GjKZhqekCSbd+0337J7YgsTQZyLXU3E9bOrZc2SWDm5b77HunBS0pR+UumLPNKkcSNX7wAzdjARxDvm6gxfQFUxepoYbZ5geGBOYJRCkY3IykSQBwOaCBIQdjQRP36/Tq6cXKnC78byWxfXavXGtdPf6k8YDchYn5a+DPtcTdetC2tdy+9e3qD7GMeEbl/8aT2qTa6d+mndjXMpHmWxUvOmDZRf/eohyZEjhyp//vwZuiYa0ETYH/RgadOmjezdu9e8yisjRoyQRx991KXY2FjzJkGDJiI8oIkgAWFHE7F38zT5utynkvuVF2TVwlGu5eNjOkmtKiWlasViUrLYe9K4bjn9PS09+J/5bqHEDGqnf1f48mPpGllPTQFMwtK5w6XiVx9LlQqfSd63cslruV+U9cvHyQ9nVsmIAW2l5n+O2bR+edm+NkFNh7lMVuitN152GQjokUce0bfZMWPGZNArr7zisYyiAlWhQoXMXxfkAYQmggSEHU0ElHY+Rdq3ru5hIurVLJNuAOLk36+9JOuXxUmT9MB//vBiuXx8uXx/bJlud2Jvsm537tAizUC0aV5V1i6NVXMwb9oAaVyvnGYiliQN0+2unlqp+y2bN0I6fFNT9zGXxwrlzZMrg4nAmygaLKIBobv+/e9/eyyjqECFBp3kwYcmggSE00xEZJuakpoyUT76IK8c2j7LZSIObpsl+d7KJQ8//Cv53//9lbyW+59qIlDtAWMwZkgHNQtDoltLt8j6erxpE3rJQw/9j+5jqH66qUCvEHN5rBBMzaOP/lp+9rOfyc9//nOtH79XYz5WZxBC/IUmggTEg2IiBvZuIWOHR+l2ezZNlWqViquJuHR8uUS0riFlSxfVKo1BvVu6qiuWzBkmpUu8r1kM8/ntosVzhkqDKgUlKjJCG9rdC5oIQoi/2MJEoBsTBmFB46Fg6n66NKILFLpioW+1+TjhoHv1x78XdjQRCbHdpPgnBeXF5/8uBfK9Jo3qlJVju+d6NRHzpg6Qj4u+IyU+KyStmlaW2tVKpZuIxdpoMrp7Uyn2cQFdB0W1rS3Xz67SBpg9OjaQ9wvlca1DdQfaUZjLZKWc0MWTEOJMLDcRGDoWdWfmURSDqXuldM0cOXLEY79w1IkTJ8wfTQbsaCKyU7s2JEqTeuXkh/+0c7h5PkUNRuK4nh7b2lU0EYSQYGGpibBq2OutW7d6zUhgUhvzPuEqjGrpLSPxoJuIo7vmSsvGlSRuRJRMiu0mQ/t9I9W//lx2b5jqsa1dRRNBCAkWlpoIqybg2rJliw7/mhkY2MW8T7gKQ/16m7DnQTcRqJrYuX6KJI7vqdkHCNUc5u3sLJoIQkiwCEsTgbdrb4ERY+Wb9wlXwUSgLUlmPOgm4kEQTQQhJFiEpYlgJsJ3hXsm4kEQTQQhJFhYaiLQJgLjupsDV7CVVZsITFxj3idcBRMRzm0iHgTRRBBCgoWlJgKgKyEyA+bgFUxhJr+s2LNnj8d+4ajMJm0yoImwv2giCCHBwnITAfDmbx6nIFjylpp3B+NEoC2Aef9wkre2EAY0EfYXTQQhJFjYwkQQ50ITYX/RRBBCggVNBAkImgj7iyaCEBIsaCJIQNBE2F80EYSQYEETQQKCJsL+ookghAQLmggSEDQR9hdNBCEkWNBEkICgibC/aCIIIcHCFiYCU4FjNk90KaRCJ3StxaihgUATYX/RRBBCgoXlJgKBbMeOHTqfBRV6YVCttLTMA0xW0ETYXzQRhJBgYamJsGoqcCqjTp06Zb41PkMTYX/RRBBCgoWlJsKqCbiojML8Jf5CE2F/0UQQQoIFTQTl01wimUETYX/RRBBCggVNBMVMxAMumghCSLCw1ERYNRU4lVEnTpww3xqfObltqNy9vN4jcFF20QY5sy063UTcNN86FzQRhBB/sdREAMwWGeqpwKn/KjU1VXvI+Mv5g7PkzPZ+6UZiwz0CGGW10k7Nl5Nb+8vdO7fMt84FTQQhxF8sNxEAY0ScO3eOskA//PCD+XbcN+f2TZHjm6PlUEp7ymY6vWuMpF05bL5lGaCJIIT4iy1MBCHEOmgiCCH+QhNBSJhDE0EI8ReaCELCHJoIQoi/0EQQEubQRBBC/IUmgpAwhyaCEOIvNBGEhDk0EYQQf6GJICTMoYkghPgLTQQhYQ5NBCHEXwI2EXfv3pVbtzIfDY8QYm9u3sx8SGxCCPFGwCaCEEIIIeEJTQQhhBBC/IImghBCCCF+QRNBCCGEEL+giSCEEEKIXwRsIn788UedypsQ4kwuXrxoXkQIIT4RsIngOBGEOBuOE0EI8ReaCELCHJoIQoi/0EQQEubQRBBC/MVyE4E2FaiTPXbsmBw6dMhSHT9+XNt3YBTOYIHr/f7770NyvUeOHJGzZ89aOiLhnTt3Qna9hw8fljNnzlh+vZcuXQrJ9WYllAFlQZm8QRNBCPEXS00EgjW+9Ddv3iwbN260hVJTU+Xy5cvmomYb586dky1btnicN5jau3evmhcrOH/+fMivd/fu3XL79m1zUUICDPHWrVs9ymSV8NnjHniDJoIQ4i+WmggEtgMHDnh88VktvMEFA7wRHjx40ON8wRZMmhU9aGAS8UZsLk8oFEwj6A1kf8xlsVq4B96yazQRhBB/sdxE7Nu3z+NLz2rhSzcYwETs37/f43zB1qZNm7RKIdQgcFllEq3qtoj/B3NZrBbuAU0EISQY0ETcQzQR2QNNhD1EE0EICRaWmwgrgmpWQko6GMBEWBFkYCKsSO8jcFlRfQOhQaEVWFV94024BzQRhJBgYKmJwBcb2h+Yv/SsFAJuVg3R/AXXe+LECT2H+bzBFBr6WdVj4dSpUyG/XjQmtOp6T58+HfLr9SaUBffAGzQRhBB/CdhE4C2nQ4cO5sU+c/36dT3Gnj17bKGjR48GtWX/jRs39G3VfN5gCdVFFy5cMBcjZKSlpWnXS3O5giWrrxfmJZTXm5VQFtwDb5QsWdK8iBBCfCJgE4HUf/v27eXWrVvmVT6Dag3sbwdl1ac+Owjl9QbTEPkKr9c6ZdW1F12sK1WqZF5MCCE+EbCJQPo2IiLCkoZ7hJDAmDFjhnTt2tW8mBBCfCJgE4EGez169NC6fkKIs4iMjJSkpCTzYkII8YmATQTS/5MmTZKpU6eaVxFCbAxG9qxcubIlA5ERQh4MAjYRAL0O3n//fcu61RFC7g+0l6hVq5Zcu3bNvIoQQnwmW0wEwORVUVFRkpycnGVrcEKIdaA3VHR0tCxYsCAkDYkJIQ8u2WYiAFp6Dx06VPr06aNVHFZOhEQI+S+YzXXRokUSExMjDRs21DEkkI0ghJBAyFYTYbBq1SrNShQuXFiee+45eeyxxyiKskh/+9vfJGfOnFp9ER8fn2W3T0II8ZWgmAhCiH3wNuQ1IYQEAk0EIYQQQvzi/wOJfAQBjDXaHwAAAABJRU5ErkJggg==>

[image2]: <data:image/png;base64,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>

[image3]: <data:image/png;base64,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>

[image4]: <data:image/png;base64,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>

[image5]: <data:image/png;base64,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>

[image6]: <data:image/png;base64,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>

[image7]: <data:image/png;base64,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>

[image8]: <data:image/png;base64,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>

[image9]: <data:image/png;base64,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>

[image10]: <data:image/png;base64,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>

[image11]: <data:image/png;base64,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>

[image12]: <data:image/png;base64,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>

[image13]: <data:image/png;base64,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>

[image14]: <data:image/png;base64,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>

[image15]: <data:image/png;base64,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>
