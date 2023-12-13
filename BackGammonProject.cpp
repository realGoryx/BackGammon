#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "conio2.h"
#include <time.h>
#include <windows.h>


#define RT return true      
#define RF return false
#define W 0x48
#define S 0x50
#define A 0x4b
#define D 0x4d
#define P 0x25CF


// STRUCTURES

typedef struct currentGame currentGame;



// FUNCTIONS

char** b();                                                              // initializing the 2D board array
void sText();                                                            // printing text that is visible to the user at the begging of the game
void initBoard(char** board, int height, int width);                     // initializing initial placement of pawns
bool checkPossible(char** board, int x, int y, currentGame* game);       // checks if the move is possible
bool checkPossibleRed(char** board, int x, int y, currentGame * game);   // checks the possibility of move for red
bool checkPossibleWhite(char** board, int x, int y, currentGame * game); // checks the possibility of move for white
int diceThrow();                                                         // simulation of dice roll
void whoStarts(currentGame* game);                                       // indicating which player starts the game
void printInterface(int x, int y, currentGame* game);                    //  printing interface of the game
void gameStatus(currentGame* game);                                      // printing the parameters of the game status
void printBoard(char** board, int height, int width, currentGame* game); // graphically showing the game board
void printBorder();                                                      // graphically showing the borders of the game board
void nameInterface();                                                    // showing the information about the project's author
void setGraphic(char** board, int height, int width, currentGame * game, int x, int y);  // setting graphic parameters such as text color, background color
void makeTour(char** board, int height, int width, currentGame* game, int x, int y);     // moving
void checkIfInBoard(int* x, int* y);                                     // checking if cursor is out of the board
void saveGame(char** board, int height, int width, currentGame* game);   // saving current game parameters and the board
void loadGame(char** board, int* height, int* width, currentGame* game); // loading the previously saved game
void rollTheDices(currentGame* game);                                    // rolling the 2 dices to indicate the move
void killRed(char** board, currentGame* game, int x, int y);             // capturing the red pawn
void killWhite(char ** board, currentGame* game, int x, int y);          // capturing the white pawn
void returnRed(char** board, currentGame* game);                         // placing red pawn out of the home to the board
void returnWhite(char** board, currentGame* game);                       // placing white pawn out of the home to the board
void checkReturn(char** board, currentGame * game);                      // checking if the return from to the home to the board is even possible
void freeMem(char** board);                                              // freeing the previously allocated memory for the board
void updateGameStatus(currentGame* game);                                // updating the game parameters
void move(char** board, currentGame * game, int h, int w, int x, int y, int zn);  // operating on player moves
void swapThrows(currentGame * game);                                     // possibility of change the order of move
bool moveBottomRed(char** board, currentGame* game, int x, int y);       // moving the red pawn that is located at the bottom of the board
bool moveBottomWhite(char** b, currentGame* g, int x, int y);            // moving the white pawn that is located at the bottom of the board
bool moveTopRed(char** board, currentGame* game, int x, int y);          // moving the red pawn that is located at the top of the board
bool moveTopWhite(char** board, currentGame* game, int x, int y);        // moving the white pawn that is located at the top of the board
bool mWt(char** board, currentGame* game, int move);                     // moving the red pawn at the bottom of the board that previously was at the top
bool mRt(char** board, currentGame * game, int move);                    // moving the white pawn at the top of the board that previously was at the bottom
void checkIfHome(char **board, currentGame * game);                      // checking if one player can remove his pawns out of the board
void checkWhite(char** board,currentGame * game);                        // checking if white player can remove his pawns out of the board
void checkRed(char** board,  currentGame * game);                        // checking if red player can remove his pawns out of the board
bool takeOffRed(currentGame * game);                                     // procedure of removing red pawn out of the board
bool takeOffWhite(currentGame * game);                                   // procedure of removing white pawn out of the board
void checkWin(currentGame * game);                                       // checking if one of the players has won the game
void redWin(currentGame * game);                                         // procedure of adding number to score of red player
void whiteWin(currentGame * game);                                       // procedure of adding number to score of white player
void saveMoves(char** board, int height, int width);                     // saving all the moves to the seperate file
void clearMovesFile();                                                   // clearing the file containg all the moves that has occured in the previous game
void end(char** board);                                                  // procedure containing all the functions needed to finish the game
void cursor();                                                           // handling the cursor properties
void start(char** board);                                                // procedure containing all the functions needed to properly start the game

