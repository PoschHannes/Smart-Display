// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library


#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#ifdef __AVR__

#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN 6 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 128 // Popular NeoPixel ring size
#define MAXCOLOR 255
// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 0.1 // Time (in milliseconds) to pause between pixels

struct RGB
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

RGB matrix[20][7]; // x geht von 0 bis 19, y geht von 0 bis 6
//RGB digits[3][7];
//digits.r = {0,0,0,1,1,0,1,0,1,0,0,0,1,0,1,1,1,0,0,0,0};

//matrix[2][0];



//makes it possible to override unused leds
void set_Pixel_Color(bool copyBlack, uint16_t n, uint8_t r, uint8_t g, uint8_t b) 
{
  if (copyBlack)  
  {
    pixels.setPixelColor(n, r, g, b);
  }
  else
  {
    if (r > 0 && b > 0 && g > 0)
    {
      pixels.setPixelColor(n, r, g, b); 
    } 
  }
}


void rowzero(bool copyBlack)
{
  int startpix;
  startpix = 0;
  if (matrix[0][3].r != 0 && matrix[0][3].g != 0 && matrix[0][3].b != 0)
  {
    set_Pixel_Color(copyBlack, startpix, matrix[0][3].r, matrix[0][3].g, matrix[0][3].b);
  }
}

void rowone(bool copyBlack)
{
  int startpix;
  startpix = 1;
  for (int i = 0; i <= 4; i++)
  {
    if (matrix[i][5 - i].r != 0 && matrix[i][5 - i].g != 0 && matrix[i][5 - i].b != 0)
    {
      set_Pixel_Color(copyBlack, startpix + i, matrix[1][5 - i].r, matrix[1][5 - i].g, matrix[1][5 - i].b);
    }
  }
}

void lastrow(bool copyBlack)
{
  int startpix;
  startpix = 125;
  for (int i = 0; i <= 2; i++)
  {
    if (matrix[i][4 - i].r != 0 && matrix[i][4 - i].g != 0 && matrix[i][4 - i].b != 0)
    {
      set_Pixel_Color(copyBlack, startpix + i, matrix[19][4 - i].r, matrix[19][4 - i].g, matrix[19][4 - i].b);
    }
  }
}

void copyrow(short rownmb, bool copyBlack)
{
  int startpix;
  //2 --> 6
  //4 --> 20
  //6 --> 34
  startpix = (rownmb - 2) * 7 + 6;
  for (int i = 0; i <= 6; i++)
  {
    if (matrix[i][6 - i].r != 0 && matrix[i][6 - i].g != 0 && matrix[i][6 - i].b != 0)
    {
      set_Pixel_Color(copyBlack, startpix + i, matrix[rownmb][i].r, matrix[rownmb][i].g, matrix[rownmb][i].b);
    }
  }
}

void copyinvrow(short rownmb, bool copyBlack)
{
  int startpix;
  //3 --> 13
  //5 --> 27
  //7 --> 41
  startpix = (rownmb - 2) * 7 + 6;

  for (int i = 0; i <= 6; i++)
  {
    if (matrix[i][6 - i].r != 0 && matrix[i][6 - i].g != 0 && matrix[i][6 - i].b != 0)
    {
      set_Pixel_Color(copyBlack, startpix + i, matrix[rownmb][6 - i].r, matrix[rownmb][6 - i].g, matrix[rownmb][6 - i].b);
    }
  }
}

//mapping the matrix to the real layout 
//matrix array prints the pixel on the right posiotion
void displ(bool copyBlack)
{
  for (int i = 0; i <= 19; i++)
  {
    if (i % 2 == 0 && i <= 18 && i >= 2)
    {
      copyrow(i, copyBlack);            
    }
    else if (i % 2 != 0 && i <= 17 && i >= 3)
    {
      copyinvrow(i, copyBlack);
    }

    else if (i == 0)
    {
      rowzero(copyBlack);
    }

    else if (i == 1)
    {
      rowone(copyBlack);
    }

    else if (i == 19)
    {
      lastrow(copyBlack);
    }
  }
}

