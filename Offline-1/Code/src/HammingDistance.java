public class HammingDistance extends HeuristicDistance {
    public int getDistance(int[][] arr, int n) {
        int distance = 0;

        for(int i=1; i<=n; i++) {
            for (int j=1; j<=n; j++) {
                if (arr[i][j]!=0 && (arr[i][j] != (n*(i-1) + j))) distance++;
            }
        }

        return distance;
    }
}
