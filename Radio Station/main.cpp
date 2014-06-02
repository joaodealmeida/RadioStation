//
//  main.cpp
//  Radio Station
//
//  T3G12 - MIEIC 2012/2013
//  Eduardo Almeida, Joao Almeida and Joao Ramos
//

#include <iostream>
#include <string>
#include <ctype.h>

#ifdef WIN32
#include <conio.h>
#endif

#include "UserManager.h"
#include "User.h"
#include "FilesIO.h"
#include "RadioStation.h"
#include "Playlist.h"
#include "Music.h"

#include "Additions.h"

//
// Instance Variables
//

static const int baseASCIINumber = 48;
    
static const int escKey = 27;

static const int returnKeyPOSIX = 13;
static const int returnKeyWindows = 10;

User *loggedInUser = NULL;

//
// Function Definitions
//

void start();
void login();
void newUser();
void addMusic();
void adminPanel();
void loggedInMenu();
void editMusicMenu();
void editMusic(Music *theMusic);
void userWorkWithSong(Music *theMusic);
void musicManager();
void searchLibraryStepTwo(bool name, bool artist, bool author, bool album, bool genre, bool year);
void playlistManager();
void topTenSongs();
void getArtistMusics(std::string artist);
void getAuthorMusics(std::string album);
void getYearMusics(int year);

//
// Code Start
//

void editMusic(Music *theMusic) {
    std::cout << "Music Manager :: Editing Song " << theMusic -> getId() << std::endl << std::endl;
    
	std::cout << "Song Title [" << theMusic->getTitle() << "]: ";

	std::string newTitle = Additions::getline();
    
    if (Additions::gotESC(newTitle)) {
        Additions::clearConsole();
        
        editMusicMenu();
    }

	if (newTitle.compare(""))
		theMusic -> setTitle(newTitle); //changes title

	std::cout << std::endl;

	std::cout << "Song Artist [" << theMusic->getArtist() << "]: ";

	std::string newArtist = Additions::getline();
    
    if (Additions::gotESC(newArtist)) {
        Additions::clearConsole();
        
        editMusicMenu();
    }

	if (newArtist.compare(""))
		theMusic -> setArtist(newArtist); //changes Artist

	std::cout << std::endl;

	std::cout << "Song Author [" << theMusic->getAuthor() << "]: ";

	std::string newAuthor = Additions::getline();
    
    if (Additions::gotESC(newAuthor)) {
        Additions::clearConsole();
        
        editMusicMenu();
    }

	if (newAuthor.compare(""))
		theMusic -> setAuthor(newAuthor); //changes Author

	std::cout << std::endl;

	std::cout << "Song Album [" << theMusic->getAlbum() << "]: ";

	std::string newAlbum = Additions::getline();
    
    if (Additions::gotESC(newAlbum)) {
        Additions::clearConsole();
        
        editMusicMenu();
    }

	if (newAlbum.compare(""))
		theMusic -> setAlbum(newAlbum); //changes Album

	std::cout << std::endl;

    std::string newYear;
    
	do {
        
        std::cout << "Song Year [" << theMusic->getYear() << "]: ";

		newYear = Additions::getline();
        
        if (Additions::gotESC(newYear)) {
            Additions::clearConsole();
            
            editMusicMenu();
        }
    
		if (newYear.compare("")) {
			if (atoi(newYear.c_str())) {
				theMusic -> setYear (atoi(newYear.c_str()));
                
                break;
            }
        } else
            break;
        
	} while (!atoi(newYear.c_str()));
    
	std::cout << std::endl;

	std::cout << "Song Genre [" << theMusic->getGenre() << "]: ";

	std::string newGenre = Additions::getline();
    
    if (Additions::gotESC(newGenre)) {
        Additions::clearConsole();
        
        editMusicMenu();
    }

	if (newGenre.compare(""))
		theMusic -> setGenre(newGenre); //changes Genre
    
    bool available = false;
    
    std::cout << std::endl;
    
    std::cout << "Available? " << (theMusic -> getAvailable() ? "[Y]" : "[N]") << " (y/n): ";
    
    while (true) {
        int ch = _getch();
        
        bool shouldBreak = false;
        
        switch (ch) {
            case 121:   // y
            case 89:    // Y
                
                available = true;
                
                shouldBreak = true;
                
                break;
                
            case 110:   // n
            case 78:    // N
                
                available = false;
                
                shouldBreak = true;
                
                break;
                
            case returnKeyPOSIX:    // Return (POSIX)
            case returnKeyWindows:    // Return (Windows)
                
                available = theMusic -> getAvailable();
                
                shouldBreak = true;
                
                break;
                
            case escKey:    // ESC

				Additions::clearConsole();
                
                editMusicMenu();
                
                break;
                
            default:
                
                break;
        }
        
        if (shouldBreak) {
            std::cout << (char) ch;
            break;
        }
    }
    
    theMusic -> setAvailable(available);

	std::cout << std::endl << std::endl << "Done! Please press Return to go back.";
    
    Additions::waitForReturn();
    Additions::clearConsole();
    
    FilesIO::Instance() -> saveAllSongs();
    
    editMusicMenu();
}

