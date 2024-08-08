
#include "ow-wifi.h"

// WiFi network credentials
const char *ssid = "Villanetz";
const char *password = "Kellerbad100%";

WiFiClient espWifiClient;

WiFiClient initWiFi()
{
	// if needed add here indication (e.g. LED) that connection starts
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	Serial.print("Connecting to WiFi ..");
	while (WiFi.status() != WL_CONNECTED)
	{
		Serial.print('.');
		// if available draw to a display to indicate ongoing connection process

		delay(1000);
	}
	// if needed add here indication (e.g. LED) that connection is established
	Serial.println(WiFi.localIP());
	// if available draw IP to a display to indicate
	return espWifiClient;
}