void rainbowCycle(uint8_t wait)
{
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++)
  { // 5 cycles of all colors on wheel
    for (i = 0; i < pixels.numPixels(); i++)
    {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait)
{
 uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel((i*1+j) & 255));
    }
  }
  pixels.show();
  delay(wait);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85)
  {
    return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  else if (WheelPos < 170)
  {
    WheelPos -= 85;
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else
  {
    WheelPos -= 170;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


// turn on the right lights for the numbers
void setDigit(int offset, int select, int r, int g, int b) // choose 1 of 5 positions, select one number, set the RGB Color(range 0 -255 each) 
{                                                     // 0 - 4; 4 is only for ":"
  //calculate the right position
  int place;
  if (offset <= 1){
    place = offset * 4;
  }
  else if (offset == 4)
  {
    place == 10;
  }
  
  else{
    place = offset * 4 + 2;
  }
//switch beteen the numbers
  switch (select){
  case 0:
    for (int i = 2; i <= 4; i++){
      for (int j = 0; j <= 6; j++){
        if ((i == 2 && (j >= 1 && j <= 5 && j != 3)) || (i == 3 && (j == 1 || j == 5)) || (i == 4 && (j == 2 || j == 4))){
          matrix[i + place][j] = {r, g, b};
        }
        else{
          //rainbow(3);
        }
      }
    }
    break;

  case 1:
    for (int i = 0; i <= 4; i++){
      for (int j = 0; j <= 6; j++){
        if ( (i == 3 && (j == 3 || j == 5 || j == 1)) || (i == 4 && (j == 2 || j == 4))){
          matrix[i + place][j] = {r, g, b};
        }
        else{
          //rainbow(3);
        }
      }
    }

    break;

  case 2:
    for (int i = 0; i <= 4; i++){
        for (int j = 0; j <= 6; j++){
            if ( (i == 2 && (j >= 1 && j <= 5 && j != 4)) || (i == 4 && (j == 1 || j == 3 || j == 5)) || (i == 4 && j == 4)){
                matrix[i + place][j] = {r, g, b};
            }
            else{
            //rainbow(3);
            }
        }
    }
    break;

  case 3:
    for (int i = 0; i <= 4; i++){
        for (int j = 0; j <= 6; j++){
            if ( (i == 2 && (j % 2 != 0)) || (i == 3 && (j % 2 != 0)) || (i == 4 && (j == 4 || j == 2))){
                matrix[i + place][j] = {r, g, b};
            }
            else{
            //rainbow(3);
            }
        }
    }
    break;

  case 4:

    for (int i = 0; i <= 4; i++){
      for (int j = 0; j <= 6; j++){
        if ((i == 2 && (j >= 3 && j <= 5)) || (i == 3 && (j == 3 || j == 1)) || (i == 4 && (j == 2 || j == 4))){
          matrix[i + place][j] = {r, g, b};
        }
        else
        {
          //rainbow(3);
        }
      }
    }
    break;

  case 5:
    for (int i = 0; i <= 4; i++){
      for (int j = 0; j <= 6; j++){
        if ((i == 2 && (j %2 != 0 || j == 4)) || (i == 3 && (j %2 != 0)) || (i == 4 && j == 2)){
          matrix[i + place][j] = {r, g, b};
        }
        else{
          //rainbow(3);
        }
      }
    }
    break;

  case 6:
    for (int i = 0; i <= 4; i++){
      for (int j = 0; j <= 6; j++){
        if ((i == 2 && (j >= 1 && j <= 5)) || (i == 3 && j &2 != 0) || (i == 4 && j == 2 )){
          matrix[i + place][j] = {r, g, b};
        }
        else{
          //rainbow(3);
        }
      }
    }
    break;

  case 7:
    for (int i = 0; i <= 4; i++){
      for (int j = 0; j <= 6; j++){
        if ((i == 2 && (j == 1 || j == 5)) || (i == 3 && (j == 2 || j == 3 || j == 5)) || (i == 4 && j == 4 )){
          matrix[i + place][j] = {r, g, b};
        }
        else{
          //rainbow(3);
        }
      }
    }
    break;

  case 8:
    for (int i = 0; i <= 4; i++){
      for (int j = 1; j <= 5; j++){
        if (i != 3 && (j != 2 || j != 4)){
          matrix[i + place][j] = {r, g, b};
        }
        else{
          //rainbow(3);
        }
      }
    }
    break;

  case 9:
    for (int i = 0; i <= 4; i++){
      for (int j = 1; j <= 5; j++){
        if ((i != 2 && j != 2) || (i != 3 && (j != 2 || j != 4))){
          matrix[i + place][j] = {r, g, b};
        }
        else{
          //rainbow(3);
        }
      }
    }
    break;

    case -1:
    for (int j = 0; j < 7; j++){
      if(j == 2 || j == 4)
      matrix[place][j] = {r,g,b};
    }
      break;
  }
}

void setup()
{
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  ekb bulmeekbb(clock_div_1);
#endif
  // END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  Serial.begin(115200);
}

void loop()
{

  //pixels.clear(); // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  //rainbow(10);
  //rainbowCycle(5);
  Serial.println("Funktioniert");
  setDigit(0, 1, 0,150, 0);
  setDigit(1, 0, 0, 150, 0);
  setDigit(2, 0, 0, 150, 0);
  setDigit(3, 0, 0, 150, 0);
  setDigit(4, -1, 0,150, 0);

  displ(true);
  pixels.show();
  delay(1000);
 

  /*for(int i = 0; i < NUMPIXELS; i++){ // For each pixel...
  
      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright green color:
    
        
        pixels.setPixelColor(i, pixels.Color(MAXCOLOR-255, MAXCOLOR-175, MAXCOLOR-75));  
        pixels.show(); // Send the updated pixel colors to the hardware.
        delay(DELAYVAL); // Pause before next pass through loop 
    } 
    */
}