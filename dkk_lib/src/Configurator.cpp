#include <string>
#include <iostream>

#ifdef WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

#include "Configurator.h"
#include "Communicator.h"
#include "Unit.h"
#include "Worker.h"
#include "Primitive.h"

namespace DKK{

Configurator::Configurator(int *argc, char ***argv)
{
	comm = new Communicator(argc, argv);
	conf_file.open((*argv)[1], std::ios::in);

	std::string line;
	while (std::getline(conf_file,line))
	{
		Option opt(line);
		if (opt.getName().length()){
			opts.insert(std::pair<std::string, Configurator::Option>(opt.getName(), opt));
		}
	}
	return;
}

Configurator::~Configurator()
{
	delete comm;
}

Configurator::Option &Configurator::getOption(std::string name)
{
	if (opts.find(name) != opts.end())
		return opts.find(name)->second;
	throw 1;
}

std::map<std::string, Unit*> Configurator::units;
void Configurator::registerUnit(Unit *unit)
{
	units.insert(std::pair<std::string, Unit*>(unit->getName(), unit));
}

Unit & Configurator::getUnit(const std::string &name)
{
	if (units.find(name) != units.end())
		return *units.find(name)->second;

	throw 1;
}

Communicator *Configurator::comm;
Communicator &Configurator::getCommunicator()
{
	return *comm;
}

std::map<std::string, Worker*> Configurator::workers;
void Configurator::registerWorker(Worker *worker)
{
	workers.insert(std::pair<std::string, Worker*>(worker->getName(), worker));
}

Worker &Configurator::getWorker(const std::string &name)
{
	if (workers.find(name) != workers.end())
		return *workers.find(name)->second;

	throw 1;
}

Reader &Configurator::getReader()
{
	if (opts.find("READER") == opts.end())
		throw 1;

	Option opt = opts.find("READER")->second;
	return (Reader &)getUnitFromOption(opt);
}

Kernel &Configurator::getKernel()
{
	if (opts.find("KERNEL") == opts.end())
		throw 1;

	Option opt = opts.find("KERNEL")->second;
	return (Kernel &)getUnitFromOption(opt);
}

Initializer &Configurator::getInitializer()
{
	if (opts.find("INITIALIZER") == opts.end())
		throw 1;

	Option opt = opts.find("INITIALIZER")->second;
	return (Initializer &)getUnitFromOption(opt);
}

Unit &Configurator::getUnitFromOption(Option &opt){
	if (units.find(opt.getValue()[0]) == units.end())
		throw 1;

	Unit &u = *units[opt.getValue()[0]];

	//check for required parameters
	for (int i = 0; i < u.getReqPrmNames().size(); i++)
		if (opt.getPrm(u.getReqPrmNames()[i]).size() == 0)
			throw 1;
	//Unit parametrization
	for (std::map<std::string, std::vector<std::string> >::const_iterator i = opt.getPrms().begin(); i != opt.getPrms().end(); i++){
		if (i->first != "VALUE")
			u.setParameter(i->first, i->second);
	}

	//check for required primitives
	if(u.getReqPrimitiveNames().size()){
		if(opt.getPrm("WORKER").size() == 0)
			throw 1;

		u.setWorker(getWorker(opt.getPrm("WORKER")[0]));
		for(int i=0; i<u.getReqPrimitiveNames().size(); i++)
			u.getWorker().getPrimitive(u.getReqPrimitiveNames()[i]);
	}

	u.init();

	return u;
}

void Configurator::loadPlugins()
{
	if (opts.find("PLUGIN_FILES") != opts.end()){
		const std::vector<std::string> &plg_paths = (opts.find("PLUGIN_FILES")->second).getValue();

		for (int i = 0; i < plg_paths.size(); i++)
			loadPluginFile(plg_paths[i]);
	}
}

void Configurator::loadPluginFile(const std::string &file)
{
#ifdef WIN32
	HMODULE handle = LoadLibrary(file.c_str());
	if (handle == NULL)
		throw 1;

	void (*reg)();
	*(void **)(&reg) = GetProcAddress(handle, "registerUnits");
	if (reg == NULL)
		throw 1;

	(*reg)();
#else
	void *handle = dlopen(file.c_str(), RTLD_LAZY | RTLD_GLOBAL);

	void(*reg)(Configurator &);
	*(void **)(&reg) = dlsym(handle, "registerUnits");
	if (reg == NULL)
		throw 1;

	(*reg)(*this);
#endif
}

Configurator::Option::Option(std::string opt_line)
{
	std::vector<std::string> stack;
	std::string tkn, prv_tkn;

	while ((tkn=getToken(opt_line)).length())
	{
		if (tkn == "{"){
			if (stack.size() != 1)
				throw 1;
		}else if (tkn == "}"){
			if (stack.size() < 2)
				throw 1;
			if (!name.length()){
				name = stack[0];
				stack[0] = "VALUE";
			}
			prms.insert(std::pair<std::string, std::vector<std::string> >(stack[0], std::vector<std::string>(stack.begin()+1, stack.end())));
			stack.clear();
		}else if (tkn == "#"){
			if (stack.size() > 0)
				throw 1;
			break;
		}else if (tkn == "="){
			if (stack.size() != 1)
				throw 1;
		}else{
			stack.push_back(tkn);
			if (prv_tkn == "="){
				if (!name.length()){
					name = stack[0];
					stack[0] = "VALUE";
				}
				prms.insert(std::pair<std::string, std::vector<std::string> >(stack[0], std::vector<std::string>(1, stack[1])));
				stack.clear();
			}
		}

		prv_tkn = tkn;
	}
}

Configurator::Option::Option(const Configurator::Option &opt){
	name = opt.getName();
	for (std::map<std::string, std::vector<std::string> >::const_iterator i=opt.getPrms().begin(); i != opt.getPrms().end(); i++){
		prms.insert(std::pair<std::string, std::vector<std::string> >(i->first, std::vector<std::string>(i->second)));
	}
}

const std::string &Configurator::Option::getName() const
{
	return name;
}

const std::vector<std::string> &Configurator::Option::getValue() const
{
	return getPrm("VALUE");
}


const std::vector<std::string> &Configurator::Option::getPrm(const std::string &name) const
{
	if (prms.find(name) != prms.end())
		return prms.find(name)->second;

	throw 1;
}

const std::map<std::string, std::vector<std::string> > &Configurator::Option::getPrms() const
{
	return prms;
}

const std::string Configurator::Option::ignore_chars = ", \t\n\r";

const std::string Configurator::Option::special_chars = "{}=#";

std::string Configurator::Option::getToken(std::string &buffer)
{
	bool cite = false;
	std::string token;

	while (buffer.length()){
		if (buffer[0] == '"'){
			buffer.erase(0, 1);
			if (cite)
				return token;
			else
				cite = true;
		}else if (!cite && ignore_chars.find(buffer[0]) != std::string::npos){
			if (!token.length())
				buffer.erase(0, 1);
			else
				return token;
		}else if (!cite && special_chars.find(buffer[0]) != std::string::npos){
			if (!token.length()){
				token.push_back(buffer[0]);
				buffer.erase(0, 1);
			}
			
			return token;
		}else{
			token.push_back(buffer[0]);
			buffer.erase(0, 1);
		}
	}

	return token;
}

}
