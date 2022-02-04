#include <stdio.h>
#include <stdlib.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "test.h"
#include "../include/sort.h"

unsigned int test_sort(int *A, int *B, int n )
{   
    int i = 0;
    unsigned int result = 0;

    sort(A, n);     

    //check output
    for(i = 0; i < n; i++)
    {
        if(A[i] != B[i])
        {
            result = result + 1; //if not match
        }
    }

    return result;
}

//Test: Already sorted list
unsigned int test_3_1_14_a()
{   
    //setup
    unsigned int score = 0;
    
    //define in/out/size of Array
    int C_n = 4;
    int C[] = {-1, 0, 0, 1};
    int C_cor[] = {-1, 0, 0, 1};

    score = test_sort(C, C_cor, C_n);

    if(score == 0)
    {
        return 1;
    }
    return 0;
}

//Test: Duplicate value
unsigned int test_3_1_14_b()
{   
    //setup
    unsigned int score = 0;
    int n = 0;      //size of A
    
    //define in/out/size of in
    int D_n = 9;
    int D[] =     {54, 23, 97, -10, 0, 00 , 0 ,0 ,0};
    int D_cor[] = {-10, 00, 0 , 0 ,0 ,0, 23, 54, 97}; //00 will replace with 0

    score = test_sort(D, D_cor, D_n);
    
    if(score == 0)
    {
        return 1;
    }
    return 0;
}

//Test: Reverse sorted list
unsigned int test_3_1_14_c()
{   
    //setup
    unsigned int score = 0;
    int n = 0;      //size of A
    
    //define in/out/size of in
    int E_n = 11;
    int E[] =     {1, -1, -2, -3, -4, -5, -6, -7, -8, -9, 0};
    int E_cor[] = {-9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1}; 

    score = test_sort(E, E_cor, E_n);
    
    if(score == 0)
    {
        return 1;
    }
    return 0;
}

//Test: Only one element
unsigned int test_3_1_14_d()
{   
    //setup
    unsigned int score = 0;
    int n = 0;      //size of A
    
    //define in/out/size of in
    int F_n = 1;
    int F[] = {1000000000};
    int F_cor[] = {1000000000};

    score = test_sort(F, F_cor, F_n);
    
    if(score == 0)
    {
        return 1; 
    }
    return 0;
}

//Test: Simple case
unsigned int test_3_1_14_e()
{   
    //setup
    unsigned int score = 0;
    int n = 0;      //size of A
    
    //define in/out/size of in
    int G_n = 2;
    int G[] = {1, 0};
    int G_cor[] = {0, 1};

    score = test_sort(G, G_cor, G_n);
    
    if(score == 0)
    {
        return 1;
    }
    return 0;
}

//Test: Binary search 
unsigned int test_3_1_14_f()
{
    unsigned int score = 0;
    int *A;         //A - input, 
    int i;          //loop iterator
    
    int H_n = 8;
    int H[] = {0, 1, 2, 3, 5, 6, 4, 7}; //value 4 -> 4 [0:5]
                                        //value 5 -> 4 [0:3]

    A = H;

    if (binarySearch(A, 4, 0, 5) != 4)
    {
        score = score + 1;
    }
    
    if (binarySearch(A, 5, 0, 3) != 4)
    {
        score = score + 1;
    }
    
    if(score == 0)
    {
        return 1;
    }
    return 0;
}


void test_3_1_14()
{
    CU_ASSERT_EQUAL(test_3_1_14_a(), 1);
    CU_ASSERT_EQUAL(test_3_1_14_b(), 1);
    CU_ASSERT_EQUAL(test_3_1_14_c(), 1);
    CU_ASSERT_EQUAL(test_3_1_14_d(), 1);
    CU_ASSERT_EQUAL(test_3_1_14_e(), 1);
    CU_ASSERT_EQUAL(test_3_1_14_f(), 1);

}