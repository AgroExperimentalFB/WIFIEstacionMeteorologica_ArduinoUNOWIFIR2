
//______________________________DATARECEIVER____________________________________
void sendToIoTCRv2(const Config & config)
{
      if (!(millis() - lastsendToDB > sendDBInterval))
      {



  //**************TRABAJE AQUÍ => CADA POSICIÓN ES UN WIDGET********************
        mqtt_data_doc["variables"][0]["last"]["value"] = config.windspeed;      //POR DEFECTO
        mqtt_data_doc["variables"][0]["last"]["save"] = 0;

        mqtt_data_doc["variables"][1]["last"]["value"] = config.winddir;
        mqtt_data_doc["variables"][1]["last"]["save"] = 0;

        mqtt_data_doc["variables"][2]["last"]["value"] = 1.0;
        mqtt_data_doc["variables"][2]["last"]["save"] = 0;

        mqtt_data_doc["variables"][3]["last"]["value"] = config.rainin;
        mqtt_data_doc["variables"][3]["last"]["save"] = 0;

        mqtt_data_doc["variables"][4]["last"]["value"] = config.windspeed;
        mqtt_data_doc["variables"][4]["last"]["save"] = 0;

        mqtt_data_doc["variables"][5]["last"]["value"] = config.rainin;
        mqtt_data_doc["variables"][5]["last"]["save"] = 0;

        mqtt_data_doc["variables"][6]["last"]["value"] = config.temp;
        mqtt_data_doc["variables"][6]["last"]["save"] = 0;

        mqtt_data_doc["variables"][7]["last"]["value"] = config.hum;
        mqtt_data_doc["variables"][7]["last"]["save"] = 0;

        mqtt_data_doc["variables"][8]["last"]["value"] = config.pressure;
        mqtt_data_doc["variables"][8]["last"]["save"] = 0;




//************************-> FIN DE LA ZONA DE TRABAJO <-***********************
        send_data_to_broker();
      }
      else{
        Serial.println("ENVIANDO A BASE DE DATOS");
        send_data_to_DB();
        lastsendToDB = millis();
        }
      }

//______________________________________________________________________________
bool get_mqtt_credentials()
{

  Serial.println("\nIniciando conexión segura para obtener tópico raíz...");
  WiFiClient wifi;


  HttpClient client_api = HttpClient(wifi, server, api_port);
  client_api.setTimeout(12000);



  if (!client_api) {
    Serial.println("Falló conexión!");
      delay(5000);
      watchDog++;

      if(watchDog == 5){
          watchdogOn();
        }
  }else {
    Serial.println("Conectados a servidor para obtener tópico - ok");
    // Make a HTTP request:
    String postData = "dId=" + dId + "&password=" + webhook_pass;
    String contentType = "application/x-www-form-urlencoded";

    client_api.post("/api/getdevicecredentials", contentType, postData);

    // read the status code and body of the response
    int statusCode = client_api.responseStatusCode();
    Serial.print("Status code: ");
    Serial.println(statusCode);

    if (statusCode < 0)
    {
      Serial.print(boldRed + "\n\n         Error Sending Post Request :( " + fontReset);
      client_api.stop();
      return false;
    }

    if (statusCode != 200)
    {
      Serial.print(boldRed + "\n\n         Error in response :(   e-> " + fontReset + " " + statusCode);
      client_api.stop();
      return false;
    }

    if (statusCode == 200)
    {
      String response  = client_api.responseBody();
      delay(500);

      Serial.print(boldGreen + "\n\n         Mqtt Credentials Obtained Successfully :) " + fontReset);
      Serial.print("Response: ");
      Serial.println(response);
      Serial.print("cantidad de digitos: ");
      Serial.println(response.length());

      DeserializationError error =  deserializeJson(mqtt_data_doc, response);
      Serial.println(error.f_str());
      client_api.stop();

    }
    return true;
  }
}

