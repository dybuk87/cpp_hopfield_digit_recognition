#include "InputHandler.h"
#include <fstream>

int isDigit(char c) {
	return (c >= '0' && c <= '9');
}

int isChar(char c) {
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int getIntFromStream(char *stream, int len, int &pos) {
	while (pos < len && !isDigit(stream[pos]) && stream[pos] != '-') pos++;
	if (pos == len) throw "no digits";
	int znak = 1;
	if (stream[pos] == '-') {
		if (pos + 1 == len || !isDigit(stream[pos + 1])) throw "no digits";
		pos++;
		znak = -1;
	}
	int wynik = 0;

	while (pos < len && isDigit(stream[pos])) {
		wynik = wynik * 10 + stream[pos] - '0';
		pos++;
	}

	return wynik * znak;
}

Digit::Digit() {
	_data = 0;
	_len = 0;
	_label = std::string("none");
}


Digit::Digit(int len, int *data, std::string label) {
	_len = len;
	_data = new int[len];
	memcpy(_data, data, len*sizeof(int));
	_label = label;
}

Digit::Digit(const Digit &digit) {
	_len = digit._len;
	_label = digit._label;
	_data = 0;
	if (digit._len > 0 && digit._data) {
		_data = new int[digit._len];
		memcpy(_data, digit._data, digit._len * sizeof(int));
	}
}

Digit& Digit::operator=(const Digit &digit) {
	if (_data) {
		delete _data;
	}
	_len = digit._len;
	_label = digit._label;

	_data = 0;
	if (digit._len > 0 && digit._data) {
		_data = new int[digit._len];
		memcpy(_data, digit._data, digit._len * sizeof(int));
	}

	return *this;
}


int *Digit::get01Data() const {
	int *tmp = new int[_len];
	for (int i = 0; i < _len; i++) {
		tmp[i] = (_data[i] == 1 ? 1 : 0);
	}
	return tmp;
}

int *Digit::getData() const {
	return _data;
}

int Digit::getLen() const {
	return _len;
}

std::string Digit::getLabel() const {
	return _label;
}

Digit::~Digit() {
	if (_data) {
		delete _data;
	}
	_data = 0;
	_label = std::string("");
	_len = 0;
}

InputHandler::InputHandler(std::string fileName) {
	std::ifstream file;
	char data[2048];

	file.open(fileName.c_str());

	file.getline(data, 2048);
	int poz = 0;
	int digitCount = getIntFromStream(data, 2048, poz);
	int digitSize = getIntFromStream(data, 2048, poz);

	int labelSize = 0;
	try {
		labelSize = getIntFromStream(data, 2048, poz);
	}
	catch (...) {}

	int *digitData = new int[digitSize];

	for (int i = 0; i < digitCount; i++) {
		file.getline(data, 2048);
		poz = 0;
		for (int j = 0; j < digitSize; j++) {
			digitData[j] = getIntFromStream(data, 2048, poz);
		}
		std::string label = std::string("");
		if (labelSize) {
			label = "";
			while (poz < 2048 && *(data + poz) && !isChar(*(data + poz)) && !isDigit(*(data + poz))) {
				poz++;
			}
			if (poz < 2048) {
				label = std::string(data + poz);
			}
		}
		_digits.push_back(Digit(digitSize, digitData, label));
	}

	delete[] digitData;

	file.close();
}

Digit& InputHandler::getDigit(unsigned int poz) {
	if (poz >= _digits.size()) throw "out of bounds";
	return _digits[poz];
}