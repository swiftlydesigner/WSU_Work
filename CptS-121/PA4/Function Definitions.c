//
//  Function Definitions.c
//  Parker_pa4
//
//  Created by Kyle Parker on 9/30/20.
//

#include "Header.h"

/*
 * Function: setupGame(BankManager)
 * Date Created: 10/05/20
 * Date Modified: 10/08/20
 * Input Parameters: bm: setValues to structure as needed.
 * Description: Display the rules for craps. Setup vars as needed.
 * Postconditions: Rules are displayed and required vars are stored.
 */

void setupGame(BankManager* bm) {
    srand((unsigned int)time(NULL));
    
    if (bm->balance != NAN) {
        bm->oldBalance = bm->balance;
    }
    
    printf("A player rolls two dice. Each die has six faces. These faces contain 1, 2, 3, 4, 5, and 6 spots. After the dice have come to rest, the sum of the spots on the two upward faces is calculated. If the sum is 7 or 11 on the first throw, the player wins. If the sum is 2, 3, or 12 on the first throw (called \"craps\"), the player loses (i.e. the \"house\" wins). If the sum is 4, 5, 6, 8, 9, or 10 on the first throw, then the sum becomes the player's \"point.\" To win, you must continue rolling the dice until you \"make your point.\" The player loses by rolling a 7 before making the point.\n\n(Press return to continue)");
    
    waitForReturn();
    
    printf("Please enter a starting bank balance: ");
    
    double balance = getDoubleFromUser();
    
    displayBalance(balance, NAN);
    
    bm->balance = balance;
    
    if (bm->oldBalance == NAN) {
        bm->oldBalance = bm->balance;
    }
    
    bm->round = 0;
    bm->isValid = 1;
    bm->point = -1;
    bm->points = 0;
}


/*
 * Function: waitForReturn()
 * Date Created: 10/05/20
 * Date Modified: 10/05/20
 * Description: wait for new line character, clear stdin stream upto first new line character.
 * Preconditions: User is prompted and expected to press return at one point.
 * Postconditions: User pressed return, and stdin is cleared up to first return character.
 */

void waitForReturn() {
    char rtnVal = '\0';
    scanf("%c", &rtnVal);
    if (rtnVal != '\n') { return waitForReturn(); }
    return;
}

///*
// * Function: setBankBalance(BankManager*)
// * Date Created: 10/08/20
// * Date Modified: 10/08/20
// * Description: get user input for staring amount.
// * Input Parameters: bm: store starting amount in the struture located at the memory addr.
// * Preconditions: bm is not NULL, with a valid address.
// * Postconditions: user input is valid and initial amount is set
// */
//
//void setBankBalance(double* balance) {
//    scanf("%lf",balance);
//}

/*
 * Function: updateBalance(BankManager*, double)
 * Date Created: 10/09/20
 * Date Modified: 10/09/20
 * Description: Update the balance of user and display current balance. If needed, end game.
 * Input Parameters: bm: set the balance to this structure. difference: positive will deduct, negative will add to balance.
 * Preconditions: bm has valid address
 * Postconditions: balance is update and displayed
 */

void displayBalance(double balance, double oldVal) {
    system("cls");
    printf("Current balance: $%.2lf\n", balance);
    displayMessage(balance, oldVal);
}

/*
 * Function: getIntFromUser()
 * Date Created: 10/11/20
 * Date Modified: 10/11/20
 * Description: Get input from the user, scan for first int.
 * Returns: An integer from the user
 * Postconditions: an integer from the user is returned.
 */

int getIntFromUser() {
    int tmpVar;
    scanf("%d",&tmpVar);
    return tmpVar;
}

/*
 * Function: getDoubleFromUser()
 * Date Created: 10/11/20
 * Date Modified: 10/11/20
 * Description: Get input from the user, scan for first long float.
 * Returns: A double from the user.
 * Postconditions: A double from the user is returned.
 */

double getDoubleFromUser() {
    double tmpVar;
    scanf("%lf",&tmpVar);
    return tmpVar;
}

