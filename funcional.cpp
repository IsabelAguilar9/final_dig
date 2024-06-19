#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <queue>
#include <DabbleESP32.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define LOGO_HEIGHT 50
#define LOGO_WIDTH 50
#define FIL 4
#define COL 4

SemaphoreHandle_t xSemaforo_Pantalla;
const int ledPins[] = {15, 2, 4, 16, 17, 5, 18, 19}; // los LEDs
const int numLeds = 8;
const int pot = 39;

const unsigned char epd_bitmap_wheel1[] PROGMEM = {0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x01,
                                                   0xfc, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x78, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00,
                                                   0x1e, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x01, 0x80,
                                                   0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x60, 0x00, 0x03,
                                                   0x00, 0x1f, 0xfe, 0x00, 0x30, 0x00, 0x07, 0x00, 0x7f, 0x3f, 0x80, 0x18, 0x00, 0x0e, 0x01, 0xe3,
                                                   0x31, 0xe0, 0x1c, 0x00, 0x0c, 0x03, 0x83, 0x30, 0x70, 0x0e, 0x00, 0x18, 0x07, 0x03, 0x30, 0x38,
                                                   0x06, 0x00, 0x18, 0x0e, 0x03, 0x30, 0x1c, 0x07, 0x00, 0x30, 0x0c, 0x03, 0x30, 0x0e, 0x03, 0x00,
                                                   0x30, 0x18, 0x07, 0xfc, 0x07, 0x03, 0x00, 0x20, 0x38, 0x1f, 0x1e, 0x07, 0x01, 0x80, 0x60, 0x3e,
                                                   0x38, 0x07, 0x1f, 0x81, 0x80, 0x60, 0x7f, 0xf0, 0x03, 0xff, 0x81, 0x80, 0x60, 0x61, 0xe0, 0x01,
                                                   0xf1, 0x80, 0x80, 0x60, 0x60, 0xc0, 0x00, 0xc1, 0xc0, 0xc0, 0x40, 0x78, 0xc0, 0x00, 0xc3, 0xc0,
                                                   0xc0, 0xc0, 0x7f, 0xc0, 0x00, 0x7f, 0xc0, 0xc0, 0xc0, 0xc7, 0xc0, 0x00, 0x7c, 0xc0, 0xc0, 0xc0,
                                                   0xc1, 0xc0, 0x00, 0x60, 0xc0, 0xc0, 0xc0, 0x40, 0xc0, 0x00, 0x60, 0xc0, 0xc0, 0x40, 0x60, 0xc0,
                                                   0x00, 0xc0, 0xc0, 0xc0, 0x60, 0x60, 0xc0, 0x00, 0xc0, 0xc0, 0xc0, 0x60, 0x60, 0x60, 0x01, 0xc1,
                                                   0x80, 0x80, 0x60, 0x70, 0x70, 0x03, 0x81, 0x81, 0x80, 0x60, 0x30, 0x78, 0x07, 0xc3, 0x81, 0x80,
                                                   0x20, 0x38, 0xde, 0x1e, 0xc3, 0x01, 0x80, 0x30, 0x19, 0xcf, 0xfc, 0x67, 0x03, 0x00, 0x30, 0x0f,
                                                   0x8c, 0x0c, 0x7e, 0x03, 0x00, 0x18, 0x0f, 0x1c, 0x06, 0x3c, 0x07, 0x00, 0x18, 0x07, 0x18, 0x03,
                                                   0x38, 0x06, 0x00, 0x0c, 0x03, 0xf0, 0x03, 0xf0, 0x0e, 0x00, 0x0e, 0x01, 0xf0, 0x01, 0xe0, 0x0c,
                                                   0x00, 0x07, 0x00, 0x7c, 0x0f, 0x80, 0x18, 0x00, 0x03, 0x00, 0x1f, 0xfe, 0x00, 0x30, 0x00, 0x01,
                                                   0x80, 0x01, 0xe0, 0x00, 0x70, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x70, 0x00,
                                                   0x00, 0x01, 0xc0, 0x00, 0x00, 0x38, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x1e,
                                                   0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x78, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x07, 0xe0, 0x00, 0x00,
                                                   0x00, 0x00, 0x3f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x00}; // Bitmap 1