void editMusicMenu() {
    std::cout << "Music Manager :: Edit Music" << std::endl;
    
    std::cout << std::endl;
    
    Playlist *allTracks = RadioStation::Instance() -> getAllTracks();
    
    std::vector<Music *> allTracksVec = allTracks -> getAllTracks();
    
    if (allTracksVec.size() == 0) {
        std::cout << "There are currently no tracks! Please press Return to go back.";
        
        Additions::waitForReturn();
        Additions::clearConsole();
        musicManager();
    }
        
    for (unsigned int i = 0 ; i < allTracksVec.size() ; i++)
        std::cout << "[" << i << "] " << allTracksVec[i]->getTitle() << " by " << allTracksVec[i]->getArtist() << " - " << (allTracksVec[i]->getAvailable() ? "(Available)" : "(Unavailable)") <<  std::endl;
    
    while (true) {
        std::cout << std::endl;
        
        std::cout << "Choose the music track you want to make changes to: ";
        
        std::string songIdStr = Additions::getline();
        
        if (Additions::gotESC(songIdStr)) {
            Additions::clearConsole();
            
            musicManager();
        }
        
        int songId = atoi(songIdStr.c_str());
        
        while (!(RadioStation::Instance()->getAllTracks()->count() > (unsigned int)songId)) {
            std::cout << std::endl << "Invalid track. Please type another id: ";
            
            songIdStr = Additions::getline();
            
            if (Additions::gotESC(songIdStr))
                musicManager();
            
            songId = atoi(songIdStr.c_str());
        }
        
        if (songId == -1) {
            Additions::clearConsole();
            adminPanel();
            break;
        } else {
            Additions::clearConsole();
            
            editMusic(allTracksVec[songId]);
        }
    }
}

void musicManager() {
    std::cout << "Music Manager :: Administrative Panel" << std::endl;
    std::cout << std::endl;
    std::cout << "1. Add New Song" << std::endl;
    std::cout << "2. Modify Existing Song" << std::endl;
    std::cout << std::endl;
    std::cout << "Please choose an option.";
    
    while (true) {
        
        int opc = _getch();
        
        switch (opc) {
            case (baseASCIINumber + 1):
                
                Additions::clearConsole();
                
                addMusic();
                
                break;
                
            case (baseASCIINumber + 2):
                
                Additions::clearConsole();
                
                editMusicMenu();
                
                break;
                
            case escKey:
                
                Additions::clearConsole();
                
                adminPanel();
                
                break;
                
            default:
                
                break;
        }
        
    }
}

void addMusic() {
    std::cout << "Music Manager :: New Music" << std::endl;
    std::cout << std::endl;
    
    std::string title;
    
    while (!title.compare("")) {
        std::cout << "Title: ";
        
        title = Additions::getline();
        
        if (Additions::gotESC(title)) {
            Additions::clearConsole();
            
            musicManager();
        }
        
        std::cout << std::endl;
        
        if (!title.compare(""))
            std::cout << "Title is a required field." << std::endl;
    }
    
    std::string artist;
    
    while (!artist.compare("")) {
        std::cout << "Artist: ";
        
        artist = Additions::getline();
        
        if (Additions::gotESC(artist)) {
            Additions::clearConsole();
            
            musicManager();
        }
        
        std::cout << std::endl;
        
        if (!artist.compare(""))
            std::cout << "Artist is a required field." << std::endl;
    }
    
    std::cout << "Author: ";
    
    std::string author = Additions::getline();
    
    if (Additions::gotESC(author)) {
        Additions::clearConsole();
        
        musicManager();
    }
    
    std::cout << std::endl;
    
    std::cout << "Album: ";
    
    std::string album = Additions::getline();
    
    if (Additions::gotESC(album)) {
        Additions::clearConsole();
        
        musicManager();
    }
    
    std::cout << std::endl;
    
    std::cout << "Genre: ";
    
    std::string genre = Additions::getline();
    
    if (Additions::gotESC(genre)) {
        Additions::clearConsole();
        
        musicManager();
    }
    
    int year = 0;
    
    while (!year) {
        std::cout << std::endl;
        
        std::cout << "Year: ";
        
        std::string yearStr = Additions::getline();
        
        if (Additions::gotESC(yearStr)) {
            Additions::clearConsole();
            
            musicManager();
        }
        
        if (atoi(yearStr.c_str()) > 0)
            year = atoi(yearStr.c_str());
        
        if (!year)
            std::cout << "Year is a required field." << std::endl;
    }
    
    bool available = false;
    
    std::cout << std::endl;
    
    std::cout << "Available? (y/n): ";
    
    while (true) {
        int ch = _getch();
        
        bool shouldBreak = false;
        
        switch (ch) {
            case 121:   // y
            case 89:    // Y
                
                available = true;
                
                shouldBreak = true;
                
                break;
                
            case 110:   // n
            case 78:    // N
                
                available = false;
                
                shouldBreak = true;
                
                break;
                
            case escKey:    // esc
                
                Additions::clearConsole();
                
                musicManager();
                
                break;
                
            default:
                
                break;
        }
        
        if (shouldBreak) {
            std::cout << (char) ch;
            break;
        }
    }
    
    std::cout << std::endl << std::endl;
    
    Music *newMusic = new Music(RadioStation::Instance() -> getAllTracks() -> count(), year, title, artist, author, album, genre, 0, 0, 0, available);
    
    if (RadioStation::Instance()->getAllTracks()->addSong(newMusic))
        std::cout << "The track was successfully added!";
    else
        std::cout << "There was an error adding your track.";
    
    FilesIO::Instance() -> saveAllSongs();
    
    std::cout << " Please press return to continue.";
    
    Additions::waitForReturn();
    
    Additions::clearConsole();
    
    musicManager();
}

void reinitializeSet() {
    std::cout << (RadioStation::Instance() -> getName().compare("") ? RadioStation::Instance() -> getName() : "Radio Station") << " :: Set Control Panel" << std::endl << std::endl;
    
    std::cout << "Are you sure you want to reinitialize the station set? (y/n): ";
    
    bool reinitialize = false;
    
    while (true) {
        int ch = _getch();
        
        bool shouldBreak = false;
        
        switch (ch) {
            case 121:   // y
            case 89:    // Y
                
                reinitialize = true;
                
                shouldBreak = true;
                
                break;
                
            case 110:   // n
            case 78:    // N
            case escKey:
                
                reinitialize = false;
                
                shouldBreak = true;
                
                break;
                
            default:
                
                break;
        }
        
        if (shouldBreak) {
            std::cout << (char) ch;
            break;
        }
    }
    
    std::cout << std::endl << std::endl;
    
    if (reinitialize) {
        RadioStation::Instance() -> generateGame();
        
        UserManager::Instance() -> resetPrizeWinner();
        
        std::cout << "The set was reinitialized! ";
    } else
        std::cout << "The set wasn't reinitialized. ";
    
    std::cout << "Please press Return to continue.";
    
    Additions::waitForReturn();
    Additions::clearConsole();
    
    adminPanel();
}

