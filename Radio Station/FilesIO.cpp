//
//  FilesIO.cpp
//  Radio Station
//
//  T3G12 - MIEIC 2012/2013
//  Eduardo Almeida, Joao Almeida and Joao Ramos
//

#include "FilesIO.h"
#include "RadioStation.h"
#include "UserManager.h"

#include "CSVParser.h"
#include "Additions.h"

static const std::string userBase = "playListUser";
static const std::string usersFile = "users.csv";
static const std::string globalsFile = "globals.csv";
static const std::string topTenFile = "topTen.csv";
static const std::string musicStoreFile = "radioStationMusics.csv";

//
//  A system to load the play count from the files is in place.
//
//  It has been disabled, though, in order to conform with the project guidelines.
//  Enabling it is as easy as setting the following flag (bool, in this case!) to true.
//

static const bool loadPlayCountFromFile = false;

FilesIO *FilesIO::fio_pInstance = NULL;

FilesIO* FilesIO::Instance() {
    if (!fio_pInstance)
        fio_pInstance = new FilesIO;
    
    return fio_pInstance;
}

FilesIO::FilesIO() {
    
}

bool FilesIO::_writeToFile(std::string filePath, std::string contents, bool replace) {
    if (replace)
        std::remove(filePath.c_str());
    
    std::ofstream thefile(filePath);
    
    if (thefile.is_open()) {
        thefile << contents;
        
        thefile.close();
        
        return true;
    }
    
    return false;
}

//
//  Globals File Format: (prone to change!)
//  key,value
//  name,the-name
//

bool FilesIO::loadGlobals() {
    CSVParser parser(globalsFile);
    
    std::vector<std::vector<std::string>> parsedCsv = parser.tableRows(true);
    
    if (parsedCsv.size() < 1)
        return false;
    
    RadioStation::Instance()->setName(parsedCsv[0][1]);
    
    return true;
}

bool FilesIO::storeGlobals() {
    CSVParser parser(globalsFile);
    
    // Check for file existance
    
    std::ifstream thefile(globalsFile);
    
    std::vector<std::vector<std::string>> parsedCsv;
    
    if (thefile.is_open()) {
        thefile.close();
        
        parsedCsv = parser.parseCSV();
        
        if (strcmp(parsedCsv[0][0].c_str(), "name") == 0)
            parsedCsv[1][1] = RadioStation::Instance()->getName();
        else
            return false;
    } else {
        std::vector<std::string> headerVec;
        
        headerVec.push_back("key");
        headerVec.push_back("value");
        
        parsedCsv.push_back(headerVec);
        
        std::vector<std::string> kvVec;
        
        kvVec.push_back("name");
        kvVec.push_back(RadioStation::Instance()->getName());
        
        parsedCsv.push_back(kvVec);
    }
    
    std::string outCsv = parser.encodeCSV(parsedCsv);
    
    return _writeToFile(globalsFile, outCsv, true);
}

//
//  Top 10 File Format: (prone to change!)
//
//  id
//
//  We ended up removing this, as we generate this at runtime.
//

/* bool FilesIO::loadTopTen() {
    CSVParser parser(topTenFile);
    
    std::vector<std::vector<std::string>> parsedCsv = parser.tableRows(true);
    
    Playlist topTenPlaylist;
    
    if (parsedCsv.size() < 1)
        return false;
    
    bool foundErrors = false;
    
    for (unsigned int i = 0; i < parsedCsv.size(); i++) {
        std::vector<Music *> search = RadioStation::Instance()->getAllTracks()->search(atoi(parsedCsv[i][0].c_str()), "", 0, "", "", "", 0);
        
        if (search.size() > 0) {
            topTenPlaylist.addSong(search[0]*/ /*, atoi(parsedCsv[i][1].c_str())*/ /*);
        } else {
            std::cout << "Couldn't find song with ID " << parsedCsv[i][0] << ". Maybe the database is corrupt? Proceeding anyway..." << std::endl;
            
            foundErrors = true;
        }
    }
    
    if (!foundErrors)
        return true;
    
    return false;
} */

//
//  Users File Format: (prone to change!)
//
//  id,name,age,gender
//

