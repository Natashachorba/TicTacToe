/* simple hangman game with an AI opponent option
*/
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

//simple struct to keep track of the tic tac toe game/players
struct ticTacToe{
  vector<vector<char> > board;
  string player1;
  string player2;
};

//initializes the board
void initBoard(vector<vector<char> >& board, int boardSize){
  for (int i=0;i<boardSize;i++){
    vector<char> temp;
    for (int j=0;j<boardSize;j++){
      temp.push_back(' ');
    }
    board.push_back(temp);
  }
}

//resets the TicTacToe board to a blank state
void resetBoard(vector<vector<char> >& board){
  int boardSize = board.size();
  for (int i=0;i<boardSize;i++){
    for (int j=0;j<boardSize;j++){
      board[i][j] = ' ';
    }
  }
}

//prints the Tic Tac Toe Board with a basic grid
void printBoard(const vector<vector<char> >& board){
  int boardSize = board.size();
  for (int i=0;i<boardSize;i++){
    cout<<"\t"<<board[i][0];
    for (int j=1;j<boardSize;j++){
      cout<<" | "<<board[i][j];
    }
    if (i<boardSize-1){
      cout<<endl<<"\t"<<string(boardSize*boardSize, '-')<<endl;
    }
  }
  cout<<endl;
}

//queries the user as to # of players and gets player names as needed
bool GamePlayers(ticTacToe &myGame){
  char response;
  cout<<"Would you like to play against the AI? Y/N : ";
  cin>>response;
  cout<<"What is Player 1's name? ";
  cin>>myGame.player1;
  if (toupper(response) == 'Y'){
    myGame.player2 = "The AI";
    return true;
  }
  cout<<"And What is Player 2's name? ";
  cin>>myGame.player2;
  return false;
}

//returns true as long as the location is unfilled and x & y are valid
int validateLocation(const vector<vector<char> >& myBoard, int x, int y){
  int size = myBoard.size();
  int xvalid = (x < size && x >= 0);
  int yvalid = (y < size && y >= 0);
  return xvalid && yvalid && (myBoard[x][y] == ' ');
}

//handles the input from players and applying input move, returns entered row/column
int playerMove(vector<vector<char> >& board, char player){
  int row; int col;
  cout<<player<<"'s: Enter a row and column seperated by a space (1-"
      <<board.size()<<"): ";
  cin>> row >> col;
  while (!validateLocation(board, row-1, col-1)){
    cout<<"Don't try to cheat! That space is no longer valid!"
        <<" Enter a new pair: ";
    cin>> row >> col;
  }
  board[row-1][col-1] = player;
  return (row-1)*10+(col-1); //returned for checkWin referencing
}

//checks if the given player has won with their most recent move
int checkWin(vector<vector<char> >& board, char player){
  int boardSize = board.size();
  
  for (int x = 0; x < boardSize;x++){ //check each row check the cols
    for (int c = 0; c < boardSize; c++){
      if (board[x][c] != player){break;}
      if (c == boardSize - 1){return 1;}
    }
  }
  for (int y = 0; y < boardSize;y++){
    for (int r = 0; r < boardSize; r++){
      if (board[r][y] != player){break;}
      if (r == boardSize - 1){return 1;}
    }
  }
  for (int diag = 0; diag < boardSize; diag++){
    if (board[diag][diag] != player){break;}
    if (diag == boardSize - 1){return 1;}
  }
  for (int ndiag = 0; ndiag < boardSize; ndiag++){
    if (board[ndiag][boardSize - 1 - ndiag] != player){break;}
    if (ndiag == boardSize - 1){return 1;}
  }
  return 0;
}

//recursive ai function to determine best score from each outcome
int negaMax(vector<vector<char> >& board, char player1, char player2){
  int bestScore = -9999;
  int thisScore = 0;
  
  if (checkWin(board, player1)){
    return 1000;
  }
  else if (checkWin(board, player2)){
    return -1000;
  }
  
  for (int i = 0; i < board.size(); i++){
    for (int j = 0; j < board.size(); j++){
      if (board[i][j] == ' '){
        board[i][j] = player1; //make a test move
        thisScore = -negaMax(board, player2, player1);
        board[i][j] = ' '; //reset the test move
        if (thisScore >= bestScore){
          bestScore = thisScore;
        }
      }
    }
  }
  
  if (bestScore == -9999 || bestScore == 0){
    return 0;
  }
  else if (bestScore < 0){
    return bestScore + 1;
  }
  else if(bestScore > 0){
    return bestScore - 1;
  }
}

//function to determine the best move for the ai using negaMax
int aiMove(vector<vector<char> >& board, char aiPlayer){
  int bestRow; int bestCol; int thisScore = 0;
  int bestScore=-9999;
  int boardSize = board.size();
  char player = (aiPlayer == 'X') ? 'O' : 'X';
  
  for (int i = 0; i < boardSize; i++){
    for (int j = 0; j < boardSize; j++){
      if (board[i][j] == ' '){
        board[i][j] = aiPlayer;
        thisScore = -negaMax(board, player, aiPlayer);
        board[i][j] = ' '; //reset the test move
        if (thisScore > bestScore){
          bestScore = thisScore;
          bestRow = i;
          bestCol = j;
        }
      }
    }
  }
  board[bestRow][bestCol] = aiPlayer;
  return bestRow*10+bestCol;
}

//runs a simple 2 player TicTacToe game
void playTTT(ticTacToe &game, bool ai){
  //used for determining who goes first
  srand(time(NULL));
  string whosGoing = rand()%2 ? game.player1 : game.player2;
  cout<< whosGoing << " gets to play as X's this game!" << endl;
  
  printBoard(game.board);
  char whosTurn = 'X';
  int coords;
  for (int moves=0;moves<game.board.size()*game.board.size();moves++){
    if (ai && whosGoing == "The AI"){
      cout<<"The computer makes its move"<<endl;
      aiMove(game.board, whosTurn);
    } else {
      playerMove(game.board, whosTurn);
    }
    
    printBoard(game.board);
    if (checkWin(game.board, whosTurn)){
      cout<< whosGoing << " won this game!"<<endl;
      return;
    }
    //now we simply swap who's going
    whosTurn = (whosTurn == 'X') ? 'O' : 'X';
    whosGoing = (whosGoing == game.player1) ? game.player2 : game.player1;
  }
  cout<<"The result is a Cat's Game this time!"<<endl;
}

int main(int argc, char *argv[]){
  int boardSize = 3; //maybe should be changeable? (after testing bigger numbers nah)
  ticTacToe game;
  int autoGame = GamePlayers(game);
  initBoard(game.board,boardSize);
  
  while (boardSize > 0){
    playTTT(game,autoGame);
    
    char playAgain;
    cout<<"Do you want to play again? Y/N : ";
    cin >> playAgain;
    if (toupper(playAgain) != 'Y'){break;}
    cout<<endl;
    resetBoard(game.board);
  }
}

