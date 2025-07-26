#include <Wire.h>
#include <U8g2lib.h>

// OLED avec broches explicites : SDA=D6(GPIO12), SCL=D7(GPIO13)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2, U8X8_PIN_NONE, 13, 12);

String input = "";
String dernierMessage = "";

void setup() {
  Serial.begin(9600);
  u8g2.begin();
  afficherTexte("En attente UART...");
}

void loop() {
  lireUART();
}

void lireUART() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      input.trim();
      dernierMessage = input;
      afficherTexte("Recu:\n" + dernierMessage);
      input = "";
    } else {
      input += c;
    }
  }
}

void afficherTexte(String txt) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  int y = 12;
  while (txt.length() > 0) {
    int idx = txt.indexOf('\n');
    String ligne = (idx >= 0) ? txt.substring(0, idx) : txt;
    u8g2.drawStr(0, y, ligne.c_str());
    y += 10;
    if (idx >= 0) txt = txt.substring(idx + 1);
    else break;
  }
  u8g2.sendBuffer();
}
