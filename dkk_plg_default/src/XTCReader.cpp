#include <stdlib.h>

#include "XTCReader.h"
#include "xdrfile.h"
#include "xdrfile_xtc.h"

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
	//open input file
	getParameter("IN", fin_name);
	openFile();

	//read number of atoms per frame
	read_xtc_natoms(fin_name.c_str(), &n_atoms);
	coord = (rvec*) malloc(n_atoms*sizeof(rvec));

	//get the atom indexes to be considered
	getParameter("ATOMS", atoms);
	if(atoms.size()%2)
		throw 1;

	//find the actual dimensionality of the dataset
	D = 0;
	for(int i=0; i<atoms.size(); i++)
		if(i%2)
			D += atoms[i]-atoms[i-1]+1;
	D *= 3;


	record_idx = 0;

	N=goToEOF();
}

void XTCReader::goTo(const int &idx)
{
	if(idx<record_idx)
		openFile();

	while(idx>record_idx)
		skipFrame();
}

int XTCReader::goToEOF()
{
	while(skipFrame());

	return record_idx;
}

void XTCReader::openFile()
{
	xdrfile_close(fin);	
	fin=xdrfile_open(fin_name.c_str(), "r");
	record_idx = 0;
}

int XTCReader::skipFrame()
{
	unsigned int offset=0;	
	int data_length=0;
	int BYTES_PER_XDR_UNIT=4;

	/* skip header: skip 3int,1float */
	offset += 16;
	/* skip box: skip 9float */
	offset += 36;
	/* skip header-data */
	offset += 36;
	if(xdrfile_skip_bytes(offset, fin))
		return 0;

	xdrfile_read_int(&data_length, 1, fin);

	/* skip data */
	offset = 0;
	int rndup = data_length % BYTES_PER_XDR_UNIT;
	rndup = rndup > 0 ? BYTES_PER_XDR_UNIT - rndup : 0;
	offset += (data_length + rndup);

	xdrfile_skip_bytes(offset, fin);

	record_idx++;

	return 1;
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
	record_idx++;

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
