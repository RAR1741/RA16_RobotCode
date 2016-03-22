/*
 * EdgeDetect.h
 *
 *  Created on: Mar 1, 2016
 *      Author: taylo
 */

#ifndef SRC_EDGEDETECT_H_
#define SRC_EDGEDETECT_H_

class EdgeDetect
{
public:
	EdgeDetect();
	bool Check(bool in);
	bool Check(int in, int expected);
private:
	bool state;
};

#endif /* SRC_EDGEDETECT_H_ */
