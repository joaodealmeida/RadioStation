//
//  User.cpp
//  Radio Station
//
//  T3G12 - MIEIC 2012/2013
//  Eduardo Almeida, Joao Almeida and Joao Ramos
//

#include "Playlist.h"
#include "Music.h"

Playlist::Playlist() {
    
}

Playlist::~Playlist() {
    
}

bool Playlist::addSong(Music *theSong) {
    for (unsigned int i = 0; i < _thePlaylist.size(); i++)
        if (_thePlaylist[i] -> getId() == theSong -> getId())
            return false;
    
    _thePlaylist.push_back(theSong);
    
    return true;
}

bool Playlist::removeSong(Music *theSong){
    for (unsigned int i = 0; i < _thePlaylist.size(); i++)
        if (_thePlaylist[i] -> getId() == theSong -> getId()) {
            _thePlaylist.erase(_thePlaylist.begin() + i);
            
            return true;
        }
    
    return false;
}

const std::vector<Music *> Playlist::getAllTracks() {
    return _thePlaylist;
}

const std::vector<Music *> Playlist::search(int musicId, std::string title,  std::string artist, std::string author, std::string album, std::string music_genre, int year) {
    std::vector<Music *> returnVec;
    
    if (musicId != -1) // Do NOT search by music id
        for (unsigned int i = 0; i < _thePlaylist.size(); i++)
            if (_thePlaylist[i] -> getId() == musicId) {
                returnVec.push_back(_thePlaylist[i]);
                
                break;
            }
    
    if (title.compare(""))
        for (unsigned int i = 0; i < _thePlaylist.size(); i++)
            if (!(_thePlaylist[i] -> getTitle().compare(title)))
                if (std::find(returnVec.begin(), returnVec.end(), _thePlaylist[i]) == returnVec.end())
                    returnVec.push_back(_thePlaylist[i]);
    
    if (artist.compare(""))
        for (unsigned int i = 0; i < _thePlaylist.size(); i++)
            if (!(_thePlaylist[i] -> getArtist().compare(artist)))
                if (std::find(returnVec.begin(), returnVec.end(), _thePlaylist[i]) == returnVec.end())
                    returnVec.push_back(_thePlaylist[i]);
    
    if (author.compare(""))
        for (unsigned int i = 0; i < _thePlaylist.size(); i++)
            if (!(_thePlaylist[i] -> getAuthor().compare(author)))
                if (std::find(returnVec.begin(), returnVec.end(), _thePlaylist[i]) == returnVec.end())
                    returnVec.push_back(_thePlaylist[i]);
    
    if (album.compare(""))
        for (unsigned int i = 0; i < _thePlaylist.size(); i++)
            if (!(_thePlaylist[i] -> getAlbum().compare(album)))
                if (std::find(returnVec.begin(), returnVec.end(), _thePlaylist[i]) == returnVec.end())
                    returnVec.push_back(_thePlaylist[i]);
    
    if (music_genre.compare(""))
        for (unsigned int i = 0; i < _thePlaylist.size(); i++)
            if (!(_thePlaylist[i] -> getGenre().compare(music_genre)))
                if (std::find(returnVec.begin(), returnVec.end(), _thePlaylist[i]) == returnVec.end())
                    returnVec.push_back(_thePlaylist[i]);
    
    if (year != -1)
        for (unsigned int i = 0; i < _thePlaylist.size(); i++)
            if (_thePlaylist[i] -> getYear() == year)
                if (std::find(returnVec.begin(), returnVec.end(), _thePlaylist[i]) == returnVec.end())
                    returnVec.push_back(_thePlaylist[i]);
    
    return returnVec;
}

void Playlist::shuffle(){
    std::random_shuffle(_thePlaylist.begin(), _thePlaylist.end());
}

const unsigned int Playlist::count() {
    return (const unsigned int) _thePlaylist.size();
}

const std::vector<Music *> Playlist::getTopTen() {
    if (_topTen.size() == 0)
        updateTopTen();
    
    return _topTen;
}

void Playlist::updateTopTen() {
    // This needs testing.
    
    std::vector<Music *> newTopTen;
    
    int topCount = 0; // The current top count of likes. Begins as 0, obviously, as we don't know yet the song that has the most likes.
    
    for (unsigned int i = 0; i < 10; i++) { // We run the loop 10 times, as we desire our top to have... 10 songs.
        int localTop = 0; // The current /local/ top of likes. Same story as up there.
        
        for (unsigned int j = 0; j < _thePlaylist.size(); j++) {
            int diff = _thePlaylist[j] -> getLikes() - _thePlaylist[j] -> getDislikes();
        
            if (diff > localTop)
                if (topCount == 0 || diff < topCount)
                    localTop = diff;
        }
        
        if (localTop != 0) {
            for (unsigned int j = 0; j < _thePlaylist.size(); j++) {
                int diff = _thePlaylist[j] -> getLikes() - _thePlaylist[j] -> getDislikes();
                
                if (localTop == diff)
                    newTopTen.push_back(_thePlaylist[j]);
            }
        }
        
        if (localTop)
            topCount = localTop;
    }
    
    _topTen = newTopTen;
}
