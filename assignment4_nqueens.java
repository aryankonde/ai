import java.util.Scanner;

class assignment4_nqueens {
    private int N;

    public assignment4_nqueens(int N) {
        this.N = N;
    }

    // Function to check if placing a queen at board[row][col] is safe
    private boolean isSafe(int board[], int row, int col) {
        for (int i = 0; i < row; i++) {
            if (board[i] == col || Math.abs(board[i] - col) == Math.abs(i - row)) {
                return false; // Column conflict or diagonal conflict
            }
        }
        return true;
    }

    // Recursive function to solve N-Queens using backtracking
    private boolean solveNQueens(int board[], int row) {
        if (row == N) {
            printSolution(board);
            return true; // Found one solution
        }

        boolean res = false;
        for (int col = 0; col < N; col++) {
            if (isSafe(board, row, col)) {
                board[row] = col; // Place queen
                res = solveNQueens(board, row + 1) || res; // Continue solving
                board[row] = -1; // Backtrack
            }
        }
        return res;
    }

    // Function to print the board
    private void printSolution(int board[]) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (board[i] == j) {
                    System.out.print(" Q ");
                } else {
                    System.out.print(" . ");
                }
            }
            System.out.println();
        }
        System.out.println();
    }

    // Wrapper function to solve the problem
    public void solve() {
        int[] board = new int[N];
        for (int i = 0; i < N; i++) {
            board[i] = -1;
        }
        if (!solveNQueens(board, 0)) {
            System.out.println("No solution exists");
        }
    }

    // Main method
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter the value of N for N-Queens: ");
        int N = scanner.nextInt();
        scanner.close();

        assignment4_nqueens nq = new assignment4_nqueens(N);
        nq.solve();
    }
}
