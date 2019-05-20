#ifndef __STLSUPPORT_H
#define __STLSUPPORT_H

#include <string>
#include <iostream>
#include <sstream>

template<class num_t, class char_t>
struct NtoT
{
	std::basic_string<char_t> operator() (num_t num) const
	{
		std::basic_ostringstream<char_t> temp;
		temp<<num;
		return temp.str();
	}
};

template <class char_t,class num_t>
struct TtoN
{
	num_t operator()(const std::basic_string<char_t> &str) const
	{
		std::basic_istringstream<char_t> temp(str);
		num_t num;
		temp>>num;

		return num;
	}
};
// usage
// std::string temp=NumToText<double>(0.001);
//
template<class num_t>
std::string NumToText(num_t n)
{
	std::string temp=NtoT<num_t,char>()(n);
	return temp;
}
//  usage
//	std::string temp="0.001";
//	double t=TextToNum<double> (temp);
//

template<class num_t>
num_t TextToNum(std::string &str)
{
	num_t temp=TtoN<char,num_t>()(str);
	return temp;
}
#endif