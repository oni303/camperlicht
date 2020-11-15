#define PINOFFSET 2
#define NUMOUTPUT 19
#define NUMSWITCHMODULES 2
#define NUMSWITCHSPERMODULES 17
#define ADDRESSOFFSET 24

#define ON LOW
#define OFF HIGH 


uint32_t relaiSwitchMapping[NUMSWITCHMODULES][NUMSWITCHSPERMODULES] = {
    {
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000
    },
    {
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000,
	0b00000000000000000
    }};
int enablePin = 19;  

uint32_t switchStates = 0;
uint32_t relaiStates = 0;

void setup()
{
  Serial.begin(9600);            // initialize serial at baudrate 9600:
  for (uint8_t i = PINOFFSET; i < NUMOUTPUT; i++){
    pinMode(i,OUTPUT); 
    digitalWrite(i, OFF);  
  }
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW);
  delay(10); 
}
void loop()
{
    while (Serial.available() > 0){
	uint8_t moduleId = getMessage();
	if (switchStates == 0){ // ignore empty messages
	    break;
	}

	relaiStates ^= switchStates;
	Serial.print("out ");        
	for (uint8_t i = PINOFFSET; i < NUMOUTPUT ; i++){
	    if ( relaiStates & (1 << i - PINOFFSET) ){
		digitalWrite(i, ON);
		Serial.print("1");    
	    }else{
		digitalWrite(i, OFF);
		Serial.print("0");     
	    }
	}
	Serial.println(" ");          
	delay(100);
    }
}

uint8_t getMessage(void)
{
    switchStates = Serial.parseInt();
    uint8_t moduleId = switchStates >> ADDRESSOFFSET;
    switchStates = switchStates & 0x3ffff;
    return moduleId;
}