// IMPLEMENTATION STRUCTURES

struct currentGame {
    int wPawns = 15; // White pawns visible currently at board
    int rPawns = 15;  // Red pawns visible currently at board
    int wScore = 0;  // Score of white player
    int rScore = 0;  // Score of red player
    int cPlayer = 0; // 0 - white, 1 - red
    bool started = false;  // false - game hasn't started, true - game has started
    int stThrow = 0;  // first dice random number
    int ndThrow = 0; // second dice random number
    bool doneThrow = false; // false - player didn't throw the dice, true - the opposite
    bool dual = false;  // false - player didn't get the same number from 2 dices, true - opposite
    bool doneMove = false; // if player has moved already
    int movesLeft = 0;  // number of remaining moves that player has
    bool whiteHome = false;  // if white player has at least one of his pawns "killed"
    bool redHome = false;  // if red player has at least one of his pawns "killed"
    bool swap = true;  // if the player has swaped the first and second throw of the dices (only possible once)
    bool wBand = false;  // if the white player is able to put his pawns to the band
    bool rBand = false; // if the red player is able to put his pawns to the band
};




// IMPLEMENTATION OF FUNCTIONS

void initBoard(char** board, int height, int width) {

    // initializing the initial state of Backgammon board

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if ((j == 0 && i <= 1) || (j == 12 && i <= 4) || (j == 5 && i >= 8) || (j == 8 && i >= 10))
                board[i][j] = 'r';
            else if ((j == 0 && i >= 11) || (j == 12 && i >= 8) || (j == 5 && i <= 4) || (j == 8 && i <= 2))
                board[i][j] = 'w';
            else if (j == 6)
                board[i][j] = 'h';
            else
                board[i][j] = '*';

        }
    }

    // setting the title of console 

    settitle("Szymon, Rozycki, 193390");

    // setting the seed for random number generator
    // seed is generated based on the current time
        
    srand(time(NULL));
}


bool checkPossible(char** board, int x, int y, currentGame* game) {

    if (game->cPlayer == 0)
        return checkPossibleWhite(board, x, y, game);
    else
        return checkPossibleRed(board, x, y, game);


}

bool checkPossibleRed(char** board, int x, int y, currentGame* game) {
    // checking if a move chosen by a player
    // is within the restrictions of
    // game rules


    if (board[x][y] == 'r' && x == 0) {     // checking for a single pawn without "neighbors" at top of the board
        if (board[x + 1][y] == '*')
            RT;
        else
            RF;
    }
    else if (board[x][y] == 'r' && x == 12) {   // checking for a single pawn without "neighbors" at the bottom of the board
        if (board[x - 1][y] == '*')
            RT;
        else
            RF;
    }
    else if (board[x][y] == 'r' && board[x + 1][y] != 'r' && board[x - 1][y] == 'r' && x <= 4) 
        RT;
    else if (board[x][y] == 'r' && board[x - 1][y] != 'r' && board[x + 1][y] == 'r' && x >= 8)
        RT;

    // ^^^^^^
    // checking for a stacked "tower" of pawns
    // if the pawn is at top of the board
    // then the most south-located pawn can be moved
    // whereas at the bottom of the board
    // only the most north-located pawn can be moved


    RF; // if any of the statements are not met then the move isn't possible
}
bool checkPossibleWhite(char** board, int x, int y, currentGame* game) {
    // checking if a move chosen by a player
    // is within the restrictions of
    // game rules

    if (board[x][y] == 'w' && x == 0) {    // checking for a single pawn without "neighbors" at top of the board
        if (board[x + 1][y] == '*')
            RT;
        else
            RF;
    }
    else if (board[x][y] == 'w' && x == 12) {   // checking for a single pawn without "neighbors" at the bottom of the board
        if (board[x - 1][y] == '*')
            RT;
        else
            RF;
    }
    else if (board[x][y] == 'w' && board[x + 1][y] != 'w' && board[x - 1][y] == 'w' && x <= 4)
        RT;
    else if (board[x][y] == 'w' && board[x - 1][y] != 'w' && board[x + 1][y] == 'w' && x >= 8)
        RT;

    // ^^^^^^
    // checking for a stacked "tower" of pawns
    // if the pawn is at top of the board
    // then the most south-located pawn can be moved
    // whereas at the bottom of the board
    // only the most north-located pawn can be moved


    RF; // if any of the statements are not met then the move isn't possible
}


