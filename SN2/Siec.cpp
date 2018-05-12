#include "Siec.h"
#include <algorithm>
#include <cmath>

int Result::maxMin(const Result &a, const Result &b) {
	return a._percent > b._percent;
}

DataVect::DataVect() {
	_dataLength = 0;
	_data = 0;
	_label = std::string("");
}

DataVect::DataVect(unsigned int dataLength, int *data, std::string label) {
	_dataLength = dataLength;
	_label = label;
	_data = new int[dataLength];
	memcpy(_data, data, sizeof(int) * dataLength);
}

DataVect::DataVect(const DataVect &m) {
	_dataLength = m._dataLength;
	_label = m._label;
	_data = 0;
	if (m._dataLength > 0) {
		_data = new int[m._dataLength];
		memcpy(_data, m._data, sizeof(int) * m._dataLength);
	}
}

DataVect& DataVect::operator=(const DataVect &m) {
	if (_data)
		delete _data;
	_dataLength = m._dataLength;
	_label = m._label;
	_data = 0;
	if (m._dataLength > 0) {
		_data = new int[m._dataLength];
		memcpy(_data, m._data, sizeof(int) * m._dataLength);
	}

	return *this;
}

DataVect::~DataVect() {
	if (_data)
		delete _data;
	_data = 0;
	_label = std::string("");
	_dataLength = 0;
}

Siec::Siec(unsigned int neuronsCount, unsigned int samplesCount) : _wMatrix(neuronsCount, neuronsCount, 0.0),
_neuronsCount(neuronsCount),
_samplesCount(samplesCount),
_userInput(neuronsCount, samplesCount, 0),
_input(neuronsCount, samplesCount, 0.0),
_output(neuronsCount, samplesCount, 0.0) {
}

void Siec::setSample(int *data, int colNo) {
	for (unsigned int i = 0; i < _neuronsCount; i++) {
		_userInput(i, colNo) = data[i];
	}

	_input = (Matrix<double>)_userInput;
}

void Siec::onePass() {
	for (unsigned int p = 0; p < _samplesCount; p++) {
		Matrix<double> _input2(192, 1);

		for (unsigned int i = 0; i < 192; i++) {
			_input2(i, 0) = _input(i, p);
		}

		Matrix<double> licznik = ((_wMatrix * _input2 - _input2) * (_wMatrix * _input2 - _input2).transpose(1));
		Matrix<double> mianownik = (_input2.transpose() * _input2 - _input2.transpose() * _wMatrix * _input2);

		for (unsigned int i = 0; i < licznik.getRowsCount(); i++) {
			for (unsigned int j = 0; j < licznik.getColsCount(); j++) {
				licznik(i, j) /= mianownik(0, 0);
			}
		}

		_wMatrix = _wMatrix + licznik;

		for (unsigned int i = 0; i < _wMatrix.getRowsCount(); i++) {
			_wMatrix(i, i) = 0.0;
		}

		for (unsigned int i = 0; i < _wMatrix.getRowsCount(); i++) {
			for (unsigned int j = 0; j < _wMatrix.getColsCount(); j++) {
				if (i < j) {
					_wMatrix(j, i) = _wMatrix(i, j);
				}
			}
		}
	}
}

void Siec::teach() {
	for (unsigned int i = 0; i < 1; i++) {
		//_wMatrix.toFile("onPass.txt");
		onePass();
	}
}

std::vector<Result> Siec::test(int *dataToTest) {
	Matrix<double> tmp(_neuronsCount, 1);

	for (unsigned int i = 0; i < _neuronsCount; i++)
		tmp(i, 0) = dataToTest[i];

	Matrix<double> yk = tmp;
	int ok = 0;
	int iter = 5000;
	do {
		Matrix<double> yk1 = _wMatrix * yk;
		for (unsigned int i = 0; i < _neuronsCount; i++)
			if (yk1(i, 0) > 0) yk1(i, 0) = 1.0;
			else if (yk1(i, 0) <= 0) yk1(i, 0) = -1.0;
			ok = 1;
			for (unsigned int i = 0; i<_neuronsCount; i++)
				if (fabs(yk1(i, 0) - yk(i, 0))>0.02)
					ok = 0;
			yk = yk1;
			iter--;
	} while (!ok && iter);

	std::vector<Result> list;

	for (unsigned int i = 0; i < _samplesCount; i++) {
		int error = 0;
		for (unsigned int j = 0; j<_neuronsCount; j++)
			if (((double)_userInput(j, i) - yk(j, 0)) > 0.01)
				error++;
		list.push_back(Result(i, ((192 - error) * 100) / 192));
	}

	std::sort(list.begin(), list.end(), Result::maxMin);

	std::vector<Result> best;
	unsigned int len = list.size();
	//if (len > 3) len = 3;
	for (unsigned int i = 0; i < len; i++)
		best.push_back(list[i]);

	return best;
}