const unsigned char epd_bitmap_wheel2[] PROGMEM = {0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x01,
                                                   0xf8, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x78, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00,
                                                   0x1e, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x03, 0x80,
                                                   0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x60, 0x00, 0x03,
                                                   0x00, 0x1f, 0xfe, 0x00, 0x30, 0x00, 0x07, 0x00, 0x7e, 0x3f, 0x80, 0x38, 0x00, 0x0e, 0x01, 0xe0,
                                                   0x33, 0xe0, 0x1c, 0x00, 0x0c, 0x03, 0x80, 0x63, 0xf0, 0x0c, 0x00, 0x1c, 0x07, 0x00, 0x63, 0x38,
                                                   0x06, 0x00, 0x18, 0x0f, 0x00, 0x62, 0x1c, 0x06, 0x00, 0x30, 0x0f, 0x80, 0xe6, 0x0e, 0x03, 0x00,
                                                   0x30, 0x19, 0xe7, 0xfe, 0x06, 0x03, 0x00, 0x30, 0x38, 0x7f, 0x1e, 0x03, 0x01, 0x00, 0x60, 0x3c,
                                                   0x38, 0x07, 0x03, 0x01, 0x80, 0x60, 0x7e, 0x70, 0x03, 0x81, 0x81, 0x80, 0x60, 0x67, 0xe0, 0x01,
                                                   0x81, 0x81, 0x80, 0x60, 0x61, 0xc0, 0x00, 0xc1, 0x81, 0x80, 0x60, 0x60, 0xc0, 0x00, 0xff, 0x80,
                                                   0x80, 0x40, 0x40, 0xc0, 0x00, 0xff, 0xc0, 0xc0, 0x40, 0x40, 0xc0, 0x00, 0xe0, 0xc0, 0xc0, 0x40,
                                                   0x40, 0xc0, 0x00, 0xc0, 0xc0, 0xc0, 0x40, 0x40, 0xc0, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x60, 0xc0,
                                                   0x00, 0xff, 0x80, 0x80, 0x60, 0x61, 0xc0, 0x00, 0xc1, 0x81, 0x80, 0x60, 0x67, 0xe0, 0x01, 0x81,
                                                   0x81, 0x80, 0x60, 0x7e, 0x70, 0x03, 0x81, 0x81, 0x80, 0x60, 0x3c, 0x38, 0x07, 0x03, 0x01, 0x80,
                                                   0x30, 0x38, 0x7e, 0x1e, 0x03, 0x01, 0x00, 0x30, 0x18, 0xe7, 0xfe, 0x06, 0x03, 0x00, 0x30, 0x1f,
                                                   0x80, 0xe6, 0x0e, 0x03, 0x00, 0x18, 0x0f, 0x00, 0x62, 0x1c, 0x06, 0x00, 0x1c, 0x07, 0x00, 0x63,
                                                   0x38, 0x06, 0x00, 0x0c, 0x03, 0x80, 0x63, 0xf0, 0x0c, 0x00, 0x0e, 0x01, 0xe0, 0x31, 0xe0, 0x1c,
                                                   0x00, 0x07, 0x00, 0x7c, 0x3f, 0x80, 0x38, 0x00, 0x03, 0x00, 0x1f, 0xfe, 0x00, 0x30, 0x00, 0x01,
                                                   0x80, 0x01, 0xe0, 0x00, 0x60, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x70, 0x00,
                                                   0x00, 0x03, 0x80, 0x00, 0x00, 0x38, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x1e,
                                                   0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x78, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x07, 0xe0, 0x00, 0x00,
                                                   0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x00}; // Bitmap 2