int diceThrow() {
    int dice;

    dice = (rand() % 6) + 1;   // simulation of throwing the dice, number from 1 to 6 can be the outcome 

    return dice;
}

void swapThrows(currentGame* game) {
    int temp = game->stThrow;   // swapping the throws from the first and the second dice
    game->stThrow = game->ndThrow;
    game->ndThrow = temp;
}


void sText() {
    gotoxy(20, 20);
    cputs("First, both player roll the dice. The higher number starts");
    gotoxy(20, 21);
    cputs("To roll press 'r'");
}

void whoStarts(currentGame* game) {
    int n, w, r;    // pressed key, w and r are helping variables that helps checking whose roll is higher

    sText();    // starting helping text


    while (true) {
        n = getch();

        if (n == 0x72) {
            w = diceThrow();     
            r = diceThrow();
            gotoxy(20, 23);
            printf("White: %d", w);
            gotoxy(20, 24);
            printf("Red: %d", r);

            if (w > r) {                   // checking which player gets a higher number from a single roll
                                           // the player with higher number starts the game
                game->cPlayer = 0;         // and the system assigns him to be the current player
            }
            else {

                game->cPlayer = 1;
            }
            game->started = true;
            return;
        }
    }
}


void printInterface(int x, int y, currentGame* game) {
    
    // printing overall interface of the game
    // here are implemented possible keys that user can choose from
    // and the described the usage of these keys
    
    gotoxy(50, 1);
    cputs("===== COMMANDS =====");

    gotoxy(50, 3);
    cputs("arrow keys = moving");
    gotoxy(50, 4);
    cputs("r          = roll the dices");
    gotoxy(50, 5);
    cputs("p          = make a move");
    gotoxy(50, 6);
    cputs("m          = swap the throws");
    gotoxy(50, 7);
    cputs("k          = return pawn to home");
    gotoxy(50, 8);
    cputs("s          = save the game");
    gotoxy(50, 9);
    cputs("l          = load the game");

    gameStatus(game);
    nameInterface();
}


void nameInterface() {

    // printing name, surname, index number
    // of the author of the project

    for (int i = 16; i < 19; ++i) {
            gotoxy(4, i);
            textcolor(LIGHTRED);
            putch(254);
            gotoxy(18, i);
            putch(254);
    }

    for (int i = 4; i < 19; ++i) {
        gotoxy(i, 19);
        putch(254);
    }

    textcolor(CYAN);
    gotoxy(9, 16);
    cputs("Szymon");
    gotoxy(8, 17);
    cputs("Rozycki");
    gotoxy(9, 18);
    cputs("193390");

}


void gameStatus(currentGame* game) {

    // printing to the console information about the game

    gotoxy(25, 1);
    cputs("==== GAME INFO ====");

    gotoxy(27, 3);
    cputs("Active player: ");

    if (game->cPlayer == 0)      // printing which player is currently at move
        cputs("White");
    else 
        cputs("Red");
    
    gotoxy(27, 5);
    printf("First throw: %d", game->stThrow);    // printing value of the first dice 
    
    gotoxy(27, 6);
    printf("Second throw: %d", game->ndThrow);  // printing value of the second dice
    
    gotoxy(27, 8);
    printf("White pawns: %d", game->wPawns);    // printing quantity of active white pawns
    
    gotoxy(27, 9);
    printf("Red pawns: %d", game->rPawns);    // printing quantity of active red pawns

    gotoxy(27, 11);
    printf("Left moves: %d", game->movesLeft);  // printing quantity of moves that are available 
                                                //for the current player

}


void printBoard(char** b, int height, int width, currentGame* game) {

    printBorder();

    // graphically presenting the board

    for (int i = 0; i < 13; ++i) {
        for (int j = 0; j < 13; ++j) {

            if (b[i][j] == 'r') {
                gotoxy(j + 5, i + 2);
                textcolor(4);
                putch(P);               // when 'r' or 'w' sign is detected
            }                           // red and white circles are placed
            else if (b[i][j] == 'w') {  // corresponding to the player
                gotoxy(j + 5, i + 2);
                textcolor(15);
                putch(P);
            }
            else if (b[i][j] == '*') {  // '*' is detecting an 'empty' place
                gotoxy(j + 5, i + 2);   // black square is put there
                textcolor(0);
                putch(219);
            }
        }
        cputs("\n");
    }


}


