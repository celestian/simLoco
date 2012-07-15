#ifndef _AREA_PATTERN_H_
#define _AREA_PATTERN_H_

#include <string>
#include <vector>
#include <tuple>

using namespace std;


namespace pval // Patterns of visual area language
{
	const int LEFT_SWITCHES 				=  1;
   	const int LEFT_DECOMP_SWITCHES			=  2;
    const int LEFT_SWITCH_A				=  3;
    const int LEFT_DECOMP_SWITCH_A 		=  4;

	const int RIGHT_SWITCHES 				=  5;
   	const int RIGHT_DECOMP_SWITCHES		=  6;
    const int RIGHT_SWITCH_A				=  7;
    const int RIGHT_DECOMP_SWITCH_A 		=  8;

	const int LEFT_DOWN_SWITCHES 			=  9;
   	const int LEFT_DOWN_DECOMP_SWITCHES	= 10;
    const int LEFT_DOWN_SWITCH_A			= 11;
    const int LEFT_DOWN_DECOMP_SWITCH_A 	= 12;
}

vector<string> addWhiteSpace (const vector<string> & matrixA);
vector<string> copyMatrix (const vector<string> & matrixA);
void printMatrix (const vector<string> & matrix);

std::vector<std::string> makePattern (const int pattern);

vector<string> replacePattern (const vector<string> & matrixA, const int pattern_id_A, const int pattern_id_B, int number);
tuple<vector<string>::size_type, string::size_type>  findPatternXY (const vector<string> & matrix, const int pattern_id);
bool isTherePattern (const vector<string> & matrix, const int pattern_id);

#endif // _AREA_PATTERN_H_
