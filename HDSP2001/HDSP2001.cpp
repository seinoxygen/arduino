#include "Arduino.h"
#include "HDSP2001.h"
#include "HDSP2001_Chars.h"

HDSP2001::HDSP2001(int data, int clock, int columns[]){
	_data = data;
	_clock = clock;
	_columns = columns;
	
	pinMode(_data, OUTPUT);
	pinMode(_clock, OUTPUT);
	digitalWrite(_data, LOW);
	digitalWrite(_clock, HIGH);
	for (int i = 0; i < 5; i++){
		pinMode(_columns[i], OUTPUT);
	}
	for (int i = 0; i < 5; i++){
		digitalWrite(_columns[i], LOW);
	}
}

void HDSP2001::setMessage(char * message){
	for (int i =0 ; i < _numOfChars; i++){
		_message[i] = message[i];
	}
	_original = message;
	_messageIndex = 0;
}

void HDSP2001::prepareDisplay(){  
  for(int i = 0; i < _numOfChars; i++){
    char c = _message[i];
    c -= 32;
 
    for (int led = 0; led < _numOfRows; led++){
      int bits = HDSP2001_Chars[c][led];
      for (int col = 0; col < _numOfColumns; col++){
        _serial[col][led+i*_numOfRows] = (bits & 1<< col);
      }
    }   
  }
}

void HDSP2001::print(){
	for (int col = 0; col < _numOfColumns; col++){// COLUMN       
		serialOut(col);
		columnChange(col);
	} 
}

void HDSP2001::columnChange(int col){
	digitalWrite(_columns[col], HIGH);
	delayMicroseconds(500);
	digitalWrite(_columns[col], LOW);
}

void HDSP2001::serialOut(int col){
	for (int led = 0; led < _numOfLeds; led++){
		digitalWrite(_clock, HIGH);
		if(_serial[col][led] > 0){
			digitalWrite(_data, HIGH);
		}
		else{
			digitalWrite(_data, LOW);
		}
		digitalWrite(_clock, LOW);
	}
}

void HDSP2001::scrollToLeft(){
	if(_scrolling == false){
		clear();
		_scrolling = true;
	}
	if(_messageIndex >= strlen(_original)){
		_messageIndex = 0;
	}
	for (int i = 1 ; i < _numOfChars; i++){
		_message[i-1] = _message[i];
	}
	_message[_numOfChars-1] = _original[_messageIndex];
	_messageIndex++;
	prepareDisplay();
}

void HDSP2001::scrollToRight(){
	if(_scrolling == false){
		clear();
		_scrolling = true;
		_messageIndex = strlen(_original) - 1;
	}
	if(_messageIndex < 0){
		_messageIndex = strlen(_original) - 1;
	}
	for (int i = _numOfChars - 1 ; i >= 0 ; i--){
		_message[i] = _message[i-1];
	}
	_message[0] = _original[_messageIndex];
	Serial.println(_messageIndex);
	_messageIndex--;
	prepareDisplay();	
}

void HDSP2001::clear(){
	for (int i =0 ; i < _numOfChars; i++){
		_message[i] = 32;
	}
}