void printBorder() {

    // printing the borders of the board
    // both the bounds and the middle part

    textcolor(LIGHTRED);

    for (int j = 0; j < 13; ++j) {
        gotoxy(11, j + 2);
        textcolor(LIGHTRED);
        putch(254);
    }

    for (int i = 0; i < 15; ++i) {
        gotoxy(4, i + 1);
        putch(254);
        gotoxy(18, i + 1);
        putch(254);
    }

    for (int i = 0; i < 15; ++i) {
        gotoxy(4 + i, 1);
        putch(254);
        gotoxy(4 + i, 15);
        putch(254);
    }

}


void returnRed(char** board, currentGame* game) {
    // functions that specializes
    // in returning red pawn that has been
    // previously "killed" to the board

    int firstThrow = diceThrow(), secondThrow = diceThrow();

    game->stThrow = 0;
    game->ndThrow = 0;

    gotoxy(30, 30);
    printf("first: %d", firstThrow);
    printf("second: %d", secondThrow);
    Sleep(2500);

    // after rolling the dice twice
    // it checks if there is a place to move

    if (moveTopRed(board, game, 0, firstThrow - 1)) {   // checking if in the home
        game->rPawns++;                               // there is a place for that pawn
        if (game->rPawns == 15)
            game->redHome = false;

        return;
    }
    else if (moveTopRed(board, game, 0, secondThrow - 1)) { 
        game->rPawns++;

        if (game->rPawns == 15)
            game->redHome = false;

        return;
    }

    // if there wasn't a place in player's home
    // nothing happens - the player loses
    // his tour

}


void returnWhite(char** board, currentGame* game) {
    // functions that specializes
    // in returning red pawn that has been
    // previously "killed" to the board

    int firstThrow = diceThrow(), secondThrow = diceThrow();

    game->stThrow = 0;
    game->ndThrow = 0;

    gotoxy(30, 30);
    printf("first: %d", firstThrow);
    printf("second: %d", secondThrow);
    Sleep(2500);

    if (moveBottomWhite(board, game, 0, firstThrow - 1)) {
        game->wPawns++;
        if (game->wPawns == 15)
            game->whiteHome = false;
        return;
    }
    else if (moveBottomWhite(board, game, 0, secondThrow - 1)) {
        game->wPawns++;
        if (game->wPawns == 15)
            game->whiteHome = false;
        return;
    }

    // if there wasn't a place in player's home
    // nothing happens - the player loses
    // his tour

}


void checkReturn(char** board, currentGame* game) {
    // if one of the players' pawn
    // has been "killed" the return proceeder
    // is activated to return the pawn
    // to the board

    if (game->redHome && game->cPlayer == 1) {
        returnRed(board, game);
        game->cPlayer = 0;
        return;
    }
    else if (game->whiteHome && game->cPlayer == 0) {
        returnWhite(board, game);
        game->cPlayer = 1;
        return;
    }
}


void makeTour(char** board, int height, int width, currentGame* game, int x, int y) {


    bool possibleMove = checkPossible(board, x, y, game), moved = false;  // possibleMove - if the move is 
                                                                          // acceptable by the game rules
                                                                          // moved - if the move is succeded
    if (!possibleMove)              // if move isn't possible then
        return;                     // current player has to choose
                                    // different pawn to move


    // checking whether current player
    // is red or white then checking
    // if the move is operated at top
    // of the board or at the bottom
    // of the board

    if (game->cPlayer == 0) {
        if (x > 6)
            moved = moveBottomWhite(board, game, x, y);
        else
            moved = moveTopWhite(board, game, x, y);
    }
    else {
        if (x > 6)
            moved = moveBottomRed(board, game, x, y);
        else
            moved = moveTopRed(board, game, x, y);
    }

    // if the move has been successful
    // status of the game is being updated

    if (moved) {
        updateGameStatus(game);
        board[x][y] = '*';
        saveMoves(board, height, width);
    }

}


