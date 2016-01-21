#include "Logger.h"
#include <cstdlib>
#include <algorithm>
#include <sstream>

Logger::Logger()
{

}

Logger::~Logger() {

}

bool Logger::Open(std::string filename) {
	this->filename = filename;
	m_log.open(filename, std::ios_base::out);
	return m_log.is_open() && m_log.good();
}

bool Logger::Close() {
	m_log.close();
	return !m_log.is_open();
}

bool Logger::Reset() {
	Close();
	Open(this->filename);
	WriteAttributes();
	return true;
}

bool Logger::HasAttribute(std::string name) {
	return (FindField(name) != -1);
}

// TODO: needs some serious optimization most likely
int Logger::FindField(std::string name) {
	std::string real_name = Normalize(name);
	for (unsigned int i = 0; i < m_fields.size(); ++i) {
		if (real_name == m_fields[i].first) {
			return i;
		}
	}
	return -1;
}


bool Logger::AddAttribute(std::string field) {
	if (HasAttribute(field)) {
		// TODO: Output warning
		return false; // We already have this attribute
	}

	std::pair<std::string, std::string> item;
	item.first = Normalize(field);
	item.second = "";

	m_fields.push_back(item);

	return true;
}

bool Logger::Log(std::string field, float data) {
	std::ostringstream ostr;
	ostr << data;
	std::string str(ostr.str());
	return Log(field, str);
}

bool Logger::Log(std::string field, std::string data) {
	std::string converted_field = Normalize(field);
	int idx = FindField(converted_field);
	if (idx < 0) return false;

	m_fields[idx].second = data;
	return true;
}

bool Logger::WriteAttributes() {
	for (unsigned int i = 0; i < m_fields.size(); ++i) {
		m_log << m_fields[i].first << ',';
	}
	m_log << std::endl; // use \n if this is too slow
	return m_log.good();
}

bool Logger::WriteLine() {
	for (unsigned int i = 0; i < m_fields.size(); ++i) {
		m_log << m_fields[i].second << ',';
	}
	m_log << std::endl;
	return m_log.good();
}

std::string Logger::Normalize(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(), tolower);
	return str;
}
