#ifndef MATRIX__H
#define MATRIX__H

#include <iostream>
#include <fstream>

template <class T>
class Matrix {
public:

	Matrix();

	Matrix(unsigned int rowsCount, unsigned int colsCount);

	Matrix(unsigned int rowsCount, unsigned int colsCount, T val);

	Matrix(unsigned int rowsCount, unsigned int colsCount, T* data);

	Matrix(const Matrix &m);

	Matrix& operator=(const Matrix &m);

	Matrix operator*(const Matrix &m);

	Matrix operator-(const Matrix &m);

	Matrix operator+(const Matrix &m);

	T& operator()(unsigned int row, unsigned int col);

	T cofactor(unsigned int row, unsigned int col);

	/* if change == 0 then create new transposed matrix and return it
		if change == 1 then modify current matrix and return this */	
	Matrix<T> transpose(int change = 0);

	/* create new matrix with crossout col and row (values set to zero)*/	
	Matrix<T> crossout(unsigned int row, unsigned int col, int change = 0) throw(...);
	
	T det() throw(...);
	
	Matrix<T> invert(int change = 0) throw(...);


	template <class B>
	operator Matrix<B>();

	void toFile(char *name);

	unsigned int getRowsCount() { return _rowsCount; }
	unsigned int getColsCount() { return _colsCount; }

	~Matrix();
private:
	T *_data;
	unsigned int _rowsCount;
	unsigned int _colsCount;
};

template <class T>
template <class B>
Matrix<T>::operator Matrix<B>() {
	Matrix<B> tmp(_rowsCount, _colsCount);

	for (unsigned int i = 0; i < _rowsCount; i++)
		for (unsigned int j = 0; j < _colsCount; j++) {
			T t = _data[i * _colsCount + j];
			B b = (B)t;
			tmp(i, j) = b;
		}

	return tmp;
}

template <class T>
Matrix<T>::Matrix() {
	_rowsCount = 0;
	_colsCount = 0;
	_data = 0;
}

template <class T>
Matrix<T>::Matrix(unsigned int rowsCount, unsigned int colsCount) {
	_rowsCount = rowsCount;
	_colsCount = colsCount;

	_data = 0;
	if (_rowsCount > 0 || _colsCount) {
		_data = new T[_rowsCount * _colsCount];
		memset(_data, 0, sizeof(T)*_rowsCount*_colsCount);

		unsigned int offset = 0;
		for (unsigned int i = 0; i < (rowsCount < colsCount ? rowsCount : colsCount); i++, offset += _colsCount + 1)
			_data[offset] = 1;
	}
}


template <class T>
Matrix<T>::Matrix(unsigned int rowsCount, unsigned int colsCount, T val) {
	_rowsCount = rowsCount;
	_colsCount = colsCount;

	_data = 0;
	if (_rowsCount > 0 && _colsCount > 0) {
		_data = new T[_rowsCount * _colsCount];
		//memset(_data, val, _rowsCount*_colsCount);		
		for (unsigned int i = 0; i < _rowsCount*_colsCount; i++)
			_data[i] = val;
	}
}

template <class T>
Matrix<T>::Matrix(unsigned int rowsCount, unsigned int colsCount, T* data) {
	_rowsCount = rowsCount;
	_colsCount = colsCount;

	_data = 0;
	if (_rowsCount > 0 && _colsCount > 0) {
		_data = new T[_rowsCount * _colsCount];
		for (unsigned int i = 0; i < _rowsCount*_colsCount; i++)
			_data[i] = data[i];
	}
}

template <class T>
Matrix<T>::Matrix(const Matrix<T> &m) {
	_rowsCount = m._rowsCount;
	_colsCount = m._colsCount;
	_data = 0;
	if (_rowsCount > 0 || _colsCount) {
		_data = new T[_rowsCount * _colsCount];
		memcpy(_data, m._data, sizeof(T)*_rowsCount*_colsCount);
	}
}

template <class T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T> &m) {
	if (_data)
		delete _data;

	_rowsCount = m._rowsCount;
	_colsCount = m._colsCount;
	_data = 0;
	if (_rowsCount > 0 || _colsCount) {
		_data = new T[_rowsCount * _colsCount];
		memcpy(_data, m._data, sizeof(T)*_rowsCount*_colsCount);
	}

	return *this;
}

template <class T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> &m) {
	if (_rowsCount > 0 && _colsCount > 0 &&
		m._rowsCount > 0 && m._colsCount > 0 &&
		_colsCount == m._rowsCount) {
		Matrix<T> tmp(_rowsCount, m._colsCount);

		for (unsigned int y = 0; y < _rowsCount; y++)
			for (unsigned int x = 0; x < m._colsCount; x++) {
				tmp._data[x + y * m._colsCount] = 0;
				for (unsigned int k = 0; k < _colsCount; k++)
					tmp._data[x + y * m._colsCount]
					+= _data[y * _colsCount + k] *
					m._data[x + k * m._colsCount];
			}

		return tmp;
	}
	else return Matrix();
}

