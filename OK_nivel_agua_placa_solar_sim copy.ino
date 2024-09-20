/* Componentes Necesarios
Placa Solar de 6V: Para generar energía.
Controlador de Carga Solar: Para gestionar la carga de la batería y proporcionar una salida de 5V.
Batería de 3.7V: La batería del móvil.
Regulador de Voltaje Step-Up (Boost Converter): Para elevar el voltaje de 3.7V a 5V si es necesario.
Arduino: Para el control del sistema.
Módulo GSM (SIM800L): Para enviar mensajes de WhatsApp.
Esquema de Conexión
Placa Solar:

Conecta los terminales del panel solar a la entrada del controlador de carga solar (positivo y negativo).
Controlador de Carga Solar:

Conecta la salida del controlador de carga a la batería (positivo y negativo).
Conecta la salida de 5V del controlador de carga al pin de 5V del Arduino y al pin GND.
Batería:

Conecta la batería al controlador de carga solar (positivo y negativo).
Ejemplo de Conexión

Placa Solar + ----> IN+ (Controlador de Carga)
Placa Solar - ----> IN- (Controlador de Carga)
Batería + ----> BAT+ (Controlador de Carga)
Batería - ----> BAT- (Controlador de Carga)
OUT+ (Controlador de Carga) ---> 5V (Arduino)
OUT- (Controlador de Carga) ---> GND (Arduino)

*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// Configura la dirección I2C del display LCD (ajusta la dirección según el escaneo).
LiquidCrystal_I2C lcd(0x27, 16, 2); // Cambia 0x27 por la dirección correcta si es diferente.

SoftwareSerial gsm(7, 8); // RX, TX

const int sensorPin = A0; // Pin analógico al que está conectado el pin OUT del sensor
const int ledPin = 13;    // Pin del LED integrado en la placa Arduino

void setup()
{
    pinMode(ledPin, OUTPUT); // Configura el pin del LED como salida
    Serial.begin(9600);      // Inicia la comunicación serie a 9600 bps
    gsm.begin(9600);
    delay(1000);
    gsm.println("AT"); // Inicializar el módulo GSM
    delay(1000);

    // Inicializa el display LCD con 16 columnas y 2 filas.
    lcd.init();
    lcd.backlight(); // Enciende la luz de fondo del LCD.

    // Mensaje de bienvenida para verificar que el LCD funciona.
    lcd.setCursor(0, 0);
    lcd.print("Iniciando...");
    delay(2000); // Espera 2 segundos para mostrar el mensaje de bienvenida.
    lcd.clear();
}

void loop()
{
    int sensorValue = analogRead(sensorPin); // Lee el estado del sensor

    // Imprime el valor del sensor en el monitor serie
    Serial.print("Valor del sensor: ");
    Serial.println(sensorValue);

    // Muestra el valor del sensor en el LCD
    lcd.setCursor(0, 0); // Coloca el cursor en la primera columna, primera fila
    lcd.print("Sensor: ");
    lcd.print(sensorValue);
    lcd.print("    "); // Añade espacios en blanco para limpiar caracteres residuales

    if (sensorValue > 1000)
    { // Ajusta este umbral según sea necesario
        Serial.println("Agua detectada");
        lcd.setCursor(0, 1); // Coloca el cursor en la primera columna, segunda fila
        lcd.print("Agua detectada ");
        digitalWrite(ledPin, HIGH); // Enciende el LED
        sendWhatsAppMessage();
    }
    else
    {
        Serial.println("No se detecta agua");
        lcd.setCursor(0, 1); // Coloca el cursor en la primera columna, segunda fila
        lcd.print("No se detecta   ");
        digitalWrite(ledPin, LOW); // Apaga el LED
    }

    delay(5000); // Espera 5 segundos antes de la siguiente lectura
}

void sendWhatsAppMessage()
{
    gsm.println("AT+CMGF=1"); // Configurar el módulo en modo texto
    delay(1000);
    gsm.println("AT+CMGS=\"+1234567890\""); // Número de teléfono de destino
    delay(1000);
    gsm.print("El nivel de agua ha alcanzado el sensor.");
    delay(1000);
    gsm.write(26); // Enviar el mensaje
    delay(1000);
}