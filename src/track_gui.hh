#ifndef _TRACK_ELEMENT_GUI_H_
#define _TRACK_ELEMENT_GUI_H_

#include <string>
#include <vector>


class TrackElement {

	public:
	
		TrackElement (const std::string & element);

		
	private:
		
		int id;
		char type;
		std::string title;
		int lenght;

};

#endif // _TRACK_ELEMENT_GUI_H_

