# what flags you want to pass to the C compiler & linker
AM_CFLAGS = --pedantic -Wall
AM_CXXFLAGS = --pedantic -Wall -std=c++0x
AM_LDFLAGS = -lm

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = drive area area_utils

drive_SOURCES = main_drive.cc rail_route.hh rail_route.cc drive.hh drive.cc

area_SOURCES = main_area.cc track.hh track.cc gui_track.hh gui_track.cc
area_CPPFLAGS = $(CLUTTERMM_CFLAGS) $(CLUTTER_GTKMM_CFLAGS)
area_LDADD = $(CLUTTERMM_LIBS) $(CLUTTER_GTKMM_LIBS)

area_utils_SOURCES = main_area_utils.cc area_pattern.hh area_pattern.cc

