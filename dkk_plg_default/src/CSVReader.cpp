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

	//read dimensionality
	std::string record;
	float field;
	getRecord(record);
	D = 0;
	while (getField(record,field))
		D++;
	rewind();

	//read length
	N=0;
	while (getRecord(record))
		N++;
	rewind();
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
		throw 1;

	if (idx<record_idx)
		rewind();
	while (record_idx < idx)
		getRecord(record);
}

int CSVReader::getField(std::string &record, float &field)
{
	if (!record.length())
		return 0;
	
	int idx;
	while (!(idx = record.find(fs, 0)))
		record.erase(0, fs.length());

	if (idx != std::string::npos){
		field = std::stof(record.substr(0, idx));
		record.erase(0, idx);
	}else{
		field = std::stof(record);
		record.clear();
	}

	return 1;
}

int CSVReader::getRecord(std::string &record)
{
	char c;

	record.clear();
	while (fin.get(c) && c != '\n')
		record.push_back(c);

	record_idx++;

	if (record.length())
		return 1;

	return 0;
}

int CSVReader::getDimensionality()
{
	return D;
}

int CSVReader::getLength()
{
	return N;
}

int CSVReader::read(int idx, float* data)
{
	std::string record;
	goTo(idx);
	getRecord(record);
	for (int i = 0; i < D; i++)
		if (!getField(record, data[i]))
			throw 1;

	return 0;
}

}