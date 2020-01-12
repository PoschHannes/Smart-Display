// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>

#include <Wire.h>
#include <ds3231.h>

#ifdef __AVR__

#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN D6  // On Trinket or Gemma, suggest changing this to 1

ts t; //ts is a struct findable in ds3231.h



// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 128  // Popular NeoPixel ring size
#define MAXCOLOR 255
#define MATRIX_DIMENSION_X 20
#define MATRIX_DIMENSION_Y 7
// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 0.1  // Time (in milliseconds) to pause between pixels
#define CLOCK_TIME 500 // Time (in milliseconds) to set clock frequenz
struct RGB {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

int hour_tenth, hour_ones;
int min_tenth, min_ones;

//void calculatetime()
//{
//  hour_tenth = t.hour/10;
//  hour_ones = t.hour%10;
//  min_tenth = t.min/10;
//  min_ones = t.min%10;
//}

RGB matrix[MATRIX_DIMENSION_X][MATRIX_DIMENSION_Y];  // x geht von 0 bis 19, y geht von 0 bis 6
// RGB digits[3][7];
// digits.r = {0,0,0,1,1,0,1,0,1,0,0,0,1,0,1,1,1,0,0,0,0};

// matrix[2][0];

// makes it possible to override unused leds
void set_Pixel_Color(bool copyBlack, uint16_t n, uint8_t r, uint8_t g,uint8_t b) {
    if (copyBlack) {
        pixels.setPixelColor(n, r, g, b);
    } else {
        if (r > 0 && b > 0 && g > 0) {
            pixels.setPixelColor(n, r, g, b);
        }
    }
}

void rowzero(bool copyBlack) {
    int startpix;
    startpix = 0;
    set_Pixel_Color(copyBlack, startpix, matrix[0][3].r, matrix[0][3].g, matrix[0][3].b);
}

void rowone(bool copyBlack) {
    int startpix;
    startpix = 1;
    for (int i = 0; i <= 4; i++) {
        set_Pixel_Color(copyBlack, startpix + i, matrix[1][5 - i].r, matrix[1][5 - i].g, matrix[1][5 - i].b);
    }
}

void lastrow(bool copyBlack) {
    int startpix;
    startpix = 125;
    for (int i = 0; i <= 2; i++) {
        set_Pixel_Color(copyBlack, startpix + i, matrix[19][4 - i].r, matrix[19][4 - i].g, matrix[19][4 - i].b);
    }
}

void copyrow(short rownmb, bool copyBlack) {
    int startpix;
    // 2 --> 6
    // 4 --> 20
    // 6 --> 34
    startpix = (rownmb - 2) * 7 + 6;
    for (int i = 0; i <= 6; i++) {
        set_Pixel_Color(copyBlack, startpix + i, matrix[rownmb][i].r, matrix[rownmb][i].g, matrix[rownmb][i].b);
    }
}

void copyinvrow(short rownmb, bool copyBlack) {
    int startpix;
    // 3 --> 13
    // 5 --> 27
    // 7 --> 41
    startpix = (rownmb - 2) * 7 + 6;

    for (int i = 0; i <= 6; i++) {
        set_Pixel_Color(copyBlack, startpix + i, matrix[rownmb][6-i].r, matrix[rownmb][6-i].g, matrix[rownmb][6-i].b);
    }
}

// mapping the matrix to the real layout
// matrix array prints the pixel on the right posiotion
void displ(bool copyBlack) {
    for (int i = 0; i <= 19; i++) {
        if (i % 2 == 0 && i <= 18 && i >= 2) {
            copyrow(i, copyBlack);
        } else if (i % 2 != 0 && i <= 17 && i >= 3) {
            copyinvrow(i, copyBlack);
        }

        else if (i == 0) {
            rowzero(copyBlack);
        }

        else if (i == 1) {
            rowone(copyBlack);
        }

        else if (i == 19) {
            lastrow(copyBlack);
        }
    }
}

void rainbowCycle(uint8_t wait) {
    uint16_t i, j;

    for (j = 0; j < 256 * 5; j++) {  // 5 cycles of all colors on wheel
        for (i = 0; i < pixels.numPixels(); i++) {
            pixels.setPixelColor(
                i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
        }
        pixels.show();
        delay(wait);
    }
}

void writeRainbowToMatrix(unsigned char iteration) {
  unsigned char wheelPos; // variable used to calculate the colors 
  unsigned char r;        // red color value
  unsigned char g;        // green color value
  unsigned char b;        // blue color value

  // iterate over all x and y values of the matrix
  for (int x = 0; x < MATRIX_DIMENSION_X; x++) {
    for (int y = 0; y < MATRIX_DIMENSION_Y; y++) {
      //in a previous example wheelPos was used to create a rainbow effect
      //this is the first try to adapt wheelPos because here the layout of the LEDs are differently aligned
      //wheelPos = (iteration + y * MATRIX_DIMENSION_Y + x) & 255;
      //alternative approach - let's try which one looks better
      unsigned char cycleCount=iteration;
      wheelPos = (cycleCount + y + x * MATRIX_DIMENSION_Y) & 255;
      
      if (wheelPos < 85) {
        r = wheelPos * 3;
        g = 255 - wheelPos * 3;
        b = 0;
      
      } else if (wheelPos < 170) {
        wheelPos -= 85;
        r = 255 - wheelPos * 3;
        g = 0;
        b = wheelPos * 3;
        
      } else {
        wheelPos -= 170;
        r = 0;
        g = wheelPos * 3;
        b = 255 - wheelPos * 3;
      } 
      //write the r, g, b values to the matrix according the current x and y value 
      matrix[x][y] = {r, g, b};  

  }
    
  }
  
}

/*
void rainbow(uint8_t wait) {
    uint16_t i, j;

    for (j = 0; j < 256; j++) {
        for (i = 0; i < pixels.numPixels(); i++) {
            pixels.setPixelColor(i, Wheel((i * 1 + j) & 255));
        }
    }
    pixels.show();
    delay(wait);
}
*/



// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
    if (WheelPos < 85) {
        return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    } else if (WheelPos < 170) {
        WheelPos -= 85;
        return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    } else {
        WheelPos -= 170;
        return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
}

// turn on the right lights for the number
// choose 1 of 5 positions, select one number, set the RGB Color(range 0 -255
// each) 0 - 4; 4 is only for ":" seperating hours and minutes
void setDigit(int offset, int select_nb, unsigned char r, unsigned char g, unsigned char b) {
    // calculate the right position
    int place;
    if (offset <= 1) {
        place = offset * 4;
    } else if (offset == 4) {
        place = 10;
    }

    else {
        place = offset * 4 + 2;
    }
    // switch beteen the numbers
    switch (select_nb) {
        case 0:
            for (int i = 2; i <= 4; i++) {
                for (int j = 0; j <= 6; j++) {
                    if ((i == 2 && (j >= 1 && j <= 5 && j != 3)) ||
                        (i == 3 && (j == 1 || j == 5)) ||
                        (i == 4 && (j == 2 || j == 4))) {
                        matrix[i + place][j] = {r, g, b};
                    }
                }
            }
            break;

        case 1:
            for (int i = 2; i <= 4; i++) {
                for (int j = 0; j <= 6; j++) {
                    if ((i == 3 && (j == 3 || j == 5 || j == 1)) ||
                        (i == 4 && (j == 2 || j == 4))) {
                        matrix[i + place][j] = {r, g, b};
                    }
                }
            }

            break;

        case 2:
            for (int i = 2; i <= 4; i++) {
                for (int j = 1; j < 6; j++) {
                    if ((i == 2 && (j >= 1 && j <= 5 && j != 4)) ||
                        (i == 3 && (j == 1 || j == 3 || j == 5)) ||
                        (i == 4 && j == 4)) {
                        matrix[i + place][j] = {r, g, b};
                    }
                }
            }
            break;

        case 3:
            for (int i = 2; i <= 4; i++) {
                for (int j = 0; j <= 6; j++) {
                    if ((i == 2 && (j % 2 != 0)) || (i == 3 && (j % 2 != 0)) ||
                        (i == 4 && (j == 4 || j == 2))) {
                        matrix[i + place][j] = {r, g, b};
                    }
                }
            }
            break;

        case 4:

            for (int i = 2; i <= 4; i++) {
                for (int j = 0; j <= 6; j++) {
                    if ((i == 2 && (j >= 3 && j <= 5)) ||
                        (i == 3 && (j == 3 || j == 1)) ||
                        (i == 4 && (j == 2 || j == 4))) {
                        matrix[i + place][j] = {r, g, b};
                    }
                }
            }
            break;

        case 5:
            for (int i = 2; i <= 4; i++) {
                for (int j = 0; j <= 6; j++) {
                    if ((i == 2 && (j % 2 != 0 || j == 4)) ||
                        (i == 3 && (j % 2 != 0)) || (i == 4 && j == 2)) {
                        matrix[i + place][j] = {r, g, b};
                    }
                }
            }
            break;

        case 6:
            for (int i = 2; i <= 4; i++) {
                for (int j = 0; j <= 6; j++) {
                    if ((i == 2 && (j >= 1 && j <= 5)) ||
                        (i == 3 && j & 2 != 0) || (i == 4 && j == 2)) {
                        matrix[i + place][j] = {r, g, b};
                    }
                }
            }
            break;

        case 7:
            for (int i = 2; i <= 4; i++) {
                for (int j = 0; j <= 6; j++) {
                    if ((i == 2 && (j == 1 || j == 5)) ||
                        (i == 3 && (j == 2 || j == 3 || j == 5)) ||
                        (i == 4 && j == 4)) {
                        matrix[i + place][j] = {r, g, b};
                    }
                }
            }
            break;

        case 8:
            for (int i = 2; i <= 4; i++) {
                for (int j = 1; j <= 5; j++) {
                    if ((i == 3 && (j == 2 || j == 4)) ||
                        (i == 4 && (j % 2 != 0))) {
                    } else {
                        matrix[i + place][j] = {r, g, b};
                    }
                }
            }
            break;

        case 9:
            for (int i = 2; i <= 4; i++) {
                for (int j = 1; j <= 5; j++) {
                    if ((i == 2 && j == 2) || (i == 3 && (j == 2 || j == 4)) ||
                        (i == 4 && (j % 2 != 0))) {
                    } else {
                        matrix[i + place][j] = {r, g, b};
                    }
                }
            }
            break;

        case -1:
            for (int j = 0; j < 7; j++) {
                if (j == 2 || j == 4) {
                    matrix[place][j] = {r, g, b};
                }
            }
            break;
    }
}

void fillMatrix(unsigned char r, unsigned char g, unsigned char b) {
    for (int x = 0; x < 20; x++) {
        for (int y = 0; y < 7; y++) {
            matrix[x][y] = {r, g, b};
        }
    }
}
void testMatrix() {
    for (int x = 0; x < 20; x++) {
        for (int y = 0; y < 7; y++) {
            matrix[x][y] = {0, 100, 100};
        }
        displ(true);
        pixels.show();
        delay(500);
    }
    delay(1000);

    for (int y = 0; y < 7; y++) {
        for (int x = 0; x < 20; x++) {
            matrix[x][y] = {100, 0, 0};
        }
        displ(true);
        pixels.show();
        delay(500);
    }
}



void Clock()
{
    unsigned char CR = 0;
    unsigned char CG = 255;
    unsigned char CB = 0;

    unsigned char AR = 0;
    unsigned char AG = 0;
    unsigned char AB = 255;



     for (int colorIterator = 0; colorIterator < 256; colorIterator++) {
    
    writeRainbowToMatrix(colorIterator);
    
    

    for (int i = 0; i < 3; i++) {
      if(i == 0){
        setDigit(0,2, CR,CG,CB);
      }
      else{
        setDigit(0,i-1, CR,CG,CB);
      }
        setDigit(0, i, AR, AG, AB);

        if(i < 2){
        for (int j = 0; j < 10; j++) {
            if (j == 0){
              setDigit(1, 9, CR, CG, CB);
            }
            else{
              setDigit(1, j-1, CR, CG, CB);
            }
            setDigit(1, j, AR, AG, AB);



            for (int k = 0; k < 6; k++) {
                if (k == 0) {
                    setDigit(2, 5, CR, CG, CB);
                } else {
                    setDigit(2, k - 1, CR, CG, CB);
                }
                setDigit(2, k, AR, AG, AB);

                for (int l = 0; l < 10; l++) {
                    if (l == 0) {
                        setDigit(3, 9, CR, CG, CB);
                    } else {
                        setDigit(3, l - 1, CR, CG, CB);
                    }

                    setDigit(3, l, AR, AG, AB);
                    if (l % 2 == 0) {
                        setDigit(4, -1, AR, AG, AB);
                    } else {
                        setDigit(4, -1, CR, CG, CB);
                    }

                    displ(true);
                    pixels.show();
                    delay(10);
                }
            }
        }
      }
      else{
          for (int j = 0; j < 4; j++) {
            if (j == 0){
              setDigit(1, 3, CR, CG, CB);
            }
            else{
              setDigit(1, j-1, CR, CG, CB);
            }
            setDigit(1, j, AR, AG, AB);


            for (int k = 0; k < 6; k++) {
                if (k == 0) {
                    setDigit(2, 5, CR, CG, CB);
                } else {
                    setDigit(2, k - 1, CR, CG, CB);
                }
                setDigit(2, k, AR, AG, AB);

                for (int l = 0; l < 10; l++) {
                    if (l == 0) {
                        setDigit(3, 9, CR, CG, CB);
                    } else {
                        setDigit(3, l - 1, CR, CG, CB);
                    }

                    setDigit(3, l, 255, 0, 0);
                    if (l % 2 == 0) {
                        setDigit(4, -1, AR, AG, AB);
                    } else {
                        setDigit(4, -1, CR, CG, CB);
                    }

                    displ(true);
                    pixels.show();
                    delay(10);
                }
            }
        }
      }
    }
  }
}
void RTC()
{
    unsigned char CR = 100;
    unsigned char CG = 100;
    unsigned char CB = 100;

  

 
    for (int colorIterator = 0; colorIterator < 256; colorIterator++) {
    int hour_tenth, hour_ones;
    int min_tenth, min_ones;

    DS3231_get(&t); //get the value and pass to the function the pointer to t, that make an lower memory fingerprint and faster execution than use return
  //DS3231_get() will use the pointer of t to directly change t value (faster, lower memory used)
    hour_tenth = t.hour/10;
    hour_ones = t.hour%10;
    min_tenth = t.min/10;
    min_ones = t.min%10;

    writeRainbowToMatrix(colorIterator);
    
    setDigit(0, hour_tenth, CR, CG, CB);

    setDigit(1, hour_ones, CR, CG, CB);

    setDigit(2, min_tenth, CR, CG, CB);

    setDigit(3, min_ones, CR, CG, CB);

    setDigit(4, -1, CR, CG, CB);
    displ(true);
    pixels.show();
    delay(10);
    
  }
    
}
void setup() {
    // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
    // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    ekb bulmeekbb(clock_div_1);
#endif
    // END of Trinket-specific code.
    Wire.begin(); //start i2c (required for connection)
    DS3231_init(DS3231_INTCN); //register the ds3231 (DS3231_INTCN is the default address of ds3231, this is set by macro for no performance loss)

    pixels.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
    Serial.begin(115200);
}



void loop() {
    // pixels.clear(); // Set all pixel colors to 'off'

    // The first NeoPixel in a strand is #0, second is 1, all the way up
    // to the count of pixels minus one.
    // rainbow(10);
    // rainbowCycle(5);
    Serial.println("Funktioniert");
    RTC();
//    for (int colorIterator = 0; colorIterator < 256; colorIterator++) {
//    
//    writeRainbowToMatrix(colorIterator);
//    
//    displ(true);
//    pixels.show();
//    delay(10);
//    
//  }



    /*for(int i = 0; i < NUMPIXELS; i++){ // For each pixel...

        // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
        // Here we're using a moderately bright green color:


          pixels.setPixelColor(i, pixels.Color(MAXCOLOR-255, MAXCOLOR-175,
    MAXCOLOR-75)); pixels.show(); // Send the updated pixel colors to the
    hardware. delay(DELAYVAL); // Pause before next pass through loop
      }
      */
}
