//
//  User.cpp
//  Radio Station
//
//  T3G12 - MIEIC 2012/2013
//  Eduardo Almeida, Joao Almeida and Joao Ramos
//

#include "User.h"

#include "UserManager.h"

User::User(int userId, int age, kSex gender, std::string name, Playlist playlist) {
    _userId = userId;
    _age = age;
    _gender = gender;
    _name = name;
    _userPlaylist = playlist;
    _trackHits = 0;
    
    if (!_userId) // If user id == 0
        _isAdministrator = true;
}

User::~User() {
    
}

bool User::setUserId(unsigned int theId) {
    if (theId > UserManager::Instance() -> userCount())
        return false;
    
    _userId = theId;
    
    if (!_userId)
        _isAdministrator = true;
    
    return true;
}

const unsigned int User::getId() {
    return _userId;
}

bool User::setGender(kSex theGender) {
    if (theGender != kSexMale && theGender != kSexMale)
        return false;
    
    _gender = theGender;
    
    return true;
}

const kSex User::getGender() {
    return _gender;
}

bool User::setAge(unsigned int theAge) {
    if (!theAge)
        return false;
    
    _age = theAge;
    
    return true;
}

const unsigned int User::getAge() {
    return _age;
}

bool User::setName(std::string theName) {
    if (theName.size() <= 0)
        return false;
    
    _name = theName;
    
    return true;
}

const std::string User::getName() {
    return _name;
}

const bool User::isAdmin() {
    if (_userId)
        return false;
    
    return true;
}

bool User::setPlaylist(Playlist thePlaylist) {
    _userPlaylist = thePlaylist;
    
    return true; // Should this ever return false? :|
}

Playlist* User::getPlaylist() {
    return &_userPlaylist;
}

void User::trackPlayed() {
    _trackHits++;
}
