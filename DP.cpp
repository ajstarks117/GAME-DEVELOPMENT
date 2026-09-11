#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <iomanip>

using namespace std;

class TicTacToe
{
private:

    char Board[3][3];
    int emptyLoc;

    // Difficulty:
    // 1 = Easy
    // 2 = Medium
    // 3 = Hard
    // 4 = Unbeatable
    int difficulty;

    // DP memoization table
    unordered_map<string, int> memo;

    // Winning combinations
    const vector<vector<int>> WIN_LINES =
    {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8},

        {0, 3, 6},
        {1, 4, 7},
        {2, 5, 8},

        {0, 4, 8},
        {2, 4, 6}
    };

public:

    TicTacToe();

    // --------------------------------------------------------
    // Board functions
    // --------------------------------------------------------

    void resetBoard();
    void printBoard(bool showPositions = false);
    void printPositionGuide();

    bool isMoveLeft();
    bool makeMove(int row, int col, char player);

    // --------------------------------------------------------
    // Game functions
    // --------------------------------------------------------

    int eval();
    bool checkGameOver();

    // --------------------------------------------------------
    // DP + Minimax
    // --------------------------------------------------------

    string getState();
    int minimaxDP(string state);
    int getBestMove(char computer);

    // --------------------------------------------------------
    // Computer functions
    // --------------------------------------------------------

    void findRandomMove();
    void findBestMove();

    // --------------------------------------------------------
    // Game modes
    // --------------------------------------------------------

    void playComputerMode();
    void playPlayerMode();

    // --------------------------------------------------------
    // Menu
    // --------------------------------------------------------

    void playGame();
};


// ============================================================
// CONSTRUCTOR
// ============================================================

TicTacToe::TicTacToe()
{
    difficulty = 4;
    resetBoard();
}


// ============================================================
// RESET BOARD
// ============================================================

void TicTacToe::resetBoard()
{
    emptyLoc = 9;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            Board[i][j] = ' ';
        }
    }
}


// ============================================================
// PRINT POSITION GUIDE
// ============================================================

void TicTacToe::printPositionGuide()
{
    cout << "\n";

    cout << "      Position Guide\n\n";

    cout << "        1   2   3\n";
    cout << "      +---+---+---+\n";
    cout << "   1  | 1 | 2 | 3 |\n";
    cout << "      +---+---+---+\n";
    cout << "   2  | 4 | 5 | 6 |\n";
    cout << "      +---+---+---+\n";
    cout << "   3  | 7 | 8 | 9 |\n";
    cout << "      +---+---+---+\n";
}


// ============================================================
// PRINT BOARD
// ============================================================

void TicTacToe::printBoard(bool showPositions)
{
    cout << "\n";

    cout << "        +---+---+---+\n";

    for (int i = 0; i < 3; i++)
    {
        cout << "        |";

        for (int j = 0; j < 3; j++)
        {
            char value = Board[i][j];

            if (value == ' ' && showPositions)
            {
                int position = i * 3 + j + 1;

                cout << " " << position << " |";
            }
            else
            {
                cout << " " << value << " |";
            }
        }

        cout << "\n";
        cout << "        +---+---+---+\n";
    }

    cout << "\n";
}


// ============================================================
// CHECK WHETHER MOVES ARE AVAILABLE
// ============================================================

bool TicTacToe::isMoveLeft()
{
    return emptyLoc > 0;
}


// ============================================================
// MAKE A MOVE
// ============================================================

bool TicTacToe::makeMove(int row, int col, char player)
{
    if (row >= 0 && row < 3 &&
        col >= 0 && col < 3 &&
        Board[row][col] == ' ')
    {
        Board[row][col] = player;
        emptyLoc--;

        return true;
    }

    return false;
}


// ============================================================
// EVALUATE BOARD
//
// +10 -> X wins
// -10 -> O wins
//   0 -> No winner yet
// ============================================================

int TicTacToe::eval()
{
    // Rows
    for (int i = 0; i < 3; i++)
    {
        if (Board[i][0] == Board[i][1] &&
            Board[i][1] == Board[i][2] &&
            Board[i][0] != ' ')
        {
            if (Board[i][0] == 'X')
                return 10;
            else
                return -10;
        }
    }

    // Columns
    for (int i = 0; i < 3; i++)
    {
        if (Board[0][i] == Board[1][i] &&
            Board[1][i] == Board[2][i] &&
            Board[0][i] != ' ')
        {
            if (Board[0][i] == 'X')
                return 10;
            else
                return -10;
        }
    }

    // Main diagonal
    if (Board[0][0] == Board[1][1] &&
        Board[1][1] == Board[2][2] &&
        Board[0][0] != ' ')
    {
        if (Board[0][0] == 'X')
            return 10;
        else
            return -10;
    }

    // Other diagonal
    if (Board[0][2] == Board[1][1] &&
        Board[1][1] == Board[2][0] &&
        Board[0][2] != ' ')
    {
        if (Board[0][2] == 'X')
            return 10;
        else
            return -10;
    }

    return 0;
}


