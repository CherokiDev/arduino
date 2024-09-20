const int sensorPin = A0; // Pin analógico al que está conectado el pin OUT del sensor
const int ledPin = 13;    // Pin del LED integrado en la placa Arduino

void setup()
{
    pinMode(ledPin, OUTPUT); // Configura el pin del LED como salida
    Serial.begin(9600);      // Inicia la comunicación serie a 9600 bps
}

void loop()
{
    int sensorValue = analogRead(sensorPin); // Lee el estado del sensor

    // Imprime el valor del sensor en el monitor serie
    Serial.print("Valor del sensor: ");
    Serial.println(sensorValue);

    if (sensorValue > 1000)
    { // Ajusta este umbral según sea necesario
        Serial.println("Agua detectada");
        digitalWrite(ledPin, HIGH); // Enciende el LED
    }
    else
    {
        Serial.println("No se detecta agua");
        digitalWrite(ledPin, LOW); // Apaga el LED
    }

    delay(1000); // Espera 1 segundo antes de la siguiente lectura
}