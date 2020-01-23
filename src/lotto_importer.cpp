//============================================================================
// Name        : lotto_importer.cpp
// Author      : F. Strati
// Version     :
// Copyright   : Apache License v.2.0
// Description : Import Lotto ascii database and export to custom binary file
//============================================================================

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <errno.h>
#include <cstdlib>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include "basic_types.h"
#include "utilities.h"

#define LOTTO_START_YEAR   (1871)
#define LOTTO_END_YEAR     (2020)

std::vector<std::string> parse_arguments(int argc, char *argv[]);
void print_usage(int argc, char *argv[]);
int32_t process_all_files(const boost::filesystem::path& file_db, uint32_t start_year, uint32_t end_year);
int32_t process_file(std::vector<extraction_t>& extraction_vec, uint32_t year);
uint64_t convert_number_non_zero(std::string number_str);
int32_t save_file_db(const std::vector<extraction_t>& extraction_vec, const boost::filesystem::path& file_db);
int32_t verify_file_db(const std::vector<extraction_t>& extraction_vec, const boost::filesystem::path& file_db);

int main(int argc, char *argv[])
{
	std::cout << "!!! this is lotto_importer !!!" << std::endl;

	// check arguments
	if( 4 != argc )
	{
		print_usage(argc, argv);
		return -1;
	}

    std::vector<std::string> arguments = parse_arguments(argc, argv);

	char *end = NULL;
    uint32_t start_year = std::strtoul(argv[1], &end, 10);
    if( end == argv[1] )
    {
		print_usage(argc, argv);
		return -1;
    }
	end = NULL;
    uint32_t end_year = std::strtoul(argv[2], &end, 10);
    if( end == argv[2] )
    {
		print_usage(argc, argv);
		return -1;
    }

    // check valid years
    if( start_year < LOTTO_START_YEAR || start_year > LOTTO_END_YEAR )
    {
		std::cout << "Error! start year out of bounds, start year = " << start_year << \
				" lower bound = " << LOTTO_START_YEAR << \
				" upper bound = " << LOTTO_END_YEAR   << "." << std::endl;
		print_usage(argc, argv);
		return -1;
    }
    if( end_year < LOTTO_START_YEAR || end_year > LOTTO_END_YEAR )
    {
		std::cout << "Error! start year out of bounds, end year = " << end_year << \
				" lower bound = " << LOTTO_START_YEAR << \
				" upper bound = " << LOTTO_END_YEAR   << "." << std::endl;
		print_usage(argc, argv);
		return -1;
    }

    // check valid path
    boost::filesystem::path p(boost::filesystem::current_path());
    p /= boost::filesystem::path(arguments[3]);
    if(boost::filesystem::exists(p))
    {
    	if(!boost::filesystem::is_regular_file(p))
    	{
    		std::cout << "Error! file " << arguments[3] << \
    				" does exist and is not a regular file." << std::endl;
    		print_usage(argc, argv);
    		return -1;
    	}
    	else
    	{
    		std::cout << "Error! file " << arguments[3] << \
    				" does exist and is a regular file." << std::endl;
    		print_usage(argc, argv);
    		return -1;
    	}
    }

    std::cout << "Processing with following info:" << std::endl;
    std::cout << "path to db: " << p.c_str() << std::endl;
    std::cout << "start year: " << start_year << std::endl;
    std::cout << "end   year: " << end_year << std::endl;

    int32_t ret = process_all_files(p, start_year, end_year);
    if(ret)
    {
		std::cout << "Error! from file processing, abort." << std::endl;
    }

    return ret;
}

std::vector<std::string> parse_arguments(int argc, char *argv[])
{
    std::vector<std::string> arguments;

    for(int i = 0; i < argc; i++)
    {
    	arguments.push_back(std::string(argv[i]));
    }

    return arguments;
}

void print_usage(int argc, char *argv[])
{
	std::cout << "Usage: " << std::string(argv[0]) << \
			" start_year (" << LOTTO_START_YEAR << "-" << LOTTO_END_YEAR << ") " << \
			"end_year   (" << LOTTO_START_YEAR << "-" << LOTTO_END_YEAR << ") file_output.db" << std::endl;
}