const unsigned char epd_bitmap_wheel3[] PROGMEM = {0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x01,
                                                   0xfc, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00,
                                                   0x1e, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x03, 0x80,
                                                   0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x60, 0x00, 0x03,
                                                   0x00, 0x1f, 0xfe, 0x00, 0x30, 0x00, 0x06, 0x00, 0x7c, 0x0f, 0x80, 0x38, 0x00, 0x0e, 0x01, 0xf0,
                                                   0x03, 0xe0, 0x1c, 0x00, 0x0c, 0x03, 0xf0, 0x03, 0xf0, 0x0c, 0x00, 0x18, 0x07, 0x38, 0x07, 0x38,
                                                   0x06, 0x00, 0x18, 0x0f, 0x18, 0x06, 0x3c, 0x06, 0x00, 0x30, 0x1f, 0x8c, 0x0c, 0x7e, 0x03, 0x00,
                                                   0x30, 0x19, 0x8f, 0xfc, 0x66, 0x03, 0x00, 0x60, 0x30, 0xde, 0x3e, 0xc3, 0x01, 0x00, 0x60, 0x70,
                                                   0xf8, 0x07, 0x83, 0x01, 0x80, 0x60, 0x60, 0x70, 0x03, 0x81, 0x81, 0x80, 0x60, 0x60, 0x60, 0x01,
                                                   0x81, 0x81, 0x80, 0x40, 0x60, 0xc0, 0x00, 0xc1, 0x80, 0x80, 0xc0, 0xc0, 0xc0, 0x00, 0xc0, 0x80,
                                                   0x80, 0xc0, 0xc1, 0x80, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc1, 0x80, 0x00, 0xe0, 0xc0, 0xc0, 0xc0,
                                                   0xc7, 0x80, 0x00, 0xf8, 0xc0, 0xc0, 0xc0, 0xff, 0x80, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xf8, 0xc0,
                                                   0x00, 0xc7, 0x80, 0x80, 0xc0, 0x60, 0xc0, 0x00, 0xc1, 0x80, 0x80, 0x60, 0x61, 0xe0, 0x01, 0xe1,
                                                   0x81, 0x80, 0x60, 0x7f, 0xf0, 0x03, 0xff, 0x81, 0x80, 0x60, 0x7e, 0x38, 0x07, 0x1f, 0x01, 0x80,
                                                   0x60, 0x38, 0x1e, 0x1e, 0x07, 0x01, 0x00, 0x30, 0x18, 0x0f, 0xf8, 0x06, 0x03, 0x00, 0x30, 0x1c,
                                                   0x03, 0x70, 0x0e, 0x03, 0x00, 0x18, 0x0e, 0x03, 0x30, 0x1c, 0x06, 0x00, 0x18, 0x07, 0x03, 0x30,
                                                   0x38, 0x06, 0x00, 0x0c, 0x03, 0x83, 0x30, 0x70, 0x0c, 0x00, 0x0e, 0x01, 0xe3, 0x31, 0xe0, 0x1c,
                                                   0x00, 0x06, 0x00, 0x7f, 0x3f, 0x80, 0x18, 0x00, 0x03, 0x00, 0x1f, 0xfe, 0x00, 0x30, 0x00, 0x01,
                                                   0x80, 0x01, 0xe0, 0x00, 0x60, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x60, 0x00,
                                                   0x00, 0x03, 0x80, 0x00, 0x00, 0x38, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x1e,
                                                   0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x78, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x07, 0xe0, 0x00, 0x00,
                                                   0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00}; // Bitmap 3

const unsigned char *wheelFrames[] = {epd_bitmap_wheel1, epd_bitmap_wheel2, epd_bitmap_wheel3};

char keys[FIL][COL] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

byte filas[FIL] = {33, 25, 26, 27};
byte columnas[COL] = {14, 12, 13, 23};

Keypad teclado = Keypad(makeKeymap(keys), filas, columnas, FIL, COL);

