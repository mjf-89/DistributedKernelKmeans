#ifndef DKK_ARRAY2D_H
#define DKK_ARRAY2D_H

#include <stdlib.h>

namespace DKK{

template<class T> 
class Array2D{
public:
	Array2D(int r, int c);
	Array2D(int r);

	~Array2D();

	inline const int &length() const { return l; }
	inline const int &rows() const { return r; }
	inline const int &cols() const { return c; }

	inline T &idx(const int &i){ return buffer[i*c]; }
	inline T &idx(const int &i, const int &j) { return buffer[i*c + j]; }
	
	inline T *bff(const int &i){ return buffer + i*c; }

	void fill(const T &f);
private:
	int l, r, c;
	T* buffer;

	void allocate();
};

template<class T>
Array2D<T>::Array2D(int r, int c)
{
	this->r = r;
	this->c = c;
	l = r*c;

	allocate();
}

template<class T>
Array2D<T>::Array2D(int r)
{
	this->r = r;
	this->c = 1;
	l = r*c;

	allocate();
}

template<class T>
Array2D<T>::~Array2D()
{
	free(buffer);
}

template<class T>
void Array2D<T>::allocate()
{
	buffer = (T*)malloc(sizeof(T)*l);
}

template<class T>
void Array2D<T>::fill(const T& f)
{
	for(int i=0; i<l; i++)
		buffer[i] = f;
}

}
#endif