void changeName() {
    std::cout << (RadioStation::Instance() -> getName().compare("") ? RadioStation::Instance() -> getName() : "Radio Station") << " :: Name Changer" << std::endl << std::endl;
    
    std::cout << "Current Name: " << (RadioStation::Instance() -> getName().compare("") ? RadioStation::Instance() -> getName() : "None!") << std::endl;
    std::cout << "New Name: ";
    
    std::string newName = Additions::getline();
    
    if (Additions::gotESC(newName)) {
        Additions::clearConsole();
        
        adminPanel();
    }
    
    RadioStation::Instance() -> setName(newName);
    
    FilesIO::Instance() -> storeGlobals();
    
    std::cout << std::endl << std::endl;
    
    if (!newName.compare("")) {
        std::cout << "The name has been deleted.";
    } else
        std::cout << "The Radio Station is now called " << RadioStation::Instance() -> getName();
    
    std::cout << std::endl << std::endl << "Please press Return to proceed.";
    
    Additions::waitForReturn();
    Additions::clearConsole();
    
    adminPanel();
}

void adminPanel() {
    std::cout << "Welcome to the administrative panel!" << std::endl << std::endl;
    
    std::cout << "1. Manage the Music Library" << std::endl;
    std::cout << "2. Reinitialize the Set" << std::endl;
    std::cout << "3. Change the Radio Station Name" << std::endl;
    std::cout << std::endl;
    std::cout << "Please choose an option.";
    
    while (true) {
		
		int opc = _getch();
        
        switch (opc) {
                
            case (baseASCIINumber + 1):
                
                Additions::clearConsole();
                
                musicManager();
                
                break;
                
            case (baseASCIINumber + 2):
                
                Additions::clearConsole();
                
                reinitializeSet();
                
                break;
                
            case (baseASCIINumber + 3):
                
                Additions::clearConsole();
                
                changeName();
                
                break;
                
            case escKey:
                
                Additions::clearConsole();
                
                loggedInMenu();
                
                break;
                
            default:
                
                break;
        }
    }
}

void topTenSongs() {
    std::cout << (RadioStation::Instance() -> getName().compare("") ? RadioStation::Instance() -> getName() : "Radio Station") << " :: Top Ten" << std::endl << std::endl;
    
    RadioStation::Instance() -> updateTopTen();
    
    std::vector<Music *> topTen = RadioStation::Instance() -> getTopTen();
    
    for (unsigned int i = 0; i < topTen.size(); i++) {
        Music *theTrack = topTen[i];
        std::cout << "[" << theTrack -> getId() << "] " << theTrack -> getTitle() << " by " << theTrack -> getArtist() << " (" << theTrack -> getPlayCount() << " plays | " << theTrack -> getLikes() << " likes | " << theTrack -> getDislikes() << " dislikes)" << std::endl;
    }
    
    if (!topTen.size())
        std::cout << "There are currently no songs." << std::endl;
    
    std::cout << std::endl << "Press Return to go back.";
    
    Additions::waitForReturn();
    
    Additions::clearConsole();
    
    loggedInMenu();
}

void searchLibrary() {
    bool name = false, artist = false, author = false, album = false, genre = false, year = false;
    
    while (true) {
        std::cout << (RadioStation::Instance() -> getName().compare("") ? RadioStation::Instance() -> getName() : "Radio Station") << " :: Library Search :: (Step 1 of 2)" << std::endl << std::endl;
        
        std::cout << "Search by..." << std::endl;
        std::cout << (name ? " - 1. Name (Selected)" : " - 1. Name") << std::endl;
        std::cout << (artist ? " - 2. Artist (Selected)" : " - 2. Artist") << std::endl;
        std::cout << (author ? " - 3. Author (Selected)" : " - 3. Author") << std::endl;
        std::cout << (album ? " - 4. Album (Selected)" : " - 4. Album") << std::endl;
        std::cout << (genre ? " - 5. Genre (Selected)" : " - 5. Genre") << std::endl;
        std::cout << (year ? " - 6. Year (Selected)" : " - 6. Year") << std::endl;
        std::cout << std::endl << "Please choose a value option to select/unselect or return to proceed.";
        
        int ch = _getch();
        
        switch (ch) {
            case (baseASCIINumber + 1):
            
                name = (name ? false : true);
            
                break;
            
            case (baseASCIINumber + 2):
            
                artist = (artist ? false : true);
                
                break;
                
            case (baseASCIINumber + 3):
                
                author = (author ? false : true);
                
                break;
                
            case (baseASCIINumber + 4):
                
                album = (album ? false : true);
                
                break;
                
            case (baseASCIINumber + 5):
                
                genre = (genre ? false : true);
                
                break;
                
            case (baseASCIINumber + 6):
                
                year = (year ? false : true);
                
                break;
                
            case returnKeyPOSIX: // Return (POSIX)
            case returnKeyWindows: // Return (Windows)
                
                Additions::clearConsole();
                
                searchLibraryStepTwo(name, artist, author, album, genre, year);
                
                break;
                
            case escKey: // ESC
                
                Additions::clearConsole();
                
                loggedInMenu();
                
                break;
            
            default:
                
                break;
        }
        
        Additions::clearConsole();
    }
}