QueueHandle_t keyQueue;
volatile int speedDelay = 500;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

TaskHandle_t id_Taskanimacion;
TaskHandle_t id_TaskBlink;
TaskHandle_t id_TasRueda;
// Prototipos de funciones de tareas
void Task_animacion_cuad(void *parameter);
void Task_bluetooth(void *parameter);
void Task_blink(void *parameter);
void Task_read_keypad(void *parameter);
void Task_process_Key(void *parameter);
void Task_rueda(void *parameter);

void setup()
{
    Serial.begin(9600);
        Dabble.begin("MyEsp32");
    // Iniciar la pantalla OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }

    // Crear cola para las teclas presionadas
    keyQueue = xQueueCreate(10, sizeof(char));

    display.display();
    delay(2000);
    display.clearDisplay();

    // Dibujar las líneas fijas
    display.drawRect(0, 0, 128, 64, SSD1306_WHITE);
    display.drawLine(64, 0, 64, 63, SSD1306_WHITE);
    display.display();
    for (int i = 0; i < numLeds; i++)
    {
        pinMode(ledPins[i], OUTPUT);   // Configurar todos los pines de los LEDs como salida
        digitalWrite(ledPins[i], LOW); // Apagar todos los LEDs al inicio
    }
    // Crear tareas
    xTaskCreatePinnedToCore( Task_animacion_cuad, "Animacion del cuadro", 2048, NULL, 1, &id_Taskanimacion, 0 );
    xTaskCreatePinnedToCore(Task_bluetooth,"bluetooth_cuadro", 2048, NULL,2,NULL, 1);
    xTaskCreatePinnedToCore(Task_blink,"Secuencia de leds",2048, NULL, 3, &id_TaskBlink, 0);
    xTaskCreatePinnedToCore(Task_read_keypad, "Task_read_keypad", 4096, NULL, 4, NULL,1);
    xTaskCreatePinnedToCore(Task_process_Key, "Task_process_Key", 4096, NULL, 5, NULL,0);
    xTaskCreatePinnedToCore(Task_rueda, "Task_rueda", 4096, NULL, 6, &id_TasRueda,1);
}

void loop()
{
    // No se usa el loop en FreeRTOS, las tareas manejan la ejecución del programa
}
int keyToMilliseconds(char key)
{
    switch (key)
    {
    case '0':
        return 0;
    case '1':
        return 100;
    case '2':
        return 200;
    case '3':
        return 300;
    case '4':
        return 400;
    case '5':
        return 500;
    case '6':
        return 600;
    case '7':
        return 700;
    case '8':
        return 800;
    case '9':
        return 900;
    case 'A':
        return 1000;
    case 'B':
        return 1100;
    case 'C':
        return 1200;
    case 'D':
        return 1300;
    case '*':
        return 1400;
    case '#':
        return 1500;
    default:
        return -1; // Valor inválido
    }
}