template <class T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> &m) {
	if (_colsCount == m._colsCount && _rowsCount == m._rowsCount) {
		T *data = new T[_rowsCount * _colsCount];
		for (unsigned int i = 0; i < _rowsCount*_colsCount; i++)
			data[i] = _data[i] - m._data[i];
		Matrix<T> tmp(_rowsCount, _colsCount, data);
		delete data;
		return tmp;
	}
	return Matrix();
}

template <class T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &m) {
	if (_colsCount == m._colsCount && _rowsCount == m._rowsCount) {
		T *data = new T[_rowsCount * _colsCount];
		for (unsigned int i = 0; i < _rowsCount*_colsCount; i++) {
			data[i] = _data[i] + m._data[i];
		}
		Matrix<T> tmp(_rowsCount, _colsCount, data);
		delete data;
		return tmp;
	}
	return Matrix();
}

template <class T>
T& Matrix<T>::operator ()(unsigned int row, unsigned int col) {
	return _data[row*_colsCount + col];
}

template <class T>
Matrix<T> Matrix<T>::transpose(int change) {
	if (!change) {
		Matrix<T> tmp(_colsCount, _rowsCount);

		for (unsigned int y = 0; y < _rowsCount; y++) {
			for (unsigned int x = 0; x < _colsCount; x++) {
				tmp(x, y) = _data[y * _colsCount + x];
			}
		}

		return tmp;
	}
	else {
		T* tmp = new T[_rowsCount * _colsCount];

		for (unsigned int y = 0; y < _rowsCount; y++) {
			for (unsigned int x = 0; x < _colsCount; x++) {
				tmp[x*_rowsCount + y] = _data[y*_colsCount + x];
			}
		}

		delete _data;
		_data = tmp;

		unsigned int tmp2 = _rowsCount;
		_rowsCount = _colsCount;
		_colsCount = tmp2;

		return *this;
	}
}


template <class T>
Matrix<T> Matrix<T>::crossout(unsigned int row, unsigned int col, int change = 0) throw(...) {
	if (_rowsCount < 2 || _colsCount < 2) throw "can't crossout";
	if (row >= _rowsCount || col >= _colsCount) throw "crossout out of range";
	unsigned int r = _rowsCount - 1;
	unsigned int c = _colsCount - 1;
	T* data = new T[r*c];

	unsigned int ro = 0;
	unsigned int co = 0;
	for (unsigned int ri = 0; ri < _rowsCount; ri++)
		if (ri != row) {
			co = 0;
			for (unsigned int ci = 0; ci < _colsCount; ci++)
				if (ci != col) {
					data[co + ro * c] = _data[ci + ri * _colsCount];
					co++;
				}
			ro++;
		}

	if (change) {
		_rowsCount = r;
		_colsCount = c;
		if (data)
			delete data;
		data = _data;
		return *this;
	}
	else {
		Matrix<T> tmp(r, c, data);
		delete data;
		return tmp;
	}
}

template <class T>
T Matrix<T>::det() throw(...) {
	T val = 0;
	if (_rowsCount != _colsCount) {
		throw "rowsCount != colsCount";
	}
	if (_rowsCount == 1) {
		val = _data[0];
	}
	else if (_rowsCount == 2) {
		val = _data[0] * _data[3] - _data[1] * _data[2];
	}
	else if (_rowsCount == 3) {
		val = _data[0] * _data[4] * _data[8]
			+ _data[1] * _data[5] * _data[6]
			+ _data[2] * _data[3] * _data[7]
			- _data[2] * _data[4] * _data[6]
			- _data[1] * _data[3] * _data[8]
			- _data[0] * _data[5] * _data[7];
	}
	else {
		for (unsigned int i = 0; i < _colsCount; i++) {
			val += _data[i] * cofactor(0, i);
		}
	}
	return val;
}

template <class T>
T Matrix<T>::cofactor(unsigned int row, unsigned int col) {
	return ((row + col) % 2 ? -1 : 1) * crossout(row, col).det();
}

template <class T>
Matrix<T> Matrix<T>::invert(int change = 0) throw(...) {
	unsigned int r = _rowsCount;
	unsigned int c = _colsCount;
	T* data = new T[r*c];

	T detVal = det();

	if (detVal == (T)0)
		throw "det = 0";

	for (unsigned int ri = 0; ri < r; ri++) {
		for (unsigned int ci = 0; ci < c; ci++) {
			data[ri * c + ci] = cofactor(ci, ri) / detVal;
		}
	}

	if (change) {
		_rowsCount = r;
		_colsCount = c;
		if (data) {
			delete _data;
		}
		_data = data;
		return *this;
	}
	else {
		Matrix<T> tmp(r, c, data);
		delete data;
		return tmp;
	}
}

template <class T>
void Matrix<T>::toFile(char *name) {
	std::ofstream out(name, std::ios_base::app);

	for (unsigned int r = 0; r < _rowsCount; r++) {
		for (unsigned int c = 0; c < _colsCount; c++)
			out << _data[r * _colsCount + c] << " ";
		out << std::endl;
	}

	out << std::endl;
	out.close();
}

template <class T>
Matrix<T>::~Matrix() {
	if (_data) {
		delete _data;
		_data = 0;
	}
	_rowsCount = 0;
	_colsCount = 0;
}

#endif