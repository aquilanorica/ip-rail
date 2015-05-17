/*
    inspired by
    http://allaboutee.com/2015/01/02/esp8266-arduino-led-control-from-webpage/

    rewritten to switch 8 Relay Module, each relay separately on/off 
    using Arduino Nano Pin PD4-PD11
    connected to esp8266 and /8_relay_module.html in your webbrowser 
    make sure, you do not connect relays to your arduino directly
    

*/


    #include <SoftwareSerial.h>
     
    #define DEBUG true
     
    SoftwareSerial esp8266(2,3); // make RX Arduino line is pin 2, make TX Arduino line is pin 3.
                                  // This means that you need to connect the TX line from the esp to the Arduino's pin 2
                                  // and the RX line from the esp to the Arduino's pin 3
    void setup()
    {
      Serial.begin(9600);
      esp8266.begin(9600); // your esp's baud rate might be different
      pinMode(4,OUTPUT);
      digitalWrite(4,LOW);
      pinMode(5,OUTPUT);
      digitalWrite(5,LOW);
      pinMode(6,OUTPUT);
      digitalWrite(6,LOW);
      pinMode(7,OUTPUT);
      digitalWrite(7,LOW);
      pinMode(8,OUTPUT);
      digitalWrite(8,LOW);
      pinMode(9,OUTPUT);
      digitalWrite(9,LOW);
      pinMode(10,OUTPUT);
      digitalWrite(10,LOW);
      pinMode(11,OUTPUT);
      digitalWrite(11,LOW);

      sendData("AT+RST\r\n",2000,DEBUG); // reset module
      sendData("AT+CWMODE=2\r\n",1000,DEBUG); // configure as access point
      sendData("AT+CIFSR\r\n",1000,DEBUG); // get ip address
      sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
      sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // turn on server on port 80
    }
     
    void loop()
      {
        if(esp8266.available()) // check if the esp is sending a message
        {
            if(esp8266.find("+IPD,"))
            {
            delay(1000); // wait for the serial buffer to fill up (read all the serial data)
                         // get the connection id so that we can then disconnect
            int connectionId = esp8266.read()-48; // subtract 48 because the read() function returns
                        // the ASCII decimal value and 0 (the first decimal number) starts at 48
            esp8266.find("pin="); // advance cursor to "pin="
            char getcmd = (esp8266.read()); 

              // Use getcmd value (here: Characters) to select semaphore LEDs "picture" (Hauptsignal 01-04, VorSignal 05-08):
              // NOTE because the serial.read() reads the ASCII value for the character and not "int",
              // the read value must be compared to character numbers.
              // For example 'a' = 97, 'b' = 98, or for two digits like '01' = 4849, '02' = 4850 or whatever, for ascii see http://www.asciitable.com/
              // hence the single quotes around the numbers in the case statement 
              // to tell the controller to get the ASCII value for the character.

              
               switch (getcmd) {
                  case '01':                      // Relay 1
                    digitalWrite(4, HIGH);
                    break;
                  case '02':                      // Relay 2
                    digitalWrite(5, HIGH);
                    break;
                  case '03':                      // Relay 3
                    digitalWrite(6, HIGH);
                    break;
                  case '04':                      // Relay 4
                    digitalWrite(7, HIGH);
                    break;       
                  case '05':                      // Relay 5
                    digitalWrite(8, HIGH);
                    break;
                  case '06':                      // Relay 6
                    digitalWrite(9, HIGH);
                    break;
                  case '07':                      // Relay 7
                    digitalWrite(10, HIGH);
                    break;
                  case '08':                      // Relay 8
                    digitalWrite(11, HIGH);
                    break;
                  default:
                    // turn all the Relays off:
                    digitalWrite(4, LOW);
                    digitalWrite(5, LOW);
                    digitalWrite(6, LOW);
                    digitalWrite(7, LOW);
                    digitalWrite(8, LOW);
                    digitalWrite(9, LOW);
                    digitalWrite(10, LOW);
                    digitalWrite(11, LOW);
                 
              }
                        // make close command
            String closeCommand = "AT+CIPCLOSE=";
            closeCommand+=connectionId; // append connection id
            closeCommand+="\r\n";
            sendData(closeCommand,1000,DEBUG); // close connection
            }
         }
      }
     
    /*
    * Name: sendData
    * Description: Function used to send data to ESP8266.
    * Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
    * Returns: The response from the esp8266 (if there is a reponse)
    */
    String sendData(String command, const int timeout, boolean debug)
      {
      String response = "";
      esp8266.print(command); // send the read character to the esp8266
      long int time = millis();
      while( (time+timeout) > millis())
        {
        while(esp8266.available())
          {
          // The esp has data so display its output to the serial window
          char c = esp8266.read(); // read the next character.
          response+=c;
          }
        }
      if(debug)
        {
        Serial.print(response);
        }
      return response;
      }

