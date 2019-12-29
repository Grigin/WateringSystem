const int alarm_delay = 2000;
unsigned long previousMillis;
unsigned int lastAlarm = 0;

int floatSwitch = 0;
int soilSensor1 = 0;
int soilSensor2 = 0;

const int soilSensorPin1 = A4;
const int soilSensorPin2 = A3;
const int floatSwitchPin = D6;
const int pumpPowerPin = D1;

const int soilSensorThreshold = 2500;

const int MsInSecond = 1000;
const int MsInMinute = 60 * MsInSecond;
const int MsInHour = 60 * MsInMinute;
const String messageWatered("WATERED");
const String messageSkipped("SKIPPED");

void setup() {
    pinMode(floatSwitchPin, INPUT_PULLUP);
    pinMode(soilSensorPin1, INPUT);
    pinMode(soilSensorPin2, INPUT);
    pinMode(pumpPowerPin, OUTPUT);

    Particle.variable("soilSensor1", &soilSensor1, INT);
    Particle.variable("soilSensor2", &soilSensor2, INT);
    Particle.variable("floatSwitch", &floatSwitch, INT);

    digitalWrite(pumpPowerPin, HIGH);

    Serial.begin(9600);
}

void loop()  {
    floatSwitch = digitalRead(floatSwitchPin);
    if (floatSwitch == 1)  {
         const unsigned long now = millis();
         if ((now - lastAlarm) > alarm_delay)  {
             Particle.publish("LOW WATER", NULL);
             lastAlarm = now;
         }
     } else {
        soilSensor1 = analogRead(soilSensorPin1);
        soilSensor2 = analogRead(soilSensorPin2);
        if (soilSensor1 > soilSensorThreshold || soilSensor2 > soilSensorThreshold) {
             Particle.publish(
                 "googleDocs",
                 (String("{\"my-name\":\"") + messageWatered + String(soilSensor1) + " " + String(soilSensor2) + "\"}").c_str(),
                 60,
                 PRIVATE
             );
            digitalWrite(pumpPowerPin, LOW);
            delay(50 * MsInSecond);
            digitalWrite(pumpPowerPin, HIGH);

        } else {
             Particle.publish(
              "googleDocs",
                 (String("{\"my-name\":\"") + messageSkipped + String(soilSensor1) + " " + String(soilSensor2) + "\"}").c_str(),
                 60,
                 PRIVATE
             );
        }
    }

    delay(12 * MsInHour);
}
