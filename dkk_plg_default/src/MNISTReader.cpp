#include <stdlib.h>

#include "MNISTReader.h"

namespace DKK{

const std::string &MNISTReader::getName()
{
	static std::string name = "MNISTReader";
	return name;
}

const std::vector<std::string> &MNISTReader::getReqPrmNames()
{
	//stupid fix to avoid c++ 11 init lists
	static std::string reqPrmNamesA[] = {
		"IN"
	};
	static std::vector<std::string> reqPrmNames(reqPrmNamesA, reqPrmNamesA+1);

	return reqPrmNames;
}

const std::vector<std::string> &MNISTReader::getReqPrimitiveNames()
{
	static std::vector<std::string> reqPrimitiveNames;

	return reqPrimitiveNames;
}

int MNISTReader::read32Int()
{
	int data=0;

	fin.read((char *)buffer,4);
	for(int i=0; i<4; i++)
		data = (data<<8)|buffer[i];

	return data;
}

unsigned char MNISTReader::readByte()
{
	unsigned char data;
	fin.read((char *)&data,1);
	return data;
}

void MNISTReader::init()
{
	//open input file
	std::string fname;
	getParameter("IN", fname);
	fin.open(fname.c_str());

	magic = read32Int();
	N = read32Int();
	D = read32Int() * read32Int();
}

void MNISTReader::goTo(const int &idx)
{
	fin.seekg(16 + idx*D);
}

int MNISTReader::getDimensionality()
{
	return D;
}

int MNISTReader::getLength()
{
	return N;
}

int MNISTReader::read(int idx, DKK_TYPE_REAL* data)
{
	goTo(idx);
	for(int i=0; i<D; i++)
		data[i]=(DKK_TYPE_REAL)readByte();

	return 0;
}

}
