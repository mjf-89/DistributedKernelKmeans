#include "Unit.h"

namespace DKK{

void Unit::setWorker(Worker &worker)
{
	this->worker = &worker;
}

Worker &Unit::getWorker()
{
	return *this->worker;
}


void Unit::setParameter(const std::string &name, const std::string &value)
{
	prms[name] = std::vector<std::string>(1, value);

	return;
}

void Unit::setParameter(const std::string &name, const std::vector<std::string> &value)
{
	prms[name] = value;

	return;
}

int Unit::getParameter(const std::string &name, std::string &prm)
{
	if (prms.find(name) == prms.end())
		throw 1;

	prm = prms[name][0];
	return 1;
}

int Unit::getParameter(const std::string &name, std::vector<std::string> &prm)
{
	if (prms.find(name) == prms.end())
		throw 1;

	prm = prms[name];
	return prm.size();
}

int Unit::getParameter(const std::string &name, int &prm)
{
	std::string sprm; 
	getParameter(name, sprm);

	prm = std::stoi(sprm);
	return 1;
}

int Unit::getParameter(const std::string &name, std::vector<int> &prm)
{
	std::vector<std::string> sprm;
	getParameter(name, sprm);

	prm.clear();
	for (int i = 0; i < sprm.size(); i++)
		prm.push_back(std::stoi(sprm[i]));

	return prm.size();
}

int Unit::getParameter(const std::string &name, float &prm)
{
	std::string sprm; 
	getParameter(name, sprm);

	prm = std::stof(sprm);
	return 1;
}

int Unit::getParameter(const std::string &name, std::vector<float> &prm)
{
	std::vector<std::string> sprm;
	getParameter(name, sprm);

	prm.clear();
	for (int i = 0; i < sprm.size(); i++)
		prm.push_back(std::stof(sprm[i]));

	return prm.size();
}

}