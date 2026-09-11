#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>
using namespace std;

class TicTacToe {
  char Board[3][3];
  int emptyLoc = 9;
  int difficulty = 1; // 1=Easy, 2=Medium, 3=Hard

public:
  TicTacToe();

  void printBoard();
  bool isMoveLeft();
  int eval();

  int minMax(bool is_Max, int alpha, int beta);

  void findBestMove();
  void findRandomMove();

  bool makeMove(int row, int col, char player);

  bool checkGameOver();

  void playGame();
};

TicTacToe::TicTacToe() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      Board[i][j] = ' ';
    }
  }
}

bool TicTacToe::makeMove(int row, int col, char player) {
  if (row >= 0 && row < 3 && col >= 0 && col < 3 && Board[row][col] == ' ') {
    Board[row][col] = player;
    if (emptyLoc > 0) {
      emptyLoc--;
    };
    return true;
  }
  return false;
}

void TicTacToe::printBoard() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      cout << Board[i][j];
    }
    cout << endl;
  }
}

bool TicTacToe::isMoveLeft() { return emptyLoc > 0; }

int TicTacToe::eval() {
  for (int i = 0; i < 3; i++) {
    if (Board[i][0] == Board[i][1] && Board[i][1] == Board[i][2] &&
        Board[i][0] != ' ') {
      if (Board[i][0] == 'X') {
        return 10;
      } else {
        return -10;
      }
    }
  }

  for (int i = 0; i < 3; i++) {
    if (Board[0][i] == Board[1][i] && Board[1][i] == Board[2][i] &&
        Board[0][i] != ' ') {
      if (Board[0][i] == 'X') {
        return 10;
      } else {
        return -10;
      }
    }
  }

  if (Board[0][0] == Board[1][1] && Board[1][1] == Board[2][2] &&
      Board[0][0] != ' ') {
    if (Board[0][0] == 'X') {
      return 10;
    } else {
      return -10;
    }
  }

  if (Board[0][2] == Board[1][1] && Board[1][1] == Board[2][0] &&
      Board[0][2] != ' ') {
    if (Board[0][2] == 'X') {
      return 10;
    } else {
      return -10;
    }
  }

  return 0;
}

// Minimax algorithm: X (P1) is maximizer, O (P2) is minimizer
int TicTacToe::minMax(bool is_Max, int alpha, int beta) {
  int val = eval();

  if (val != 0)
    return val; // someone won (+10 or -10)
  if (emptyLoc == 0)
    return 0; // board full, draw

  if (is_Max == true) {
    int mx = -10;
    bool pruned = false;
    for (int i = 0; i < 3 && !pruned; i++) {
      for (int j = 0; j < 3; j++) {
        if (Board[i][j] == ' ') {
          Board[i][j] = 'X';
          emptyLoc--;
          val = minMax(false, alpha, beta);
          Board[i][j] = ' '; // always undo
          emptyLoc++;
          mx = max(mx, val);
          alpha = max(alpha, val);
          if (alpha >= beta) {
            pruned = true;
            break;
          }
        }
      }
    }
    return mx;
  } else {
    int mn = 10;
    bool pruned = false;
    for (int i = 0; i < 3 && !pruned; i++) {
      for (int j = 0; j < 3; j++) {
        if (Board[i][j] == ' ') {
          Board[i][j] = 'O';
          emptyLoc--;
          val = minMax(true, alpha, beta);
          Board[i][j] = ' '; // always undo
          emptyLoc++;
          mn = min(mn, val);
          beta = min(beta, val);
          if (alpha >= beta) {
            pruned = true;
            break;
          }
        }
      }
    }
    return mn;
  }
}

// Pick a random empty cell for the computer
void TicTacToe::findRandomMove() {
  int emptyCells[9][2];
  int count = 0;
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      if (Board[i][j] == ' ') {
        emptyCells[count][0] = i;
        emptyCells[count][1] = j;
        count++;
      }

  int pick = rand() % count;
  int r = emptyCells[pick][0], c = emptyCells[pick][1];
  Board[r][c] = 'X';
  emptyLoc--;
  cout << "Computer plays at (" << r + 1 << ", " << c + 1 << ")" << endl;
}

// Find and play the best move for the computer (P1 = X)
void TicTacToe::findBestMove() {
  bool useRandom = false;
  if (difficulty == 1) {
    // Easy: 80% chance of a random move
    useRandom = (rand() % 100) < 80;
  } else if (difficulty == 2) {
    // Medium: 40% chance of a random move
    useRandom = (rand() % 100) < 40;
  } else if (difficulty == 3) {
    // Hard: 10% chance of a random move (gives the player a slight chance to win)
    useRandom = (rand() % 100) < 10;
  }
  // difficulty 4 (Unbeatable) is 0% random

  if (useRandom) {
    findRandomMove();
    return;
  }

  int bestVal = -10;
  int bestRow = -1, bestCol = -1;

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (Board[i][j] == ' ') {
        Board[i][j] = 'X';
        emptyLoc--;
        int val = minMax(false, INT_MIN, INT_MAX);
        if (val > bestVal) {
          bestRow = i;
          bestCol = j;
          bestVal = val;
        }
        Board[i][j] = ' ';
        emptyLoc++;
      }
    }
  }

  Board[bestRow][bestCol] = 'X';
  emptyLoc--;
  cout << "Computer plays at (" << bestRow + 1 << ", " << bestCol + 1 << ")"
       << endl;
}

// Check if the game is over (win or draw). Returns true if game ended.
bool TicTacToe::checkGameOver() {
  int score = eval();

  if (score == 10) {
    printBoard();
    cout << "Computer (X) wins!" << endl;
    return true;
  }
  if (score == -10) {
    printBoard();
    cout << "You (O) win! Congratulations!" << endl;
    return true;
  }
  if (!isMoveLeft()) {
    printBoard();
    cout << "It's a draw!" << endl;
    return true;
  }

  return false;
}

// Main game loop: human plays O, computer plays X
void TicTacToe::playGame() {
  srand(time(0));
  int row, col;

  cout << "=== Tic Tac Toe ===" << endl;
  cout << "Select difficulty:" << endl;
  cout << "  1. Easy       (computer mostly plays randomly)" << endl;
  cout << "  2. Medium     (computer makes mistakes often)" << endl;
  cout << "  3. Hard       (computer is tough, but you have a chance to win!)" << endl;
  cout << "  4. Unbeatable (computer plays perfectly)" << endl;
  cout << "Choice (1-4): ";
  cin >> difficulty;
  if (difficulty < 1 || difficulty > 4) difficulty = 2;
  cout << endl;

  cout << "You are O, Computer is X." << endl;
  cout << "Enter moves as row and column (1-3)." << endl << endl;

  printBoard();

  while (true) {
    // Human's turn (O)
    cout << endl << "Your turn (row col): ";
    cin >> row >> col;
    row--;
    col--; // convert to 0-indexed

    if (!makeMove(row, col, 'O')) {
      cout << "Invalid move! Try again." << endl;
      continue;
    }

    if (checkGameOver())
      break;

    // Computer's turn (X)
    cout << endl;
    findBestMove();

    if (checkGameOver())
      break;

    cout << endl;
    printBoard();
  }
}

int main() {
  TicTacToe game;
  game.playGame();
  return 0;
}