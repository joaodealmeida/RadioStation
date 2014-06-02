//
//  FilesIO.h
//  Radio Station
//
//  T3G12 - MIEIC 2012/2013
//
//  This class manages everything that has to do with files.
//  Eduardo Almeida, Joao Almeida and Joao Ramos
//

#ifndef FilesIOHeader
#define FilesIOHeader

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include "Music.h"
#include "Playlist.h"
#include "User.h"

class FilesIO {
    
private:
    
	FilesIO(); // We hope we aren't using too many singletons!

	FilesIO(FilesIO const&); // Copy constructor is private.
	FilesIO& operator=(FilesIO const&); // Assignment operator is private.

	static FilesIO *fio_pInstance;
    
    bool _writeToFile(std::string filePath, std::string contents, bool replace);
    
    int _userCount();

public:
    
	static FilesIO* Instance();
    
    //
    //  The Top10 is generated at runtime, and not saved into any file.
    //
    //  Removed:
    //      - bool loadTopTen();
    
    //
    // The global files
    //  - radioStationMusics.csv
    //  - topTen.csv
    //  - users.csv
    //
    // Usually only need to be loaded / written to
    // a few times per session.
    //
    // So we can merge them, for simplicity.
    //
    
    bool loadGlobals();
    bool storeGlobals();
    
    //
    // Loads the playlistUserXXX.csv or searches
    // for an user with that username.
    //
    
    Playlist playlistForUser(int userId);
    Playlist playlistForUser(std::string userName);
    
    //
    // Saves the user playlist.
    //

    bool storePlaylistForUser(int userId);
    
    //
    // Music I/O Related
    //
    
    bool loadAllSongs();
    bool saveAllSongs();
    
    //
    // Load / Save user details
    //
    
    User* loadUser(int userId);
    bool saveUser(User *theUser);
    bool removeUser(User *theUser);
    
    void loadAllUsers();

};

#endif