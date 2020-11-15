#define PINOFFSET 2
#define NUMINPUT 19 
#define SWITCH_MODULE_ID 2
#define ADDRESSOFFSET 24
int enablePin = NUMINPUT;  

uint32_t switchStates = 0;
uint32_t oldSwitchStates = 0;

void setup()
{
  Serial.begin(9600);            // initialize serial at baudrate 9600:
  for (uint8_t i = PINOFFSET; i < NUMINPUT; i++){
    pinMode(i,INPUT); 
    digitalWrite(i, HIGH);  //  (always high as Master Writes data to Slave)
  }
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, HIGH);  //  (always high as Master Writes data to Slave)
  delay(10); 
}
void loop()
{
    switchStates = 0;
    for (uint8_t i = PINOFFSET; i < NUMINPUT; i++){
	if ( i != 13){
	    uint8_t state = digitalRead(i);
	    switchStates |= state << i - PINOFFSET;
	} else {
	    switchStates |= 1 << i - PINOFFSET;
	}
    }
    switchStates = ~switchStates;
    uint32_t moduleId = SWITCH_MODULE_ID;
    moduleId = moduleId << ADDRESSOFFSET;
    switchStates |= moduleId;
    if (switchStates != oldSwitchStates){
	digitalWrite(enablePin,HIGH);
	Serial.println(switchStates);          //Serial Write POTval to RS-485 Bus
	delay(20);
	digitalWrite(enablePin,LOW);
    }
    oldSwitchStates = switchStates;
    delay(100);
}


