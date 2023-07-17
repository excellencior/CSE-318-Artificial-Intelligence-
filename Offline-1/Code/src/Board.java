import java.util.Arrays;

public class Board extends Boards {
    private int[][] state;
    private int moves;
    private int heuristicDistance;
    private int[] blankIndex;
    private Boards parent;

    public Board(int[][] state, Boards parent, int boardSize, int moves) {
        this.state = state;
        this.parent = parent;
        this.moves = moves;
        this.boardSize = boardSize;
    }

    public Boards getParent() {
        return this.parent;
    }

    public int getMoves() {
        return this.moves;
    }

    public int getBoardSize() {
        return this.boardSize;
    }

    public void setBlankIdx(int[] blankIndex) {
        this.blankIndex = blankIndex;
    }

    public int[] getBlankIdx() {
        return this.blankIndex;
    }

    public int[][] getState() {
        return this.state;
    }

    public void calculateHeuristicDistance(HeuristicDistance hd) {
        this.heuristicDistance = hd.getDistance(state, boardSize);
    }

    public int getDistance() {
        return this.heuristicDistance;
    }

    public void printBoard() {
        for (int i=1; i<=this.boardSize; i++) {
            for (int j=1; j<=this.boardSize; j++) {
                System.out.print(state[i][j] + " ");
            }
            System.out.println();
        }
        System.out.println();
    }
}
