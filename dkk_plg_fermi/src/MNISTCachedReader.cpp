#include <stdlib.h>

#include "MNISTCachedReader.h"

namespace DKK{

const std::string &MNISTCachedReader::getName()
{
	static std::string name = "MNISTCachedReader";
	return name;
}

const std::vector<std::string> &MNISTCachedReader::getReqPrmNames()
{
	//stupid fix to avoid c++ 11 init lists
	static std::string reqPrmNamesA[] = {
		"IN",
		"CACHE_SIZE"
	};
	static std::vector<std::string> reqPrmNames(reqPrmNamesA, reqPrmNamesA+2);

	return reqPrmNames;
}

const std::vector<std::string> &MNISTCachedReader::getReqPrimitiveNames()
{
	static std::vector<std::string> reqPrimitiveNames;

	return reqPrimitiveNames;
}

int MNISTCachedReader::read32Int()
{
	int data=0;

	for(int i=0; i<4; i++)
		data = (data<<8)|cache[i];

	cache += 4;

	return data;
}

unsigned char MNISTCachedReader::readByte()
{
	unsigned char data;

	data = cache[0];
	cache++;
	return data;
}

void MNISTCachedReader::init()
{
	//open input file
	std::string fname;
	getParameter("IN", fname);
	fin.open(fname.c_str());

	getParameter("CACHE_SIZE", cache_size);
	cache_buffer = (unsigned char*)malloc(sizeof(unsigned char)*cache_size);

	fin.read((char *)cache_buffer, 16);
	cache = cache_buffer;
	magic = read32Int();
	N = read32Int();
	D = read32Int() * read32Int();

	cache_N = cache_size/D;
	cache_idx = -cache_size-1;
}

void MNISTCachedReader::goTo(const int &idx)
{
	if(idx < cache_idx || idx >= cache_idx+cache_N){
		fin.seekg(16 + idx*D);
		fin.read((char *)cache_buffer, cache_size);		
		cache_idx = idx;
	}

	cache = cache_buffer+(idx-cache_idx)*D;
}

int MNISTCachedReader::getDimensionality()
{
	return D;
}

int MNISTCachedReader::getLength()
{
	return N;
}

int MNISTCachedReader::read(int idx, DKK_TYPE_REAL* data)
{
	goTo(idx);
	for(int i=0; i<D; i++)
		data[i]=(DKK_TYPE_REAL)readByte()/255.0;

	return 0;
}

}
