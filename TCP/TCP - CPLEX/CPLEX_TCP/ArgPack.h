#ifndef ARGPACK_H_
#define ARGPACK_H_

#include <string>
#include <assert.h>



class ArgPack {

public:

	//------------
	// program parameters
	//------------

	bool verbose;

	unsigned primal_bound;

	int L;
	
	int R;

	long memory;

	long time;

	std::string input_name, program_name;

	//------------
	// singleton functions
	//------------

	static const ArgPack &ap() { assert(def_ap_); return *def_ap_; }

//	static ArgPack &write_ap() { assert(def_ap_); return *def_ap_; }

	ArgPack(int argc, char * const argv []);

	~ArgPack() { assert(def_ap_); def_ap_ = 0; }

private:

	//------------
	// singleton instance
	//------------

	static ArgPack *def_ap_;

};



#endif /* ARGPACK_H_ */