void Task_animacion_cuad(void *parameter)
{
    int y = 15; // Posición vertical inicial del cuadro
    int x = 15; // Posición horizontal 
    const int cuadGrande = 15;
    const int cuadMediano = 13;
    const int cuadPequeno = 11;
    const int cuad1 = 9;
    const int cuad2 = 7;
    const int cuad3 = 5;
    const int cuad4 = 3;
    const int cuad5 = 1;
    TickType_t lastTime = xTaskGetTickCount();
    int movementSpeed = 5;

        while (true)
        {
    
            Dabble.processInput();

            TickType_t currentTime = xTaskGetTickCount();
            if (currentTime - lastTime >= movementSpeed / portTICK_PERIOD_MS)
            {
                lastTime = currentTime;

                if (GamePad.isDownPressed())
                {
                    y += 4; // Mover hacia abajo 
                    if (y > (SCREEN_HEIGHT - cuadGrande / 2 - 1))
                        y = SCREEN_HEIGHT - cuadGrande / 2 - 1; // Limitar 
                }
                if (GamePad.isUpPressed())
                {
                    y -= 4; // Mover hacia arriba 
                    if (y < (cuadGrande / 2))
                        y = cuadGrande / 2; // Limitar 
                }
                if (GamePad.isLeftPressed())
                {
                    x -= 4; // Mover hacia la izquierda 
                    if (x < (cuadGrande / 2))
                        x = cuadGrande / 2; // Limitar el movimiento hacia la izquierda
                }
                if (GamePad.isRightPressed())
                {
                    x += 4; // Mover hacia la derecha
                    if (x > (56))
                        x = 56; // Limitar 
                }
                display.drawRect(0, 0, 128, 64, SSD1306_WHITE);
                display.drawLine(64, 0, 64, 63, SSD1306_WHITE);
                // animacion del cuadro
                display.drawRect(x - cuadGrande / 2, y - cuadGrande / 2, cuadGrande, cuadGrande, SSD1306_WHITE); // cuadro grande
                display.display();
                vTaskDelay(100 / portTICK_PERIOD_MS);
                display.drawRect(x - cuadMediano / 2, y - cuadMediano / 2, cuadMediano, cuadMediano, SSD1306_WHITE); // cuadro2
                display.display();
                vTaskDelay(100 / portTICK_PERIOD_MS);
                display.drawRect(x - cuadPequeno / 2, y - cuadPequeno / 2, cuadPequeno, cuadPequeno, SSD1306_WHITE); // cuadro3
                display.display();
                vTaskDelay(100 / portTICK_PERIOD_MS);
                display.drawRect(x - cuad1 / 2, y - cuad1 / 2, cuad1, cuad1, SSD1306_WHITE); // cuad4
                display.display();
                vTaskDelay(100 / portTICK_PERIOD_MS);
                display.drawRect(x - cuad2 / 2, y - cuad2 / 2, cuad2, cuad2, SSD1306_WHITE); // cuad5
                display.display();
                vTaskDelay(100 / portTICK_PERIOD_MS);
                display.drawRect(x - cuad3 / 2, y - cuad3 / 2, cuad3, cuad3, SSD1306_WHITE); // cuad6
                display.display();
                vTaskDelay(100 / portTICK_PERIOD_MS);
                display.drawRect(x - cuad4 / 2, y - cuad4 / 2, cuad4, cuad4, SSD1306_WHITE); // cuad7
                display.display();
                vTaskDelay(100 / portTICK_PERIOD_MS);
                display.drawRect(x - cuad5 / 2, y - cuad5 / 2, cuad5, cuad5, SSD1306_WHITE); // cuad8
                display.display();
                vTaskDelay(100 / portTICK_PERIOD_MS);
                // Desaparecer animacion
                display.drawRect(x - cuadGrande / 2, y - cuadGrande / 2, cuadGrande, cuadGrande, SSD1306_BLACK); // cuad1
                display.display();
                vTaskDelay(100 / portTICK_PERIOD_MS);
                display.drawRect(x - cuadMediano / 2, y - cuadMediano / 2, cuadMediano, cuadMediano, SSD1306_BLACK); // cuad2
                display.display();
                vTaskDelay(100 / portTICK_PERIOD_MS);

                // Desaparecer el cuadro pequeño
                display.drawRect(x - cuadPequeno / 2, y - cuadPequeno / 2, cuadPequeno, cuadPequeno, SSD1306_BLACK); // cuad3
                display.display();
                vTaskDelay(100 / portTICK_PERIOD_MS);
                display.drawRect(x - cuad1 / 2, y - cuad1 / 2, cuad1, cuad1, SSD1306_BLACK); // cuad4
                display.display();
                vTaskDelay(100 / portTICK_PERIOD_MS);
                display.drawRect(x - cuad2 / 2, y - cuad2 / 2, cuad2, cuad2, SSD1306_BLACK); // cuad5
                display.display();
                vTaskDelay(100 / portTICK_PERIOD_MS);
                display.drawRect(x - cuad3 / 2, y - cuad3 / 2, cuad3, cuad3, SSD1306_BLACK); // cuad6
                display.display();
                vTaskDelay(100 / portTICK_PERIOD_MS);
                display.drawRect(x - cuad4 / 2, y - cuad4 / 2, cuad4, cuad4, SSD1306_BLACK); // cuad7
                display.display();
                vTaskDelay(100 / portTICK_PERIOD_MS);
                display.drawRect(x - cuad5 / 2, y - cuad5 / 2, cuad5, cuad5, SSD1306_BLACK); // cuad8
                display.display();
                vTaskDelay(100 / portTICK_PERIOD_MS);
            }
        }

}

