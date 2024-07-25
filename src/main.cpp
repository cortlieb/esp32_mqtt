#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char *ssid = "Villanetz";
const char *password = "Kellerbad100%";
const char *mqttServer = "192.168.11.58";
const int mqttPort = 1883;
const char *mqttUser = "cortlieb";
const char *mqttPassword = "zV6N6im$qN%NeeRo";

WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
	Serial.begin(115200);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.println("Connecting to WiFi..");
	}
	Serial.println("Connected to the WiFi network");

	client.setServer(mqttServer, mqttPort);
	while (!client.connected())
	{
		Serial.println("Connecting to MQTT...");
		if (client.connect("ESP32Client", mqttUser, mqttPassword))
		{
			Serial.println("connected");
		}
		else
		{
			Serial.print("failed with state ");
			Serial.print(client.state());
			delay(2000);
		}
	}
	client.publish("esp32/status", "online");
}

void loop()
{
	static int fuellstand = 0;
	char buf[5];

	sprintf(buf, "%d", fuellstand);
	client.publish("esp32/fuellstand", buf);
	client.loop();
	delay(5000);

	fuellstand++;
	if (fuellstand > 1000)
	{
		fuellstand = 0;
	}
}