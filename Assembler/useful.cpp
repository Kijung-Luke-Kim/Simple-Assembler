#include "useful.h"

auto toBinary(int num)
{
	int result = 0;

	int p = 0;
	while (!(pow(2, p) > num))p++;
	p--;

	for (; p >= 0; p--)
	{
		if (pow(2, p) <= num)
		{
			num -= static_cast<int>(pow(2, p));
			result += static_cast<int>(pow(10, p));
		}
	}
	return result;
}

std::string toBinaryString(int num)
{
	std::string result{ "" };

	int p = 7;

	for (; p >= 0; p--)
	{
		if (pow(2, p) <= num)
		{
			num -= static_cast<int>(pow(2, p));
			result.append("1");
		}
		else
			result.append("0");
	}
	return result;
}

std::string regnum(int n)
{
	std::string ret{ "" };
	switch (n)
	{
	case 0:
		ret += "$zero";
		break;
	case 1:
		ret += "$at";
		break;
	case 2:
	case 3:
		ret += "$v";
		ret += std::to_string(n - 2);
		break;
	case 4:
	case 5:
	case 6:
	case 7:
		ret += "$a";
		ret += std::to_string(n - 4);
		break;
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		ret += "$t";
		ret += std::to_string(n - 8);
		break;
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
		ret += "$s";
		ret += std::to_string(n - 16);
		break;
	case 24:
	case 25:
		ret += "$t";
		ret += std::to_string(n - 24 + 8);
		break;
	case 26:
	case 27:
		ret += "$k";
		ret += std::to_string(n - 26);
		break;
	case 28:
		ret += "$gp";
		break;
	case 29:
		ret += "$sp";
		break;
	case 30:
		ret += "$fp";
		break;
	case 31:
		ret += "$ra";
		break;
	}
	ret += " ";
	return ret;
}

int toHexString(std::string s)
{
	if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
		s = s.substr(2);
	int count = 0;
	int iterate_count = s.length();
	for (int i = 0; i < iterate_count; i++)
	{
		int objnum = 0;
		switch (s[s.length() - 1])
		{
		case '0':
			objnum = 0;
			break;
		case '1':
			objnum = 1;
			break;
		case '2':
			objnum = 2;
			break;
		case '3':
			objnum = 3;
			break;
		case '4':
			objnum = 4;
			break;
		case '5':
			objnum = 5;
			break;
		case '6':
			objnum = 6;
			break;
		case '7':
			objnum = 7;
			break;
		case '8':
			objnum = 8;
			break;
		case '9':
			objnum = 9;
			break;
		case 'a':
		case 'A':
			objnum = 10;
			break;
		case 'b':
		case 'B':
			objnum = 11;
			break;
		case 'c':
		case 'C':
			objnum = 12;
			break;
		case 'd':
		case 'D':
			objnum = 13;
			break;
		case 'e':
		case 'E':
			objnum = 14;
			break;
		case 'f':
		case 'F':
			objnum = 15;
			break;
		default:
			std::cerr << "Error From toHexString(): Input error\n";
			return 0;
			break;
		}
		count += objnum * pow(16, i);
		s = s.substr(0, s.length() - 1);
	}
	return count;
}