void getSongsFromKey() {
    std::cout << (RadioStation::Instance() -> getName().compare("") ? RadioStation::Instance() -> getName() : "Radio Station") << " :: Get Tracks" << std::endl << std::endl;
    
    std::cout << "1. All songs from Artist..." << std::endl;
    std::cout << "2. All songs from Author..." << std::endl;
    std::cout << "3. All songs from Year..." << std::endl;
    
    std::cout << std::endl;
    
    if (RadioStation::Instance() -> getAllTracks() -> count())
        std::cout << "Please choose an option.";
    else {
        std::cout << "This operation can't be performed at this time. There are no songs yet." << std::endl << std::endl << "Press Return to continue.";
        
        Additions::waitForReturn();
        
        Additions::clearConsole();
        
        loggedInMenu();
    }
    
    while (true) {
        int ch = _getch();
        
        switch (ch) {
            case (baseASCIINumber + 1): {
                
                Additions::clearConsole();
                
                std::cout << (RadioStation::Instance() -> getName().compare("") ? RadioStation::Instance() -> getName() : "Radio Station") << " :: Artist List" << std::endl << std::endl;
                
                std::vector<std::string> artistList;
                
                for (unsigned int i = 0; i < RadioStation::Instance() -> getAllTracks() -> count(); i++) {
                    std::string theArtist = RadioStation::Instance() -> getAllTracks() -> getAllTracks()[i] -> getArtist();
                    
                    bool foundArtist = false;
                    
                    for (unsigned int i = 0; i < artistList.size(); i++)
                        if (!artistList[i].compare(theArtist)) {
                            foundArtist = true;
                            
                            break;
                        }
                    
                    if (!foundArtist)
                        artistList.push_back(theArtist);
                }
                
                std::sort(artistList.begin(), artistList.end());
                
                for (unsigned int i = 0; i < artistList.size(); i++)
                    std::cout << "[" << i << "] " << artistList[i] << std::endl;
                
                std::cout << std::endl << "Please type the choosen artist name or number: ";
                
                std::string artistName = Additions::getline();
                
                if (Additions::gotESC(artistName)) {
                    Additions::clearConsole();
                    
                    getSongsFromKey();
                    
                    break;
                }
                
                if (atoi(artistName.c_str())) { // We got a number!
                    
                    unsigned int result = atoi(artistName.c_str());
                    
                    if (artistList.size() > result) {
                        Additions::clearConsole();
                        
                        getArtistMusics(artistList[result]);
                        
                        break;
                    }
                    
                } else if (!artistName.compare("0")) { // We got 0.
                    
                    Additions::clearConsole();
                    
                    getArtistMusics(artistList[0]);
                    
                    break;
                
                } else { // We got a name.
                    
                    for (unsigned int i = 0; i < artistList.size(); i++) {
                        if (!artistList[i].compare(artistName)) {
                            Additions::clearConsole();
                            
                            getArtistMusics(artistList[i]);
                            
                            break;
                        }
                    }
                    
                }
                
                std::cout << std::endl << std::endl << "You have inserted an invalid option. Press Return to continue.";
                
                Additions::waitForReturn();
                
                Additions::clearConsole();
                
                getSongsFromKey();
                
                break;
                
            }
                
            case (baseASCIINumber + 2): {
                
                Additions::clearConsole();
                
                std::cout << (RadioStation::Instance() -> getName().compare("") ? RadioStation::Instance() -> getName() : "Radio Station") << " :: Author List" << std::endl << std::endl;
                
                std::vector<std::string> authorList;
                
                for (unsigned int i = 0; i < RadioStation::Instance() -> getAllTracks() -> count(); i++) {
                    std::string theAuthor = RadioStation::Instance() -> getAllTracks() -> getAllTracks()[i] -> getAuthor();
                    
                    bool foundAuthor = false;
                    
                    for (unsigned int i = 0; i < authorList.size(); i++)
                        if (!authorList[i].compare(theAuthor)) {
                            foundAuthor = true;
                            
                            break;
                        }
                    
                    if (!foundAuthor)
                        authorList.push_back(theAuthor);
                }
                
                std::sort(authorList.begin(), authorList.end());
                
                for (unsigned int i = 0; i < authorList.size(); i++)
                    std::cout << "[" << i << "] " << authorList[i] << std::endl;
                
                std::cout << std::endl << "Please type the choosen author name or number: ";
                
                std::string authorName = Additions::getline();
                
                if (Additions::gotESC(authorName)) {
                    Additions::clearConsole();
                    
                    getSongsFromKey();
                    
                    break;
                }
                
                 if (atoi(authorName.c_str())) { // We got a number!
                    
                    unsigned int result = atoi(authorName.c_str());
                    
                    if (authorList.size() > result) {
                        Additions::clearConsole();
                        
                        getAuthorMusics(authorList[result]);
                        
                        break;
                    }
                    
                } else if (!authorName.compare("0")) { // We got 0.
                    
                    Additions::clearConsole();
                    
                    getAuthorMusics(authorList[0]);
                    
                    break;
                
                } else { // We got a name.
                    
                    for (unsigned int i = 0; i < authorList.size(); i++) {
                        if (!authorList[i].compare(authorName)) {
                            Additions::clearConsole();
                            
                            getAuthorMusics(authorList[i]);
                            
                            break;
                        }
                    }
                    
                }
                
                std::cout << std::endl << std::endl << "You have inserted an invalid option. Press Return to continue.";
                
                Additions::waitForReturn();
                
                Additions::clearConsole();
                
                getSongsFromKey();
                
                break;
                
            }
                
            case (baseASCIINumber + 3): {
                
                Additions::clearConsole();
                
                std::cout << (RadioStation::Instance() -> getName().compare("") ? RadioStation::Instance() -> getName() : "Radio Station") << " :: Year List" << std::endl << std::endl;
                
                std::vector<int> yearList;
                
                for (unsigned int i = 0; i < RadioStation::Instance() -> getAllTracks() -> count(); i++) {
                    int theYear = RadioStation::Instance() -> getAllTracks() -> getAllTracks()[i] -> getYear();
                    
                    bool foundYear = false;
                    
                    for (unsigned int i = 0; i < yearList.size(); i++)
                        if (yearList[i] == theYear) {
                            foundYear = true;
                            
                            break;
                        }
                    
                    if (!foundYear)
                        yearList.push_back(theYear);
                }
                
                for (unsigned int i = 0; i < yearList.size(); i++)
                    std::cout << "[" << i << "] " << yearList[i] << std::endl;
                
                std::cout << std::endl << "Please type the year: ";
                
                std::string yearStr = Additions::getline();
                
                if (Additions::gotESC(yearStr)) {
                    Additions::clearConsole();
                    
                    getSongsFromKey();
                    
                    break;
                }
                
                unsigned int result = atoi(yearStr.c_str());
                
                for (unsigned int i = 0; i < yearList.size(); i++)
                    if (yearList[i] == result) {
                        Additions::clearConsole();
                        
                        getYearMusics(yearList[i]);
                        
                        break;
                    }
                
                if (result < yearList.size()) {
                    Additions::clearConsole();
                    
                    getYearMusics(yearList[result]);
                    
                    break;
                }
                
                std::cout << std::endl << std::endl << "You have inserted an invalid option. Press Return to continue.";
                
                Additions::waitForReturn();
                
                Additions::clearConsole();
                
                getSongsFromKey();
                
                break;
                
            }
                
            case escKey: // ESC
                
                Additions::clearConsole();
                
                loggedInMenu();
                
                break;
                
            default:
                
                break;
        }
    }
}

