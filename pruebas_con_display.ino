#include <Wire.h>
#include <LiquidCrystal_I2C.h>

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
    static int counter = 0;
    lcd.setCursor(0, 0);
    lcd.print("Contador: ");
    lcd.print(counter);
    counter++;
    delay(1000); // Incrementa el contador cada segundo
}