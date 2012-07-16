#ifndef _GUI_TRACK_H_
#define _GUI_TRACK_H_

#include <cluttermm.h>


class TrackGUI {

	public:
	
		TrackGUI (int _id, float _x, float _y);

		
	protected:
		
		int id;
		float x;
		float y;
		
};


class RailGUI : public TrackGUI {

	public:
		
		RailGUI (int _id, float _x, float _y, int _xsize);
		void show (const Glib::RefPtr<Clutter::Stage> stage);
	
	private:
	
		int xsize;
};


class JunctionGUI : public TrackGUI {

	public:
		
		JunctionGUI (int _id, float _x, float _y);
		void show (const Glib::RefPtr<Clutter::Stage> stage);
		
};

#endif // _GUI_TRACK_H_

