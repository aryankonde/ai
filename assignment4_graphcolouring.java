import java.util.Scanner;

public class assignment4_graphcolouring {
    private int V;
    private int[][] graph;
    private int[] colors;

    public assignment4_graphcolouring(int[][] graph, int V) {
        this.graph = graph;
        this.V = V;
        this.colors = new int[V];
    }

    private boolean isSafe(int v, int c) {
        for (int i = 0; i < V; i++) {
            if (graph[v][i] == 1 && colors[i] == c) {
                return false;
            }
        }
        return true;
    }

    private boolean solveGraphColoring(int v, int M) {
        if (v == V) {
            printSolution();
            return true;
        }

        for (int c = 1; c <= M; c++) {
            if (isSafe(v, c)) {
                colors[v] = c;
                if (solveGraphColoring(v + 1, M)) {
                    return true;
                }
                colors[v] = 0;
            }
        }
        return false;
    }

    private void printSolution() {
        System.out.println("Vertex Coloring:");
        for (int i = 0; i < V; i++) {
            System.out.println("Vertex " + i + " = Color " + colors[i]);
        }
    }

    public void solve(int M) {
        if (!solveGraphColoring(0, M)) {
            System.out.println("Solution does not exist");
        }
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.print("Enter number of vertices: ");
        int V = scanner.nextInt();

        int[][] graph = new int[V][V];
        System.out.println("Enter the adjacency matrix (row by row):");
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                graph[i][j] = scanner.nextInt();
            }
        }

        System.out.print("Enter the number of colors (M): ");
        int M = scanner.nextInt();

        assignment4_graphcolouring gc = new assignment4_graphcolouring(graph, V);
        gc.solve(M);

        scanner.close();
    }
}
