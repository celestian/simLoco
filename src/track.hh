#ifndef _TRACK_H_
#define _TRACK_H_

#include <string>
#include <vector>
#include <iostream>


class Track {

	public:
	
		Track (const std::string & descp);
		int getId () { return id; };
		void print () {	 printf("%s\t|  %i\t%c\t%i\t%i\t%i\t%i\t%i\t%i\t%i\n", title.c_str(), id, type, lenght, back_left, back_direct, back_right, forward_left, forward_direct, forward_right); };
		
		int back_left;
		int	back_direct;
		int	back_right;

		int	forward_left;
		int	forward_direct;
		int forward_right;


		
	private:
		
		int id;
		char type;
		std::string title;
		int lenght;



};

#endif // _TRACK_H_

