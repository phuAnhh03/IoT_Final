// final test main.cpp

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// Wi-Fi credentials
const char* ssid = "Chitose";       // Replace with your Wi-Fi SSID
const char* password = "password"; // Replace with your Wi-Fi password
const char* mqtt_user = "Phuanh03"; // Replace with your username
const char* mqtt_password = "Phuanh03"; // Replace with your password

// MQTT Broker settings
const char* mqtt_server = "c90a1cfa6b9e4929a050840ad458da07.s1.eu.hivemq.cloud"; // HiveMQ broker
const int mqtt_port = 8883;                   // Default MQTT port

// LED settings
unsigned long comparator = 0;  // Get the counting time
const unsigned long interval = 10000;  // Interval in milliseconds (1 second)
bool flag = true;
bool enableU = true;

WiFiClientSecure espClient;
PubSubClient client(espClient);

const char* ca_cert = R"~~~(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)~~~";

void setupWiFi() {
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void setupMQTT() {
  client.setServer(mqtt_server, mqtt_port);

  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32Client",mqtt_user,mqtt_password)) { // Client ID can be any unique string
      Serial.println(" connected!");
      client.subscribe("esp32/led"); // Subscribe to the correct topic

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 2 seconds...");
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String payloadStr = ""; // Convert byte array to String
  for (int i = 0; i < length; i++) {
    payloadStr += (char)payload[i];  // Collect each byte as a char
  }

  // Compare payload with "ON" or "OFF"
  if (strcmp(topic, "esp32/led") == 0) {
    if (payloadStr == "ON") {
      digitalWrite(LED_PIN, HIGH);  // Turn LED ON
    } else if (payloadStr == "OFF") {
      digitalWrite(LED_PIN, LOW);   // Turn LED OFF
    } else if (payloadStr == "ENABLE") {
      enableU = true;
    } else if (payloadStr == "DISABLE") {
      enableU = false; 
    }
  }
}
void ultrasound() {
  if(enableU == true){
    // Send a pulse to the trigger pin
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    
    // Read the pulse duration from the echo pin
    long duration = pulseIn(ECHO_PIN, HIGH);
    
    // Calculate the distance in cm (speed of sound = 343 m/s, divide by 2 for round trip)
    long distance = (duration / 2) / 29.1;
    
    // Print the distance to the serial monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    
    // If the distance is less than or equal to 20 cm, turn on the LED
    if (distance <= 20) {
      digitalWrite(LED_PIN, HIGH);  // Turn on LED
      bool flag = true;     // safe flag true
    } else {
        // Negate safe flag if its true
      if(flag == true){
        comparator = millis();
        flag = false;
      }
      else
      {
        // Check if 10 second has passed since flag negateds
        if(millis()-comparator>interval){
          digitalWrite(LED_PIN, LOW);
        }
      }  
  }
    
    // Wait a short time before taking another measurement
    delay(500);
  }
}
void setup() {
  Serial.begin(115200);
  setupWiFi();
  espClient.setCACert(ca_cert);
  setupMQTT();
  client.setCallback(callback);

   // Set the pins for the sensor and LED
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  
  // Ensure the LED is off initially
  digitalWrite(LED_PIN, LOW);
  
}

void loop() {
  if (!client.connected()) {
    setupMQTT();
  }
  client.loop();
  ultrasound();
}