int32_t process_all_files(const boost::filesystem::path& file_db, uint32_t start_year, uint32_t end_year)
{
	std::vector<extraction_t> extraction_vec;

	for(uint32_t i = start_year; i <= end_year; i++)
	{
		std::cout << "... processing year: " << i << std::endl;
		int32_t ret = process_file(extraction_vec, i);
		if(ret)
		{
			std::cout << "Error from year: " << i << " abort." << std::endl;
			return ret;
		}
	}

	// save file db
	int32_t ret = save_file_db(extraction_vec, file_db);
	if(ret)
	{
		std::cout << "Error from save_file_db." << " abort." << std::endl;
		return ret;
	}

	// verify file db
	ret = verify_file_db(extraction_vec, file_db);
	if(ret)
	{
		std::cout << "Error from verify_file_db." << " abort." << std::endl;
		return ret;
	}

	return ret;
}

int32_t process_file(std::vector<extraction_t>& extraction_vec, uint32_t year)
{
	char year_cstr[256];
	std::sprintf(year_cstr,"%04u.txt",year);
	std::string filename = std::string(year_cstr);

	// check valid file exists
    boost::filesystem::path p(boost::filesystem::current_path());
    p /= boost::filesystem::path(filename);
    if(boost::filesystem::exists(p) && boost::filesystem::is_regular_file(p))
    {
		std::cout << "... found file: " << filename << std::endl;
    }
    else
    {
		std::cout << "Error: not found file: " << filename << std::endl;
    	return -1;
    }

    // open the file
    std::ifstream infile;
    infile.open(p.c_str());

    // read header
    std::string header;
    if(infile.is_open())
    {
    	std::getline(infile,header);
    }
    else
    {
		std::cout << "Error: could not open file: " << filename << std::endl;
		infile.close();
    	return -1;
    }

    // parse the header
    std::vector<ruota_t> current_ruote;
    typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
    boost::char_separator<char> sep{" "};
    tokenizer tok{header, sep};
    bool is_first = true;
    for (const auto &t : tok)
    {
    	// first the year
    	if(is_first)
    	{
			char *end = NULL;
		    uint32_t current_year = std::strtoul(t.c_str(), &end, 10);
		    if( end == t.c_str() )
		    {
		    	std::cout << "Error: not a valid current year: " << t.c_str() << std::endl;
		    	infile.close();
		    	return -1;
		    }
		    if( current_year != year )
		    {
		    	std::cout << "Error: current year: " << current_year << std::endl;
		    	std::cout << "Error: asked   year: " << year << std::endl;
		    	infile.close();
		    	return -1;
		    }
		    is_first = false;
    	}
    	else
    	{
    		ruota_t ruota = convert_string_to_ruota(std::string(t.c_str()));
    		if( ruota_t::UNKNOWN != ruota )
    		{
    			current_ruote.push_back(ruota);
    		}
    		else
    		{
    			char *end = NULL;
    		    uint32_t current_year = std::strtoul(t.c_str(), &end, 10);
    		    if( end == t.c_str() )
    		    {
    		    	std::cout << "Error: not a valid current year: " << t.c_str() << std::endl;
    		    	infile.close();
    		    	return -1;
    		    }
    		    if( current_year != year )
    		    {
    		    	std::cout << "Error: current year: " << current_year << std::endl;
    		    	std::cout << "Error: asked   year: " << year << std::endl;
    		    	infile.close();
    		    	return -1;
    		    }
    		}
    	}
    }

    // parse all the records
    uint32_t line_counter = 0;
    bool do_exit = false;
	while(!infile.eof())
	{
		line_counter++;
		std::string record;
    	std::getline(infile,record);
	    boost::char_separator<char> sep{" "};
	    tokenizer tok_record{record, sep};
	    bool is_first_record = true;
	    bool is_second_record = false;
	    uint64_t current_day = 0;
	    mese_t   current_month = mese_t::NULL_MESE;
	    std::vector<ruota_t>::const_iterator current_r_iter = current_ruote.begin();
	    uint64_t a_num = 0, b_num = 0, c_num = 0, d_num = 0, e_num = 0;
	    uint32_t current_number_iter = 1;
	    size_t tok_size = 0;
	    for (const auto &t : tok_record)
	    {
	    	(void) t;
	    	tok_size++;
	    }
	    // check record well formed
		if( tok_size == 1 && std::string("END") == std::string(*(tok_record.begin())) )
		{
			std::cout << "End of file at line " << line_counter << std::endl;
			break;
		}
	    if( tok_size != 2 + (5*current_ruote.size()) + 1 )
	    {
			std::cout << "Error: ill formed record at line " << line_counter << std::endl;
			std::cout << "tok_size " << tok_size << " requested " << (2 + (5*current_ruote.size()) + 1) << std::endl;
			break;
	    }
	    for (const auto &t : tok_record)
	    {
	    	if(is_first_record)
	    	{
	    		if( std::string("END") == std::string(t.c_str()) )
	    		{
	    			std::cout << "End of file at line " << line_counter << std::endl;
	    			do_exit = true;
	    			break;
	    		}
	    		current_day = convert_number_non_zero(std::string(t.c_str()));
	    		if( 0 == current_day || ( current_day < 1 || current_day > 31 ) )
	    		{
	    			std::cout << "Error at line: " << line_counter << " current_day " << current_day << std::endl;
	    			infile.close();
	    			return -1;
	    		}
	    		is_first_record = false;
	    		is_second_record = true;
	    		continue;
	    	}
	    	if(is_second_record)
	    	{
	    		current_month = convert_string_to_mese(std::string(t.c_str()));
	    		if( mese_t::NULL_MESE == current_month )
	    		{
	    			std::cout << "Error: invalid month " << t.c_str() << std::endl;
	    			std::cout << "Error at line: " << line_counter << std::endl;
	    			infile.close();
	    			return -1;
	    		}
	    		is_second_record = false;
	    		continue;
	    	}
	    	// parse the ruote numbers
	    	uint64_t c_number = 0;
	    	if( std::string("--") != std::string(t.c_str()) )
	    	{
		    	c_number = convert_number_non_zero(std::string(t.c_str()));
	    		if( 0 == c_number || ( c_number < 1 || c_number > 90 ) )
	    		{
	    			std::cout << "Error at line: " << line_counter << " c_number " << c_number << std::endl;
	    			infile.close();
	    			return -1;
	    		}
	    	}
	    	switch (current_number_iter)
	    	{
				case 1:
					a_num = c_number;
					break;
				case 2:
					b_num = c_number;
					break;
				case 3:
					c_num = c_number;
					break;
				case 4:
					d_num = c_number;
					break;
				case 5:
					e_num = c_number;
					break;
			}
	    	current_number_iter++;
	    	if( current_number_iter > 5 )
	    	{
	    		// add ruota extraction
	    		if( a_num != 0 )
	    		{
	    			extraction_t ex;
	    			ex.bits.a = a_num;
	    			ex.bits.b = b_num;
	    			ex.bits.c = c_num;
	    			ex.bits.d = d_num;
	    			ex.bits.e = e_num;
	    			ex.bits.ruota = *current_r_iter;
	    			ex.bits.year = (uint64_t) year;
	    			ex.bits.month = (uint64_t) current_month;
	    			ex.bits.day = current_day;
	    			extraction_vec.push_back(ex);
	    		}
	    		current_number_iter = 1;
	    		current_r_iter++;
	    		if( current_r_iter == current_ruote.end() )
	    		{
	    			break;
	    		}
	    	}
	    } // for (const auto &t : tok_record)
	    if(do_exit)
	    	break;
	}
	infile.close();

    return 0;
}