User* FilesIO::loadUser(int userId) {
    CSVParser parser(usersFile);
    
    std::vector<std::vector<std::string>> parsedCsv = parser.tableRows(true);
    
    User *aUser = NULL;
    
    for (unsigned int i = 0; i < parsedCsv.size(); i++) {
        if (atoi(parsedCsv[i][0].c_str()) == userId) { // We found the record we want.
            std::vector<std::string> userRecord = parsedCsv[i];
            
            aUser = new User(userId,
                             atoi(userRecord[2].c_str()),
                             (atoi(userRecord[3].c_str()) == 0 ? kSexMale : kSexFemale),
                             userRecord[1],
                             playlistForUser(userId));
        }
    }
    
    return aUser;
}

int FilesIO::_userCount() {
    CSVParser parser(usersFile);
    
    return (int) parser.tableRows(true).size();
}

void FilesIO::loadAllUsers() {
    UserManager::Instance() -> removeAllUsers();
    
    for (int i = 0; i < _userCount(); i++)
        UserManager::Instance() -> addUser(loadUser(i));
}

bool FilesIO::saveUser(User *theUser) {
    // Load the whole user list
    
    CSVParser parser(usersFile.c_str());
    
    std::vector<std::vector<std::string>> userList = parser.tableRows(true);
    
    // Do the change on the row
    
    bool foundRow = false;
    
    std::vector<std::string> vecToReplace;
    
    vecToReplace.push_back(std::to_string(theUser->getId()));
    vecToReplace.push_back(theUser->getName());
    vecToReplace.push_back(std::to_string(theUser->getAge()));
    vecToReplace.push_back(std::to_string(theUser->getGender()));
    
    for (unsigned int i = 0; i < userList.size(); i++)
        if (atoi(userList[i][0].c_str()) == theUser->getId()) {
            userList[i] = vecToReplace;
            
            foundRow = true;
        }
    
    if (!foundRow)
        userList.push_back(vecToReplace);
    
    std::vector<std::vector<std::string>> newVec;
    
    std::vector<std::string> headerVec;
    
    headerVec.push_back("id");
    headerVec.push_back("name");
    headerVec.push_back("age");
    headerVec.push_back("gender");
    
    newVec.push_back(headerVec);
    
    for (unsigned int i = 0; i < userList.size(); i++)
        newVec.push_back(userList[i]);
    
    userList = newVec;
    
    // Re-convert back to CSV
    
    std::string outCsv = parser.encodeCSV(userList);
    
    // Save the changes back to the file
    
    return _writeToFile(usersFile, outCsv, true);
}

bool FilesIO::removeUser(User *theUser) {
    std::ifstream thefile(usersFile);
    
    if (!thefile.is_open())
        return false;
    
    thefile.close();
    
    CSVParser parser(usersFile.c_str());
    
    std::vector<std::vector<std::string>> userList = parser.tableRows(true);
    
    bool found = false;
    
    for (unsigned int i = 0; i < userList.size(); i++)
        if (userList[i][0].compare(std::to_string(theUser->getId()))) {
            userList[i].erase(userList[i].begin() + i);
            
            found = true;
            
            break;
        }
    
    if (!found)
        return false;
    
    std::vector<std::vector<std::string>> newVec;
    
    std::vector<std::string> headerVec;
    
    headerVec.push_back("id");
    headerVec.push_back("name");
    headerVec.push_back("age");
    headerVec.push_back("gender");
    
    newVec.push_back(headerVec);
    
    for (unsigned int i = 0; i < userList.size(); i++)
        newVec.push_back(userList[i]);
    
    userList = newVec;
    
    // Re-convert back to CSV
    
    std::string outCsv = parser.encodeCSV(userList);
    
    // Save the changes back to the file
    
    return _writeToFile(usersFile, outCsv, true);
}

//
//  Playlist File Format: (prone to change!)
//
//  musicId,playCount
//

Playlist FilesIO::playlistForUser(int userId) {
    Playlist returnPlaylist;
    
    std::string playlistFileName = "playListUser";
    
    std::ostringstream oss;
    
    oss << userId;
    
    playlistFileName += oss.str();
    
    playlistFileName += ".csv";
    
    CSVParser parser(playlistFileName);
    
    std::vector<std::vector<std::string>> rows = parser.tableRows(true);
    
    for (unsigned int i = 0; i < rows.size(); i++) {
        std::vector<Music *> searchResult = RadioStation::Instance() -> getAllTracks() -> search(atoi(rows[i][0].c_str()), "", "", "", "", "", -1);
        
        if (searchResult.size() != 1) {
            std::cout << "A music track wasn't found for ID " << rows[i][0] << ", or the result was ambiguous. Please look into this!" << std::endl;
        } else
            returnPlaylist.addSong(searchResult[0]);
    }
    
    return returnPlaylist;
}

