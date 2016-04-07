/*
 * Dashboard.h
 *
 *  Created on: Mar 24, 2016
 *      Author: taylo
 */

#ifndef SRC_DASHBOARD_H_
#define SRC_DASHBOARD_H_

using namespace std;

class Dashboard
{
public:
	Dashboard();
	void WriteBool(string name, bool write);
	void WriteString(string name, string write);
	void WriteDouble(string name, double write);
	bool ReadBool(string name, bool rDefault);
	string ReadString(string name, string rDefault);
	double ReadDouble(string number, double rDefault);
	bool CheckTrigger(string name);
private:
	std::shared_ptr<NetworkTable> station;
};

#endif /* SRC_DASHBOARD_H_ */
