#include "ArgPack.h"


#include <string>
#include <cstring>
#include <unistd.h> // for getopt
#include <iostream>
#include <stdlib.h> 
#include <sstream>

extern int optind;



namespace my
{
    template < typename T > std::string to_string( const T& v )
    {
        std::ostringstream stm ;
        return ( stm << v ) ? stm.str() : "{*** error ***}" ;
    }
}


using namespace std;




ArgPack *ArgPack::def_ap_ = 0;

ArgPack::ArgPack(int argc, char * const argv []) :
	verbose(true),
	primal_bound(0),
	L(0),
	R(0),
	memory(0),
	time(0)
{

	assert(!def_ap_);
	def_ap_ = this;
	program_name = argv[0];

	string usage = string("Usage: ") + program_name + " [options] <file>\n" +
	               "Compile time: " + __DATE__ + " " + __TIME__ + "\n" +
	               "	-h			: show this help\n" +
	               "	-b<bound>		: primal (lower) bound [default: no bound] \n" +
                   "	-L<routers>		: L value [default: " + my::to_string(L) + "] \n" +
                   "	-R<routers>		: R value [default: " + my::to_string(R) + "] \n" +
                   "	-m<memory>		: memory limit m [default: no memory limit] \n" +
                   "	-t<time>		: time limit t [default: no time limit] \n" +
	               "	-v			: disable verbose mode \n";
//	               "	-o<output>      : output solution file [default: " + output_name + "]\n" +
	string help = "Use -h for more information\n";

	const char *opt_str = "hb:vL:R:t:m:";

	long ch;

	while ((ch = getopt(argc, argv, opt_str)) != -1)
	{
		switch (ch)
		{
		case 'h':
			throw usage;
		case 'b':
			primal_bound = strtoul(optarg, NULL, 10);
			break;
		case 't':
			time = strtoul(optarg, NULL, 10);
			break;
		case 'L':
			L = stoi(optarg, NULL);
			break;
		case 'R':
			R = stoi(optarg, NULL);
			break;
		case 'm':
			memory = strtoul(optarg, NULL, 10);
			break;
		case 'v':
			verbose = false;
			break;
		case '?':
			throw string(help);
		}
	}
	argc -= optind;
	argv += optind;

	if (argc > 1)
	{
		throw string("Too many arguments\n" + help);
	}
	else if (argc < 1)
	{
		throw string("Too few arguments\n" + help );
	}
	else
	{
		input_name = argv[0];
	}
}


