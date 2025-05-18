import java.util.Scanner;

public class assignment4_sudokuinput {

    public static boolean isValid(char[][] board, int row, int col, char digit) {
        for (int i = 0; i < 9; i++) {
            if (board[row][i] == digit || board[i][col] == digit) return false;
        }

        int startRow = row / 3 * 3;
        int startCol = col / 3 * 3;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[startRow + i][startCol + j] == digit) return false;
            }
        }

        return true;
    }

    public static boolean solve(char[][] board, int row, int col) {
        if (row == 9) return true;

        int nextRow = (col == 8) ? row + 1 : row;
        int nextCol = (col == 8) ? 0 : col + 1;

        if (board[row][col] != '.') {
            return solve(board, nextRow, nextCol);
        }

        for (char digit = '1'; digit <= '9'; digit++) {
            if (isValid(board, row, col, digit)) {
                board[row][col] = digit;

                if (solve(board, nextRow, nextCol)) return true;

                board[row][col] = '.'; // backtrack
            }
        }

        return false;
    }

    public static void solveSudoku(char[][] board) {
        solve(board, 0, 0);
    }

    public static void printBoard(char[][] board) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                System.out.print(board[i][j] + " ");
            }
            System.out.println();
        }
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        char[][] board = new char[9][9];

        System.out.println("Enter the Sudoku board (use '.' for empty cells):");
        System.out.println("Enter 9 lines with 9 characters each (digits 1-9 or '.'):");

        for (int i = 0; i < 9; i++) {
            String line;
            while (true) {
                System.out.print("Row " + (i + 1) + ": ");
                line = scanner.nextLine().trim();
                if (line.length() == 9 && line.matches("[1-9.]{9}")) {
                    break;
                } else {
                    System.out.println("Invalid input. Please enter exactly 9 characters using digits 1-9 or '.' only.");
                }
            }

            for (int j = 0; j < 9; j++) {
                board[i][j] = line.charAt(j);
            }
        }

        System.out.println("\nOriginal Sudoku Board:");
        printBoard(board);

        if (solveSudoku(board)) {
            System.out.println("\nSolved Sudoku Board:");
            printBoard(board);
        } else {
            System.out.println("\nNo solution exists for the given Sudoku board.");
        }

        scanner.close();
    }
}
