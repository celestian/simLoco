# what flags you want to pass to the C compiler & linker
AM_CFLAGS = --pedantic -Wall
AM_CXXFLAGS = --pedantic -Wall -std=c++0x
AM_LDFLAGS = -lm

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = drive area
drive_SOURCES = main_drive.cc rail_route.hh rail_route.cc drive.hh drive.cc
area_SOURCES = main_area.cc area_pattern.hh area_pattern.cc track.hh track.cc area.hh area.cc
