#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define sensorInterrupt 0  // 0 = digital pin 2
#define flowPin         2
#define buzzerPin       3
#define relayValvePin   11 
#define relayPumpPin    12
#define ledPin          13
#define levelPin1       A0
#define levelPin2       A1

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {10, 9, 8, 7}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 5, 4}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

LiquidCrystal_I2C lcd(0x3F, 16, 2);

int lvlBakAir;
int lvlKaporit;
String inputKaporit;
int kaporitPool;
float calibrationFactor = 4.5;
String inputCode;
double waterpoolLitre;
volatile byte pulseCount;  
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
unsigned long oldTime;

void setup() {
  Serial.begin(9600);
  pinMode(relayValvePin, OUTPUT);
  pinMode(relayPumpPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(flowPin, INPUT);
  digitalWrite(flowPin, HIGH);

  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;

  // The Hall-effect sensor is connected to pin 2 which uses interrupt 0.
  // Configured to trigger on a FALLING state change (transition from HIGH
  // state to LOW state)
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);

  lcd.begin();
  lcd.backlight();
  lcd.setCursor(4,0);
  lcd.print("Welcome");
  delay(1000);
  lcd.clear();
}

void loop() {
  lvlBakAir = getLevelData1();
  Serial.println(lvlBakAir);
  if(lvlBakAir > 4){
    digitalWrite(relayValvePin, LOW);
  }
  else{
    digitalWrite(relayValvePin, HIGH);
  }
  char key = keypad.getKey();
  if(key=='*'){
    digitalWrite(buzzerPin, LOW);
    lcd.clear();
    delay(200);
    lcd.print("Kaporit in mL:");
    lcd.setCursor(0,1);
    while(1){
      char x = keypad.getKey();
      if(x){
        if(x!='#'){
          lcd.print(x);
          inputKaporit+=x;
        }
        if(x=='#'){
          break;
        }
      }
    }
  }
  kaporitPool = inputKaporit.toInt();
  inputKaporit = "";
  while(kaporitPool>0){
    digitalWrite(relayPumpPin, HIGH);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Kaporit Out:");
    lcd.setCursor(0,1);
    lcd.print(kaporitPool);
    kaporitPool-=getFlowData();
    delay(500);
  }
  digitalWrite(relayPumpPin, LOW);
  lvlKaporit = getLevelData2();
  if(lvlKaporit < 3){
    digitalWrite(buzzerPin, !digitalRead(buzzerPin));
  }
  else{
    digitalWrite(buzzerPin, LOW);
  }
  lcd.setCursor(0,0);
  lcd.print("Bak Air: ");
  lcd.setCursor(9,0);
  lcd.print("Level ");
  lcd.print(lvlBakAir);
  lcd.setCursor(0,1);
  lcd.print("Kaporit: ");
  lcd.setCursor(9,1);
  lcd.print("Level ");
  lcd.print(lvlKaporit);
}
