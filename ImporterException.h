#pragma once

#ifndef _IMPORTER_EXCEPTION_H_
#define _IMPORTER_EXCEPTION_H_

#include <exception>

using namespace std;

class ImporterException : public exception
{
private:
	const char* error_message;

public:
	const char* what() const throw() override { return error_message; }

public:
	ImporterException(const char* msg) : error_message(msg)
	{
	}

	~ImporterException() throw()
	{
	}
};

#endif
