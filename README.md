# Estación Meteorológica WIFI - Microcontrolador ArduinoUNO WIFI R2

> La mayoría de componentes disponibles en: [CRCibernetica](https://www.crcibernetica.com).

Colección de proyectos IoT para el programa de Hortalizas de la Estación Experimental Fabio Baudrit [UCR](https://eeafbm.ucr.ac.cr/).


### Empezando

Estas instrucciones le proporcionarán una copia del proyecto en funcionamiento en su máquina local para fines de desarrollo y prueba. Este proyecto es una excelente herramienta de dimensioonamiento para crear plataformas, interactuar con varios datos provenientes de un mismo microcontrolador, administrar tópicos MQTT y ajustar backend.

### Requisitos previos

Qué cosas necesita para compilar y cargar el código y cómo instalarlas

1. [Arduino UNO WIFI R2](https://store-usa.arduino.cc/products/arduino-uno-wifi-rev2?selectedStore=us)

2. [Sparkfun Weather Shield V12 (ATENCIÓN: V11 no es compatible)](https://learn.sparkfun.com/tutorials/arduino-weather-shield-hookup-guide-v12/all)

3. [Sparkfun Weather Meter Kit](https://www.sparkfun.com/products/15901)

4. Módulo DHT22 (Usado en este proyecto) ó DHT11

5. Windows 10, Linux, MacOS con VSCode y Platformio instalado (mi versión VSCode: 1.68.1 (Universal))

6. Cable USB con Micro-B para conectar a ESPTTGO

7. Servidor MQTT (estoy ejecutando EMQX en Docker)



### Instalación y Uso

Un paso a paso que le indica cómo poner en marcha un entorno de desarrollo/producción

1. Abra el VSCODE
2. Clone el proyecto git clone https://github.com/AgroExperimentalFB/WIFIEstacionMeteorologica_ArduinoUNOWIFIR2.git
3. Agregue su información de red, endpoint, MQTT y servidor en user-variables.h
4. El main.cpp contiene un objeto llamado Config con las variables de cada sensor de la estación meteorológica que se modificaran con el dato leído en la función process_sensors().
5. Este proyecto utiliza el broker EMQX, cuyas funciones de conexión se encuentran en el iotcrv2-conector.h. Recomendamos tomar este archivo como inspiración para realizar la conexión, envío y recepción de parametros desde y hacía tu propio broker MQTT.


### Funcionamiento

El WIFIEstacionMeteorologica_ArduinoUNOWIFIR2 está en permanente ejecución del loop y escucha de los sensores, no posee estado de reposo ni ahorro de energía. La función watchdogOn() está presente en cada condición de verificación de estado de conección WIFI o MQTT para reiniciar el dispositivo en caso de muchas reconexiones fallidas o ciclos infinitos.

Este proyecto lee cada sensor y lo guarda en las variables correspondientes de un objeto llamado config, por último lo envía mediante MQTT a todos los dispositivos suscritos al mismo tópico (actuadores, sensores, paginas web, backends) en la función .

El envío por http no está implementado ya que recomendamos manejar las reglas de guardado en bases de datos desde un broker MQTT o en tu propio backend.

La función sendToIoTCRv2 emplea tópicos dinamicos de conexión para la plataforma demostrativa IoTCRProjects, esta función debe personalizarse dependiendo del broker mqtt, los tópicos y subtópicos de su propio backend.  

### [Video Demostración](https://youtu.be/IRQaIouT5iE)


### Despliegue

Consulte las instrucciones en **Requisitos previos**

### Versión

1.5 Actualización de versión principal, introducción de registro, cambio a PubSubClient, y correcciones de errores menores.

1.6 Se utiliza Sensor de temperatura/humedad externo al shield

### Autores

* [ISProjectsIoTCR (colaborador)](https://github.com/ISProjectsIoTCR)

### Licencia

Momentaneament: Este proyecto tiene la licencia MIT; consulte el archivo [LICENSE.md](LICENSE.md) para obtener más información.





