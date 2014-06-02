//
//  UserManager.h
//  Radio Station
//
//  T3G12 - MIEIC 2012/2013
//  Eduardo Almeida, Joao Almeida and Joao Ramos
//
//  This class manages the different users and their permissions.
//

#ifndef UserManagerHeader
#define UserManagerHeader

#include <iostream>
#include <string>
#include <vector>

#include "User.h"

class UserManager {
    
private:
    
	UserManager(); // There can only be one user manager, so a singleton here is completely appropriate.

	UserManager(UserManager const&); // Copy constructor is private.
	UserManager& operator=(UserManager const&); // Assignment operator is private.

	static UserManager *um_pInstance;

	std::vector<User *> _userVector;
    
    User *_prizeWinner;

public:
    
	static UserManager* Instance();

	bool addUser(User *aUser);

	bool removeUser(User *aUser);
	bool removeUser(int userId);
    
    void removeAllUsers();
    
    User* getUser(int userId);
    User* getUser(std::string userName);
    
    const unsigned int userCount();
    
    User *getPrizeWinner();
    
    void resetPrizeWinner();

};

#endif