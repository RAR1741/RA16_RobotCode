#ifndef LOGGER_H__
#define LOGGER_H__

#include <fstream>
#include <string>
#include <vector>

class Logger {
public:
	Logger();
	~Logger();

	bool Open(std::string filenames);
	bool Close();

	bool Reset(); // Close file, start over

	bool AddAttribute(std::string field);
	bool HasAttribute(std::string field);

	bool Log(std::string field, float data);
	bool Log(std::string field, std::string data);

	bool WriteAttributes();
	bool WriteLine();
private:
	int FindField(std::string name);


	// Normalize a string (make it lowercase, etc.)
	std::string Normalize(std::string str);

	std::string filename;

	std::fstream m_log;
	std::vector< std::pair<std::string, std::string> > m_fields;
};

#endif
