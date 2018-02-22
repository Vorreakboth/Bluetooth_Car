#include <arduino.h>

class MOTOR_Initialize{
	private:
		byte input1;
		byte input2;
		boolean isStart; 
		unsigned int value;
	public:
		MOTOR_Initialize(byte _input1, byte _input2);		
		void start();		
		void forward(unsigned int _value);		
		void reverse(unsigned int _value);		
		void stop();
};

MOTOR_Initialize::MOTOR_Initialize(byte _input1, byte _input2){
	input1 = _input1;
	input2= _input2;
	pinMode(input1, OUTPUT);
	pinMode(input2, OUTPUT);
	isStart = false;
	value = 0;
}

void MOTOR_Initialize::start(){
	isStart = true;
}

void MOTOR_Initialize::forward(unsigned int _value){
	if(isStart){
		analogWrite(input1, _value);
		if(digitalRead(input2)) digitalWrite(input2, 0);
	}
}

void MOTOR_Initialize::reverse(unsigned int _value){
	if(isStart){
    analogWrite(input2, _value);
		if(digitalRead(input1)) digitalWrite(input1, 0);
	}
}

void MOTOR_Initialize::stop(){
	if(isStart){
		isStart = false;
		digitalWrite(input1, 0);
		digitalWrite(input2, 0);
	}
}
