#include <Arduino.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <U8x8lib.h>
#include <U8g2lib.h>

void initialiseSerial()
{
  Serial.begin(9600);
}

int ENTRANCE_BUTTON_PIN = 2;
int EXIT_BUTTON_PIN = 3;
int CANCEL_BUTTON_PIN = 4;
int LED_PIN = 13;
int SIM_TX_PIN = 9;
int SIM_RX_PIN = 8;

void initialisePins()
{

  pinMode(ENTRANCE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(EXIT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(CANCEL_BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
}

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);

void initialiseDisplay()
{
  u8g2.begin();
}

void sendMessageToLCD(String message, int duration)
{

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_logisoso16_tf); // https://github.com/olikraus/u8g2/wiki/fntlistall
  const char *msg = message.c_str();     // convert String to char*
  u8g2.drawStr(0, 28, msg);
  u8g2.sendBuffer();

  delay(duration);
}

SoftwareSerial SimSerial(SIM_TX_PIN, SIM_RX_PIN);

String sendCommandToSim(String command, int wordIndex)
{
  SimSerial.println(command);

  delay(1000); // delay to allow sim to send response

  // separate the response into words
  String words[5];
  int wordCount = 0;
  String newWord = "";

  while (SimSerial.available())
  {
    byte byte = SimSerial.read();

    const char character = (char)byte; // convert the byte to a char

    if (!isSpace(character))
    {
      newWord += character;
    }
    else
    {
      // if its a space and we have a word, save it and reset it
      if (newWord.length() > 0)
      {
        words[wordCount] = newWord;
        newWord = "";
        wordCount += 1;
      }
    }
  }

  return words[wordIndex];
}

void initialiseSim()
{
  Serial.println("Initialising...");

  SimSerial.begin(9600);

  delay(1000); // give it some time to initialise

  String command = "AT";
  String response = sendCommandToSim(command, 1);
  String message = command + ": " + response;
  sendMessageToLCD(message, 1000);

  command = "AT+CSQ";
  response = sendCommandToSim(command, 2);
  message = command + ": " + response;
  sendMessageToLCD(message, 1000);

  command = "AT+CCID";
  response = sendCommandToSim(command, 2);
  message = command + ": " + response;
  sendMessageToLCD(message, 1000);

  command = "AT+CREG?";
  response = sendCommandToSim(command, 2);
  message = command + ": " + response;
  sendMessageToLCD(message, 1000);

  command = "AT+COPS?";
  response = sendCommandToSim(command, 2);
  message = command + ": " + response;
  sendMessageToLCD(message, 1000);

  command = "AT+CMEE?";
  response = sendCommandToSim(command, 2);
  message = command + ": " + response;
  sendMessageToLCD(message, 1000);
}

void setup(void)
{
  initialiseSerial();

  initialisePins();

  initialiseDisplay();

  initialiseSim();
}

// boolean isCallingEntrance = false;
// boolean isCallingExit = false;

void loop(void)
{
  // int entranceButtonVal = digitalRead(ENTRANCE_BUTTON_PIN);
  // int exitButtonVal = digitalRead(EXIT_BUTTON_PIN);
  // int cancelButtonVal = digitalRead(CANCEL_BUTTON_PIN);

  // boolean entranceButtonIsPressed = entranceButtonVal == LOW;
  // boolean exitButtonIsPressed = exitButtonVal == LOW;
  // boolean cancelButtonIsPressed = cancelButtonVal == LOW;

  // // toggle led
  // boolean aButtonIsPressed = entranceButtonIsPressed || exitButtonIsPressed || cancelButtonIsPressed;
  // if (aButtonIsPressed)
  // {
  //   digitalWrite(LED_PIN, HIGH);
  // }
  // else
  // {
  //   digitalWrite(LED_PIN, LOW);
  // }

  // // call state
  // if (entranceButtonIsPressed)
  // {
  //   isCallingEntrance = true;
  //   isCallingExit = false; // cancel exit call
  // }
  // if (exitButtonIsPressed)
  // {
  //   isCallingExit = true;
  //   isCallingEntrance = false; // cancel entrance call
  // }
  // if (cancelButtonIsPressed)
  // {
  //   isCallingEntrance = false;
  //   isCallingExit = false;
  // }

  // // lcd screen

  // if (isCallingEntrance)
  // {
  //   sendMessageToLCD("CALL ENTRANCE");
  // }
  // if (isCallingExit)
  // {
  //   sendMessageToLCD("CALL EXIT");
  // }

  // if (entranceButtonIsPressed)
  // {
  //   // +27723528417
  //   SimSerial.println("ATD+ +27787045424;");
  // }

  // if (exitButtonIsPressed)
  // {
  //   // +27638345505
  //   SimSerial.println("ATD+ +27833771130;");
  // }
  // if (cancelButtonIsPressed)
  // {
  //   SimSerial.println("ATH");
  // }

  // sendCommandToSim();
}