void updateGameStatus(currentGame* game) {
    // successful move results in 
    // decreamenting moves that the
    // current player has available

    (game->movesLeft)--;
    game->swap = false;

    if (game->movesLeft == 1 && !game->dual)
    {
        
        // swapping 1st and 2nd throw
        // because the move is being operated
        // by the first throw (that can be 
        // swapped manually as well)

        int temp = game->stThrow;
        game->stThrow = game->ndThrow;
        game->ndThrow = temp;
    }
    if (game->movesLeft == 0) {
        game->cPlayer++;

        if (game->cPlayer == 2)
            game->cPlayer = 0;

        game->doneThrow = false;
        game->stThrow = 0;
        game->ndThrow = 0;
        game->dual = false;
        game->swap = true;
    }

}


bool moveBottomRed(char** board, currentGame* game, int x, int y) {
    
    int nP = y - game->stThrow;     // new position of the pawn after the move

    if (y > 6 && nP <= 6)  // If there is a wall in the pawn's path, the position adjusts
        nP--;

    if (nP < 0)    // if the new position goes outside the map, the rule of removing pawns is checked
        return takeOffRed(game);


    for (int i = 7; i < 12; ++i) {
        if (i == 11) {
            if (board[i + 1][nP] == 'w') {      // if there's only 1 enemy pawn, capturing is taken place
                killWhite(board, game, i + 1, nP);
                RT;
            }
            if (board[i + 1][nP] == '*') {   // situation without any pawns present at this place
                board[i + 1][nP] = 'r';
                RT;
            }
        }
        if (board[i + 1][nP] == 'w')    // if more than 2 pawns are on the new position, move cannot be completed
            RF;
        if (board[i + 1][nP] == 'r') {  // if at the new position there are 5 pawns move cannot be completed, if there is maximum 4 pawns, move is possible
            if (i == 7)
                RF;
            board[i][nP] = 'r';
            RT;
        }
    }


    RF;
}

bool moveBottomWhite(char** b, currentGame* g, int x, int y) {

    int nP = y + g->stThrow;         // new position of the pawn after the move

    if (y < 6 && nP >= 6)  // If there is a wall in the pawn's path, the position adjusts
        nP++;

    if (nP > 12)    // if the new position goes outside the map, the pawns goes to another quarter of the map
        return mWt(b, g, g->stThrow - (12 - y));


    for (int i = 7; i < 12; ++i) {
        if (i == 11) {
            if (b[i + 1][nP] == 'r') {  // if there's only 1 enemy pawn, capturing is taken place
                killRed(b, g, i + 1, nP);
                RT;
            }
            if (b[i + 1][nP] == '*') {   // situation without any pawns present at this place
                b[i + 1][nP] = 'w';
                RT;
            }
        }
        if (b[i + 1][nP] == 'r')    // if more than 2 pawns are on the new position, move cannot be completed
            RF;
        if (b[i + 1][nP] == 'w') {  // if at the new position there are 5 pawns move cannot be completed, if there is maximum 4 pawns, move is possible
            if (i == 7)
                RF;
            b[i][nP] = 'w';
            RT;
        }
    }

    RF;
}


bool mWt(char** board, currentGame *game, int move) {

    int nP = 13 - move;     // new position of the pawn after the move


    for (int i = 5; i > 0; --i) {
        if (i == 1) {
            if (board[i - 1][nP] == 'r') {      // if there's only 1 enemy pawn, capturing is taken place
                killRed(board, game, i - 1, nP);
                RT;
            }
            if (board[i - 1][nP] == '*') {   // situation without any pawns present at this place
                board[i - 1][nP] = 'w';
                RT;
            }
        }
        if (board[i - 1][nP] == 'r')    // if more than 2 pawns are on the new position, move cannot be completed
            RF;
        if (board[i - 1][nP] == 'w') {  // if at the new position there are 5 pawns move cannot be completed, if there is maximum 4 pawns, move is possible
            if (i == 5)
                RF;
            board[i][nP] = 'w';
            RT;
        }

    }


    RF;
}


