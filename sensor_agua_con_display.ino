#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define el pin al que está conectado el sensor de agua.
const int sensorPin = A0; // Pin analógico A0

// Variable para almacenar el valor leído del sensor.
int sensorValue = 0;

// Configura la dirección I2C del display LCD (ajusta la dirección según el escaneo).
LiquidCrystal_I2C lcd(0x27, 16, 2); // Cambia 0x27 por la dirección correcta si es diferente.

void setup()
{
    // Inicializa la comunicación serie a 9600 baudios.
    Serial.begin(9600);

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
    // Lee el valor del sensor de agua.
    sensorValue = analogRead(sensorPin);

    // Imprime el valor leído en el monitor serie.
    Serial.print("Valor del sensor de agua: ");
    Serial.println(sensorValue);

    // Muestra el valor en el display LCD.
    lcd.clear();
    lcd.setCursor(0, 0); // Coloca el cursor en la primera línea.
    lcd.print("Sensor de Agua:");
    lcd.setCursor(0, 1); // Coloca el cursor en la segunda línea.
    lcd.print(sensorValue);

    // Espera 500 milisegundos antes de la siguiente lectura.
    delay(500);
}