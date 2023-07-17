public class InversionCount {
    int merge(int[] arr, int left, int mid, int right) {
        int[] temp = new int[arr.length];
        
        int leftArr = left;
        int midArr = mid;
        int leftTemp = left;
        int invCount = 0;
        
        while ((leftArr <= mid-1) && (midArr <= right)) {
            if (arr[leftArr] > arr[midArr]) {
                temp[leftTemp] = arr[midArr];
                leftTemp++;
                midArr++;
                
                invCount += (mid - leftArr);
            }
            else {
                temp[leftTemp] = arr[leftArr];
                leftTemp++;
                leftArr++;
            }
        }

        // fill in the remaining array
        // the remaining values would surely be greater values than the present values of tempArray

        for (; leftArr <= mid-1; leftArr++, leftTemp++)
            temp[leftTemp] = arr[leftArr];

        for (; midArr <= right; midArr++, leftTemp++)
            temp[leftTemp] = arr[midArr];

        // update the original array
        for (int i=left; i<=right; i++)
            arr[i] = temp[i];

        return invCount;
    }
    int mergeSort(int[] arr, int left, int right) {
        int invCount = 0;
        
        if (right > left) {
            int mid = (left + right) / 2;
            
            invCount += mergeSort(arr, left, mid);
            invCount += mergeSort(arr, mid+1, right);
            
            invCount += merge(arr, left, mid+1, right);
        }
        
        return invCount;
    }
    
    int getInversionCount(int[] arr) {
        int[] t = new int[arr.length-1]; // getting rid of the blank tile (0)
        int idx = 0;

        for (int i: arr) if (i != 0) t[idx++] = i;

        return mergeSort(t, 0, t.length-1);
    }
}