void getArtistMusics (std::string artist) {
    std::cout << (RadioStation::Instance() -> getName().compare("") ? RadioStation::Instance() -> getName() : "Radio Station") << " :: Artist Detail (" << artist << ")" << std::endl << std::endl;
    
	std::vector<Music *> allTracksVec = RadioStation::Instance() -> getAllTracks() -> getAllTracks();
	
	for (unsigned int i = 0; i < allTracksVec.size(); i++)
		if (!allTracksVec[i]->getArtist().compare(artist))
			std::cout << "[" << i << "] " << allTracksVec[i]->getTitle() << " by " << allTracksVec[i]->getArtist() << std::endl;
    
    std::cout << std::endl << "Please type a song ID to view more info: ";
    
    std::string songID = Additions::getline();
    
    if (Additions::gotESC(songID)) {
        Additions::clearConsole();
        
        loggedInMenu();
    }
    
    bool foundSong = false;
    
    if (atoi(songID.c_str()) || songID.size() == 1)
        for (unsigned int i = 0; i < allTracksVec.size(); i++)
            if (allTracksVec[i] -> getId() == atoi(songID.c_str())) {
                Additions::clearConsole();
                
                userWorkWithSong(allTracksVec[i]);
                
                foundSong = true;
            }
    
    while (!foundSong) {
        std::cout << std::endl << std::endl << "The requested song wasn't found. Please try again: ";
        
        std::string songID = Additions::getline();
        
        if (Additions::gotESC(songID)) {
            Additions::clearConsole();
            
            loggedInMenu();
        }
        
        if (atoi(songID.c_str()) || songID.size() == 1)
            for (unsigned int i = 0; i < allTracksVec.size(); i++)
                if (allTracksVec[i] -> getId() == atoi(songID.c_str())) {
                    Additions::clearConsole();
                    
                    userWorkWithSong(allTracksVec[i]);
                    
                    foundSong = true;
                }
    }
}

void getAuthorMusics (std::string author) {
	std::cout << (RadioStation::Instance() -> getName().compare("") ? RadioStation::Instance() -> getName() : "Radio Station") << " :: Author Detail (" << author << ")" << std::endl << std::endl;
    
	std::vector<Music *> allTracksVec = RadioStation::Instance() -> getAllTracks() -> getAllTracks();
	
	for (unsigned int i = 0; i < allTracksVec.size(); i++)
		if (!allTracksVec[i]->getAuthor().compare(author))
			std::cout << "[" << i << "] " << allTracksVec[i]->getTitle() << " by " << allTracksVec[i]->getAuthor() << std::endl;
    
    std::cout << std::endl << "Please type a song ID to view more info: ";
    
    std::string songID = Additions::getline();
    
    if (Additions::gotESC(songID)) {
        Additions::clearConsole();
        
        loggedInMenu();
    }
    
    bool foundSong = false;
    
    if (atoi(songID.c_str()) || songID.size() == 1)
        for (unsigned int i = 0; i < allTracksVec.size(); i++)
            if (allTracksVec[i] -> getId() == atoi(songID.c_str())) {
                Additions::clearConsole();
                
                userWorkWithSong(allTracksVec[i]);
                
                foundSong = true;
            }
    
    while (!foundSong) {
        std::cout << std::endl << std::endl << "The requested song wasn't found. Please try again: ";
        
        std::string songID = Additions::getline();
        
        if (Additions::gotESC(songID)) {
            Additions::clearConsole();
            
            loggedInMenu();
        }
        
        if (atoi(songID.c_str()) || songID.size() == 1)
            for (unsigned int i = 0; i < allTracksVec.size(); i++)
                if (allTracksVec[i] -> getId() == atoi(songID.c_str())) {
                    Additions::clearConsole();
                    
                    userWorkWithSong(allTracksVec[i]);
                    
                    foundSong = true;
                }
    }
}

