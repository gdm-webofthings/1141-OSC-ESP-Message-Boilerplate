// #define ARDUINOOSC_DEBUGLOG_ENABLE

// Please include ArduinoOSCWiFi.h to use ArduinoOSC on the platform
// which can use both WiFi and Ethernet
#include <ArduinoOSCWiFi.h>
// this is also valid for other platforms which can use only WiFi
// #include <ArduinoOSC.h>

// WiFi stuff
const char* ssid = "iFlone";
const char* pwd = "Florian8";
const IPAddress ip(192, 168, 1, 201);
const IPAddress gateway(192, 168, 1, 1);
const IPAddress subnet(255, 255, 255, 0);

// for ArduinoOSC
const char* host = "192.168.2.75";
const int recv_port = 54321;
const int bind_port = 54321;
const int send_port = 54321;
const int publish_port = 54321;
// send / receive variables
int i;
float f;
String s;

void onOscReceived(const OscMessage& m) {
    Serial.print(m.remoteIP());
    Serial.print(" ");
    Serial.print(m.remotePort());
    Serial.print(" ");
    Serial.print(m.size());
    Serial.print(" ");
    Serial.print(m.address());
    Serial.print(" ");
    Serial.print(m.arg<int>(0));
    Serial.print(" ");
    Serial.print(m.arg<float>(1));
    Serial.print(" ");
    Serial.print(m.arg<String>(2));
    Serial.println();
}

void setup() {
    Serial.begin(115200);
    delay(2000);

    // WiFi stuff (no timeout setting for WiFi)
#ifdef ESP_PLATFORM
    WiFi.disconnect(true, true);  // disable wifi, erase ap info
    delay(1000);
    WiFi.mode(WIFI_STA);
#endif
    WiFi.begin(ssid, pwd);
    WiFi.config(ip, gateway, subnet);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("WiFi connected, IP = ");
    Serial.print(WiFi.localIP());

    // publish osc messages (default publish rate = 30 [Hz])

    OscWiFi.publish(host, publish_port, "/publish/Client-01", 1, 1, 1)
        ->setFrameRate(60.f);

//    OscWiFi.publish(host, publish_port, "/Client-01", 1);

    // subscribe osc messages
    OscWiFi.subscribe(recv_port, "/Client-01", onOscReceived);
}

void loop() {
    OscWiFi.update();  // should be called to receive + send osc

    // or do that separately
    // OscWiFi.parse(); // to receive osc
    // OscWiFi.post(); // to publish osc
}
