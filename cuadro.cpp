#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DabbleESP32.h>

#define SCREEN_WIDTH 128    // Ancho de la pantalla OLED en píxeles
#define SCREEN_HEIGHT 64    // Alto de la pantalla OLED en píxeles
#define OLED_RESET -1       // pinPWM de reset (o -1 si se comparte el pinPWM de reset de Arduino)
#define SCREEN_ADDRESS 0x3C ///< Ver la hoja de datos para la dirección; 0x3D para 128x64, 0x3C para 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void Task_animacion_cuad(void* parameter);
void Task_bluetooth(void* parameter);

void setup()
{
  Serial.begin(9600);
  Dabble.begin("MyEsp32");
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  
  display.display();
  delay(2000);
  display.clearDisplay();

  // Dibujar las líneas fijas
  display.drawRect(0, 0, 128, 64, SSD1306_WHITE);
  display.drawLine(64, 0, 64, 63, SSD1306_WHITE);
  display.display();

  xTaskCreatePinnedToCore(
    Task_animacion_cuad,
    "Animacion del cuadro",
    2048,
    NULL,
    2,
    NULL,
    1
  );

  xTaskCreatePinnedToCore(
    Task_bluetooth,
    "bluetooth_cuadro",
    2048,
    NULL,
    1,
    NULL,
    1
  );
}

void loop()
{

}

void Task_animacion_cuad(void* parameter) {
  int y = 16; // Posición vertical inicial del cuadro
  int x = 16; // Posición horizontal inicial del cuadro
 int cuadGrande = 15; // Tamaño del cuadro grande (32x32)
  int cuadMediano = 11; // Tamaño del cuadro mediano (24x24)
  int cuadPequeno = 7 ; // Tamaño del cuadro pequeño (16x16)
  int cuadMini = 3; 
  while (true) {
    // Procesar entrada del GamePad
    Dabble.processInput();
    
    if (GamePad.isDownPressed()) {
      y += 2; // Mover hacia abajo si se presiona el botón "Down"
      if (y > (SCREEN_HEIGHT - cuadGrande / 2 - 1)) y = SCREEN_HEIGHT - cuadGrande / 2 - 1; // Limitar el movimiento hacia abajo
    }
    if (GamePad.isUpPressed()) {
      y -= 2; // Mover hacia arriba si se presiona el botón "Up"
      if (y < (cuadGrande / 2)) y = cuadGrande / 2; // Limitar el movimiento hacia arriba
    }
    if (GamePad.isLeftPressed()) {
      x -= 2; // Mover hacia la izquierda si se presiona el botón "Left"
      if (x < (cuadGrande / 2)) x = cuadGrande / 2; // Limitar el movimiento hacia la izquierda
    }
    if (GamePad.isRightPressed()) {
      x += 2; // Mover hacia la derecha si se presiona el botón "Right"
      if (x > (56)) x = 56; // Limitar el movimiento hacia la derecha
    }

    // Limpiar el buffer
    display.clearDisplay();
    
    // Redibujar las líneas fijas
    display.drawRect(0, 0, 128, 64, SSD1306_WHITE);
    display.drawLine(64, 0, 64, 63, SSD1306_WHITE);
 // Dibujar el cuadro grande
    display.drawRect(x - cuadGrande / 2, y - cuadGrande / 2, cuadGrande, cuadGrande, SSD1306_WHITE);
    display.display();
    vTaskDelay(100 / portTICK_PERIOD_MS);

    // Dibujar el cuadro mediano
    display.drawRect(x - cuadMediano / 2, y - cuadMediano / 2, cuadMediano, cuadMediano, SSD1306_WHITE);
    display.display();
    vTaskDelay(100 / portTICK_PERIOD_MS);

    // Dibujar el cuadro pequeño
    display.drawRect(x - cuadPequeno / 2, y - cuadPequeno / 2, cuadPequeno, cuadPequeno, SSD1306_WHITE);
    display.display();
    vTaskDelay(100 / portTICK_PERIOD_MS);

    // Dibujar el cuadro mini
    display.drawRect(x - cuadMini / 2, y - cuadMini / 2, cuadMini, cuadMini, SSD1306_WHITE);
    display.display();
    vTaskDelay(100 / portTICK_PERIOD_MS);

    display.display();
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void Task_bluetooth(void* parameter) {
  while (true) {
    Dabble.processInput();
    
    if (GamePad.isDownPressed()) {
      Serial.print("Down\n");
    }
    if (GamePad.isUpPressed()) {
      Serial.print("Up\n");
    }
    if (GamePad.isLeftPressed()) {
      Serial.print("Left\n");
    }
    if (GamePad.isRightPressed()) {
      Serial.print("Right\n");
    }

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