void getYearMusics (int year) {
    std::cout << (RadioStation::Instance() -> getName().compare("") ? RadioStation::Instance() -> getName() : "Radio Station") << " :: Year Detail (" << year << ")" << std::endl << std::endl;
    
	std::vector<Music *> allTracksVec = RadioStation::Instance() -> getAllTracks() -> getAllTracks();
	
	for (unsigned int i = 0; i < allTracksVec.size(); i++)
		if (allTracksVec[i]->getYear() == year)
			std::cout << "[" << i << "] " << allTracksVec[i]->getTitle() << " by " << allTracksVec[i]->getArtist() << std::endl;
    
    std::cout << std::endl << "Please type a song ID to view more info: ";
    
    std::string songID = Additions::getline();
    
    if (Additions::gotESC(songID)) {
        Additions::clearConsole();
        
        loggedInMenu();
    }
    
    bool foundSong = false;
    
    if (atoi(songID.c_str()) || songID.size() == 1)
        for (unsigned int i = 0; i < allTracksVec.size(); i++)
            if (allTracksVec[i] -> getId() == atoi(songID.c_str())) {
                Additions::clearConsole();
                
                userWorkWithSong(allTracksVec[i]);
                
                foundSong = true;
            }
    
    while (!foundSong) {
        std::cout << std::endl << std::endl << "The requested song wasn't found. Please try again: ";
        
        std::string songID = Additions::getline();
        
        if (Additions::gotESC(songID)) {
            Additions::clearConsole();
            
            loggedInMenu();
        }
        
        if (atoi(songID.c_str()) || songID.size() == 1)
            for (unsigned int i = 0; i < allTracksVec.size(); i++)
                if (allTracksVec[i] -> getId() == atoi(songID.c_str())) {
                    Additions::clearConsole();
                    
                    userWorkWithSong(allTracksVec[i]);
                    
                    foundSong = true;
                }
    }
}

void searchLibraryStepTwo(bool name, bool artist, bool author, bool album, bool genre, bool year) {
    std::cout << (RadioStation::Instance() -> getName().compare("") ? RadioStation::Instance() -> getName() : "Radio Station") << " :: Library Search :: (Step 2 of 2)" << std::endl << std::endl;
    
    std::string nameStr, artistStr, authorStr, albumStr, genreStr;
    
    int yearInt = 0;
    
    if (name) {
        std::cout << "Name: ";
        
        nameStr = Additions::getline();
        
        std::cout << std::endl;
        
        if (Additions::gotESC(nameStr)) {
            Additions::clearConsole();
            
            loggedInMenu();
        }
    }
    
    if (artist) {
        std::cout << "Artist: ";
        
        artistStr = Additions::getline();
        
        std::cout << std::endl;
        
        if (Additions::gotESC(artistStr)) {
            Additions::clearConsole();
            
            loggedInMenu();
        }
    }
    
    if (author) {
        std::cout << "Author: ";
        
        authorStr = Additions::getline();
        
        std::cout << std::endl;
        
        if (Additions::gotESC(authorStr)) {
            Additions::clearConsole();
            
            loggedInMenu();
        }
    }
    
    if (album) {
        std::cout << "Album: ";
        
        albumStr = Additions::getline();
        
        std::cout << std::endl;
        
        if (Additions::gotESC(albumStr)) {
            Additions::clearConsole();
            
            loggedInMenu();
        }
    }
    
    if (genre) {
        std::cout << "Genre: ";
        
        genreStr = Additions::getline();
        
        std::cout << std::endl;
        
        if (Additions::gotESC(genreStr)) {
            Additions::clearConsole();
            
            loggedInMenu();
        }
    }
    
    if (year) {
        std::string yearStr;
        
        while (!atoi(yearStr.c_str())) {
            std::cout << "Year: ";
            
            yearStr = Additions::getline();
            
            if (Additions::gotESC(yearStr)) {
                Additions::clearConsole();
                
                loggedInMenu();
            }
            
            if (atoi(yearStr.c_str()))
                yearInt = atoi(yearStr.c_str());
            
            std::cout << std::endl;
        }
    }
    
    std::cout << std::endl << "Search Results:";
    
    std::vector<Music *> searchResult = RadioStation::Instance() -> getAllTracks() -> search(-1, nameStr, artistStr, authorStr, albumStr, genreStr, (yearInt == 0 ? -1 : yearInt));
    
    if (searchResult.size() == 0) {
        std::cout << std::endl << std::endl << "Your search returned no results. Please press Return to retry.";
        
        Additions::waitForReturn();
        
        Additions::clearConsole();
        
        searchLibrary();
    }
    
    std::cout << std::endl;
    
    for (unsigned int i = 0; i < searchResult.size(); i++)
        std::cout << "[" << searchResult[i] -> getId() << "] " << searchResult[i] -> getTitle() << " by " << searchResult[i] -> getArtist() << " " << (searchResult[i] -> getAvailable() ? "(Available)" : "(Unavailable)") << std::endl;
    
    std::cout << std::endl << "Type a song ID to select it: ";
    
    while (true) {
        std::string songIdStr = Additions::getline();
        
        std::cout << std::endl;
        
        if (Additions::gotESC(songIdStr)) {
            Additions::clearConsole();
            
            loggedInMenu();
        }
        
        int songId = atoi(songIdStr.c_str());
        
        bool found = false;
        
        for (unsigned int i = 0; i < searchResult.size(); i++) {
            if (searchResult[i] -> getId() == songId) {
                Additions::clearConsole();
                
                userWorkWithSong(searchResult[i]);
                
                found = true;
                
                break;
            }
        }
        
        if (found)
            break;
        
        std::cout << "Song not found. Please try again: ";
    }
}

