#include <stdlib.h>

#include "Exception.h"
#include "XTCReader.h"
#include "xdrfile.h"
#include "xdrfile_xtc.h"

#include "FileUtils.h"

namespace DKK{

const std::string &XTCReader::getName()
{
	static std::string name = "XTCReader";
	return name;
}

const std::vector<std::string> &XTCReader::getReqPrmNames()
{
	//stupid fix to avoid c++ 11 init lists
	static std::string reqPrmNamesA[] = {
		"IN",
		"ATOMS"
	};
	static std::vector<std::string> reqPrmNames(reqPrmNamesA, reqPrmNamesA+2);

	return reqPrmNames;
}

const std::vector<std::string> &XTCReader::getReqPrimitiveNames()
{
	static std::vector<std::string> reqPrimitiveNames;

	return reqPrimitiveNames;
}

void XTCReader::init()
{
	fin=NULL;
	//open input file
	getParameter("IN", fin_name);
	openFile();

	//read number of atoms per frame
	read_xtc_natoms(fin_name.c_str(), &n_atoms);
	coord = (rvec*) malloc(n_atoms*sizeof(rvec));

	//get the atom indexes to be considered
	getParameter("ATOMS", atoms);
	if(atoms.size()%2)
		throw Exception("ATOMS parameter should specify couples of values as index bounds.");

	//find the actual dimensionality of the dataset
	D = 0;
	for(int i=0; i<atoms.size(); i++)
		if(i%2)
			D += atoms[i]-atoms[i-1]+1;
	D *= 3;

	createFrameIndex();

	N=frameIndex.size();
}

void XTCReader::createFrameIndex()
{
	frameIndex.clear();
	DKK_TYPE_OFF offset=0;
	while(true){
		frameIndex.push_back(offset);
		try{
			offset = skipFrame();
		}catch(Exception &e){
			break;
		}
	}
	frameIndex.pop_back();
	goTo(0);
}

void XTCReader::goTo(const int &idx)
{
	dkk_fseek(xdrfile_get_cfile(fin), frameIndex[idx], 0);
}

void XTCReader::openFile()
{
	if(fin!=NULL)
		xdrfile_close(fin);
	fin=xdrfile_open(fin_name.c_str(), "r");
}

DKK_TYPE_OFF XTCReader::skipFrame()
{
	DKK_TYPE_OFF offset=0;	
	int data_length=0;
	int BYTES_PER_XDR_UNIT=4;

	//retrive current file offset
	offset = dkk_ftell(xdrfile_get_cfile(fin));
	//skip header: skip 3int,1float
	offset += 16;
	//skip box: skip 9float
	offset += 36;
	//skip header-data
	offset += 36;

	//seek file
	dkk_fseek(xdrfile_get_cfile(fin), offset, 0);
	if(ferror(xdrfile_get_cfile(fin)))
		return 0;

	//get frame data length
	if(!xdrfile_read_int(&data_length, 1, fin))
		throw Exception("IO XTC error, cannot read data length.");

	offset += 4;
	//round the number of bytes to fill xdr units
	int rndup = data_length % BYTES_PER_XDR_UNIT;
	rndup = rndup > 0 ? BYTES_PER_XDR_UNIT - rndup : 0;

	//skip frame data
	offset += (data_length + rndup);

	//seek file
	fseek(xdrfile_get_cfile(fin), offset, 0);

	return offset;
}

int XTCReader::getDimensionality()
{
	return D;
}

int XTCReader::getLength()
{
	return N;
}

int XTCReader::read(int idx, DKK_TYPE_REAL* data)
{
	goTo(idx);
	read_xtc(fin, n_atoms, &step, &time, box, coord, &prec);

	int k = 0;
	for(int i=0; i<atoms.size(); i+=2){
		for(int j=atoms[i]-1; j<atoms[i+1]; j++){
			data[k++] = coord[j][0];
			data[k++] = coord[j][1];
			data[k++] = coord[j][2];
		} 
	}

	return k;
}

}
