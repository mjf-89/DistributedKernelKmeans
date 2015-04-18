#ifndef DKK_DISTRIBUTEDARRAY2D_H
#define DKK_DISTRIBUTEDARRAY2D_H

#include <stdlib.h>
#include "Communicator.h"
#include "Array2D.h"

namespace DKK{

template<class T> 
class DistributedArray2D: public Array2D<T>{
public:
	DistributedArray2D(Communicator &comm, int gr, int gc);
	DistributedArray2D(Communicator &comm, int gr);

	inline const int &glength() const { return gl; }
	inline const int &grows() const { return gr; }
	inline const int &gcols() const { return gc; }

	inline T &gidx(const int &i) {return this->idx(i-r_off);}
	inline T &gidx(const int &i, const int &j) {return this->idx(i-r_off, j);}

	void ltgIdx(const int &li, const int &lj, int &gi, int &gj);
	void gtlIdx(const int &gi, const int &gj, int &li, int &lj);
private:
	int gl, gr, gc;
	int r_off;
};

template<class T>
DistributedArray2D<T>::DistributedArray2D(Communicator &comm, int gr, int gc)
:Array2D<T>((int)(gr/comm.getSize())+(comm.getRank()<gr%comm.getSize()?1:0),gc)
{
	this->gr = gr;
	this->gc = gc;
	gl = gr*gc;

	r_off = comm.getRank() * (int)(gr/comm.getSize()) + (comm.getRank()>gr%comm.getSize()?gr%comm.getSize():comm.getRank());
}

template<class T>
DistributedArray2D<T>::DistributedArray2D(Communicator &comm, int gr)
:Array2D<T>((int)(gr/comm.getSize())+(comm.getRank()<gr%comm.getSize()?1:0))
{
	this->gr = gr;
	this->gc = 1;
	gl = gr*gc;

	r_off = comm.getRank() * (int)(gr/comm.getSize()) + (comm.getRank()>gr%comm.getSize()?gr%comm.getSize():comm.getRank());
}

template<class T>
void DistributedArray2D<T>::ltgIdx(const int &li, const int &lj, int &gi, int &gj)
{
	gi = r_off+li;
	gj = lj;
}

template<class T>
void DistributedArray2D<T>::gtlIdx(const int &gi, const int &gj, int &li, int &lj)
{
	li = gi-r_off;
	lj = gj;
}

}

#endif
