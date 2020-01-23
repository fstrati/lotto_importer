/*
 * utilities.cpp
 *
 *  Created on: Jan 18, 2020
 *      Author: fstrati
 */

#include <algorithm>
#include <string>
#include "basic_types.h"

std::string convert_ruota_to_string(ruota_t ruota)
{
	std::string res;

	switch (ruota)
	{
		case ruota_t::NAZIONALE:
			res = "NAZIONALE";
			break;
		case ruota_t::BARI:
			res = "BARI";
			break;
		case ruota_t::CAGLIARI:
			res = "CAGLIARI";
			break;
		case ruota_t::FIRENZE:
			res = "FIRENZE";
			break;
		case ruota_t::GENOVA:
			res = "GENOVA";
			break;
		case ruota_t::MILANO:
			res = "MILANO";
			break;
		case ruota_t::NAPOLI:
			res = "NAPOLI";
			break;
		case ruota_t::PALERMO:
			res = "PALERMO";
			break;
		case ruota_t::ROMA:
			res = "ROMA";
			break;
		case ruota_t::TORINO:
			res = "TORINO";
			break;
		case ruota_t::VENEZIA:
			res = "VENEZIA";
			break;
		case ruota_t::TUTTE:
			res = "TUTTE";
			break;
		default:
			res = "UNKNOWN";
			break;
	}

	return res;
}

std::string my_str_toupper(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::toupper(c); }
                  );
    return s;
}

ruota_t convert_string_to_ruota(std::string ruota_name)
{
	ruota_t res;

	std::string upper_name = my_str_toupper(ruota_name);

	if(std::string("NAZIONALE") == upper_name)
	{
		res = ruota_t::NAZIONALE;
	}
	else if(std::string("BARI") == upper_name)
	{
		res = ruota_t::BARI;
	}
	else if(std::string("CAGLIARI") == upper_name)
	{
		res = ruota_t::CAGLIARI;
	}
	else if(std::string("FIRENZE") == upper_name)
	{
		res = ruota_t::FIRENZE;
	}
	else if(std::string("GENOVA") == upper_name)
	{
		res = ruota_t::GENOVA;
	}
	else if(std::string("MILANO") == upper_name)
	{
		res = ruota_t::MILANO;
	}
	else if(std::string("NAPOLI") == upper_name)
	{
		res = ruota_t::NAPOLI;
	}
	else if(std::string("PALERMO") == upper_name)
	{
		res = ruota_t::PALERMO;
	}
	else if(std::string("ROMA") == upper_name)
	{
		res = ruota_t::ROMA;
	}
	else if(std::string("TORINO") == upper_name)
	{
		res = ruota_t::TORINO;
	}
	else if(std::string("VENEZIA") == upper_name)
	{
		res = ruota_t::VENEZIA;
	}
	else if(std::string("TUTTE") == upper_name)
	{
		res = ruota_t::TUTTE;
	}
	else
	{
		res = ruota_t::UNKNOWN;
	}

	return res;
}

std::string convert_mese_to_string(mese_t mese)
{
	std::string res;

	switch (mese)
	{
		case mese_t::GEN:
			res = "GEN";
			break;
		case mese_t::FEB:
			res = "FEB";
			break;
		case mese_t::MAR:
			res = "MAR";
			break;
		case mese_t::APR:
			res = "APR";
			break;
		case mese_t::MAG:
			res = "MAG";
			break;
		case mese_t::GIU:
			res = "GIU";
			break;
		case mese_t::LUG:
			res = "LUG";
			break;
		case mese_t::AGO:
			res = "AGO";
			break;
		case mese_t::SET:
			res = "SET";
			break;
		case mese_t::OTT:
			res = "OTT";
			break;
		case mese_t::NOV:
			res = "NOV";
			break;
		case mese_t::DIC:
			res = "DIC";
			break;
		default:
			res = "UNKNOWN";
			break;
	}

	return res;
}

mese_t convert_string_to_mese(std::string mese_name)
{
	mese_t res;

	std::string upper_name = my_str_toupper(mese_name);

	if(std::string("GEN") == upper_name)
	{
		res = mese_t::GEN;
	}
	else if(std::string("FEB") == upper_name)
	{
		res = mese_t::FEB;
	}
	else if(std::string("MAR") == upper_name)
	{
		res = mese_t::MAR;
	}
	else if(std::string("APR") == upper_name)
	{
		res = mese_t::APR;
	}
	else if(std::string("MAG") == upper_name)
	{
		res = mese_t::MAG;
	}
	else if(std::string("GIU") == upper_name)
	{
		res = mese_t::GIU;
	}
	else if(std::string("LUG") == upper_name)
	{
		res = mese_t::LUG;
	}
	else if(std::string("AGO") == upper_name)
	{
		res = mese_t::AGO;
	}
	else if(std::string("SET") == upper_name)
	{
		res = mese_t::SET;
	}
	else if(std::string("OTT") == upper_name)
	{
		res = mese_t::OTT;
	}
	else if(std::string("NOV") == upper_name)
	{
		res = mese_t::NOV;
	}
	else if(std::string("DIC") == upper_name)
	{
		res = mese_t::DIC;
	}
	else
	{
		res = mese_t::NULL_MESE;
	}

	return res;
}

