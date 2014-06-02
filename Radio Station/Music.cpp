//
//  User.cpp
//  Radio Station
//
//  T3G12 - MIEIC 2012/2013
//  Eduardo Almeida, Joao Almeida and Joao Ramos
//

#include <ctime>

#include "Music.h"
#include "Playlist.h"
#include "RadioStation.h"

Music::Music (unsigned int musicId , unsigned int year , std::string title, std::string artist, std::string author, std::string album, std::string music_genre, unsigned int likes , unsigned int dislikes , unsigned int playCount, bool available) {
	_musicId = musicId;
	_year = year;
	_title = title;
	_artist = artist;
	_author = author;
	_album = album;
	_music_genre = music_genre;
	_likes = likes;
	_dislikes = dislikes;
	_playCount = playCount;
	_available = available;
}

Music::~Music() {
    
}

bool Music::setMusicId(unsigned int TheId) {
	if (TheId > RadioStation::Instance()->getAllTracks()->count())
		return false;

	_musicId = TheId;
    
    return true;
}

bool Music::setYear(unsigned int TheYear) {
	if ((TheYear - 1970) * 60 * 60 * 24 * 365 > time(NULL))
		return false; // Hey, the song wasn't even released yet!!!

	_year = TheYear;
    
    return true;
}

bool Music::setTitle(std::string title) {
	if (title.size() <= 0)
		return false;
	
    _title = title;
    
    return true;
}

const std::string Music::getTitle() {
	return _title;
}

bool Music::setArtist (std::string artist) {
	if (artist.size() <= 0)
		return false;
    
	_artist = artist;
    
    return true;
}

const std::string Music::getArtist() {
	return _artist;
}

bool Music::setAuthor(std::string author) {
	if (author.size() <= 0)
		return false;
    
	_author = author;
    
    return true;
}

bool Music::setAlbum(std::string album) {
	if (album.size() <= 0)
		return false;
    
	_album = album;
    
    return true;
}

const std::string Music::getAuthor() {
	return _author;
}

bool Music::setGenre(std::string music_genre) {
	if (music_genre.size() <= 0)
		return false;
    
	_music_genre = music_genre;
    
    return true;
}

const std::string Music::getGenre() {
	return _music_genre;
}

void Music::addLike() {
	_likes++;
}

void Music::addDislike() {
	_dislikes++;
}

void Music::addPlay() {
	_playCount++;
}

bool Music::setPlayCount(int count) {
    if (count < 0)
        return false;
        
    _playCount = count;
    
    return true;
}

const unsigned int Music::getLikes() {
    return _likes;
}

const unsigned int Music::getDislikes() {
    return _dislikes;
}

const unsigned int Music::getPlayCount() {
    return _playCount;
}

const unsigned int Music::getId() {
    return _musicId;
}

const unsigned int Music::getYear() {
    return _year;
}

const std::string Music::getAlbum() {
    return _album;
}

void Music::setAvailable(bool availability) {
	_available = availability;
}

const bool Music::getAvailable() {
    return _available;
}
