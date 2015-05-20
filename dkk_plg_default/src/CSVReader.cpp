#include <stdlib.h>

#include "Exception.h"
#include "CSVReader.h"

namespace DKK{

const std::string &CSVReader::getName()
{
	static std::string name = "CSVReader";
	return name;
}

const std::vector<std::string> &CSVReader::getReqPrmNames()
{
	//stupid fix to avoid c++ 11 init lists
	static std::string reqPrmNamesA[] = {
		"IN",
		"SEPARATOR"
	};
	static std::vector<std::string> reqPrmNames(reqPrmNamesA, reqPrmNamesA+2);

	return reqPrmNames;
}

const std::vector<std::string> &CSVReader::getReqPrimitiveNames()
{
	static std::vector<std::string> reqPrimitiveNames;

	return reqPrimitiveNames;
}

void CSVReader::init()
{
	//set index to 0
	record_idx = 0;

	//open input file
	std::string fname;
	getParameter("IN", fname);
	fin.open(fname.c_str());

	//set separator string
	getParameter("SEPARATOR", fs);
	escapeString(fs);

	//read dimensionality
	std::string record;
	DKK_TYPE_REAL field;
	try{
		record=getRecord();
	}catch(Exception &e){
		throw Exception("IO error, empty CSV file.");
	}
	D = 0;
	while (getField(record,field))
		D++;
	rewind();

	//read length
	N=0;
	try{
		while (1){
			getRecord();
			N++;
		}
	}catch(Exception &e){
		rewind();
	}
}

void CSVReader::escapeString(std::string &str)
{
	int i=0;
	while(i<str.length()){
		if(str[i]=='\\' && i+1<str.length()){
			str.erase(i,1);
			switch(str[i]){
			case 'n':
				str[i]='\n';
				break;
			case 't':
				str[i]='\t';
				break;
			case '\\':
				str[i]='\\';
				break;
			}
		}
		i++;
	}
}

void CSVReader::rewind()
{
	record_idx = 0;
	fin.clear();
	fin.seekg(0);
}

void CSVReader::goTo(const int &idx)
{
	std::string record;

	if (idx > N)
		throw(Exception("IO error, trying to read a record beyond EOF."));

	if (idx<record_idx)
		rewind();
	while (record_idx < idx)
		record=getRecord();
}

int CSVReader::getField(std::string &record, DKK_TYPE_REAL &field)
{
	int idx;
	//stripe initial field separators
	while (!(idx = record.find(fs, 0)))
		record.erase(0, fs.length());

	//check if the record is not empty
	if (!record.length())
		return 0;

	//read the field
	if (idx != std::string::npos){
		field = atof(record.substr(0, idx).c_str());
		record.erase(0, idx);
	}else{
		field = atof(record.c_str());
		record.clear();
	}

	return 1;
}

std::string CSVReader::getRecord()
{
	std::string record;
	char c;

	record.clear();
	while (fin.get(c) && c != '\n')
		record.push_back(c);

	record_idx++;

	if (!record.length())
		throw(Exception("IO error, EoF"));

	return record;
}

int CSVReader::getDimensionality()
{
	return D;
}

int CSVReader::getLength()
{
	return N;
}

int CSVReader::read(int idx, DKK_TYPE_REAL* data)
{
	std::string record;
	goTo(idx);
	record=getRecord();
	for (int i = 0; i < D; i++)
		if (!getField(record, data[i]))
			throw("IO error, bad CSV file.");

	return 0;
}

}