Playlist FilesIO::playlistForUser(std::string userName) {
    return playlistForUser(UserManager::Instance()->getUser(userName)->getId());
}

bool FilesIO::storePlaylistForUser(int userId) {
    Playlist *userPlaylist = UserManager::Instance()->getUser(userId)->getPlaylist();
    
    std::vector<Music *> result = userPlaylist -> getAllTracks();
    
    std::vector<std::vector<std::string>> csvVec;
    
    std::vector<std::string> headerVec;
    
    headerVec.push_back("musicId");
    headerVec.push_back("playCount");
    
    csvVec.push_back(headerVec);
    
    for (unsigned int i = 0; i < result.size(); i++) {
        Music *song = result[i];
        
        std::vector<std::string> songVec;
        
        songVec.push_back(std::to_string(song->getId()));
        songVec.push_back(std::to_string(song->getPlayCount()));
        
        csvVec.push_back(songVec);
    }
    
    CSVParser parser;
    
    std::string outCsv = parser.encodeCSV(csvVec);
    
    std::string userFileName = userBase;
    
    userFileName.append(std::to_string(userId));
    
    userFileName.append(".csv");
    
    return _writeToFile(userFileName, outCsv, true);
}

//
//  All Songs File Format: (prone to change!)
//
//  musicId,title,artist,album,author,genre,year,likes,dislikes,cpc,available
//
//  cpc means comulative play count
//

bool FilesIO::loadAllSongs() {
    CSVParser parser(musicStoreFile);
    
    std::vector<std::vector<std::string>> parsedCsv = parser.tableRows(true);
    
    if (parsedCsv.size() < 1)
        return false;
    
    bool allWentGood = true;
    
    for (unsigned int i = 0; i < parsedCsv.size(); i++) {
        std::vector<std::string> musicData = parsedCsv[i];
        
        Music *newSong = new Music(atoi(musicData[0].c_str()), atoi(musicData[6].c_str()), musicData[1], musicData[2], musicData[4], musicData[3], musicData[5], atoi(musicData[7].c_str()), atoi(musicData[8].c_str()), (loadPlayCountFromFile ? atoi(musicData[9].c_str()) : 0), (atoi(musicData[10].c_str()) ? true : false));
        
        if (!RadioStation::Instance() ->getAllTracks() -> addSong(newSong)) {
            std::cout << "An error has occoured while loading the track with ID " << newSong -> getId() << "." << std::endl;
            
            allWentGood = false;
        }
    }
    
    return allWentGood;
}

bool FilesIO::saveAllSongs() {
    Playlist *playlist = RadioStation::Instance() -> getAllTracks();
    
    std::vector<Music *> result = playlist -> getAllTracks();
    
    std::vector<std::vector<std::string>> csvVec;
    
    std::vector<std::string> headerVec;
    
    //  musicId,title,artist,album,author,genre,year,likes,dislikes,cpc,available
    
    headerVec.push_back("musicId");
    headerVec.push_back("title");
    headerVec.push_back("artist");
    headerVec.push_back("album");
    headerVec.push_back("author");
    headerVec.push_back("genre");
    headerVec.push_back("year");
    headerVec.push_back("likes");
    headerVec.push_back("dislikes");
    headerVec.push_back("cpc");
    headerVec.push_back("available");
    
    csvVec.push_back(headerVec);
    
    for (unsigned int i = 0; i < result.size(); i++) {
        Music *song = result[i];
        
        std::vector<std::string> songVec;
        
        songVec.push_back(std::to_string(song->getId()));
        songVec.push_back(song->getTitle());
        songVec.push_back(song->getArtist());
        songVec.push_back(song->getAlbum());
        songVec.push_back(song->getAuthor());
        songVec.push_back(song->getGenre());
        songVec.push_back(std::to_string(song->getYear()));
        songVec.push_back(std::to_string(song->getLikes()));
        songVec.push_back(std::to_string(song->getDislikes()));
        songVec.push_back(std::to_string(song->getPlayCount()));
        songVec.push_back(song->getAvailable() ? "1" : "0");
        
        csvVec.push_back(songVec);
    }
    
    CSVParser parser;
    
    std::string outCsv = parser.encodeCSV(csvVec);
    
    return _writeToFile(musicStoreFile, outCsv, true);
}
