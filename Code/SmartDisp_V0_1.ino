// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define  PIN        D6 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 128 // Popular NeoPixel ring size
#define MAXCOLOR 255
// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 0.1 // Time (in milliseconds) to pause between pixels

struct RGB{
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

RGB matrix[20][7]; // x geht von 0 bis 19, y geht von 0 bis 6
//RGB digits[3][7];
//digits.r = {0,0,0,1,1,0,1,0,1,0,0,0,1,0,1,1,1,0,0,0,0};


//matrix[2][0];


void rowzero()
{
  int startpix;
  startpix = 0;
  pixels.setPixelColor(startpix, pixels.Color(matrix[0][3].r,matrix[0][3].g,matrix[0][3].b));
}

void rowone()
{
  int startpix;
  startpix = 1;
  for(int i = 0; i <=4; i++){
      pixels.setPixelColor(startpix+i, pixels.Color(matrix[0][i].r,matrix[0][i].g,matrix[0][i].b));
  }
}

void lastrow()
{
    int startpix;
  startpix = 125;
  for(int i = 0; i <=2; i++){
      pixels.setPixelColor(startpix+i, pixels.Color(matrix[0][i].r,matrix[0][i].g,matrix[0][i].b));
  }
}

void copyrow(short rownmb)
{
  int startpix;
  //2 --> 6
  //4 --> 20
  //6 --> 34
  startpix = (rownmb -2)*7+6;
  for(int i = 0; i <= 6; i++){
     pixels.setPixelColor(startpix + i, pixels.Color(matrix[rownmb][i].r,matrix[rownmb][i].g,matrix[rownmb][i].b));
  }

   
}

void copyinvrow(short rownmb)
{
  int startpix;
  //3 --> 13
  //5 --> 27
  //7 --> 41
  startpix = (rownmb -2)*7+6;
  

  for(int i = 0; i <= 6; i++){
     pixels.setPixelColor(startpix + i, pixels.Color(matrix[rownmb][6-i].r,matrix[rownmb][6-i].g,matrix[rownmb][6-i].b ));
  }

   
}


void displ()
{
  for(int i = 0; i<=19; i++){
    if(i%2 == 0 && i <=18 && i >= 2){
      copyrow(i);
    }
     else if(i%2!=0 && i <=17 && i >= 3){
      copyinvrow(i);
    }

    else if(i == 0){
      rowzero();
    }
    
    else if(i == 1){
      rowone();
    }
    
    else if(i == 19){
      lastrow();
    }
  }
}

void rel(int place, int select, int r, int g, int b)
{
  int offset = 2;

  if (place <=1)
  {
    place = place+3;
  }
  else
  {
      place = place*3+offset;
  }
  
  rainbow(3);

  switch(select){
  case 0:
     for(int i = 2; i<4; i++){
      for(int j = 0; j<=6; j++){
        if((i==2 && (j >=1 && j <=5 && j !=3)) || (i==3 &&(j ==1 || j == 5)) || (i==4 &&(j == 2 && j == 4)))  
        matrix[i+place][j] = {r,g,b};
      }
     }
    break;

  case 1:
    for(int i = 0; i<3; i++){
      for(int j = 0; j<=6; j++){
        if((i == 2 && (j>= 3 && j <=5 )) || (i == 3 && (j==3 || j == 5)) || (i ==4 && (j ==2 || j == 4)) )  
        matrix[i+place][j] = {r,g,b};
      }
    }

    break;

  case 2:
    break;

  case 3:
    break;

  case 4:

    for(int i = 0; i<20; i++){
      for(int j = 0; j<=6; j++){
        if((i == 2 && (j>= 3 && j <=5 )) || (i == 3 && (j==3 || j == 5)) || (i ==4 && (j ==2 || j == 4)) )  
        matrix[i+place][j] = {r,g,b};
      }
    }
    break;

  case 5:
    break;

  case 6:
    break;

  case 7: 
    break;

  case 8:
   /* for(int i = 0; i<20; i++){
      for(int j = 0; j<=6; j++){
        if((i >=1 && i <= 5) ||(i== 1|| i == 3 || i == 5) ||() )  
        matrix[i+place][j] = {170,0,0};
      }
     }*/
    break;

  case 9:
    break;

  }
  
}
/*void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< pixels.numPixels(); i++) {
    
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}
*/



void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<pixels.numPixels(); i++) {
         pixels.setPixelColor(i, Wheel((i*1+j) & 255));
      
     
    }
    pixels.show();
    delay(wait);
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


void white(uint8_t wait) {
  uint16_t i;

  for(i=0; i<NUMPIXELS; i++) {
   
        pixels.setPixelColor(i, pixels.Color(255,255,255));
  }
   pixels.show();
   delay(wait);
   
  }

void setnmbr()
{
}











void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  ekb bulmeekbb(clock_div_1);
#endif
  // END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}
   
void loop() {


  //pixels.clear(); // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.

  for (int i = 0; i < 20; i++)
  {
    for (int j = 2; j < 4; j++)
    {
      
        matrix[i][j] = {150,0,0};
    }
  }
  
  

   
  displ();
  pixels.show();
    /*for(int i = 0; i < NUMPIXELS; i++){ // For each pixel...
  
      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright green color:
    
        
        pixels.setPixelColor(i, pixels.Color(MAXCOLOR-255, MAXCOLOR-175, MAXCOLOR-75));  
        pixels.show(); // Send the updated pixel colors to the hardware.
        delay(DELAYVAL); // Pause before next pass through loop 
    } 
    */
}
