//
//  Header.h
//  Parker_pa4
//
//  Created by Kyle Parker on 9/30/20.
//

#ifndef Header_h
#define Header_h

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <strings.h>

// Define Structures
typedef struct BankManager BankManager;

struct BankManager {
    double balance, bet, oldBalance;
    int point, points, isValid, round, userWon;
};

// Declare function stubs

void setupGame(BankManager*);
void waitForReturn(void);
void setBankBalance(double*);
void displayBalance(double,double);
void getBetForRound(int,double*,double);
void playGame(BankManager);
void displayMessage(double,double);

int getIntFromUser(void);
double getDoubleFromUser(void);
double getInputFromUserWithType(const char *restrict);
int getRandom(int);
int getYNResponse(void);
int endGame(int,int);

#endif /* Header_h */
