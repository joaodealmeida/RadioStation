//
//  UserManager.cpp
//  Radio Station
//
//  T3G12 - MIEIC 2012/2013
//  Eduardo Almeida, Joao Almeida and Joao Ramos
//

#include "UserManager.h"

#include "FilesIO.h"

UserManager *UserManager::um_pInstance = NULL;

UserManager* UserManager::Instance() {
    if (!um_pInstance)
        um_pInstance = new UserManager;
    
    return um_pInstance;
}

UserManager::UserManager() {
    _prizeWinner = NULL;
}

bool UserManager::addUser(User *aUser) {
    for (unsigned int i = 0; i < _userVector.size(); i++)
        if (_userVector[i] -> getId() == aUser -> getId())
            return false;
    
    _userVector.push_back(aUser);
    
    FilesIO::Instance() -> saveUser(aUser);
    
    return true;
}

bool UserManager::removeUser(User *aUser) {
    for (unsigned int i = 0; i < _userVector.size(); i++)
        if (_userVector[i] -> getId() == aUser -> getId()) {
            _userVector.erase(_userVector.begin() + i);
            
            return true;
        }
    
    return false;
}

bool UserManager::removeUser(int userId) {
    for (unsigned int i = 0; i < _userVector.size(); i++)
        if (_userVector[i] -> getId() == userId) {
            _userVector.erase(_userVector.begin() + i);
            
            return true;
        }
    
    return false;
}

void UserManager::removeAllUsers() {
    _userVector.clear();
}

User* UserManager::getUser(int userId) {
    for (unsigned int i = 0; i < _userVector.size(); i++)
        if (_userVector[i] -> getId() == userId)
            return _userVector[i];
    
    return NULL;
}

User* UserManager::getUser(std::string userName) {
    for (unsigned int i = 0; i < _userVector.size(); i++)
        if (!(_userVector[i] -> getName().compare(userName)))
            return _userVector[i];
    
    return NULL;
}

void UserManager::resetPrizeWinner() {
    _prizeWinner = NULL;
}

User* UserManager::getPrizeWinner() {
    if (!_prizeWinner) {
        User *currentWinner = NULL;
        int currentCount = 0;
        
        for (unsigned int i = 0; i < _userVector.size(); i++) {
            std::vector<Music *> songVec = _userVector[i] -> getPlaylist() -> getAllTracks();
            
            int userCount = 0;
            
            for (unsigned int j = 0; j < songVec.size(); j++)
                if (songVec[j] -> getAvailable())
                    userCount += songVec[j] -> getPlayCount();
            
            if (userCount > currentCount) {
                currentWinner = _userVector[i];
                currentCount = userCount;
            }
        }
        
        _prizeWinner = currentWinner;
    }
    
    return _prizeWinner;
}

const unsigned int UserManager::userCount() {
    return (const unsigned int) _userVector.size();
}