bool mRt(char** board, currentGame *game, int move) {

    int nP = 13 - move;     // new position of the pawn after the move


    for (int i = 7; i < 12; ++i) {
        if (i == 11) {
            if (board[i + 1][nP] == 'w') {      // if there's only 1 enemy pawn, capturing is taken place
                killWhite(board, game, i + 1, nP);
                RT;
            }
            if (board[i + 1][nP] == '*') {   // situation without any pawns present at this place
                board[i + 1][nP] = 'r';
                RT;
            }
        }
        if (board[i + 1][nP] == 'w')    // if more than 2 pawns are on the new position, move cannot be completed
            RF;
        if (board[i + 1][nP] == 'r') {  // if at the new position there are 5 pawns move cannot be completed, if there is maximum 4 pawns, move is possible
            if (i == 7)
                RF;
            board[i][nP] = 'r';
            RT;
        }

    }

    RF;
}


bool moveTopRed(char** b, currentGame* g, int x, int y) {

    int nP = y + g->stThrow;     // new position of the pawn after the move

    if (y < 6 && nP >= 6)  // If there is a wall in the pawn's path, the position adjusts
        nP++;

    if (nP > 12)    // if the new position goes outside the map, the pawns goes to another quarter of the map
        return mRt(b, g, g->stThrow - (12 - y));
    

    for (int i = 5; i > 0; --i) {
        if (i == 1) {
            if (b[i - 1][nP] == 'w') {      // if there's only 1 enemy pawn, capturing is taken place
                killWhite(b, g, i - 1, nP);
                RT;
            }
            if (b[i - 1][nP] == '*') {  // situation without any pawns present at this place
                b[i - 1][nP] = 'r';
                RT;
            }
        }
        if (b[i - 1][nP] == 'w')    // if more than 2 pawns are on the new position, move cannot be completed
            RF;
        if (b[i - 1][nP] == 'r') {  // if at the new position there are 5 pawns move cannot be completed, if there is maximum 4 pawns, move is possible
            if (i == 5)
                RF;
            b[i][nP] = 'r';
            RT;
        }

    }

    RF;
}


bool moveTopWhite(char** board, currentGame* game, int x, int y) {

    int nP = y - game->stThrow;     // new position of the pawn after the move

    if (y > 6 && nP <= 6)   // If there is a wall in the pawn's path, the position adjusts
        nP--;

    if (nP < 0)     // if the new position goes outside the map, the rule of removing pawns is checked
        return takeOffWhite(game);

    for (int i = 5; i > 0; --i) {
        if (i == 1) {
            if (board[i - 1][nP] == 'r') {      // if there's only 1 enemy pawn, capturing is taken place
                killRed(board, game, i - 1, nP);
                RT;
            }
            if (board[i - 1][nP] == '*') {  // situation without any pawns present at this place
                board[i - 1][nP] = 'w';
                RT;
            }
        }
        if (board[i - 1][nP] == 'r')    // if more than 2 pawns are on the new position, move cannot be completed
            RF;
        if (board[i - 1][nP] == 'w') {  // if at the new position there are 5 pawns move cannot be completed, if there is maximum 4 pawns, move is possible
            if (i == 5)
                RF;
            board[i][nP] = 'w';
            RT;
        }

    }

    RF;
}


void killRed(char** board, currentGame* game, int x, int y) {
    // procedure of capturing the red pawn
    // white pawn is being placed at the 
    // previous red's place

    board[x][y] = 'w';
    game->rPawns--;
    game->redHome = true;
}


void killWhite(char** board, currentGame* game, int x, int y) {
    // procedure of capturing the white pawn
    // red pawn is being placed at the 
    // previous white's place

    board[x][y] = 'r';
    game->wPawns--;
    game->whiteHome = true;
}


void checkIfInBoard(int* x, int* y) {
    // procedure of checking whether
    // a cursor is withing bound
    // of the board

    // if the player has gone out of the board
    // then his coordinates are updated accordingly

    if (*x < 5)     // Left border
        ++(*x);
    else if (*y > 14)   // Bottom border
        --(*y);
    else if (*x > 17)   // Right border
        --(*x);
    else if (*y < 2)    // Top border
        ++(*y);
}


void clearMovesFile() {
    // clears the file with the moves of some game

    FILE* history = fopen("movehistory.txt", "w");  // file is opened and there is nothing inserted - it clears its content
    if (history == NULL) {
        return;
    }

    fclose(history);
}


