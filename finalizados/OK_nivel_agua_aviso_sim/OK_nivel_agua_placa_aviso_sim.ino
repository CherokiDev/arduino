#include <SoftwareSerial.h>

SoftwareSerial gsm(7, 8); // RX, TX

const int sensorPin = A0; // Pin analógico al que está conectado el pin OUT del sensor
const int ledPin = 13;    // Pin del LED integrado en la placa Arduino
bool smsEnviado = false;  // Variable para controlar el envío de SMS

void setup()
{
    pinMode(ledPin, OUTPUT); // Configura el pin del LED como salida
    Serial.begin(9600);      // Inicia la comunicación serie a 9600 bps
    gsm.begin(9600);         // Inicia la comunicación serie con el módulo SIM800L
    delay(3000);             // Aumentamos el retardo para asegurar que el módulo esté listo

    // Reiniciar el módulo SIM800L
    pinMode(9, OUTPUT); // Pin para reiniciar el módulo SIM800L
    digitalWrite(9, LOW);
    delay(100);
    digitalWrite(9, HIGH);
    delay(5000); // Aumentamos el retardo para asegurar que el módulo se reinicie correctamente

    Serial.println("Módulo SIM800L inicializado. Verificando estado de la tarjeta SIM...");

    // Enviar comando AT+CPIN?
    gsm.println("AT+CPIN?");
    delay(1000); // Esperamos un segundo para recibir la respuesta
    printGSMResponse();

    Serial.println("Verificando la señal GSM...");

    // Enviar comando AT+CSQ
    gsm.println("AT+CSQ");
    delay(1000); // Esperamos un segundo para recibir la respuesta
    printGSMResponse();

    Serial.println("Verificando el registro en la red GSM...");

    // Enviar comando AT+CREG?
    gsm.println("AT+CREG?");
    delay(1000); // Esperamos un segundo para recibir la respuesta
    printGSMResponse();

    // Reintentar el registro en la red si no está registrado
    if (!isRegistered())
    {
        Serial.println("Reintentando el registro en la red GSM...");
        gsm.println("AT+CREG=1"); // Intentar registrarse manualmente
        delay(1000);              // Esperamos un segundo para recibir la respuesta
        printGSMResponse();
    }

    // Añadir un retardo adicional para asegurar que el módulo tenga tiempo de registrarse
    delay(10000);

    // Verificar nuevamente el registro en la red GSM
    Serial.println("Verificando nuevamente el registro en la red GSM...");
    gsm.println("AT+CREG?");
    delay(1000); // Esperamos un segundo para recibir la respuesta
    printGSMResponse();

    // Añadir un retardo adicional para asegurar que el sensor esté activo
    delay(10000); // Espera 10 segundos adicionales
}

void loop()
{
    int sensorValue = analogRead(sensorPin); // Lee el estado del sensor

    // Imprime el valor del sensor en el monitor serie
    Serial.print("Valor del sensor: ");
    Serial.println(sensorValue);

    if (sensorValue <= 1000 && !smsEnviado)
    { // Ajusta este umbral según sea necesario
        Serial.println("No se detecta agua");
        digitalWrite(ledPin, HIGH); // Enciende el LED
        sendSMS("+34xxxxxxxxx", "El deposito de agua ha llegado al sensor.");
        smsEnviado = true; // Marcar que el SMS ha sido enviado
    }
    else if (sensorValue > 1000)
    {
        Serial.println("Agua detectada");
        digitalWrite(ledPin, LOW); // Apaga el LED
    }

    delay(1800000); // Espera 30 minutos (1800000 milisegundos) antes de la siguiente lectura
}

void printGSMResponse()
{
    while (gsm.available())
    {
        char c = gsm.read();
        Serial.write(c); // Los desplegamos en el monitor serial
    }
}

bool isRegistered()
{
    gsm.println("AT+CREG?");
    delay(1000); // Esperamos un segundo para recibir la respuesta
    while (gsm.available())
    {
        String response = gsm.readString();
        Serial.print(response); // Los desplegamos en el monitor serial
        if (response.indexOf("+CREG: 0,1") != -1 || response.indexOf("+CREG: 0,5") != -1 ||
            response.indexOf("+CREG: 1,1") != -1 || response.indexOf("+CREG: 1,5") != -1)
        {
            return true; // Registrado en la red
        }
    }
    return false; // No registrado en la red
}

void sendSMS(const char *phoneNumber, const char *message)
{
    gsm.println("AT+CMGF=1"); // Configurar el módulo en modo texto
    delay(1000);
    gsm.print("AT+CMGS=\"");
    gsm.print(phoneNumber);
    gsm.println("\"");
    delay(1000);
    gsm.print(message);
    delay(1000);
    gsm.write(26); // Enviar Ctrl+Z para finalizar el mensaje
    delay(1000);
    Serial.println("SMS enviado!");
}