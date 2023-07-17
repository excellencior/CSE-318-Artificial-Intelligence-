public class ManhattanDistance extends HeuristicDistance{
    public int getDistance(int[][] arr, int n) {
        int distance = 0;

        for (int i=1; i<=n; i++) {
            for (int j=1; j<=n; j++) {
                if (arr[i][j] != 0) {
                    int i_0 = arr[i][j]%n == 0 ? arr[i][j]/n : (arr[i][j]/n) + 1;
                    int j_0 = arr[i][j] - (n * (i_0-1));

                    distance += Math.abs(i-i_0);
                    distance += Math.abs(j-j_0);
                }
            }
        }

        return distance;
    }
}
