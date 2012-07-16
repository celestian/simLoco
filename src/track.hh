#ifndef _TRACK_H_
#define _TRACK_H_

#include <string>


class Track {

	public:
		
		Track (const std::string & descp);
		virtual void print() {;};

	protected:
	
		int id;
		int type;
		int lenght;
		
		int back_left;
		int back_direct;
		int back_right;

		int forward_left;
		int forward_direct;
		int forward_right;

};


class Rail : public Track {

	public:
	
		Rail (const std::string & descp);
		virtual void print();
		
		
	private:


};


class Junction : public Track {

	public:
	
		Junction (const std::string & descp);
		virtual void print();
	
	
	private:


};

#endif // _TRACK_H_