uint64_t convert_number_non_zero(std::string number_str)
{
	if( number_str.size() != 2 )
		return 0;

	if( '0' == number_str[0] )
	{
		number_str.erase(number_str.begin());
		// std::cout << "info number with leading 0: " << number_str << std::endl;
	}

	char *end = NULL;
	const char *start = number_str.c_str();
	uint64_t number_conv = std::strtoul(start, &end, 10);
    if( end == start || 0 == number_conv )
    {
		std::cout << "Error: invalid number: " << number_str << std::endl;
		return 0;
    }

    return number_conv;
}

int32_t save_file_db(const std::vector<extraction_t>& extraction_vec, const boost::filesystem::path& file_db)
{
    // open the file
	std::FILE *write_ptr = std::fopen(file_db.c_str(),"wb");

	if( NULL != write_ptr )
	{
		uint8_t value_64bit[8] = { 0 };
    	for( const auto& e : extraction_vec )
    	{
    		std::memset(value_64bit, 0, sizeof(value_64bit));

    		value_64bit[0] = (uint8_t) ((e.raw >> 56) & 0xFF);
    		value_64bit[1] = (uint8_t) ((e.raw >> 48) & 0xFF);
    		value_64bit[2] = (uint8_t) ((e.raw >> 40) & 0xFF);
    		value_64bit[3] = (uint8_t) ((e.raw >> 32) & 0xFF);
    		value_64bit[4] = (uint8_t) ((e.raw >> 24) & 0xFF);
    		value_64bit[5] = (uint8_t) ((e.raw >> 16) & 0xFF);
    		value_64bit[6] = (uint8_t) ((e.raw >>  8) & 0xFF);
    		value_64bit[7] = (uint8_t) ((e.raw >>  0) & 0xFF);

    		std::fwrite(value_64bit,sizeof(value_64bit),1,write_ptr);
    	}
    	std::fclose(write_ptr);
	}
	else
	{
		std::cout << "Error: could not open file " << file_db.c_str() << std::endl;
		return -1;
	}

    return 0;
}