void userWorkWithSong(Music *theMusic) {
    while (true) {
        std::cout << (RadioStation::Instance() -> getName().compare("") ? RadioStation::Instance() -> getName() : "Radio Station") << " :: Song Details" << std::endl << std::endl;
        
        std::vector<Music *> songSearch = loggedInUser -> getPlaylist() -> search(theMusic->getId(), "", "", "", "", "", -1);
        
        std::cout << "ID: " << theMusic -> getId() << std::endl;
        std::cout << "Title: " << theMusic -> getTitle() << std::endl;
        std::cout << "Artist: " << theMusic -> getArtist() << std::endl;
        std::cout << "Author: " << theMusic -> getAuthor() << std::endl;
        std::cout << "Album: " << theMusic -> getAlbum() << std::endl;
        std::cout << "Genre: " << theMusic -> getGenre() << std::endl;
        
        std::cout << std::endl;
        
        std::cout << "Likes: " << theMusic -> getLikes() << std::endl;
        std::cout << "Dislikes: " << theMusic -> getDislikes() << std::endl;
        
        std::cout << std::endl;
        
        std::cout << "Play Count: " << theMusic -> getPlayCount() << std::endl;
        
        std::cout << std::endl;
        
        if (!theMusic -> getAvailable() && !loggedInUser -> isAdmin()) {
            std::cout << "This song is unavailable, so its data is read-only. Press Return to go back.";
            
            Additions::waitForReturn();
            
            Additions::clearConsole();
            
            loggedInMenu();
        }
        
        std::cout << "1. Like this track" << std::endl;
        std::cout << "2. Dislike this track" << std::endl;
        std::cout << "3. " << (songSearch.size() ? "Remove from" : "Add to") << " Playlist" << std::endl;
        
        std::cout << std::endl;
        
        std::cout << "Please choose your option.";
        
        int ch = _getch();
        
        switch (ch) {
            case (baseASCIINumber + 1):
                
                theMusic -> addLike();
                
                FilesIO::Instance() -> saveAllSongs();
                
				break;
            case (baseASCIINumber + 2):
                
                theMusic -> addDislike();
                
                FilesIO::Instance() -> saveAllSongs();
                
                break;
                
            case (baseASCIINumber + 3):
                
                if (songSearch.size()) {
                    if (!loggedInUser -> getPlaylist() -> removeSong(theMusic)) {
                        std::cout << std::endl << std::endl << "An error has occoured. We could not remove the song from the playlist. Press Return to continue.";
                        
                        Additions::waitForReturn();
                    }
                } else {
                    if (!loggedInUser -> getPlaylist() -> addSong(theMusic)) {
                        std::cout << std::endl << std::endl << "An error has occoured. We could not add the song to the playlist. Press Return to continue.";
                        
                        Additions::waitForReturn();
                    }
                }
                
                FilesIO::Instance()->saveUser(loggedInUser);
                
                FilesIO::Instance()->storePlaylistForUser(loggedInUser->getId());
                
                break;
                
            case escKey: // ESC
                
                Additions::clearConsole();
                
                loggedInMenu();
                
                break;
                
            default:
                
                break;
        }
        
        Additions::clearConsole();
    }
}

void playlistManager() {
	std::cout << (RadioStation::Instance() -> getName().compare("") ? RadioStation::Instance() -> getName() : "Radio Station") << " :: Playlist Manager" << std::endl;
    
    Playlist *allTracks = RadioStation::Instance() -> getAllTracks();
    
    std::vector<Music *> userTracks = loggedInUser -> getPlaylist() -> getAllTracks();
	
	std::vector<Music *> allTracksVec = allTracks -> getAllTracks();
    
    std::cout << std::endl;
    
    if (!userTracks.size()) {
        std::cout << "Your playlist currently has no tracks. Please add some first! Press Return to continue.";
        
        Additions::waitForReturn();
        
        Additions::clearConsole();
        
        loggedInMenu();
    }
    
    for (unsigned int i = 0 ; i < userTracks.size(); i++)
        std::cout << "[" << userTracks[i]->getId() << "] " << userTracks[i]->getTitle() << " by " << userTracks[i]->getArtist() <<  std::endl;
    
    std::cout << std::endl << "Please type the song ID you want to see details of: ";
    
    std::string selectedSong = Additions::getline();
    
    if (Additions::gotESC(selectedSong)) {
        Additions::clearConsole();
        
        loggedInMenu();
    }
    
    Music *selTrack = NULL;
    
    while (!selTrack) {
        for (unsigned int i = 0; i < userTracks.size(); i++)
            if (userTracks[i]->getId() == atoi(selectedSong.c_str()))
                selTrack = userTracks[i];
        
        if (selTrack)
            break;
        
        std::cout << std::endl << "Invalid Track ID. Please type the song ID you want to see details of: ";
        
        selectedSong = Additions::getline();
        
        if (Additions::gotESC(selectedSong)) {
            Additions::clearConsole();
            
            loggedInMenu();
        }
    }
    
    Additions::clearConsole();
    
    userWorkWithSong(selTrack);
}

void loggedInMenu() {
    std::cout << "Welcome to " << (!RadioStation::Instance()->getName().compare("") ? "the radio station" : RadioStation::Instance()->getName()) << ", " << loggedInUser -> getName() << "!" << std::endl << std::endl;
    
    if (UserManager::Instance() -> getPrizeWinner() != NULL)
        std::cout << "The current winner is " << UserManager::Instance() -> getPrizeWinner() -> getName() << ". Congratulations!" << std::endl << std::endl;
    
    std::cout << "1. Top 10 Songs" << std::endl;
    std::cout << "2. Search our Music Library" << std::endl;
    std::cout << "3. List Songs by Author/Artist/Year" << std::endl;
    std::cout << "4. Manage your Personal Playlist" << std::endl;
    if (loggedInUser -> isAdmin())  std::cout << "5. Administration Panel" << std::endl;
    std::cout << std::endl;
    std::cout << "Please choose an option.";
    
    while (true) {
        int opc = _getch();
        
        switch (opc) {
                
            case (baseASCIINumber + 1):
                
                Additions::clearConsole();
                
                topTenSongs();
                
                break;
                
            case (baseASCIINumber + 2):
                
                Additions::clearConsole();
                
                searchLibrary();
                
                break;
                
            case (baseASCIINumber + 3):
                
                Additions::clearConsole();
                
                getSongsFromKey();
                
                break;
                
            case (baseASCIINumber + 4):
                
                Additions::clearConsole();
                
				playlistManager();
                
                break;
                
            case (baseASCIINumber + 5):
                
                if (loggedInUser -> isAdmin()) {
                    Additions::clearConsole();
                    
                    adminPanel();
                }
                
                break;
                
            case escKey:
                
                Additions::clearConsole();
                
                loggedInUser = NULL;
                
                start();
                
                break;
                
            default:
                
                break;
        }
    }
}

