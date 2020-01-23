
#ifndef LOTTO_UTILITIES_H
#define LOTTO_UTILITIES_H

#include <string>
#include "basic_types.h"

std::string convert_ruota_to_string(ruota_t ruota);

std::string my_str_toupper(std::string s);

ruota_t convert_string_to_ruota(std::string ruota_name);

std::string convert_mese_to_string(mese_t mese);

mese_t convert_string_to_mese(std::string mese_name);


#endif // LOTTO_UTILITIES_H
