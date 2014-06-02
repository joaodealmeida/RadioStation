//
//  RadioStation.cpp
//  Radio Station
//
//  T3G12 - MIEIC 2012/2013
//  Eduardo Almeida, Joao Almeida and Joao Ramos
//

#include "RadioStation.h"
#include "Music.h"
#include "Playlist.h"

const int setDivisor = 4;

const int maxPlaysPerSong = 15;

RadioStation *RadioStation::rs_pInstance = NULL;

RadioStation* RadioStation::Instance() {
    if (!rs_pInstance)
        rs_pInstance = new RadioStation;
    
    return rs_pInstance;
}

RadioStation::RadioStation() {
    
}

bool RadioStation::setName(std::string name) {
	if (name.size() <= 0)
		return false;
	
	_name = name;
	
	return true;
}

std::string RadioStation::getName() {
    return _name;
}

void RadioStation::generateGame() {
    srand((unsigned)time(NULL));
    
    for (unsigned int i = 0; i < _allTracks.count(); i++)
        _allTracks.getAllTracks()[i] -> setPlayCount(rand() % maxPlaysPerSong);
}

Playlist* RadioStation::getAllTracks() {
    return &_allTracks;
}

std::vector<Music *> RadioStation::getTopTen() {
    return _allTracks.getTopTen();
}

void RadioStation::updateTopTen() {
    _allTracks.updateTopTen();
}