void login() {
	User *theUser = NULL;
    
    FilesIO::Instance() -> loadAllUsers();
    
    std::cout << (RadioStation::Instance() -> getName().compare("") ? RadioStation::Instance() -> getName() : "Radio Station") << " :: Login" << std::endl << std::endl;
    
    do {
        std::cout << "Enter ID or Name: ";
        
        std::string username = Additions::getline();
        
        if (Additions::gotESC(username)) {
            Additions::clearConsole();
            
            start();
        }
        
        if (atoi(username.c_str()) || !username.compare("0")) // if (int)0, this would return false, so... yeah.
            theUser = UserManager::Instance()->getUser(atoi(username.c_str()));
        else
            theUser = UserManager::Instance()->getUser(username);
        
        std::cout << std::endl;
        
        if (!theUser)
            std::cout << "ID/Name not found! Please try again!" << std::endl;
        else {
            std::cout << std::endl << "You are now logged-in with name " << theUser -> getName() << " and ID " << theUser->getId() << "." << std::endl << std::endl << "Press Return to proceed. ";

			Additions::waitForReturn();
		}

    } while (!theUser);
    
    loggedInUser = theUser;
    
    Additions::clearConsole();
    
    loggedInMenu();
}
		
void newUser() {
    std::cout << (RadioStation::Instance() -> getName().compare("") ? RadioStation::Instance() -> getName() : "Radio Station") << " :: New User" << std::endl << std::endl;
    
    if (UserManager::Instance() -> userCount() == 0)
        std::cout << "There are currently no registered users. This will be the admin user." << std::endl << std::endl;
    
	int age = 0;
    
	std::string name;
    std::string age_str;
    std::string sex;
    
    do {
		std::cout << "Name: ";
        
        name = Additions::getline();
        
        if (Additions::gotESC(name)) {
            Additions::clearConsole();
            
            start();
        }
        
		std::cout << std::endl;
        
        if (UserManager::Instance() -> getUser(name)) {
            name = "1";
            
            std::cout << "An user with this name already exists. Please choose another name." << std::endl;
        }
    } while (atoi(name.c_str()));
    
    do {
        std::cout << "Age: ";
		
        age_str = Additions::getline();
        
        if (Additions::gotESC(age_str)) {
            Additions::clearConsole();
            
            start();
        }
        
        if (atoi(age_str.c_str()))
            age = atoi(age_str.c_str());
        
		std::cout << std::endl;
    } while (!age);
    
    std::cout << "Gender (M/F): ";
    
    while (true) {
        int ch = _getch();
        
        bool shouldBreak = false;
        
        switch (ch) {
            case 109:   // m
            case 77:    // M
                
                sex = "M";
                
                shouldBreak = true;
                
                break;
                
            case 102:   // f
            case 70:    // F
                
                sex = "F";
                
                shouldBreak = true;
                
                break;
                
            case escKey:
                
                Additions::clearConsole();
                
                start();
                
                break;
                
            default:
                
                break;
        }
        
        if (shouldBreak) {
            std::cout << (char) ch;
            break;
        }
    }
    
    std::cout << std::endl << std::endl;

	char sexChar = sex[0];
    
    Playlist newPlaylist;
    
    User *theUser = new User(UserManager::Instance()->userCount(), age, (sexChar == 'F' ? kSexFemale : kSexMale), name, newPlaylist);
    
    if (UserManager::Instance() -> addUser(theUser))
        std::cout << "You are now registered and may now login! User ID: " << theUser -> getId() << "." << std::endl << std::endl <<  "Press any key to go back to the main menu. ";
    else
        std::cout << "There was a problem. Please try again." << std::endl << "Press any key to go back to the main menu. ";
    
    Additions::waitForReturn();
    
    Additions::clearConsole();
}

void start() {
	std::cout << "Welcome to " << (RadioStation::Instance() -> getName().compare("") ? RadioStation::Instance() -> getName() : "our radio station") << "!" << std::endl << std::endl;
    
	std::cout << "1. Existing Users: Login!" << std::endl;
	std::cout << "2. New Users: Register!" << std::endl;
    
    std::cout << std::endl;
    
    std::cout << "The ESC key can be pressed at any time to go to the previous menu (or, in this case, to terminate the program)." << std::endl;
    
    std::cout << std::endl;
    
    std::cout << "Please Select an Option." << std::endl << std::endl;
    
    while (true) {
        int opc = _getch();
        
        switch (opc) {
                
            case (baseASCIINumber + 1):
                
                Additions::clearConsole();
                
                login();
                
                break;
                
            case (baseASCIINumber + 2):
                
                Additions::clearConsole();
                
                newUser();
                
                start();
                
                break;
                
            case escKey:
                
                Additions::clearConsole();
                
                std::cout << "Radio Station (1.0)" << std::endl << std::endl;
				
#ifdef WIN32
				std::cout << "(c) 2013 MIEIC 2012/2013 - T3G12 (PROG)"; // The copyright character may not be correctly encoded under Windows.
#else
				std::cout << "© 2013 MIEIC 2012/2013 - T3G12 (PROG)"; // Works fine under OS X, though!
#endif
					
				std::cout << std::endl << std::endl << "Thanks for using this program. Please come back soon!" << std::endl << std::endl << "Press Return to terminate the process. ";
                
                Additions::waitForReturn();
                
                exit(0);
                
                break;
                
            default:
                
                break;
        }
    }
}

int main() {
    FilesIO::Instance() -> loadAllSongs();
    
    FilesIO::Instance() -> loadAllUsers();
    
    FilesIO::Instance() -> loadGlobals();
    
    RadioStation::Instance() -> generateGame();
    
	start();

	return 0;
}