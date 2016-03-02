/*
 * EdgeDetect.cpp
 *
 *  Created on: Mar 1, 2016
 *      Author: taylo
 */

EdgeDetect::EdgeDetect()
{
	state = false;
}

bool EdgeDetect::Check(bool in)
{
	bool out;
	if(in && state)
	{
		out = true;
	}
	else
	{
		out = false;
	}

	if(in)
	{
		state = false;
	}
	else
	{
		state = true;
	}
	return out;
}
