# 1 "C:\\Users\\anton.angervall\\Documents\\Arduino\\projekt1\\projekt1.ino"
# 2 "C:\\Users\\anton.angervall\\Documents\\Arduino\\projekt1\\projekt1.ino" 2
# 3 "C:\\Users\\anton.angervall\\Documents\\Arduino\\projekt1\\projekt1.ino" 2

// Define color sensor pins






// Calibration Values
// *Get these from Calibration Sketch
int redMin = 108; // Red minimum value
int redMax = 220; // Red maximum value
int greenMin = 95; // Green minimum value
int greenMax = 270; // Green maximum value
int blueMin = 90; // Blue minimum value
int blueMax = 255; // Blue maximum value
// Variables for Color Pulse Width Measurements

// Variables for Color Pulse Width Measurements
int redPW = 0;
int greenPW = 0;
int bluePW = 0;

// Variables for final Color values
int redValue;
int greenValue;
int blueValue;

//counts the number of times a color is detected
int Greenounter = 0;
int Redcounter = 0;
int Bluecounter = 0;

U8GLIB_SSD1306_128X64 oled(2);

void setup() {
 // Set S0 - S3 as outputs
 pinMode(4, 0x1);
 pinMode(5, 0x1);
 pinMode(6, 0x1);
 pinMode(7, 0x1);
 pinMode(13, 0x0);
 pinMode(10, 0x1);
 pinMode(11, 0x1);
 pinMode(12, 0x1);

 // Set Sensor output as input
 pinMode(8, 0x0);

 // Set Frequency scaling to 20%
 digitalWrite(4,0x1);
 digitalWrite(5,0x0);

 // Setup Serial Monitor
 Serial.begin(9600);
   Wire.begin();
    oled.setFont(u8g_font_helvB10);

}

void loop() {
 //starts the loop over if the button is pressed
 if (digitalRead(13) == 0x0)
 {return;}

 // turns off the leds
 digitalWrite(10, 0x0);
 digitalWrite(11, 0x0);
 digitalWrite(12, 0x0);

 // Read Red value
 redPW = getRedPW();
 // Map to value from 0-255
 redValue = map(redPW, redMin,redMax,255,0);
 // Delay to stabilize sensor
 delay(200);

 // Read Green value
 greenPW = getGreenPW();
 // Map to value from 0-255
 greenValue = map(greenPW, greenMin,greenMax,255,0);
 // Delay to stabilize sensor
 delay(200);

 // Read Blue value
 bluePW = getBluePW();
 // Map to value from 0-255
 blueValue = map(bluePW, blueMin,blueMax,255,0);
 // Delay to stabilize sensor
 delay(200);

 // Print output to Serial Monitor
 Serial.print(" - Red = ");
 Serial.print(redValue);
 Serial.print(" - Green = ");
 Serial.print(greenValue);
 Serial.print(" - Blue = ");
 Serial.println(blueValue);

//if the green value is the highest
  if (greenValue >= blueValue && greenValue >= redValue){
 Greenounter++;
 //show the color and the chans of it being detected
    updateOled("GREEN", "1/" + String(power(3,Greenounter)));
 digitalWrite(11, 0x1); //turns on the green led
 Redcounter = 0; //resets the red counter
 Bluecounter = 0; //resets the blue counter
  }
  //if the red value is the highest
  if (redValue >= blueValue && redValue >= greenValue){
 Redcounter++;
 //show the color and the chans of it being detected
    updateOled("RED", "1/" + String(power(3, Redcounter)));
 digitalWrite(10, 0x1); //turns on the red led
 Greenounter = 0; //resets the green counter
 Bluecounter = 0; //resets the blue counter
  }
  //if the blue value is the highest
  if (blueValue >= redValue && blueValue >= greenValue){
 Bluecounter++;
 //show the color and the chans of it being detected
    updateOled("BLUE", "1/" + String(power(3,Bluecounter)));
 digitalWrite(12, 0x1); //turns on the blue led
 Greenounter = 0; //resets the green counter
 Redcounter = 0; //resets the red counter
  }
}


// Function to read Red Pulse Widths
int getRedPW() {
 // Set sensor to read Red only
 digitalWrite(6,0x0);
 digitalWrite(7,0x0);
 // Define integer to represent Pulse Width
 int PW;
 // Read the output Pulse Width
 PW = pulseIn(8, 0x0);
 // Return the value
 return PW;
}

// Function to read Green Pulse Widths
int getGreenPW() {
 // Set sensor to read Green only
 digitalWrite(6,0x1);
 digitalWrite(7,0x1);
 // Define integer to represent Pulse Width
 int PW;
 // Read the output Pulse Width
 PW = pulseIn(8, 0x0);
 // Return the value
 return PW;
}

// Function to read Blue Pulse Widths
int getBluePW() {
 // Set sensor to read Blue only
 digitalWrite(6,0x0);
 digitalWrite(7,0x1);
 // Define integer to represent Pulse Width
 int PW;
 // Read the output Pulse Width
 PW = pulseIn(8, 0x0);
 // Return the value
 return PW;
}
//function to update the oled
void updateOled(String text,String text2){
    oled.firstPage();
    do{
        oled.drawStr(30, 30, text.c_str()); //where the oled string shows
  oled.drawStr(50, 50, text2.c_str()); //where the oled string shows

    } while (oled.nextPage());
}


int power(int base, int exponent) {
    int result = 1;
    for (int i = 0; i < exponent; ++i) {
        result *= base;
    }
    return result;
}
