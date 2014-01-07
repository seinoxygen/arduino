#include "Print.h"
class HDSP2001 {
	public:
		HDSP2001(int data, int clock, int * columns);
		void setChars(int chars);
		void setMessage(char * message);
		void prepareDisplay();
		void print();
		void scrollToLeft();
		void scrollToRight();
		
	private:
		static const int _numOfColumns = 5;
		static const int _numOfRows = 7;
		static const int _numOfChars = 4;
		static const int _numOfLeds = _numOfRows * _numOfChars;
		
		int _data;
		int _clock;
		int * _columns;
		
		char _message[_numOfChars];
		char * _original;
		int _messageIndex;
		
		bool _scrolling;
		
		int _serial[_numOfColumns][_numOfLeds];
		
		void serialOut(int column);
		void columnChange(int column);
		void clear();		
};