void Task_blink(void *parameter)
{

    while (true)
    {
        int potValue = analogRead(pot); // Leer el valor del potenciómetro
        int delayTime = potValue / 10;
        // Secuencia de parpadeo de los LEDs
        for (int i = 0; i < numLeds; i += 2)
        {
            digitalWrite(ledPins[i], HIGH); // Encender
            vTaskDelay(delayTime / portTICK_PERIOD_MS);
        }

        for (int i = 0; i < numLeds; i += 2)
        {
            digitalWrite(ledPins[i], LOW); // Apagar
            vTaskDelay(delayTime / portTICK_PERIOD_MS);
        }

        // impar
        for (int i = 1; i < numLeds; i += 2)
        {
            digitalWrite(ledPins[i], HIGH); // Encender
            vTaskDelay(delayTime / portTICK_PERIOD_MS);
        }

        for (int i = 1; i < numLeds; i += 2)
        {
            digitalWrite(ledPins[i], LOW); // Apagar
            vTaskDelay(delayTime / portTICK_PERIOD_MS);
        }
    }
}

void Task_bluetooth(void *parameter)
{
    while (true)
    {
        Dabble.processInput();

        if (GamePad.isDownPressed())
        {
            Serial.print("Down\n");
        }
        if (GamePad.isUpPressed())
        {
            Serial.print("Up\n");
        }
        if (GamePad.isLeftPressed())
        {
            Serial.print("Left\n");
        }
        if (GamePad.isRightPressed())
        {
            Serial.print("Right\n");
        }
        if (GamePad.isRightPressed())
        {
            Serial.print("Right\n");
        }
        if (GamePad.isCrossPressed())
        {
            Serial.print("cruz\n");
            vTaskSuspend(id_Taskanimacion);
            vTaskSuspend(id_TaskBlink);
             vTaskSuspend(id_TasRueda);
        }
        if (GamePad.isCirclePressed())
        {
            Serial.print("circulo\n");
            vTaskResume(id_Taskanimacion);
            vTaskResume(id_TaskBlink);
            vTaskResume(id_TasRueda);
        }
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

// Tarea para leer el teclado y enviar las teclas a la cola
void Task_read_keypad(void *parameter)
{
    char key;
    for (;;)
    {
        key = teclado.getKey();
        if (key)
        {
            xQueueSend(keyQueue, &key, portMAX_DELAY);
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

// Tarea para procesar las teclas recibidas desde la cola
void Task_process_Key(void *parameter)
{
    char key;
    while (1)
    {
        if (xQueueReceive(keyQueue, &key, portMAX_DELAY) == pdTRUE)
        {
            int delayMs = keyToMilliseconds(key);
            if (delayMs != 0)
            {
                speedDelay = delayMs;
            }
        }
    }
}

// Tarea principal para la animación de la rueda
void Task_rueda(void *parameter)
{
    int frame = 0;
    

        while (1)
        {
            display.fillRect(65, 2, 50, 50, SSD1306_BLACK);
            display.drawBitmap(72, 7, wheelFrames[frame], LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE);
            display.display();
            frame = (frame + 1) % 3;
            vTaskDelay(speedDelay / portTICK_PERIOD_MS);
        }


}
