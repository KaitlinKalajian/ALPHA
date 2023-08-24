#include <Servo.h>
#include <string.h>
//data in string $abc,1;/r/n
Servo batt_servo;  //create servo object that coltrols a servo

int pwminput = 2050;
const unsigned int USB_in = 32;  //amount of bytes

void setup() {
  Serial.begin(1500);                  //establish serial communication with baud rate (bits per sec) 1500
  batt_servo.attach(9);                //attaches servo on pin 9 to servo object (500, 2500 max PWM range)
  batt_servo.writeMicroseconds(2050);  //set servo to top position
}
void loop() {
  static char message[USB_in];        //making character aray to store incoming bytes
  static unsigned int input_pos = 0;  //sets message position to start at 0
  static bool recording = false;      //controls recording state
  while (Serial.available() > 0)      //analyzing how many bytes available to be read
  {
    char incomingByte = Serial.read();  //read next available byte
    //Incoming message (check for terminating character)
    if (incomingByte == '$') {
      recording = true;
      input_pos = 0;  //reset buffer index to 0
    }

    if (recording) {
      if (incomingByte != '\n' && input_pos < USB_in - 1)  //if its not new line & exceeding array bounds
      {
          message[input_pos] = incomingByte;  //Add incoming byte to message
          input_pos++;                        //incriment message position
        }
      else {
        recording = false;          //stop recording
        message[input_pos] = '\0';  //add null character to string
        Serial.print("Message recieved:");
        Serial.println(message);
        float data;
        int num = sscanf(message, "$abc,%f;", &data);
        Serial.println(num);
        Serial.print("Extracted data:");
        Serial.println(data);
        delay(1000);
        //Converting data to PWM value
        int pwmValue = map(data, 0, 1, 2050, 1500);  // Map the input to servo range
        Serial.print("PWM value:");
        Serial.println(pwmValue);
        batt_servo.writeMicroseconds(pwmValue);
        //reset for next message
        input_pos = 0;
      }
    }
  }
}