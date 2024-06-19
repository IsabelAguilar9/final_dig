#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Definiciones para la pantalla OLED
#define SCREEN_WIDTH 128    // Ancho de la pantalla OLED, en píxeles
#define SCREEN_HEIGHT 64    // Altura de la pantalla OLED, en píxeles
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< Dirección I2C de la pantalla OLED (0x3C para 128x64, 0x3D para 128x32)

// Inicialización del objeto display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Definiciones para los gráficos
#define LOGO_HEIGHT 50
#define LOGO_WIDTH 50

uint8_t rueda = 0;

// Gráficos en formato bitmap
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
                                                   0x00, 0x00, 0x3f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x00};
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
                                                   0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x00};
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
                                                   0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00};

void setup()
{
    // Inicializar la pantalla OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("Fallo en la asignación de SSD1306"));
        for (;;)
            ;
    }

    display.display();
    delay(2000); // Pausa inicial de 2 segundos
    display.clearDisplay();
}

void loop()
{
    display.clearDisplay();

    // Mostrar gráfico en base al valor de 'rueda'
    switch (rueda)
    {
    case 0:
        display.drawBitmap((display.width() - LOGO_WIDTH) , (display.height() - LOGO_HEIGHT) / 2, epd_bitmap_wheel1, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE);
        rueda = 1;
        break;
    case 1:
        display.drawBitmap((display.width() - LOGO_WIDTH) , (display.height() - LOGO_HEIGHT) / 2, epd_bitmap_wheel2, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE);
        rueda = 2;
        break;
    case 2:
        display.drawBitmap((display.width() - LOGO_WIDTH), (display.height() - LOGO_HEIGHT) / 2, epd_bitmap_wheel3, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE);
        rueda = 0;
        break;
    }

    display.display();
    delay(1000); // Pausa de medio segundo entre frames
}
