// This example uses an ESP32 Development Board
// to connect to shiftr.io.
// bowter
// You can check on your device after a successful
// connection here: https://www.shiftr.io/try.
//
// by Joël Gähwiler
// https://github.com/256dpi/arduino-mqtt

#include <WiFiClientSecure.h>
#include <MQTT.h>
int relay = 14;
const char ssid[] = "ulveseth_Guest";
const char pass[] = "pass";

WiFiClientSecure net;
MQTTClient client; 
unsigned long lastMillis = 0;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  // do not verify tls certificate
  // check the following example for methods to verify the server:
  // https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFiClientSecure/examples/WiFiClientSecure/WiFiClientSecure.ino
  net.setInsecure();
  while (!client.connect("arduino", "kfbvYA.b4cxoA", "gJ5v9EaxHRSyFJ5ZhOMx0DYkFY6rew84UXvzTNxD1zE")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("1");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  if(payload == "started") {
    Serial.println("jag startar båten");
    digitalWrite(relay, 1);
  }
  if(payload == "stopped") {
    Serial.println("jag stoppar båten");
    digitalWrite(relay, 0);
  }

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid);
  client.begin("mqtt.ably.io", 8883, net);
  client.onMessage(messageReceived);
  connect();
  pinMode(relay, OUTPUT);
}

void loop() {
  client.loop();
  delay(2000);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
    Serial.println("\nconnected!");
  }

  // publish a message roughly every second.
  //if (millis() - lastMillis > 5000) {
  //  lastMillis = millis();
    //client.publish("1", "100%");
  //}
}