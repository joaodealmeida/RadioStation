//
//  Playlist.h
//  Radio Station
//
//  T3G12 - MIEIC 2012/2013
//  Eduardo Almeida, Joao Almeida and Joao Ramos
//
//  This class manages a single playlist object and its songs/song count.
//

#ifndef PlaylistHeader
#define PlaylistHeader

#include <iostream>
#include <vector>
#include <algorithm>

#include "Music.h"

class Playlist {
    
private:
    
	std::vector <Music *> _thePlaylist;
    
    std::vector <Music *> _topTen;

public:
    
    Playlist();
    ~Playlist();
    
    //
    // Add and remove songs to the playlist, nothing major here.
    //
    
	bool addSong(Music *theSong);
	bool removeSong(Music *theSong);
    
    //
    // As we are using the Music objects by reference, and using
    // the same object on the global/user/... playlists, we can
    // generate the top ten songs list dynamically.
    //

    // Returns Music * as the key and the like / dislike difference as the object.
    
    // As maps are ordered, we can just loop through this.
    
	// const std::map <Music *, int> topTenSongs();
    
    const std::vector<Music *> getTopTen();
    
    void updateTopTen();
    
    // Searches through the playlist and returns a vector with matches.

	const std::vector<Music *> search(int musicId, std::string title,  std::string artist, std::string author , std::string album, std::string music_genre, int year);
    
    const unsigned int count(); // Adicionado por razoes obvias
    
    const std::vector<Music *> getAllTracks();
    
    void shuffle();
};

#endif