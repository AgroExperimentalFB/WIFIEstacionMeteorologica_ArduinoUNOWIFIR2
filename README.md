# ArduinoWiFi_WeatherStation
Estación meteorológica utilizando Arduino Uno WiFi R2 + Sparkfun Weather Shield + Plataforma IoTCR

LA MAYORIA DE LOS MATERIALES ESTÁN DISPONIBLES EN CR CIBERNETICA:

-ARDUINO UNO WIFI R2

-SPARKFUN WEATHER SHIELD

-WEATHER METER KIT

-DASHBOARD IoTProjects

Base de código para ARDUINO UNO WIFI R2 compatible con la plataforma IoTProjects.

¿Para que es esto?

IoTCR es una plataforma en la Nube para microcontroladores Arduino, ESP y cualquier dispositivo que soporte el protocolo MQTT. Su principal cualidad es que no depende de ningún servicio de terceros como Adafruit, Ubidots, Cayenne u otro.

Aquí un DEMO: https://app.iotcostarica.ml/demologin

¿Como funciona?

PASO_1: Ingrese a su cuenta en IoTCR y comience a crear el TEMPLATE de su proyecto

PASO_2: Agrege un dispositivo y seleccione el template creado. Automaticamente la plataforma le generará un PASSWORD de conexión.

PASO_3: Descargue este proyecto y en "INCLUDE>USER-VARIABLES.H" complete la información correspondiente a WIFI, IoTCR CONECTION

PASO_4: Gestione las lecturas de sus sensores con un constructor Json y envíelo como parametro a la función sendToDashboard(config);