// ============================================================
// CHECK GAME OVER
//
// Returns true if:
// X wins
// O wins
// Draw
// ============================================================

bool TicTacToe::checkGameOver()
{
    int score = eval();

    if (score == 10)
    {
        printBoard();

        cout << "\n  =================================\n";
        cout << "        X WINS THE GAME!\n";
        cout << "  =================================\n";

        return true;
    }

    if (score == -10)
    {
        printBoard();

        cout << "\n  =================================\n";
        cout << "        O WINS THE GAME!\n";
        cout << "  =================================\n";

        return true;
    }

    if (!isMoveLeft())
    {
        printBoard();

        cout << "\n  =================================\n";
        cout << "          GAME IS A DRAW!\n";
        cout << "  =================================\n";

        return true;
    }

    return false;
}


// ============================================================
// CONVERT CURRENT BOARD INTO A STRING
//
// Example:
// X O
//  X
//
// becomes something like:
// "XO X     "
//
// This string is used as the DP state/key.
// ============================================================

string TicTacToe::getState()
{
    string state = "";

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            state += Board[i][j];
        }
    }

    return state;
}


// ============================================================
// MINIMAX + DYNAMIC PROGRAMMING
//
// This replaces the old Minimax + Alpha-Beta function.
//
// DP Technique:
// Memoization
//
// State:
// Board configuration
//
// Return:
// +1 -> X wins
//  0 -> Draw
// -1 -> O wins
// ============================================================

int TicTacToe::minimaxDP(string state)
{
    // --------------------------------------------------------
    // 1. Check whether state already exists in DP table
    // --------------------------------------------------------

    auto it = memo.find(state);

    if (it != memo.end())
    {
        return it->second;
    }


    // --------------------------------------------------------
    // 2. Check terminal states
    // --------------------------------------------------------

    // Check rows
    for (int i = 0; i < 3; i++)
    {
        if (state[i * 3] != ' ' &&
            state[i * 3] == state[i * 3 + 1] &&
            state[i * 3 + 1] == state[i * 3 + 2])
        {
            int result = (state[i * 3] == 'X') ? 1 : -1;

            memo[state] = result;

            return result;
        }
    }

    // Check columns
    for (int i = 0; i < 3; i++)
    {
        if (state[i] != ' ' &&
            state[i] == state[i + 3] &&
            state[i + 3] == state[i + 6])
        {
            int result = (state[i] == 'X') ? 1 : -1;

            memo[state] = result;

            return result;
        }
    }

    // Check main diagonal
    if (state[0] != ' ' &&
        state[0] == state[4] &&
        state[4] == state[8])
    {
        int result = (state[0] == 'X') ? 1 : -1;

        memo[state] = result;

        return result;
    }

    // Check other diagonal
    if (state[2] != ' ' &&
        state[2] == state[4] &&
        state[4] == state[6])
    {
        int result = (state[2] == 'X') ? 1 : -1;

        memo[state] = result;

        return result;
    }


    // --------------------------------------------------------
    // 3. Check draw
    // --------------------------------------------------------

    if (state.find(' ') == string::npos)
    {
        memo[state] = 0;

        return 0;
    }


    // --------------------------------------------------------
    // 4. Determine whose turn it is
    //
    // X always starts.
    // If X count == O count -> X's turn.
    // Otherwise -> O's turn.
    // --------------------------------------------------------

    int xCount = count(state.begin(), state.end(), 'X');
    int oCount = count(state.begin(), state.end(), 'O');

    bool xTurn = (xCount == oCount);


    // --------------------------------------------------------
    // 5. X maximizes
    // O minimizes
    // --------------------------------------------------------

    int bestScore;

    if (xTurn)
        bestScore = -10;
    else
        bestScore = 10;


    // --------------------------------------------------------
    // 6. Generate all possible moves
    // --------------------------------------------------------

    for (int i = 0; i < 9; i++)
    {
        if (state[i] == ' ')
        {
            string nextState = state;

            if (xTurn)
                nextState[i] = 'X';
            else
                nextState[i] = 'O';


            // Recursively solve next state
            int score = minimaxDP(nextState);


            // X wants maximum
            if (xTurn)
            {
                bestScore = max(bestScore, score);
            }

            // O wants minimum
            else
            {
                bestScore = min(bestScore, score);
            }
        }
    }


    // --------------------------------------------------------
    // 7. Store result in DP table
    // --------------------------------------------------------

    memo[state] = bestScore;

    return bestScore;
}


