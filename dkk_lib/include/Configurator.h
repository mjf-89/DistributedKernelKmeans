#ifndef DKK_CONFIGURATOR_H
#define DKK_CONFIGURATOR_H

#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "WindowsExport.h"
#include "Unit.h"
#include "Reader.h"
#include "Kernel.h"
#include "Worker.h"

namespace DKK{

class DKK_EXPORT Configurator{
public:
	class Option{
	private:
		//chars to be trimmed while parsing the option line
		static const std::string ignore_chars;
		//special chars to be treated as tokens while parsing the option line
		static const std::string special_chars;
		//return the first token appearing in the buffer
		static std::string getToken(std::string &buffer);

		//option name and parameters
		std::string name;
		std::map<std::string, std::vector<std::string> > prms;
	public:
		/* parse an option line building the parameters map
		** the option line should have the following format:
		**
		** OPT_NAME=opt_value PRM_NAME=prm_value
		**
		** where both opt_value and prm_value can be a list of values in the form {"val1" ... "val_n"}
		*/
		Option(std::string opt_line);

		//copy constructor
		Option(const Option &opt);

		const std::string &getName() const;

		//equivalent to getPrm("VALUE")
		const std::vector<std::string> &getValue() const;
		const std::vector<std::string> &getPrm(const std::string &name) const;
		const std::map<std::string, std::vector<std::string> > &getPrms() const;
	};

	Configurator(std::string conf_file_name);

	Option &getOption(std::string name);

	static void registerUnit(Unit *unit);
	static Unit & getUnit(const std::string &name);
	static void registerWorker(Worker *worker);
	static Worker & getWorker(const std::string &name);

	void loadPlugins();
	Reader &getReader();
	Kernel &getKernel();

private:
	static std::map<std::string, Unit*> units;
	static std::map<std::string, Worker*> workers;

	std::ifstream conf_file;
	std::map<std::string, Option> opts;

	void loadPluginFile(const std::string &file);
	Unit &getUnitFromOption(Option &opt);
};

}

#endif