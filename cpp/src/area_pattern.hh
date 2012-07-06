#ifndef _AREA_PATTERN_H_
#define _AREA_PATTERN_H_

#include <string>
#include <vector>


namespace pval // Patterns of visual area language
{
	const int LEFT_SWITCHES 			= 1;
   	const int LEFT_DECOMP_SWITCHES		= 2;
    const int LEFT_SWITCH_A			= 3;
    const int LEFT_DECOMP_SWITCH_A 	= 4;

	const int RIGHT_SWITCHES 			= 5;
   	const int RIGHT_DECOMP_SWITCHES	= 6;
    const int RIGHT_SWITCH_A			= 7;
    const int RIGHT_DECOMP_SWITCH_A 	= 8;

    const int LEFT_SWITCH_B			= 9;
}


std::vector<std::string> make_pattern (const int pattern);


#endif // _AREA_PATTERN_H_