void saveMoves(char** board, int height, int width) {
    // function that is saving moves and constantly updating
    // that has been placed throughout the game


    FILE* history = fopen("movehistory.txt", "a");
    if (history == NULL) {
        return;
    }

    fwrite(&height, sizeof(int), 1, history);
    fwrite(&width, sizeof(int), 1, history);


    for (int i = 0; i < height; i++) {
        fwrite(board[i], sizeof(char), width, history);
    }

    fputc('\n', history);

    fclose(history);
}


void saveGame(char** board, int h, int w, currentGame* game) {
    // function that saves the current
    // state of the game (board information)
    // and information about the players attributes


    FILE* file = fopen("gamestate.txt", "w");
    if (file == NULL) {
        return;
    }

    // saving height and width of the board

    fwrite(&h, sizeof(int), 1, file);  
    fwrite(&w, sizeof(int), 1, file);   


    // saving each and every character within the board

    for (int i = 0; i < h; i++) {
        fwrite(board[i], sizeof(char), w, file);    
    }


    // saving the current game attributes

    fprintf(file, " %d %d %d %d %d %d %d %d %d %d %d %d %d",
        game->wPawns, game->rPawns, game->wScore, game->rScore,
        game->cPlayer, game->started, game->stThrow, game->ndThrow,
        game->doneThrow, game->dual, game->doneMove, game->movesLeft,
        game->swap);


    fclose(file);
}


void loadGame(char** board, int* h, int* w, currentGame* game) {
    // function that loads the saved
    // state of the game (board information)
    // and information about the players attributes
    
    FILE* file = fopen("gamestate.txt", "r");
    if (file == NULL) {
        return;
    }

    // loading height and width of the board

    fread(h, sizeof(int), 1, file);
    fread(w, sizeof(int), 1, file);

    // reading characters that are present in the board

    for (int i = 0; i < *h; i++) {
        fread(board[i], sizeof(char), *w, file);
    }

    // reading saved game parameters

    fscanf(file, "%d %d %d %d %d %d %d %d %d %d %d %d %d",
        &game->wPawns, &game->rPawns, &game->wScore, &game->rScore,
        &game->cPlayer, &game->started, &game->stThrow, &game->ndThrow,
        &game->doneThrow, &game->dual, &game->doneMove, &game->movesLeft,
        &game->swap);

    fclose(file);
}


void rollTheDices(currentGame* game) {
    // rolling dices and assigning
    // those results to the first
    // and the second throws of the player

    game->stThrow = diceThrow();
    game->ndThrow = diceThrow();
    game->doneThrow = true;         // parameter that helps to identify whether a dice roll has really occured

    if (game->stThrow == game->ndThrow)     // if 1st and 2nd throw are equal then the player has scored a dual
        game->dual = true;

    if (game->dual)             // dual means that the player has 4 available moves in his tour
        game->movesLeft = 4;
    else
        game->movesLeft = 2;    // no dual means that the player has only 2 available moves in his tour

}


void pawnKill(currentGame* game, int whichPlayer) {
    // dealing with the parameters
    // after some pawn has been captured
    
    if (whichPlayer == 0){
        game->whiteHome = true;
        game->wPawns--;
    
    }
    else {
        game->redHome = true;
        game->wPawns--;
    }

}


char** b() {

    // allocating memory for the 2D array

    char** board = (char**)malloc(13 * sizeof(char*));    
    for (int i = 0; i < 13; ++i) {
        board[i] = (char*)malloc(13 * sizeof(char));
    }

    return board;
}


void setGraphic(char** board, int height, int width, currentGame* game, int x, int y) {
    textbackground(BLACK);
    clrscr();
    textcolor(7);   // color the of visible text to the user is chosen to be light gray

    gotoxy(1, 1);
    printBoard(board, height, width, game);      // printing the board in a graphic way

    if (game->started == false)        // if the game hasn't started yet, a function needs to check who is first is needed
        whoStarts(game);

    textcolor(7);

    printInterface(x, y, game); // printing the whole interface


    gotoxy(x, y);   // going to the current x and y coordinates of the cursor
    textcolor(13);  // color of the sign is magenta
    textbackground(15); // color of the background is white
    putch(4);   // prints the sign that represents the cursor, sign is a diamond

    checkIfHome(board, game);   // constantly checking if the player is able move his pawns out of the board
    checkWin(game);     // constantly checking if the win has occured by some player
}


