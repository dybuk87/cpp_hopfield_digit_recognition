#ifndef INPUTHANDLER__H
#define INPUTHANDLER__H

#include <iostream>
#include <vector>

class Digit {
public:
	Digit();
	Digit(int len, int *data, std::string label);

	Digit(const Digit& digit);

	Digit& operator=(const Digit&);

	int *getData() const;
	int *get01Data() const;

	int getLen() const;

	std::string getLabel() const;

	~Digit();
private:
	int *_data;
	int _len;

	std::string _label;
};

class InputHandler {
public:
	InputHandler(std::string fileName);

	Digit& getDigit(unsigned int poz) throw(...);

private:

	std::vector<Digit> _digits;
};

int getIntFromStream(char *stream, int len, int &pos) throw(...);

#endif