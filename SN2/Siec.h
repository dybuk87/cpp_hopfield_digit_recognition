#ifndef SIEC__H
#define SIEC__H

#include <iostream>
#include <vector>
#include <string>
#include "Matrix.h"

class Result {
public:
	Result(unsigned int labelId, unsigned int percent) :
		_labelId(labelId), _percent(percent) { }

	unsigned int getLabelId() { return _labelId; }
	unsigned int getPercent() { return _percent; }

	static int maxMin(const Result& a, const Result& b);

private:
	unsigned int _labelId;
	unsigned int _percent;
};

class DataVect {
public:
	DataVect();

	DataVect(unsigned int dataLength, int *data, std::string label);

	DataVect(const DataVect &m);

	DataVect& operator=(const DataVect &m);


	~DataVect();
private:
	unsigned int _dataLength;
	int *_data;
	std::string _label;
};

class Siec {
public:
	Siec(unsigned int neuronsCount, unsigned int samplesCount);

	/* data musi mieæ d³ugoœæ neuronsCount */
	void setSample(int *data, int colNo);

	void onePass();

	void teach();

	/* musi mieæ d³ugoœæ neuronsCount */
	std::vector<Result> test(int *dataToTest);
private:

	unsigned int _neuronsCount;
	unsigned int _samplesCount;

	Matrix<int> _userInput;
	Matrix<double> _input;
	Matrix<double> _output;
	Matrix<double> _wMatrix;
};

#endif