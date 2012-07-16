#include "gui_track.hh"

TrackGUI::TrackGUI (int _id, float _x, float _y) {

	id = _id;
	x = _x;
	y = _y;

};


RailGUI::RailGUI (int _id, float _x, float _y, int _xsize) : TrackGUI (_id, _x, _y) {

	xsize = _xsize;

}


void RailGUI::show (const Glib::RefPtr<Clutter::Stage> stage) {

	Clutter::Color actor_color (0x99, 0x99, 0x99, 0xff);

	// Add a rectangle to the stage:
	const Glib::RefPtr<Clutter::Rectangle> rect1 = Clutter::Rectangle::create(actor_color);
	rect1->set_size(xsize-2, 2);
	rect1->set_position(x+1, y);
	stage->add_actor(rect1);
	rect1->show();

}


JunctionGUI::JunctionGUI (int _id, float _x, float _y) : TrackGUI (_id, _x, _y) {

	;

}


void JunctionGUI::show (const Glib::RefPtr<Clutter::Stage> stage) {

	Clutter::Color actor_color (0x99, 0x99, 0x99, 0xff);

	// Add a rectangle to the stage:
	const Glib::RefPtr<Clutter::Rectangle> rect1 = Clutter::Rectangle::create(actor_color);
	rect1->set_size(38, 2);
	rect1->set_position(x+1, y);
	stage->add_actor(rect1);
	rect1->show();

	const Glib::RefPtr<Clutter::Rectangle> rect2 = Clutter::Rectangle::create(actor_color);
	rect2->set_size(20, 2);
	rect2->set_position(x+19, y);
	rect2->set_rotation(Clutter::Z_AXIS, 45, 0, 0, 0);
	stage->add_actor(rect2);
	rect2->show();

}
