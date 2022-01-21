#ifndef JQR_3_1_14_SORT_H
#define JQR_3_1_14_SORT_H
/*
    JQR Items Covered: 

    3.1.14
    title: Demonstrate skill in creating and implementing a sort routine
    condition: C
    standard: B
    sub_items:
*/

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

void sort(int *A, int n);

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
int binarySearch(int *A, int item, int low, int high);

#endif
