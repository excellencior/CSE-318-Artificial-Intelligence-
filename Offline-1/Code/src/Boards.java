public abstract class Boards {
    protected int boardSize;
    protected abstract Boards getParent();
    protected abstract int getMoves();
    protected abstract int getBoardSize();
    protected abstract void setBlankIdx(int[] blankIndex);
    protected abstract int[] getBlankIdx();
    protected abstract int[][] getState();
    public abstract void printBoard();
    public abstract void calculateHeuristicDistance(HeuristicDistance hd);
    public abstract int getDistance();
}