//______________________________________________________________________________
bool connect_to_IoTCRv2()
{

  if (!get_mqtt_credentials())
  {
    Serial.println(boldRed + "\n\n      Error getting mqtt credentials :( \n\n RESTARTING IN 5 SECONDS");
    Serial.println(fontReset);
    delay(5000);
    watchdogOn();                                      //FOR ARDUINO UNO WIFI R2
    //ESP.restart();                                                 //FOR ESP32
  }

  //Setting up Mqtt Server
  client.setServer(server, 1883);

  Serial.print(underlinePurple + "\n\n\nTrying MQTT Connection" + fontReset + Purple + "  ⤵");

  String str_client_id = "device_" + dId + "_" + random(1, 9999);

  const char *username = mqtt_data_doc["username"];
  const char *password = mqtt_data_doc["password"];
  String str_topic = mqtt_data_doc["topic"];



  if (client.connect(str_client_id.c_str(), username, password))
  {
    Serial.print(boldGreen + "\n\n         Mqtt Client Connected :) " + fontReset);
    delay(2000);

    client.subscribe((str_topic + "+/actdata").c_str());
  }
  else
  {
    Serial.print(boldRed + "\n\n         Mqtt Client Connection Failed :( " + fontReset);
  }
}

//______________________________________________________________________________
void send_data_to_broker()
{
  long now = millis();

  for (int i = 0; i < mqtt_data_doc["variables"].size(); i++)
  {

    if (mqtt_data_doc["variables"][i]["variableType"] == "output")
    {
      continue;
    }

    long freq = mqtt_data_doc["variables"][i]["variableSendFreq"];
    if (now - varsLastSend[i] > freq * 1000)
    {
      varsLastSend[i] = millis();

      String str_root_topic = mqtt_data_doc["topic"];
      String str_variable = mqtt_data_doc["variables"][i]["variable"];
      String topic = str_root_topic + str_variable + "/sdata";

      String toSend = "";

      serializeJson(mqtt_data_doc["variables"][i]["last"], toSend);
      client.publish(topic.c_str(), toSend.c_str());
      Serial.print(boldGreen + "\n\n         Mqtt ENVIADO:) " + fontReset);

      //STATS
      long counter = mqtt_data_doc["variables"][i]["counter"];
      counter++;
      mqtt_data_doc["variables"][i]["counter"] = counter;
    }
  }
}

//______________________________________________________________________________
void send_data_to_DB()
{
  long now = millis();

  for (int i = 0; i < mqtt_data_doc["variables"].size(); i++)
  {

    if (mqtt_data_doc["variables"][i]["variableType"] == "output")
    {
      continue;
    }

    mqtt_data_doc["variables"][i]["last"]["save"] = 1;

    String str_root_topic = mqtt_data_doc["topic"];
    String str_variable = mqtt_data_doc["variables"][i]["variable"];
    String topic = str_root_topic + str_variable + "/sdata";

    String toSend = "";

    serializeJson(mqtt_data_doc["variables"][i]["last"], toSend);
    client.publish(topic.c_str(), toSend.c_str());
    Serial.print(boldGreen + "\n\n         Mqtt ENVIADO:) " + fontReset);

      //STATS
    long counter = mqtt_data_doc["variables"][i]["counter"];
    counter++;
    mqtt_data_doc["variables"][i]["counter"] = counter;
  }
}


//______________________________________________________________________________
//CALLBACK ⤵
void callback(char *topic, byte *payload, unsigned int length)
{
  String incoming = "";

  for (int i = 0; i < length; i++)
  {
    incoming += (char)payload[i];
  }

  incoming.trim();

  process_incoming_msg(String(topic), incoming);
}


//______________________________________________________________________________
//TEMPLATE ⤵
void process_incoming_msg(String topic, String incoming){

  last_received_topic = topic;
  last_received_msg = incoming;

  String variable = splitter.split(topic, '/', 2);

  for (int i = 0; i < mqtt_data_doc["variables"].size(); i++ ){

    if (mqtt_data_doc["variables"][i]["variable"] == variable){

      DynamicJsonDocument doc(256);
      deserializeJson(doc, incoming);
      mqtt_data_doc["variables"][i]["last"] = doc;

      long counter = mqtt_data_doc["variables"][i]["counter"];
      counter++;
      mqtt_data_doc["variables"][i]["counter"] = counter;
    }
  }
  process_actuators();
}
