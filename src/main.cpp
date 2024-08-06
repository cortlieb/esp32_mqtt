/* TODO:
   - Kommentare
   - WLAN-Code aus anderen Projekten (wo er sich bewährt hat)
	 in externe, wiederverwendbare Datei mit .h-file auslagern
   - MQTT-Code in externe, wiederverwendbare Datei mit .h-file auslagern
   - In HomeAssistant Info haben wenn ESP32 nicht mehr sendet (--> 0l), evtl. über "Retain" Einstellung
   */

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char *ssid = "Villanetz";
const char *password = "Kellerbad100%";
const char *mqttServer = "192.168.11.58";
const int mqttPort = 1883;
const char *mqttUser = "cortlieb";
const char *mqttPassword = "zV6N6im$qN%NeeRo";

const float BARREL_CONST = 98.95718;
const float BARREL_SENSOR_HEIGHT = 9.25;

int calcLiter();

WiFiClient espClient;
PubSubClient client(espClient);

// TODO. mit PIN 6 und 7 komische Effekte (Download geht nicht, Fehlermeldungen wenn angeschlossen)
//--> dokumentieren
const int TRIGGER = 15;
const int ECHO = 2;

void setup()
{
	Serial.begin(115200);

	pinMode(TRIGGER, OUTPUT);
	pinMode(ECHO, INPUT);

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
	int volume = 0;
	char buf[5];
	volume = calcLiter();
	Serial.print(F("Volumen: "));
	Serial.println(volume);

	sprintf(buf, "%d", volume);
	client.publish("esp32/fuellstand", buf);
	client.loop();
	delay(5000);
}

int calcLiter()
{
	float distance = 0;
	unsigned long duration = 0; // TODO: muss das ein long seinoder reicht int?
	float waterHeight = 0;

	digitalWrite(TRIGGER, LOW);
	delay(5);
	digitalWrite(TRIGGER, HIGH);
	delay(10);
	digitalWrite(TRIGGER, LOW);
	duration = pulseIn(ECHO, HIGH);
	Serial.print(F("Duration: "));
	Serial.println(duration);
	// Prüfen ob Messwert zwischen 200cm und 0cm liegt
	// Calculate distance in dm for easier liter calculation
	distance = ((float)duration / 2.0) / 291.0;
	Serial.print(F("Entfernung: "));
	Serial.println(distance);
	// TODO: negative Werte vermeinden (z.B. wenn Deckel angehoben wird)
	waterHeight = BARREL_SENSOR_HEIGHT - distance;
	Serial.print(F("Wasserhöhe: "));
	Serial.println(waterHeight);

	return (int)round(waterHeight * BARREL_CONST);

	// if (entfernung >= 200 || entfernung <= 0)
	// {
	// 	entfernung = 0;
	// }
	// else
	// {
	// 	Liter = 199 - entfernung;
	// 	Liter = Liter * 84.7457;
	// }

	// return String(Liter);
}
