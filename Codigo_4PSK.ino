#include "si5351.h"
#include "Wire.h"

Si5351 si5351;

#define START_BUTTON 22
#define BASE_FREQUENCY 7038600UL
#define TONE_SEPARATION 14648

unsigned long timeReference;
unsigned int  h, m, s; //used inside printTime()

unsigned long randomShift;

const unsigned char message[] = "312200023022113022300103133200222012032300002032330231012001123002211230101210232032330003301010221222021003221312310213232203312222210302112200222132303120031000";

void printTime(unsigned long currentTime)
{
  currentTime -= timeReference;
  s = currentTime / 1000;
  m = s / 60;
  h = m / 60;

  s %= 60;
  m %= 60;

  Serial.print("{");
  if(h < 10)
  {
    Serial.print(0);
    Serial.print(h);
  }
  else Serial.print(h);
  Serial.print(":");
  if(m < 10)
  {
    Serial.print(0);
    Serial.print(m);
  }
  else Serial.print(m);
  Serial.print(":");
  if(s < 10)
  {
    Serial.print(0);
    Serial.print(s);
  }
  else Serial.print(s);
  Serial.print("}");
}

void setup()
{
  Serial.begin(115200);
  delay(500);

  si5351.output_enable(SI5351_CLK2, 0); //turn OFF clk2 since it's not used

  pinMode(START_BUTTON, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.println("Press and let go of the start button to begin.");

  while(digitalRead(START_BUTTON)==HIGH) //waits for button press
  {

    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  }
  digitalWrite(LED_BUILTIN, HIGH);
  while(!digitalRead(START_BUTTON)); //waits until the button is let go
  timeReference = millis();
  digitalWrite(LED_BUILTIN, LOW);

  printTime(millis());
  Serial.println(" {setup()} {Setting up the transmission}");

  while(!si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0)) //detects whether the SI5351 is connected properly
  {
    printTime(millis());
    Serial.println(" {setup()} {SI5351 not found, please check the connection}");
    Serial.println("Once the SI5351 is properly connected, press the start button again to continue.");
    while(digitalRead(START_BUTTON));
  }

  printTime(millis());
  Serial.println(" {setup()} {SI5351 found}");
}

void loop()
{
  printTime(millis());
  Serial.println(" {loop()} {Starting transmission}");

  si5351.output_enable(SI5351_CLK0, 1); //reference signal
  si5351.output_enable(SI5351_CLK1, 1); //modulated signal

  randomShift = random(0, 2500);

  printTime(millis());
  Serial.print(" {loop()} {This message will have a randomly selected offset of ");
  Serial.print(randomShift);
  Serial.println("Hz}");

  digitalWrite(LED_BUILTIN, HIGH); //ON while transmitting

  si5351.set_freq((BASE_FREQUENCY + randomShift) * 100ULL, SI5351_CLK0); //multiplying by 100ULL because the function expects a multiple of 0.01Hz

  for(unsigned char i = 0; i < 162; i++)
  {
    printTime(millis());
    Serial.print(" {loop()} {Transmitting symbol ");
    Serial.print(i + 1);
    Serial.println("}");

    si5351.set_freq(((message[i] - 48) * TONE_SEPARATION + BASE_FREQUENCY + randomShift) * 100ULL, SI5351_CLK1);
    delay(683);
  }

  si5351.output_enable(SI5351_CLK0, 0);
  si5351.output_enable(SI5351_CLK1, 0);
  digitalWrite(LED_BUILTIN, LOW);

  printTime(millis());
  Serial.println(" {loop()} {Transmission finished, will wait about 7 minutes before resend}");

  delay(442368);
}