int32_t verify_file_db(const std::vector<extraction_t>& extraction_vec, const boost::filesystem::path& file_db)
{
    if(! (boost::filesystem::exists(file_db) && boost::filesystem::is_regular_file(file_db)) )
    {
		std::cout << "Error! file " << file_db.c_str() << \
				" does not exist or is not a regular file." << std::endl;
		return -1;
    }

    // open the file
	std::FILE *read_ptr = std::fopen(file_db.c_str(),"rb");

	if( NULL != read_ptr )
	{
    	uint64_t current_ex = 0;
    	size_t current_ex_kounter = 0;
		uint8_t value_64bit[8] = { 0 };
    	for( const auto& e : extraction_vec )
    	{
    		current_ex = 0;
    		current_ex_kounter++;

    		std::memset(value_64bit, 0, sizeof(value_64bit));
    		size_t ret = std::fread(value_64bit,sizeof(value_64bit),1,read_ptr);
    		if( ret < 1 )
    		{
				std::cout << "Error! inconsistent read. abort." << std::endl;
				std::fclose(read_ptr);
				return -1;
    		}

    		current_ex  = (uint64_t) ( (((uint64_t) value_64bit[0]) << 56) | \
    				                   (((uint64_t) value_64bit[1]) << 48) | \
    						           (((uint64_t) value_64bit[2]) << 40) | \
    								   (((uint64_t) value_64bit[3]) << 32) | \
    								   (((uint64_t) value_64bit[4]) << 24) | \
    								   (((uint64_t) value_64bit[5]) << 16) | \
    								   (((uint64_t) value_64bit[6]) <<  8) | \
    								   (((uint64_t) value_64bit[7]) <<  0) );

			if( current_ex != e.raw )
			{
				std::cout << "Error! inconsistent extraction found from file " << file_db.c_str() << std::endl;
				std::cout << "Extraction number " << current_ex_kounter << std::endl;
				std::cout << "Expected:" << std::endl;
				std::cout << "   e.bits.year:  " << e.bits.year << std::endl;
				std::cout << "   e.bits.month: " << e.bits.month << std::endl;
				std::cout << "   e.bits.day:   " << e.bits.day << std::endl;
				std::cout << "   e.bits.a:     " << e.bits.a << std::endl;
				std::cout << "   e.bits.b:     " << e.bits.b << std::endl;
				std::cout << "   e.bits.c:     " << e.bits.c << std::endl;
				std::cout << "   e.bits.d:     " << e.bits.d << std::endl;
				std::cout << "   e.bits.e:     " << e.bits.e << std::endl;
				std::cout << "   e.bits.ruota: " << e.bits.ruota << std::endl;

				extraction_t ex_found;
				ex_found.raw = current_ex;
				std::cout << "Found:" << std::endl;
				std::cout << "   e.bits.year:  " << ex_found.bits.year << std::endl;
				std::cout << "   e.bits.month: " << ex_found.bits.month << std::endl;
				std::cout << "   e.bits.day:   " << ex_found.bits.day << std::endl;
				std::cout << "   e.bits.a:     " << ex_found.bits.a << std::endl;
				std::cout << "   e.bits.b:     " << ex_found.bits.b << std::endl;
				std::cout << "   e.bits.c:     " << ex_found.bits.c << std::endl;
				std::cout << "   e.bits.d:     " << ex_found.bits.d << std::endl;
				std::cout << "   e.bits.e:     " << ex_found.bits.e << std::endl;
				std::cout << "   e.bits.ruota: " << ex_found.bits.ruota << std::endl;

				std::fclose(read_ptr);
				return -1;
			}
    	}
    	std::fclose(read_ptr);
	}
	else
	{
		std::cout << "Error: could not open file " << file_db.c_str() << std::endl;
		return -1;
	}

	return 0;
}