// ============================================================
// GET BEST MOVE FOR COMPUTER
//
// Computer can be X or O.
//
// X -> maximize
// O -> minimize
// ============================================================

int TicTacToe::getBestMove(char computer)
{
    int bestMove = -1;

    int bestScore;

    if (computer == 'X')
        bestScore = -10;
    else
        bestScore = 10;


    string currentState = getState();


    for (int i = 0; i < 9; i++)
    {
        if (currentState[i] == ' ')
        {
            string nextState = currentState;

            nextState[i] = computer;

            int score = minimaxDP(nextState);


            if (computer == 'X')
            {
                if (score > bestScore)
                {
                    bestScore = score;
                    bestMove = i;
                }
            }
            else
            {
                if (score < bestScore)
                {
                    bestScore = score;
                    bestMove = i;
                }
            }
        }
    }

    return bestMove;
}


// ============================================================
// RANDOM COMPUTER MOVE
//
// Used only for Easy / Medium / Hard.
// ============================================================

void TicTacToe::findRandomMove()
{
    int emptyCells[9];
    int countEmpty = 0;


    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (Board[i][j] == ' ')
            {
                emptyCells[countEmpty] = i * 3 + j;

                countEmpty++;
            }
        }
    }


    if (countEmpty == 0)
        return;


    int pick = rand() % countEmpty;

    int position = emptyCells[pick];

    int row = position / 3;
    int col = position % 3;


    Board[row][col] = 'X';

    emptyLoc--;

    cout << "\n  Computer chooses position "
         << position + 1 << ".\n";
}


// ============================================================
// FIND BEST COMPUTER MOVE
//
// Difficulty:
//
// Easy:
//     80% random
//
// Medium:
//     40% random
//
// Hard:
//     10% random
//
// Unbeatable:
//     0% random
//     Always uses DP
// ============================================================

void TicTacToe::findBestMove()
{
    bool useRandom = false;


    if (difficulty == 1)
    {
        useRandom = (rand() % 100) < 80;
    }

    else if (difficulty == 2)
    {
        useRandom = (rand() % 100) < 40;
    }

    else if (difficulty == 3)
    {
        useRandom = (rand() % 100) < 10;
    }


    // --------------------------------------------------------
    // Random move for easier difficulty
    // --------------------------------------------------------

    if (useRandom)
    {
        findRandomMove();

        return;
    }


    // --------------------------------------------------------
    // DP optimal move
    //
    // In this implementation computer = X.
    // --------------------------------------------------------

    int bestMove = getBestMove('X');


    if (bestMove != -1)
    {
        int row = bestMove / 3;
        int col = bestMove % 3;

        Board[row][col] = 'X';

        emptyLoc--;

        cout << "\n  Computer chooses position "
             << bestMove + 1 << ".\n";

        cout << "  [Move selected using Minimax + DP]\n";
    }
}


// ============================================================
// PLAYER VS COMPUTER
//
// Human = O
// Computer = X
//
// Computer goes first, same as your original program.
// ============================================================

void TicTacToe::playComputerMode()
{
    resetBoard();

    cout << "\n";
    cout << "  =====================================\n";
    cout << "          PLAYER VS COMPUTER\n";
    cout << "  =====================================\n";

    cout << "\n  You are O\n";
    cout << "  Computer is X\n";


    // --------------------------------------------------------
    // Difficulty selection
    // --------------------------------------------------------

    cout << "\n  Select Difficulty\n";
    cout << "  -----------------\n";
    cout << "  1. Easy\n";
    cout << "  2. Medium\n";
    cout << "  3. Hard\n";
    cout << "  4. Unbeatable\n";


    cout << "\n  Enter choice: ";

    cin >> difficulty;


    if (difficulty < 1 || difficulty > 4)
    {
        difficulty = 4;

        cout << "  Invalid choice. Unbeatable selected.\n";
    }


    printPositionGuide();


    while (true)
    {
        // ----------------------------------------------------
        // COMPUTER TURN
        // ----------------------------------------------------

        cout << "\n  -------------------------------------\n";
        cout << "             COMPUTER'S TURN\n";
        cout << "  -------------------------------------\n";

        findBestMove();

        printBoard();


        if (checkGameOver())
            break;


        // ----------------------------------------------------
        // PLAYER TURN
        // ----------------------------------------------------

        cout << "  -------------------------------------\n";
        cout << "               YOUR TURN\n";
        cout << "  -------------------------------------\n";


        int row, col;

        while (true)
        {
            cout << "  Enter row (1-3): ";
            cin >> row;

            cout << "  Enter column (1-3): ";
            cin >> col;


            row--;
            col--;


            if (makeMove(row, col, 'O'))
                break;


            cout << "\n  Invalid move! Try again.\n\n";
        }


        printBoard();


        if (checkGameOver())
            break;
    }
}


