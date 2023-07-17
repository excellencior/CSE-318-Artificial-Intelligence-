import java.util.List;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        int n, idx = 0, inp;

        Scanner scn = new Scanner(System.in);
        n = scn.nextInt();

        int[] arr = new int[n*n];
        int[][] inpArr = new int[n+1][n+1];
        int blankTileRowPos = 0, blankTileColPos = 0;

        for (int i=1; i<=n; i++) {
            for (int j=1; j<=n; j++){
                inp = scn.nextInt();
                arr[idx++] = inp; // ignoring the blank tile
                inpArr[i][j] = inp;
                
                if (inp == 0) {
                    blankTileRowPos = i;
                    blankTileColPos = j;
                }
            }
        }

        Boards board = new Board(inpArr, null, n, 0);
        board.setBlankIdx(new int[] {blankTileRowPos, blankTileColPos});

        int ic = new InversionCount().getInversionCount(arr);

        if ((n&1) == 1) {
            if ((ic & 1) != 0) { // not even inversion count
                System.out.println("Unsolvable puzzle");
                System.exit(0);
            }
        }
        else {
            int x = ic + ((n-blankTileRowPos+1) % 2 == 1 ? 0 : 1); // row pos from the last
            if ((x & 1) != 0) {// not even inversion count
                System.out.println("Unsolvable puzzle");
                System.exit(0);
            }
        }

        Search search = new Search(board);
//        search.solve(new ManhattanDistance());
        search.solve(new HammingDistance());
    }
}
