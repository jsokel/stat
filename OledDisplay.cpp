// John Sokel
#include "OledDisplay.h"

//#include "stdlib.h"
#include "spark_wiring_spi.h"
#include "rgbled.h"

#include "font5x7.h"
#include "font8x16.h"
#include "fontlargenumber.h"

static byte screen_buf[] = {
  /* LCD Memory organised in 64 horizontal pixel and 6 rows of byte
   B  B .............B  -----
   y  y .............y        \
   t  t .............t         \
   e  e .............e          \
   0  1 .............63          \
                                  \
   D0 D0.............D0            \
   D1 D1.............D1            / ROW 0
   D2 D2.............D2           /
   D3 D3.............D3          /
   D4 D4.............D4         /
   D5 D5.............D5        /
   D6 D6.............D6       /
   D7 D7.............D7  ----
  */
  //SparkFun Electronics LOGO

  // ROW0, BYTE0 to BYTE63
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x0F, 0x07, 0x07, 0x06, 0x06, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  // ROW1, BYTE64 to BYTE127
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x81, 0x07, 0x0F, 0x3F, 0x3F, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFC, 0xFC, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFC, 0xF8, 0xE0,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  // ROW2, BYTE128 to BYTE191
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC,
  0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF1, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xF0, 0xFD, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  // ROW3, BYTE192 to BYTE255
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x1F, 0x07, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  // ROW4, BYTE256 to BYTE319
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x1F, 0x1F, 0x0F, 0x0F, 0x0F, 0x0F,
  0x0F, 0x0F, 0x0F, 0x0F, 0x07, 0x07, 0x07, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  // ROW5, BYTE320 to BYTE383
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
  0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const page_t FULL_PAGE = {0, 5, 32, 95};

const byte *OledDisplay::fonts[] = {
   font5x7,
   font8x16,
   fontlargenumber
};

OledDisplay::OledDisplay(int reset, int dc, int cs) {
    rstPin = reset;
    dcPin = dc;
    csPin = cs;
}

void OledDisplay::begin() {
  pinMode(rstPin, OUTPUT);
  pinMode(dcPin, OUTPUT);
  pinMode(csPin, OUTPUT);

  // Default them high
  digitalWrite(dcPin, HIGH);
  digitalWrite(csPin, HIGH);

    // Display reset
  digitalWrite(rstPin, HIGH);
  delay(5);
  digitalWrite(rstPin, LOW);
  delay(10); // Needs to stay low for 10ms
  digitalWrite(rstPin, HIGH);

  // Init SPI
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setDataMode(SPI_MODE0);

  selectDevice(true, true);

  // Display Init sequence
  write(OLED_DISPLAY_OFF);

  write(OLED_DISPLAY_CLOCKDIV);
  write(0x80); // the suggested ratio 0x80

  write(OLED_MULTIPLEX);
  write(0x2F);

  write(OLED_DISPLAY_OFFSET);
  write(0x0); // no offset

  write(OLED_CHARGEPUMP);
  write(0x14); // charge pump enable

  write(OLED_START_LINE | 0x0);
  write(OLED_DISPLAY_NORMAL);
  write(OLED_DISPLAY_RESUME);

  write(OLED_SEGMENT_REMAP | 0x1);
  write(OLED_COMSCAN_DEC);

  write(OLED_MEMORY_MODE);
  write(0x00); // Horizontal

  write(OLED_COM_PINS);
  write(0x12);

  write(OLED_CONTRAST);
  write(0x8F);

  write(OLED_PRECHARGE);
  write(0xF1);

  write(OLED_VCOM_DESELECT);
  write(0x40);

  write(OLED_DISPLAY_ON);

  setPage(FULL_PAGE);

  setFont(0);
  for (int i=0; i<50; i++) {
    screen_buf[i] = activeFont.data[240+i];
  }

  display();
}

void OledDisplay::end() {
  selectDevice(true, true);
  write(OLED_DISPLAY_OFF);
  write(OLED_CHARGEPUMP);
  write(0x10); // charge pump enable
  selectDevice(false, false);

  SPI.end();
}

void OledDisplay::resetPage() {
  setPage(FULL_PAGE);
}

void OledDisplay::setPage(page_t page) {
  activePage.pageStart = page.pageStart;
  activePage.pageEnd = page.pageEnd;
  activePage.colStart = page.colStart;
  activePage.colEnd = page.colEnd;

  selectDevice(true, true);
  write(OLED_PAGE_ADDRESS);
  write(page.pageStart);
  write(page.pageEnd);

  write(OLED_COLUMN_ADDRESS);
  write(page.colStart);
  write(page.colEnd);
  selectDevice(false, true);
}

void OledDisplay::command(byte cmd) {
  selectDevice(true, true);
  SPI.transfer(cmd);
  selectDevice(false, true);
}

void OledDisplay::setByte(int page, int col, byte val) {
  screen_buf[page*64 + col] = val;
}

void OledDisplay::fill(byte val) {
  for (int i=0; i<384; i++) {
    screen_buf[i] = val;
  }
}

void OledDisplay::clear(int mode) {
  bool clearBuff = (mode & 0x01) == 0x01;
  bool clearOled = (mode & 0x02) == 0x02;

  if (clearBuff) {
    memset(screen_buf, 0x00, 384); // (64 x 48) / 8 = 384
  }
  if (clearOled) {
    resetPage(); // Do I need this?
    selectDevice(true, false);
    for (int i=0; i<=PAGE_MAX; i++) {
      for (int j=0; j<=COL_MAX; j++) {
        SPI.transfer(0x00);
      }
    }
    selectDevice(false, true);
  }
}

void OledDisplay::setFont(int fontId) {
  const byte *font = fonts[fontId];
  activeFont.id = fontId;
  activeFont.width = *font;
  activeFont.height = *(font+1);
  activeFont.startChar = *(font+2);
  activeFont.totalChars = *(font+3);
  activeFont.mapSize = (*(font+4))*100 + *(font+5);
  activeFont.data = font+6;
}

void OledDisplay::writeChar(int x, int y, char c) {
  // for now, this is using standard pages for rows
  const byte *font = activeFont.data;
  int cols = x * (activeFont.width + 1);
  int charPages = activeFont.height / 8;
  int charsPerRow = activeFont.mapSize / activeFont.width;
  //int charsRows = activeFont.totalChars / charsPerRow;
  int charOffset = c - activeFont.startChar;
  int rowOffset = (activeFont.mapSize * charPages) * (charOffset / charsPerRow);
  page_t page = {
      y * charPages,
      (y * charPages) + charPages - 1,
      FULL_PAGE.colStart + cols,
      FULL_PAGE.colStart + cols + activeFont.width - 1
  };
  //resetPage();
  setPage(page);
  int offset = rowOffset + ((charOffset % charsPerRow) * activeFont.width);
  //int offset = (c * charlen) - (activeFont.startChar * charlen);
  selectDevice(true, false);
  for (int i=0; i<charPages; i++) {
    for (int j=0; j<activeFont.width; j++) {
      SPI.transfer(*(font + offset + (activeFont.mapSize*i) + j));
    }
  }
  selectDevice(false, true);
  setPage(activePage);
}
void OledDisplay::writeText(int x, int y, char *text) {
  int i = x;
  for (int j=0; j<strlen(text); j++) {
    writeChar(i++, y, *(text + j));
  }
}

void OledDisplay::display(void) {
  int i;
  resetPage();
  selectDevice(true, false);
  for (i=0; i<384; i++)  {
    SPI.transfer(screen_buf[i]);
  }
  selectDevice(false, true);
}

void OledDisplay::write(byte data) {
  SPI.transfer(data);
}


void OledDisplay::line(int begX, int begY, int endX, int endY) {
}

void OledDisplay::selectDevice(bool enable, bool command) {
  digitalWrite(dcPin, command ? LOW : HIGH);
  digitalWrite(csPin, enable  ? LOW : HIGH);
}
