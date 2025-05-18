#include <stdio.h>

#define SIZE 3
#define WIN_SUM 15

int board[SIZE][SIZE] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
int magicSquare[SIZE][SIZE] = {{8, 1, 6}, {3, 5, 7}, {4, 9, 2}};
int player, computer;

void printBoard()
{
    printf("\nCurrent Board:\n");
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (board[i][j] == 0)
                printf(" . ");
            else if (board[i][j] == 1)
                printf(" X ");
            else
                printf(" O ");
        }
        printf("\n");
    }
}

int checkWin(int player)
{
    int sum;
    // checking rows and columns
    for (int i = 0; i < SIZE; i++)
    {
        sum = 0;
        for (int j = 0; j < SIZE; j++)
            sum += (board[i][j] == player) ? magicSquare[i][j] : 0;
        if (sum == WIN_SUM)
            return 1;

        sum = 0;
        for (int j = 0; j < SIZE; j++)
            sum += (board[j][i] == player) ? magicSquare[j][i] : 0;
        if (sum == WIN_SUM)
            return 1;
    }

    // checking diagonals
    sum = 0;
    for (int i = 0; i < SIZE; i++)
        sum += (board[i][i] == player) ? magicSquare[i][i] : 0;
    if (sum == WIN_SUM)
        return 1;

    sum = 0;
    for (int i = 0; i < SIZE; i++)
        sum += (board[i][SIZE - i - 1] == player) ? magicSquare[i][SIZE - i - 1] : 0;
    if (sum == WIN_SUM)
        return 1;

    return 0;
}

void playerMove()
{
    int move;
    printf("Enter your move (1-9): ");
    scanf("%d", &move);
    move--; // 0 based indexing
    int row = move / SIZE;
    int col = move % SIZE;
    if (board[row][col] == 0)
        board[row][col] = player;
    else
    {
        printf("Invalid move. Try again.\n");
        playerMove();
    }
}

void computerMove()
{
    // check if the computer can win
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (board[i][j] == 0)
            {
                board[i][j] = computer;
                if (checkWin(computer))
                    return;
                board[i][j] = 0;
            }
        }
    }

    // block the player
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (board[i][j] == 0)
            {
                board[i][j] = player;
                if (checkWin(player))
                {
                    board[i][j] = computer;
                    return;
                }
                board[i][j] = 0;
            }
        }
    }

    // take the center
    if (board[1][1] == 0)
    {
        board[1][1] = computer;
        return;
    }

    // take any corner
    if (board[0][0] == 0)
    {
        board[0][0] = computer;
        return;
    }
    if (board[0][2] == 0)
    {
        board[0][2] = computer;
        return;
    }
    if (board[2][0] == 0)
    {
        board[2][0] = computer;
        return;
    }
    if (board[2][2] == 0)
    {
        board[2][2] = computer;
        return;
    }

    // take any remaining move
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (board[i][j] == 0)
            {
                board[i][j] = computer;
                return;
            }
        }
    }
}

int main()
{
    char choice;
    printf("Do you want to be X or O? ");
    scanf(" %c", &choice);

    if (choice == 'X' || choice == 'x')
    {
        player = 1;
        computer = 2;
    }
    else
    {
        player = 2;
        computer = 1;
    }

    int turn = 1;
    int moves = 0;
    while (1)
    {
        printBoard();
        if ((turn == 1 && player == 1) || (turn == 2 && player == 2))
        {
            playerMove();
            if (checkWin(player))
            {
                printBoard();
                printf("Player wins!\n");
                break;
            }
            if (turn == 1)
            {
                turn = 2;
            }
            else
            {
                turn = 1;
            }
        }
        else
        {
            computerMove();
            if (checkWin(computer))
            {
                printBoard();
                printf("Computer wins!\n");
                break;
            }
            if (turn == 1)
            {
                turn = 2;
            }
            else
            {
                turn = 1;
            }
        }
        moves++;
        if (moves == 9)
        {
            printBoard();
            printf("It's a draw!\n");
            break;
        }
    }
    return 0;
}