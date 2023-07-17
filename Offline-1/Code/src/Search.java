import java.util.*;

public class Search {
    private int n;
    private Boards board;
    private LinkedHashSet<String> myset;
    private int numberOfExploredNodes;

    public Search(Boards board) {
        this.board = board;
        this.n = board.getBoardSize();
        myset = new LinkedHashSet<String>();
        this.numberOfExploredNodes = 1;
    }

    public Boards getNeighbor(Boards board, String swapDir) {
        int[][] temp = new int[this.n+1][this.n+1];
        int[][] state = board.getState();
        int[] blankIndex = board.getBlankIdx();

        int x = blankIndex[0], y = blankIndex[1];

        for (int i=1; i<=this.n; i++) {
            if (this.n >= 0) System.arraycopy(state[i], 1, temp[i], 1, this.n);
        }

        if (swapDir.equalsIgnoreCase("right")) {
            temp[x][y] = temp[x][y+1];
            temp[x][y+1] = 0;
            y = y+1;
        }
        else if (swapDir.equalsIgnoreCase("left")) {
            temp[x][y] = temp[x][y-1];
            temp[x][y-1] = 0;
            y = y-1;
        }
        else if (swapDir.equalsIgnoreCase("up")) {
            temp[x][y] = temp[x-1][y];
            temp[x-1][y] = 0;
            x = x-1;
        }
        else if (swapDir.equalsIgnoreCase("down")) {
            temp[x][y] = temp[x+1][y];
            temp[x+1][y] = 0;
            x = x+1;
        }

        String sb = Arrays.deepToString(temp);
        if (myset.contains(sb)) return null;
        myset.add(sb); // is not in the array, so it is a new board config
//        System.out.println(sb);

        Boards newboard = new Board(temp, board, this.n, board.getMoves()+1);
        newboard.setBlankIdx(new int [] {x, y});

        return newboard;
    }

    public List<Boards> getNeighbors(Boards board) {
        int x = board.getBlankIdx()[0], y = board.getBlankIdx()[1];
        List<Boards> boards = new ArrayList<>();

        if (y+1 <= n) {
            Boards b = getNeighbor(board, "right");
            if (b != null) boards.add(b);
        }
        if (y-1 >= 1) {
            Boards b = getNeighbor(board, "left");
            if (b != null) boards.add(b);
        }
        if (x+1 <= n) {
            Boards b = getNeighbor(board, "down");
            if (b != null) boards.add(b);
        }
        if (x-1 >= 1) {
            Boards b = getNeighbor(board, "up");
            if (b != null) boards.add(b);
        }

        this.numberOfExploredNodes += boards.size();
        
        return boards;
    }

    public void solve(HeuristicDistance hd) {
        PriorityQueue<Boards> pq = new PriorityQueue<Boards>(new CostComparator()); // priority queue based on h(n)
        board.calculateHeuristicDistance(hd);
        pq.add(board); // Initial board
        Boards tempBoard = null;

        myset.add(Arrays.deepToString(board.getState()));

        while (!pq.isEmpty()) {
            tempBoard = pq.poll();
            List<Boards> neighbors = getNeighbors(tempBoard);

            if (tempBoard.getDistance() == 0) {
                break;
            }

            for (Boards b: neighbors) {
                b.calculateHeuristicDistance(hd);
                pq.add(b);
            }
        }

        System.out.println("Minimum number of moves = " + tempBoard.getMoves() + "\n");

        printPath(tempBoard);
    }

    private void printPath(Boards tempBoard) {
        if (Arrays.deepToString(tempBoard.getState()).equalsIgnoreCase(Arrays.deepToString(this.board.getState()))) {
            this.board.printBoard();
            return;
        }
        printPath(tempBoard.getParent());
        tempBoard.printBoard();
    }

    public int getNumberOfExploredNodes() {
        return this.numberOfExploredNodes;
    }
}