// ============================================================
// PLAYER VS PLAYER
//
// Player 1 = X
// Player 2 = O
// ============================================================

void TicTacToe::playPlayerMode()
{
    resetBoard();

    char currentPlayer = 'X';


    cout << "\n";
    cout << "  =====================================\n";
    cout << "            PLAYER VS PLAYER\n";
    cout << "  =====================================\n";

    cout << "\n  Player 1 = X\n";
    cout << "  Player 2 = O\n";


    printPositionGuide();


    while (true)
    {
        printBoard();


        cout << "  -------------------------------------\n";

        if (currentPlayer == 'X')
            cout << "             PLAYER 1 (X)\n";
        else
            cout << "             PLAYER 2 (O)\n";

        cout << "  -------------------------------------\n";


        int row, col;


        while (true)
        {
            cout << "  Enter row (1-3): ";
            cin >> row;

            cout << "  Enter column (1-3): ";
            cin >> col;


            row--;
            col--;


            if (makeMove(row, col, currentPlayer))
                break;


            cout << "\n  Invalid move! Try again.\n\n";
        }


        if (checkGameOver())
            break;


        // Switch player

        if (currentPlayer == 'X')
            currentPlayer = 'O';
        else
            currentPlayer = 'X';
    }
}


// ============================================================
// MAIN MENU
// ============================================================

void TicTacToe::playGame()
{
    srand(static_cast<unsigned int>(time(nullptr)));


    while (true)
    {
        cout << "\n\n";

        cout << "  =============================================\n";
        cout << "              TIC - TAC - TOE\n";
        cout << "       Dynamic Programming Edition\n";
        cout << "  =============================================\n";

        cout << "\n";
        cout << "  1. Player vs Computer\n";
        cout << "  2. Player vs Player\n";
        cout << "  3. DP Analysis\n";
        cout << "  4. Exit\n";

        cout << "\n  ---------------------------------------------\n";
        cout << "  Enter your choice: ";


        int choice;

        cin >> choice;


        // ----------------------------------------------------
        // PLAYER VS COMPUTER
        // ----------------------------------------------------

        if (choice == 1)
        {
            playComputerMode();
        }


        // ----------------------------------------------------
        // PLAYER VS PLAYER
        // ----------------------------------------------------

        else if (choice == 2)
        {
            playPlayerMode();
        }


        // ----------------------------------------------------
        // DP ANALYSIS
        // ----------------------------------------------------

        else if (choice == 3)
        {
            cout << "\n";
            cout << "  =====================================\n";
            cout << "              DP ANALYSIS\n";
            cout << "  =====================================\n";

            string emptyState = "         ";

            int result = minimaxDP(emptyState);

            cout << "\n  Empty board result: ";

            if (result == 1)
                cout << "X can force a win";
            else if (result == -1)
                cout << "O can force a win";
            else
                cout << "DRAW with perfect play";

            cout << "\n";

            cout << "\n  DP states currently cached: "
                 << memo.size();

            cout << "\n\n";

            cout << "  DP Representation:\n";
            cout << "  Board State -> Optimal Result\n";

            cout << "\n  +1 = X wins\n";
            cout << "   0 = Draw\n";
            cout << "  -1 = O wins\n";
        }


        // ----------------------------------------------------
        // EXIT
        // ----------------------------------------------------

        else if (choice == 4)
        {
            cout << "\n";
            cout << "  =====================================\n";
            cout << "          Thanks for playing!\n";
            cout << "  =====================================\n";

            break;
        }


        else
        {
            cout << "\n  Invalid choice! Please select 1-4.\n";
        }
    }
}


// ============================================================
// MAIN
// ============================================================

int main()
{
    TicTacToe game;

    game.playGame();

    return 0;
}