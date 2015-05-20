#ifndef DKK_EXCEPTION_H
#define DKK_EXCEPTION_H

#include <exception>
#include <string.h>
#include "WindowsExport.h"

#define DKK_LIMIT_EXCEPTION_LEN 120

namespace DKK{

class Exception : public std::exception{
protected:
	char message[DKK_LIMIT_EXCEPTION_LEN];
public:
	Exception(const char *message)
	{
		strcpy(this->message,message);
	}

	const char *what() const throw()
	{
		return message;
	}
};

}

#endif
