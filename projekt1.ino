#include <Wire.h>
#include <U8glib.h>

// Define color sensor pins
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

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

U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NO_ACK);

void setup() {
	// Set S0 - S3 as outputs
	pinMode(S0, OUTPUT);
	pinMode(S1, OUTPUT);
	pinMode(S2, OUTPUT);
	pinMode(S3, OUTPUT);
	pinMode(13, INPUT);
	pinMode(10, OUTPUT);
	pinMode(11, OUTPUT);
	pinMode(12, OUTPUT);

	// Set Sensor output as input
	pinMode(sensorOut, INPUT);

	// Set Frequency scaling to 20%
	digitalWrite(S0,HIGH);
	digitalWrite(S1,LOW);

	// Setup Serial Monitor
	Serial.begin(9600);
   Wire.begin();
    oled.setFont(u8g_font_helvB10);
  
}

void loop() {
	//starts the loop over if the button is pressed
	if (digitalRead(13) == LOW)
	{return;}
	
	// turns off the leds
	digitalWrite(10, LOW);
	digitalWrite(11, LOW);
	digitalWrite(12, LOW);
	
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
	digitalWrite(11, HIGH); //turns on the green led
	Redcounter = 0; //resets the red counter
	Bluecounter = 0; //resets the blue counter
  }
  //if the red value is the highest
  if (redValue >= blueValue && redValue >= greenValue){
	Redcounter++;
	//show the color and the chans of it being detected
    updateOled("RED", "1/" + String(power(3, Redcounter)));
	digitalWrite(10, HIGH); //turns on the red led
	Greenounter = 0; //resets the green counter
	Bluecounter = 0; //resets the blue counter
  }
  //if the blue value is the highest
  if (blueValue >= redValue && blueValue >= greenValue){
	Bluecounter++;
	//show the color and the chans of it being detected
    updateOled("BLUE", "1/" + String(power(3,Bluecounter)));
	digitalWrite(12, HIGH); //turns on the blue led
	Greenounter = 0; //resets the green counter
	Redcounter = 0; //resets the red counter
  }
}


// Function to read Red Pulse Widths
int getRedPW() {
	// Set sensor to read Red only
	digitalWrite(S2,LOW);
	digitalWrite(S3,LOW);
	// Define integer to represent Pulse Width
	int PW;
	// Read the output Pulse Width
	PW = pulseIn(sensorOut, LOW);
	// Return the value
	return PW;
}

// Function to read Green Pulse Widths
int getGreenPW() {
	// Set sensor to read Green only
	digitalWrite(S2,HIGH);
	digitalWrite(S3,HIGH);
	// Define integer to represent Pulse Width
	int PW;
	// Read the output Pulse Width
	PW = pulseIn(sensorOut, LOW);
	// Return the value
	return PW;
}

// Function to read Blue Pulse Widths
int getBluePW() {
	// Set sensor to read Blue only
	digitalWrite(S2,LOW);
	digitalWrite(S3,HIGH);
	// Define integer to represent Pulse Width
	int PW;
	// Read the output Pulse Width
	PW = pulseIn(sensorOut, LOW);
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