/*
 * Function: getBetForRount(int,double*)
 * Date Created: 10/11/20
 * Date Modified: 10/11/20
 * Description: Get a bet for the current round.
 * Input Parameters: round: the round number, bet: pointer to the bet.
 * Preconditions: round is > 0
 * Postconditions: the bet is set.
 */

void getBetForRound(int round, double* bet, double balance) {
    printf("Enter a bet (less than %.2lf) for round %d: ", balance, round);
    scanf("%lf",bet);
    if (*bet > balance) {
        printf("INVLAID! BET MUST BE LESS THAN YOUR CURRENT BALANCE!\n");
        return getBetForRound(round, bet, balance);
    }
    printf("\n");
}

/*
 * Function: endGame()
 * Date Created: 10/11/20
 * Date Modified: 10/11/20
 * Description: End the game when the user wins or loses.
 * Preconditions: BankManger.isValid must be false.
 * Postconditions: Game exited.
 */

int endGame(int didWin, int points) {
    if (didWin) {
        printf("You won with %d points!\nPlay again? (y/n): ", points);
    } else {
        printf("The house won! Play again? (y/n): ");
    }
    
    return getYNResponse();
}

/*
 * Function: getRandom(int)
 * Date Created: 10/11/20
 * Date Modified: 10/11/20
 * Description: Return a random number with a max of `max`
 * Input Parameters: max: The max number random can be
 * Returns: a random number between 1 and `max`, inclusive
 * Preconditions: max is > 0
 * Postconditions: a random number in range is returned
 */

int getRandom(int max) {
    return rand() % max + 1;
}

/*
 * Function: getYNResponse()
 * Date Created: 10/11/20
 * Date Modified: 10/11/20
 * Description: returns 1 or zero, depending on user input, y/n.
 * Returns: response from user as 1 or 0 from yn response.
 * Preconditions: user prompted to enter y or n
 * Postconditions: user responded with y or n
 */

int getYNResponse() {
    char resp;
    scanf("%c", &resp);
    if (resp == 'y' || resp == 'Y') {
        return 1;
    } else if (resp == 'n' || resp == 'N') {
        return 0;
    } else if (resp == '\n' || resp == '\t') {
        return getYNResponse();
    }
    
    printf("INVALID INPUT. ENTER Y OR N: ");
    
    return getYNResponse();
}

/*
 * Function: playGame(BankManger)
 * Date Created: 10/11/20
 * Date Modified: 10/11/20
 * Description: Game play here. Calls all needed functions.
 * Input Parameters: bank: a BankManger instance.
 * Preconditions: `bank` is setup.
 * Postconditions: Game is over.
 */

void playGame(BankManager bank) {
    while (bank.isValid) {
        bank.round += 1;
        
        printf("Bet this round? y/n: ");
        if (!getYNResponse()) {
            bank.isValid = 0;
            bank.userWon = 1;
            break;
        }
        
        getBetForRound(bank.round, &bank.bet, bank.balance);
        int rand1 = getRandom(6), rand2 = getRandom(6), total = rand1 + rand2;
        
        printf("Die 1: %d\tDie 2: %d\tTotal: %d\n", rand1, rand2, total);
        
        if (bank.round == 1) {
            switch (total) {
            case 7: case 11:
                bank.isValid = 0;
                bank.userWon = 1;
                bank.balance += bank.bet;
                break;
            case 2: case 3: case 12:
                bank.isValid = 0;
                bank.userWon = 0;
                break;
            default:
                bank.point = total;
                bank.points = total;
                bank.balance += bank.bet;
                break;
            }
        } else if (total == 7) {
            bank.isValid = 0;
            bank.userWon = 0;
        } else if (bank.point == total) {
            bank.balance += bank.bet;
        }
        
        displayBalance(bank.balance, bank.oldBalance);
    }
    
    if (endGame(bank.userWon, bank.points)) {
        setupGame(&bank);
        return playGame(bank);
    }
}



void displayMessage(double new, double old) {
    if (old > new) {
        printf("You are losing! haha!");
    } else if (new > old) {
        printf("So far you are ahead, hopefully the house doesn't steal you money soon...\n");
    } else {
        printf("You broke even this time around!\n");
    }
}