void freeMem(char** board) {

    // freeing the memory that has been previously allocated 

    for (int i = 0; i < 13; ++i) {    
        free(board[i]);
    }
    free(board);
}


void move(char** board, currentGame* game, int h, int w, int x, int y, int zn) {

    // dealing with input and launching
    // functions accordint to it 

    if (zn == 0x70) {
        if (game->doneThrow)
            makeTour(board, h, w, game, (y - 2), (x - 5));
    }
    else if (zn == 0x73) {
        saveGame(board, h, w, game);
    }
    else if (zn == 0x6c) {
        loadGame(board, &h, &w, game);
    }
    else if (zn == 0x72) {
        if (!game->doneThrow)
            if ((game->cPlayer == 0 && !game->whiteHome) || (game->cPlayer == 1 && !game->redHome))
                rollTheDices(game);
    }
    else if (zn == 0x6b) {
        checkReturn(board, game);
    }
    else if (zn == 0x6d && game->swap){
        swapThrows(game);
    }

}


void checkIfHome(char **board, currentGame* game) {
    // functions that checks if the player is able
    // to move his pawns out of the board

    if (game->cPlayer == 0)
        checkWhite(board, game);
    else checkRed(board, game);
}


void checkWhite(char** board, currentGame* game){
    
    int whiteCounter = 0;

    // counting white pawns
    // present in the white home

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 6; ++j) {
            if (board[i][j] == 'w')
                whiteCounter++;
        }
    }

    // if each and every white pawns
    // are present in the white's home
    // then player is able to move
    // his pawns out of the board

    if (whiteCounter == game->wPawns)
        game->wBand = true;
    else
        game->wBand = false;

}


void checkRed(char** board, currentGame* game) {

    int redCounter = 0;

    // counting red pawns
    // present in the red home

    for (int i = 7; i < 13 ; ++i) {
        for (int j = 0; j < 6; ++j) {
            if (board[i][j] == 'r')
                redCounter++;
        }
    }

    // if each and every red pawns
    // are present in the red's home
    // then player is able to move
    // his pawns out of the board

    if (redCounter == game->rPawns)
        game->rBand = true;
    else
        game->rBand = false;


}


bool takeOffRed(currentGame* game) {
    // moving red pawn out of the board

    game->rPawns--;
    
    return true;
}


bool takeOffWhite(currentGame* game) {
    // moving white pawn out of the board

    game->wPawns--;
    
    return true;
}


void checkWin(currentGame* game) {
    // checking if one of two players
    // has won the current game

    if (game->rPawns <= 0)    // no active pawns on the board means the end of the match
        redWin(game);
    else if (game->wPawns <= 0)
        whiteWin(game);
}


void redWin(currentGame* game) {

    // updating the red score
    // according to the game rules

    if (game->wPawns == 15)
        (game->rScore) += 2;
    else if (game->whiteHome)
        (game->rScore) += 3;
    else
        (game->rScore) += 1;

}


void whiteWin(currentGame* game) {

    // updating the white score
    // according to the game rules

    if (game->rPawns == 15)
        (game->wScore) += 2;
    else if (game->redHome)
        (game->wScore) += 3;
    else
        (game->wScore)++;
}


void end(char** board) {
    freeMem(board);
    gotoxy(1, 16);  // Initializing player's positiion at the end of the game in order to 'exit text' not cover the board
}


void cursor() {
    _setcursortype(_NOCURSOR);          // hide the blinking cursor
}


void start(char**board){
    clearMovesFile();
    initBoard(board, 13, 13);      // initializing pawn's initial positions
    cursor();          // hide the blinking cursor
}


// MAIN

int main() {
    int zn = 0, x = 12, y = 8, z = 0, h = 13, w = 13, m = 0;
    currentGame g;
    char** board = b();
    
    start(board);
    
    do {
        setGraphic(board, h, w, &g, x, y);

        z = 0;
        zn = getch();

        if (zn == 0) {
            z = 1;
            m = getch();
            if (m == W) y--;
            else if (m == S) y++;
            else if (m == A) x--;
            else if (m == D) x++;
            checkIfInBoard(&x, &y); 
        }
        else
            move(board, &g,h ,w ,x ,y ,zn );

    } while (zn != 'q');

    end(board);

    return 0;
}