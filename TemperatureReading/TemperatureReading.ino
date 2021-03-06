#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int sensorPin = A0;
const float baselineTemp = 20.0;

/*Variables for Time Graphing*/
unsigned long previousTime = 0;
long interval = 1000; //5 Seconds


/*Variables for Averaging Temp in a Given Time */
unsigned long currentSum = 0;
unsigned long numTemps = 0;


/* QUEUE IMPLEMENTATION IN C TAKEN FROM 
 *  https://www.tutorialspoint.com/data_structures_algorithms/queue_program_in_c.htm
 */
#include <stdlib.h>
#include <stdbool.h>

#define MAX 16

float intArray[MAX];
int front = 0;
int rear = -1;
int itemCount = 0;

int peek() {
   return intArray[front];
}

bool isEmpty() {
   return itemCount == 0;
}

bool isFull() {
   return itemCount == MAX;
}

int size() {
   return itemCount;
}  

void insert(int data) {

   if(!isFull()) {
 
      if(rear == MAX-1) {
         rear = -1;            
      }       

      intArray[++rear] = data;
      itemCount++;
   }
}

int removeData() {
   int data = intArray[front++];
  
   if(front == MAX) {
      front = 0;
   }
  
   itemCount--;
   return data;  
}



/* GRAPH ICONS - Each will be set up as custom chars for the graphing*/

byte one_line[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
};


byte two_line[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
};

byte three_line[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
};

byte four_line[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
};


byte five_line[8] = {
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte six_line[8] = {
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};



byte seven_line[8] = {
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte eight_line[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};






void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  /* Each of the Lines of the Graph is Set up as a custom char 
     See https://www.arduino.cc/en/Reference/LiquidCrystalCreateChar
  */
  
  lcd.createChar(0, one_line);
  lcd.createChar(1, two_line);
  lcd.createChar(2, three_line);
  lcd.createChar(3, four_line);
  lcd.createChar(4, five_line);
  lcd.createChar(5, six_line);
  lcd.createChar(6, seven_line);
  lcd.createChar(7, eight_line);

  
  lcd.begin(16, 2);
  
  
}

void graph(){
  lcd.setCursor(0, 1);

  for (int i = 0; i < MAX; i++) {
    float graphReading = intArray[i];
    if (graphReading == 0) lcd.print(" ");
    if (graphReading >= 0 && graphReading < 100) lcd.write(byte(0));
    if (graphReading >= 000 && graphReading < 200) lcd.write(byte(1));
    if (graphReading >= 200 && graphReading < 300) lcd.write(byte(2));
    if (graphReading >= 300 && graphReading < 400) lcd.write(byte(3));
    if (graphReading >= 400 && graphReading < 500) lcd.write(byte(4));
    if (graphReading >= 500 && graphReading < 600) lcd.write(byte(5));
    if (graphReading >= 600 && graphReading < 700) lcd.write(byte(6));
    if (graphReading >= 700) lcd.write(byte(7));
    

    
    
  }
}

void loop() {
  /* Code to Analyze Sensor Readings*/
      int sensorVal = analogRead(sensorPin);
      Serial.print("Sensor Value: ");
      Serial.print(sensorVal);
    /*
      //convert the ADC reading to voltage
      float voltage = (sensorVal/1024.0) * 5.0;
    
      //Serial.print(", Volts: ");
      //Serial.print(voltage);
    
      //convert the voltage to temperature in degrees
      //Serial.print(", degrees C: ");
      float celcius = (voltage - .5) * 100;
      //Serial.print(celcius);
    
      float fahrenheit = celcius * 1.8 + 32;
      //Serial.print(", degrees F: ");
      //Serial.println(fahrenheit);
    */
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Noise: ");
      lcd.setCursor(8, 0);
      lcd.print(sensorVal);
      lcd.setCursor(0, 1);
      /*
      lcd.write(byte(0));
      lcd.write(byte(1));
      lcd.write(byte(2));
      lcd.write(byte(3));
      lcd.write(byte(4));
      lcd.write(byte(5));
      lcd.write(byte(6));
      lcd.write(byte(7));
      */

  /* Insert the Averaged Time into the Queue to Graph */
  unsigned long currentTime = millis();
  if (currentTime - previousTime < interval) {
    numTemps++;
    currentSum += sensorVal;
  }

  if (currentTime - previousTime > interval) {
    previousTime = currentTime;
    
    if (isFull()){
      removeData();
    }
    
    insert(sensorVal);
    //insert (currentSum / 1.0*numTemps);
    numTemps = 0;
    currentSum = 0;
    for (int i = 0; i < MAX; i++) {
      Serial.print(intArray[i]);
      Serial.print(" ");
    }
    Serial.println(" ");    
  }
  //graph();
  delay(10); 
}
