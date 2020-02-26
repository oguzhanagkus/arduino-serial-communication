/*

  This is Arduino client code.
  Upload your board and then keep it connected with USB cable.
  The C code is communicating over the cable.

*/

int led = 13, flag = 0, data;
unsigned int result;
char temp, reply[6];

void setup()
{
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  Serial.begin(9600);
}

void loop()
{
  while(Serial.available())
  {
    if(flag == 0) // Read opcode
    {
      data = Serial.read();
      temp = char(data);

      switch(temp)
      {
        case '1': digitalWrite(led, HIGH);
                  break;

        case '2': digitalWrite(led, LOW);
                  break;

        case '3': digitalWrite(13, LOW); delay(100); // If light is already up

                  for(int i = 0; i < 3; i++)
                  {
                    digitalWrite(led, HIGH); delay(1000);
                    digitalWrite(led, LOW); delay(1000);
                  }

                  break;

        case '4': flag = 1;
                  break;
      }
    }

    else // Read value for opcode 4
    {
      data = Serial.parseInt();
      result = data * data;

      String s = String(result);
      s.toCharArray(reply, 6);
      delay(100);

      Serial.write(reply);
      Serial.write("*");

      flag = 0;
    }
  }
}
