//
//  main.c
//  Parker_pa4
//
//  Created by Kyle Parker on 9/30/20.
//

#include "Header.h"

int main() {

    BankManager bank;
    setupGame(&bank);
    
    playGame(bank);
    
    return 0;
}
