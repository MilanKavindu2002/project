#include <DHT.h>

#define DHTPIN 5         // Pin which is connected to the DHT sensor (changed to pin D5)
#define DHTTYPE DHT11    // DHT 11

#define TRIGGER_PIN 2    // Pin connected to the ultrasonic sensor's trigger pin
#define ECHO_PIN 4       // Pin connected to the ultrasonic sensor's echo pin
#define BUZZER_PIN 11    // Pin connected to the buzzer
#define LED_PIN 6        // Pin connected to the LED

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(TRIGGER_PIN, OUTPUT); // Define Arduino pin for ultrasonic trigger
  pinMode(ECHO_PIN, INPUT);     // Define Arduino pin for ultrasonic echo
  pinMode(BUZZER_PIN, OUTPUT);  // Define Arduino pin for buzzer
  pinMode(LED_PIN, OUTPUT);     // Define Arduino pin for LED
  Serial.begin(9600);           // Enable serial communication
  dht.begin();                  // Initialize DHT sensor
}

void loop() {
  // DHT Sensor Reading
  float humidity = dht.readHumidity();
  float temperatureC = dht.readTemperature();

  // Ultrasonic Sensor Reading
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  long cm = duration / 29 / 2;     // Time convert to distance (in cm)

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperatureC)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Determine if motion is detected
  bool motionDetected = cm < 10;

  // Control buzzer and LED based on motion detection
  if (motionDetected) {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
  }

  // Create JSON string
  String jsonData = "{";
  jsonData += "\"temperature\": " + String(temperatureC) + ",";
  jsonData += "\"humidity\": " + String(humidity) + ",";
  jsonData += "\"distance\": " + String(cm) + ",";
  jsonData += "\"motion\": " + String(motionDetected);
  jsonData += "}";

  // Send JSON data to Raspberry Pi via serial
  Serial.println(jsonData);

  delay(2000); // Delay before next reading
}
