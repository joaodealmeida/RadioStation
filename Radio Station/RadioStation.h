//
//  RadioStation.h
//  Radio Station
//
//  T3G12 - MIEIC 2012/2013
//  Eduardo Almeida, Joao Almeida and Joao Ramos
//
//  This class takes care of the main Radio Station.
//

#ifndef RadioStationHeader
#define RadioStationHeader

#include <iostream>
#include <vector>
#include <string>

#include <ctime>

#include "Music.h"
#include "Playlist.h"

class RadioStation {
    
private:
    
	RadioStation(); // There can only be one radio station, so a singleton here is completely appropriate.

	RadioStation(RadioStation const&); // Copy constructor is private.
	RadioStation& operator=(RadioStation const&); // Assignment operator is private.

	static RadioStation *rs_pInstance;

	std::string _name;

	Playlist _allTracks;

	Playlist _currentPlaySet;
    
    Playlist _topTen;

public:
    
	static RadioStation* Instance();
    
    bool setName(std::string name);
    
    std::string getName();

	void generateGame(); // Generates the game.

	Playlist* getAllTracks(); // Adicionado por razoes obvias
    
    std::vector<Music *> getTopTen(); // Noone should be able to edit it, so let's not pass it by reference.
    
    void updateTopTen();
	
};

#endif