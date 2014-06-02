//
//  Music.h
//  Radio Station
//
//  T3G12 - MIEIC 2012/2013
//  Eduardo Almeida, Joao Almeida and Joao Ramos
//
//  This class manages a single song object and its data.
//

#ifndef MusicHeader
#define MusicHeader

#include <iostream>
#include <string>

class Music {
    
private:
    
	unsigned int _musicId;

	std::string _title;
	std::string _artist;
	std::string _author;
	std::string _album;
	std::string _music_genre;
	
	unsigned int _year;
    
    unsigned int _likes;
    unsigned int _dislikes;
    
    unsigned int _playCount;
    
    bool _available;

public:
    
	// Doing this with constructors and deconstructors is much better as we can
	// automatically fill the song id by accessing the radio music database.
    
	Music(unsigned int musicId, unsigned int year, std::string title, std::string artist, std::string author, std::string album, std::string music_genre, unsigned int _likes, unsigned int _dislikes, unsigned int playCount, bool available); 
	// Adicionado por questoes obvias
	~Music();
    
    //
    // User variables.
    //
    
    bool setMusicId(unsigned int theId);
    const unsigned int getId();
    
    bool setYear(unsigned int theYear);
    const unsigned int getYear();

	bool setTitle(std::string title);
	const std::string getTitle();
	
	bool setArtist(std::string artist);
	const std::string getArtist();
    
	bool setAuthor(std::string author);
	const std::string getAuthor();
    
	bool setAlbum(std::string album);
	const std::string getAlbum();
    
	bool setGenre(std::string music_genre);
	const std::string getGenre();
    
	const unsigned int getLikes();
    const unsigned int getDislikes();
    
    void addLike(); // Por razoes Obvias de bool para void
    void addDislike(); // Por razoes Obvias de bool para void
    
    const unsigned int getPlayCount();
    void addPlay(); // Por razoes Obvias de bool para void
    bool setPlayCount(int count); // In order to generate the set, as I am generating a random number.
    
    const bool getAvailable();
    void setAvailable(bool availability); // Por razoes Obvias de bool para void
    
};

#endif