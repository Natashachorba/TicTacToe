/*
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

void ticTacToeBoard(const vector<vector<char> >& board){
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

int move(vector<vector<char> >& board, int x, int y, char player){
  board[x][y] = player;
  int boardSize = board.size();
  
  for (int c = 0; c < boardSize; c++){
    if (board[x][c] != player){break;}
    if (c == boardSize - 1){return 1;}
  }
  for (int r = 0; r < boardSize; r++){
    if (board[r][y] != player){break;}
    if (r == boardSize - 1){return 1;}
  }
  if (x == y){
    for (int diag = 0; diag < boardSize; diag++){
      if (board[diag][diag] != player){break;}
      if (diag == boardSize - 1){return 1;}
    }
  }
  if (x + y == boardSize - 1){
    for (int ndiag = 0; ndiag < boardSize; ndiag++){
      if (board[ndiag][boardSize - 1 - ndiag] != player){break;}
      if (ndiag == boardSize - 1){return 1;}
    }
  }
  return 0;
}

//returns true as long as the location is unfilled and x & y are valid
int validateLocation(const vector<vector<char> >& myBoard, int x, int y){
  int size = myBoard.size();
  int xvalid = (x < size && x >= 0);
  int yvalid = (y < size && y >= 0);
  return xvalid && yvalid && (myBoard[x][y] == ' ');
}

int playTTT(int boardSize){
  //create a NxN board (I'm sure there's a better way but idk)
  vector<vector<char> > myBoard;
  for (int i=0;i<boardSize;i++){
    vector<char> temp;
    for (int j=0;j<boardSize;j++){
      temp.push_back(' ');
    }
    myBoard.push_back(temp);
  }
  cout<< "Starting a game of Tic Tac Toe"<<endl;
  ticTacToeBoard(myBoard);
  int turnsTaken = 0;
  char whosTurn = 'X';
  
  while (turnsTaken<(boardSize*boardSize)){
    int row; int col;
    cout<<"Enter a row and column seperated by a space (1-"<<boardSize<<"): ";
    cin>> row >> col;
    while (!validateLocation(myBoard, row-1, col-1)){
      cout<<"Don't try to cheat! That space is no longer valid!"
          <<" Enter a new pair: ";
      cin>> row >> col;
    }
    
    if (move(myBoard,row-1,col-1,whosTurn)){
      ticTacToeBoard(myBoard);
      return (whosTurn == 'X') ? 1 : 2;
    }
    ticTacToeBoard(myBoard);
    ++turnsTaken;
    whosTurn = (whosTurn == 'X') ? 'O' : 'X';
  }
  
  return 0;
}

int main(int argc, char *argv[]){
  int boardSize = 3; //maybe should be changeable? idk
  string firstPlayer = "Player 1";
  string secondPlayer = "AI";
  
  //player can enter their name and enter the opponents name (if not, seoncd is AI)
  if (argc>1){
    firstPlayer = argv[1];
    if (argc>2){
      secondPlayer = argv[2];
    }
  }
  
  while (boardSize>0){
    int result = playTTT(boardSize);
    if (result == 1){
      cout<<firstPlayer<<" won!"<<endl;
    }
    else if (result == 2){
      cout<<secondPlayer<<" won!"<<endl;
    }
    else {
      cout<<"The game ended in a draw!"<<endl;
    }
    
    char playAgain;
    cout<<"Do you want to play again? Y/N : ";
    cin >> playAgain;
    if (toupper(playAgain) != 'Y'){break;}
    cout<<endl;
  }
}