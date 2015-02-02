// This #include statement was automatically added by the Spark IDE.
#include "OledDisplay.h"

#include "rgbled.h"

// Define the pins we're going to call pinMode on
int ledRed = D6;
int ledBlue = D7;
int btnUp = D4;
int btnDn = D3;
int btnPad = D5;

bool btnUpLatch = false;
bool btnDnLatch = false;
bool btnPadLatch = false;

int tmpSensor = A0;
bool startup = true;

int reset = D1;
int cs = D0;
int dc = D2;
int count = 0;

double reading = 0.0;
double volts = 0.0;
double temp = 0.0;

int red, green, blue = 0;

int drawMode = 0;
int textMode = 0;

OledDisplay display = OledDisplay(reset, dc, cs);;

// This routine runs only once upon reset
void setup() {
    // Initialize D0 + D7 pin as output
    // It's important you do this here, inside the setup() function rather than outside it or in the loop function.
    pinMode(ledRed, OUTPUT);
    pinMode(ledBlue, OUTPUT);

    digitalWrite(ledRed, LOW);
    digitalWrite(ledBlue, LOW);

    pinMode(reset, OUTPUT);
    pinMode(dc, OUTPUT);
    pinMode(cs, OUTPUT);

    pinMode(btnUp, INPUT_PULLUP);
    pinMode(btnDn, INPUT_PULLUP);
    pinMode(btnPad, INPUT);

    Spark.variable("reading", &reading, DOUBLE);
    Spark.variable("volts", &volts, DOUBLE);
    Spark.variable("temp", &temp, DOUBLE);

    RGB.control(true);
    RGB.color(0,0,0);

    display.begin();
}

// This routine gets called repeatedly, like once every 5-15 milliseconds.
// Spark firmware interleaves background CPU activity associated with WiFi + Cloud activity with your code.
// Make sure none of your code delays or blocks for too long (like more than 5 seconds), or weird things can happen.
void loop() {
    //getTemp();

    if (digitalRead(btnPad) == HIGH) {
        if (btnPadLatch) { // only draw once per press
            drawPattern();
            btnPadLatch = false;
        }
    } else {
        btnPadLatch = true;
    }

    if (digitalRead(btnUp) == LOW) {
        digitalWrite(ledBlue, HIGH);
        if (btnUpLatch) { // only draw once per press
            drawText(true);
            btnUpLatch = false;
        }
    } else {
        btnUpLatch = true;
        digitalWrite(ledBlue, LOW);
    }

    if (digitalRead(btnDn) == LOW) {
        digitalWrite(ledRed, HIGH);
        if (btnDnLatch) { // only draw once per press
            drawText(false);
            btnDnLatch = false;
        }
    } else {
        btnDnLatch = true;
        digitalWrite(ledRed, LOW);
    }
}

void initStat() {
}

void drawPattern() {

    switch (drawMode) {
        case 0: // horizontal lines
            RGB.color(255,0,0);
            display.clear(CLEAR_BUFF);
            break;
        case 1: // horizontal lines
            RGB.color(0,255,0);
            display.fill(0xaa);
            break;
        case 2: // vertical lines
            RGB.color(0,0,255);
            for (int i=0; i<6; i++) {
                for (int j=0; j<32; j++) {
                    int c = j*2;
                    display.setByte(i, c, 0xff);
                    display.setByte(i, c+1, 0x00);
                }
            }
            break;
        case 3: {// count
            RGB.color(255,255,0);
            byte val = 0x00;
            for (int i=0; i<6; i++) {
                for (int j=0; j<64; j++) {
                    display.setByte(i, j, val++);
                    if (val > 0xff) val = 0x00;
                }
            }
            break;
        }
        case 4: // diagnal lines
            RGB.color(255,0,255);
            for (int i=0; i<6; i++) {
                for (int j=0; j<8; j++) {
                    int c = j*8;
                    display.setByte(i, c,   0x01);
                    display.setByte(i, c+1, 0x02);
                    display.setByte(i, c+2, 0x04);
                    display.setByte(i, c+3, 0x08);
                    display.setByte(i, c+4, 0x10);
                    display.setByte(i, c+5, 0x20);
                    display.setByte(i, c+6, 0x40);
                    display.setByte(i, c+7, 0x80);
                }
            }
            break;
        case 5: // progression
            RGB.color(0,255,255);
            for (int i=0; i<6; i++) {
                for (int j=0; j<8; j++) {
                    int c = j*8;
                    display.setByte(i, c,   0x01);
                    display.setByte(i, c+1, 0x03);
                    display.setByte(i, c+2, 0x07);
                    display.setByte(i, c+3, 0x0f);
                    display.setByte(i, c+4, 0x1f);
                    display.setByte(i, c+5, 0x3f);
                    display.setByte(i, c+6, 0x7f);
                    display.setByte(i, c+7, 0xff);
                }
            }
            break;
    }
    display.display();

    drawMode += 1;
    if (drawMode > 5) {
        drawMode = 0;
    }
}

void drawText(bool tempMode) {

    if (tempMode) {

        return;
    }

    display.clear(CLEAR_OLED);
    switch (textMode) {
        case 0: // small
            RGB.color(255,0,0);
            display.writeChar(0, 0, 'H');
            display.writeChar(1, 0, 'e');
            display.writeChar(2, 0, 'l');
            display.writeChar(3, 0, 'l');
            display.writeChar(4, 0, 'o');
            display.writeChar(1, 1, 'W');
            display.writeChar(2, 1, 'o');
            display.writeChar(3, 1, 'r');
            display.writeChar(4, 1, 'l');
            display.writeChar(5, 1, 'd');
            break;
        case 1: // med
            RGB.color(0,255,0);
            display.writeChar(0, 0, 'H');
            display.writeChar(1, 0, 'e');
            display.writeChar(2, 0, 'l');
            display.writeChar(3, 0, 'l');
            display.writeChar(4, 0, 'o');
            display.writeChar(1, 1, 'W');
            display.writeChar(2, 1, 'o');
            display.writeChar(3, 1, 'r');
            display.writeChar(4, 1, 'l');
            display.writeChar(5, 1, 'd');
            break;
        case 2: // large
            RGB.color(0,0,255);
            display.writeChar(0, 0, '1');
            display.writeChar(1, 0, '2');
            display.writeChar(2, 0, '3');
            display.writeChar(3, 0, '4');
            display.writeChar(4, 0, '5');
            break;
    }

    textMode += 1;
    if (textMode > 2) {
        textMode = 0;
    }
}

void getTemp() {
  reading = analogRead(tmpSensor);
  volts = (reading * 3.3) / 4095;
  temp = (volts - 0.5) * 100;
}
