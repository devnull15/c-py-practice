#include <stdio.h>

/**
 * @brief Implement an Insertion sort routine using any algorithm of your choice.
 *
 * Input:
 * @param A - An Array of integer (pass by reference)
 * @param n - Size of input 'A' (integer array)
 *
 * Output:
 * @return An Input 'A' should hold the sorted list of integers
 * No other explicit output
 *
 */


void print_array(int *A, int n) {
  printf("[");
  for(int i = 0; i<n; i++) {
    printf("%i,",A[i]);
  }
  printf("]\n");
  return;
}

void sort(int *A, int n) {
  for(int i = 1; i < n; i++) {
    int temp = A[i]; //get current element
    int j = i-1;
    
    while(j >= 0 && temp < A[j]) { // iterate backwards through "sorted" list until there's nothing small
      A[j+1] = A[j];
      j--;
    } //while
    
    A[j+1] = temp; //set last element looked at to current element
  } //for

}

/**
 * @brief Implement a binary search routine which return correct index for an 'item'
 * 
 * Input:
 * @param A - An Array of integer (call by reference)
 * @param item - Look up value
 * @param low  - left most index for sorting bound [0,...]
 * @param high - right most index for sorting bound [...,(n-1)-1]
 * 
 * Output:
 * @return It returns the correct location to place the 'item' in array 'A'
 * If the 'item' has the lowest value in the array(between low and high), 
 *      it returns index 0 or 'low' 
 * If the 'item' has higher value than the highest value in the array(between low and high), 
 *      it returns the high plus one (high + 1)
 * 
 * Notes:
 * No changes should be made to input array. 
 * 
 */
int binarySearch(int *A, int item, int low, int high) {
  
  if(A[low] > item) { return 0; } // item is lower than array
  if(A[high] < item) { return high+1; } // item is higher than array

  int diff = high-low;
  // when we're down to adjacent elements, check to see if item is the high element, if not,
  // it is either the low element or inbetween low and high so return low
  if(diff == 1) {
    if(A[high] == item) { return high; }
    else { return low; }
  }

  // find what half to search
  int middle = low+diff/2;
  if(A[middle] == item) { return middle; } // item is in the array and we found it at the middle;
  if(A[middle] < item) { return binarySearch(A,item,middle+1,high); } //item is bigger than A[middle] - search right side
  if(A[middle] > item) { return binarySearch(A,item,low,middle-1); } //item is bigger than A[middle] - search left side
  
  //how did you get here?
  return -1;
}
