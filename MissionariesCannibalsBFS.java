import java.util.*;

class State {
    int M_left, C_left, boat; // boat: 0 = left, 1 = right

    State(int M_left, int C_left, int boat) {
        this.M_left = M_left;
        this.C_left = C_left;
        this.boat = boat;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof State)) return false;
        State state = (State) o;
        return M_left == state.M_left && C_left == state.C_left && boat == state.boat;
    }

    @Override
    public int hashCode() {
        return Objects.hash(M_left, C_left, boat);
    }

    @Override
    public String toString() {
        return "(M_left: " + M_left + ", C_left: " + C_left + ", Boat: " + (boat == 0 ? "Left" : "Right") + ")";
    }
}

public class MissionariesCannibalsBFS {
    static int M_total, C_total, boat_capacity;

    static boolean isValidState(int M_left, int C_left) {
        int M_right = M_total - M_left;
        int C_right = C_total - C_left;

        //negative numbers
        if (M_left < 0 || C_left < 0 || M_left > M_total || C_left > C_total) return false;
        //outnumbered on left bank
        if (M_left > 0 && M_left < C_left) return false;
        //outnumbered on right bank
        if (M_right > 0 && M_right < C_right) return false;

        return true;
    }

    static List<int[]> generateMoves(int capacity) {
        List<int[]> moves = new ArrayList<>();
        for (int m = 0; m <= capacity; m++) {
            for (int c = 0; c <= capacity; c++) {
                int total = m + c;
                if (total >= 1 && total <= capacity) {
                    moves.add(new int[]{m, c});
                }
            }
        }
        return moves;
    }

    static List<State> getNextStates(State s, List<int[]> moves) {
        List<State> nextStates = new ArrayList<>();
        int M_left = s.M_left;
        int C_left = s.C_left;
        int boat = s.boat;

        for (int[] move : moves) {
            int M_move = move[0];
            int C_move = move[1];
            State newState;
            if (boat == 0) { // boat on left going right
                newState = new State(M_left - M_move, C_left - C_move, 1);
            } else { // boat on right going left
                newState = new State(M_left + M_move, C_left + C_move, 0);
            }
            if (isValidState(newState.M_left, newState.C_left)) {
                nextStates.add(newState);
            }
        }
        return nextStates;
    }

    static void printPath(List<State> path) {
        int step = 0;
        for (State s : path) {
            System.out.println("Step " + step++ + ": " + s);
        }
    }

    static List<State> bfs() {
        State start = new State(M_total, C_total, 0);
        State goal = new State(0, 0, 1);
        List<int[]> moves = generateMoves(boat_capacity);

        Queue<List<State>> queue = new LinkedList<>();
        Set<State> visited = new HashSet<>();

        queue.offer(Arrays.asList(start));
        visited.add(start);

        while (!queue.isEmpty()) {
            List<State> path = queue.poll();
            State current = path.get(path.size() - 1);

            if (current.equals(goal)) {
                return path;
            }

            for (State next : getNextStates(current, moves)) {
                if (!visited.contains(next)) {
                    visited.add(next);
                    List<State> newPath = new ArrayList<>(path);
                    newPath.add(next);
                    queue.offer(newPath);
                }
            }
        }
        return null;
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter number of missionaries: ");
        M_total = scanner.nextInt();
        System.out.print("Enter number of cannibals: ");
        C_total = scanner.nextInt();
        System.out.print("Enter boat capacity: ");
        boat_capacity = scanner.nextInt();

        List<State> solution = bfs();
        if (solution != null) {
            System.out.println("\nSolution found (BFS):");
            printPath(solution);
        } else {
            System.out.println("No solution found.");
        }
    }
}
