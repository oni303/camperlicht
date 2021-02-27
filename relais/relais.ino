#define PINOFFSET 2
#define NUMOUTPUT 19
#define NUMSWITCHMODULES 2
#define NUMSWITCHSPERMODULES 17
#define ADDRESSOFFSET 24

#define ON LOW
#define OFF HIGH 


uint32_t relaiSwitchMapping[NUMSWITCHMODULES][NUMSWITCHSPERMODULES] = {
    {  //relay3 relai2 relai1
	0b00000000000000000, //switch1
	0b00000000000000000, //switch2
	0b00000000000000000, //switch3
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
	0b00000000000000101,
	0b00000000000000010,
	0b00000000000000100,
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

typedef struct {
    uint8_t moduleId;
    uint32_t switchStates;
} message_t;

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
    message_t message;
    while (Serial.available() > 0){
	message = getMessage();
	if (message.switchStates == 0){ // ignore empty messages
	    break;
	}
	Serial.print("in ");        
	Serial.print(message.moduleId);    
	Serial.print(" ");        
        printBits(message.switchStates);
        uint32_t relaisToEffect = getRelaisToEffect(message, relaiStates);

	relaiStates ^= relaisToEffect;
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

void printBits(uint32_t bits)
{
	for (uint8_t i = 0; i <  NUMSWITCHSPERMODULES ; i++){
	    if ( bits & (1 << i) ){
		Serial.print("1");    
	    }else{
		Serial.print("0");     
	    }
	}
	Serial.println(" ");          
}

uint32_t getRelaisToEffect(message_t &m, uint32_t relaiStates)
{
    uint32_t toEffect = 0;


    printBits(m.switchStates);
    for (uint8_t switchNum = 0; switchNum < NUMSWITCHSPERMODULES; switchNum++){
        if (m.switchStates & (1 << switchNum)){
            // if only some of the relais of this switch are on, we trun all off 
            uint32_t onRelais = relaiStates & relaiSwitchMapping[m.moduleId][switchNum];
            if (onRelais != 0){
                toEffect |= onRelais;
            } else {
                toEffect |= relaiSwitchMapping[m.moduleId][switchNum];
            }
        }
    }    
    return toEffect;
}

message_t getMessage(void)
{
    message_t m;
    uint32_t switchStates = Serial.parseInt();
    m.moduleId = switchStates >> ADDRESSOFFSET;
    m.moduleId -= 1;
    m.switchStates = switchStates & 0x3ffff;
    return m;
}
