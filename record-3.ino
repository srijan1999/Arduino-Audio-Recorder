/* PIN LAYOUT FOR NANO

MicroSD Module    MAX9814           Pushbutton
CS      D10       GND     GND      PIN1   5V
SCK     D13       Vdd     3V3      PIN2   D2
MOSI    D11       Gain     -       PIN2   Connect a 10K resistor, on the
MISO    D12       Out     A0              other end of resistor, connect 
VCC     5V        AR       -              to GND.
GND     GND
*/


#include <TMRpcm.h>
#include <SD.h>
#include <SPI.h>
TMRpcm audio;

int LEDState = 0;
const int LEDPin = 4;     // Connect the LED to Digital Pin 5
const int buttonPin = 2;  // Connect the Button to Digital pin 2
int buttonNew;
int buttonOld = 1;  // the current state of button

int file_number = 0;
char filePrefixname[50] = "REC00";
char exten[10] = ".wav";

const int mic_pin = A0;   // Connect the 
const int sample_rate = 16000;
#define SD_CSPin 10

// delay function for with serial log.
void wait_min(int mins) {
  int count = 0;
  int secs = mins * 60;
  while (1) {
    Serial.print('.');
    delay(1000);
    count++;
    if (count == secs) {
      count = 0;
      break;
    }
  }
  Serial.println();
  return;
}

void setup() {
  // Start serial communication
  Serial.begin(9600);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  pinMode(LEDPin, OUTPUT);
  pinMode(mic_pin, INPUT);
  Serial.println("loading... SD card");
  if (!SD.begin(SD_CSPin)) {
    Serial.println("An Error has occurred while mounting SD");
  }
  while (!SD.begin(SD_CSPin)) {
    Serial.print(".");
    delay(500);
  }
  audio.CSPin = SD_CSPin;
}

void loop() {
  // read the state of the pushbutton
  buttonNew = digitalRead(buttonPin);
  // Code to toggle the LED
  // Press Once to turn ON, press again to OFF
  if (buttonOld == 0 && buttonNew == 1) {
    char fileSlNum[20] = "";
    itoa(file_number, fileSlNum, 10);
    char file_name[50] = "";
    strcat(file_name, filePrefixname);
    strcat(file_name, fileSlNum);
    strcat(file_name, exten);
    Serial.print("New File Name: ");
    Serial.println(file_name);
    delay(200);
    if (LEDState == 0) {
      digitalWrite(LEDPin, HIGH);
      LEDState = 1;
      Serial.println("Toggle ON");
      audio.startRecording(file_name, sample_rate, mic_pin);
      Serial.println("Recording...");
      
    } else {
      digitalWrite(LEDPin, LOW);
      LEDState = 0;
      Serial.println("Toggle OFF");
      audio.stopRecording(file_name);
      Serial.println("Recording Stopped!");
      file_number++;
      delay(500);
    }
  }
  buttonOld = buttonNew;
  delay(100);
}
