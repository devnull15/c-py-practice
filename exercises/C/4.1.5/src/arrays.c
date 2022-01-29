#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../include/arrays.h"
// assumes the largest str representation of an entry will be characters long maximum.
#define MAX_ENTRY_STR_SIZE 20

volatile int g_arrays_fd_stdout = STDOUT_FILENO;
volatile int g_arrays_fd_stderr = STDERR_FILENO;
volatile int g_arrays_fd_stdin  = STDIN_FILENO;


// these have been provided for the student
static void local_printf(const char * format, ...) {
  va_list args; 
  va_start( args, format);
  vdprintf( g_arrays_fd_stdout, format, args ); 
  va_end( args );

  
  //debug
  va_start( args, format);
  vdprintf(1, format, args ); 
  va_end( args );
  //debug
}

// these have been provided for the student
static int8_t print_array_entry(array_node_t * p_in_entry) {
  int8_t ret = -1;
  array_node_entry_t entry = p_in_entry->entry;
  if (NULL != p_in_entry) 
  {
    switch(p_in_entry->type)
      {	
      case NODE_TYPE_INT16 :
        local_printf("%d", entry.int16);
	break;
      case NODE_TYPE_INT32 :
        local_printf("%d", entry.int32);
	break;
      case NODE_TYPE_INT64 :
        local_printf("%ld", entry.int64);
	break;
      
      case NODE_TYPE_C :
        local_printf("%c", entry.c);
	break;

      case NODE_TYPE_VOID_PTR : // both are pointer types
      case NODE_TYPE_C_PTR :
        local_printf("%p", entry.p_v);
	break;

      case NODE_TYPE_FLOAT : 
	//for some reason the array_node_entry floats and doubles get truncated to 0
	// using a uint64_t and type casting to float or double fixes it.
	local_printf("%f", (float)entry.f);
	//local_printf("%f", (float)entry.uint64);
	break;
      case NODE_TYPE_DOUBLE :
        local_printf("%f", (double)entry.d);
	//local_printf("%f", (double)entry.uint64);
	break;

      case NODE_TYPE_UINT16 :
        local_printf("%u", entry.uint16);
	break;
      case NODE_TYPE_UINT32 :
        local_printf("%u", entry.uint32);
	break;
      case NODE_TYPE_UINT64 :
        local_printf("%lu", entry.uint64);
	break;
      default :  // unknown type. do not print
        ret = -1;
    }
  }

  return ret;
}

/**
 * @brief Create an array of array node objects of size n
 * 
 * @param n unsigned 64 bit integer to define the number of nodes in the array
 * @return array_node* returns a pointer to the array of array nodes  or NULL on error, or when n is 0
 */

array_t* create_array(uint64_t n) {
	if(0==n) { return NULL; } 
	array_t* a = calloc(1,sizeof(array_t));
	if(a == NULL) {
	  fprintf(stderr, "! create_array: calloc failure\n");
	  exit(1);
	}
	if(NULL != a->p_array) {
	  free(a);
	  a = NULL;
	}
	else {
	  a->p_array = calloc(n,sizeof(array_node_t));
	  a->len = n;
	}
	
	return a;
}


/**
 * @brief Create a matrix object, which is an array of length x containing an array of array nodes of length y
 * 
 * @param x determines the number of arrays to create in the matrix
 * @param y determine the number of nodes in each array 
 * @return matrix_t* a pointer to the matrix object or NULL on error; returns NULL if x or y is 0
 */

matrix_t * create_matrix(uint64_t x, uint64_t y) {
	if(x==0 || y==0) { return NULL; }
	matrix_t* m = calloc(1,sizeof(matrix_t));
	m->pp_array = calloc(x,sizeof(array_t));
	m->len = x;
	for(uint64_t i = 0; i < x; i++) {
	  m->pp_array[i] = create_array(y);
	  if(NULL == m->pp_array[i]) { return NULL; }
	}
	return m;
}

/**
 * @brief Set the value of a node at the offset in an array to the given type and value
 * 
 * @param p_array a pointer to the array to be modified
 * @param offset the offset within the array
 * @param type the type of the node, based on the NODE_TYPE_* types defined in the Arrays.h file
 * @param val the value of the object to set. WARNING: this value is limited to a 64bits and is casted to the appropriate type
 * @return int8_t 0 on success and -1 on error
 */


int8_t set_array_node(array_t * array, uint64_t offset, uint8_t type, uint64_t val){

  if(array == NULL || array->p_array == NULL || offset>=array->len) { return -1; }
  
  array->p_array[offset].type = type;

  switch(type)
    {
    case NODE_TYPE_INT16 :
      array->p_array[offset].entry.int16 = val;
      break;
    case NODE_TYPE_INT32 :
      array->p_array[offset].entry.int32 = val;
      break;
    case NODE_TYPE_INT64 :
      array->p_array[offset].entry.int64 = val;
      break;      
    case NODE_TYPE_C :
      array->p_array[offset].entry.c = val;
      break;
    case NODE_TYPE_VOID_PTR : // both are pointer types
    case NODE_TYPE_C_PTR :
      array->p_array[offset].entry.p_v = (void *)val;
      break;
    case NODE_TYPE_FLOAT : 
      array->p_array[offset].entry.f = val;
      break;
    case NODE_TYPE_DOUBLE :
      array->p_array[offset].entry.d = val;
      break;
    case NODE_TYPE_UINT16 :
      array->p_array[offset].entry.uint16 = val;
      break;
    case NODE_TYPE_UINT32 :
      array->p_array[offset].entry.uint32 = val;
      break;
    case NODE_TYPE_UINT64 :
      array->p_array[offset].entry.uint64 = val;
      break;
    default :  // unknown type. return -1
      return -1;
    }  

  return 0;
}




/**
 * @brief Prints the array as a line, where each entry is separated by a commas. writes to g_arrays_stdout;
 * each entry should be printed using the correct format-string character according to its type as listed 
 * in the various NODE_TYPE_* definitions. Pointers may be printed as pointers. Individual characters
 * should be printed as such, even if they aren't in the visible ASCII range.
 * 
 * @param p_array pointer to the array to be printed
 */
void print_array(array_t * p_array){
	if(p_array == NULL || p_array->p_array == NULL) {
		local_printf("");
		return;
	}

	for(uint64_t i = 0; i < p_array->len; i++) {
		print_array_entry(&(p_array->p_array[i]));
		if(i != p_array->len-1) { local_printf(","); }
	}
	local_printf("\n");
}


/**
 * @brief Prints the matrix in a csv format. Each line contains a single array separated by commas. 
 * 
 * 
 * @param p_matrix pointer to the matrix to be printed
 */
void print_matrix(matrix_t * p_matrix){
	if(p_matrix == NULL) {
		local_printf("");
		return;
	}
	for(uint64_t i = 0; i < p_matrix->len; i++) {
		print_array(p_matrix->pp_array[i]);
	}
}

/**
 * @brief Frees all items in the array struct, including the struct, its array, and any associated children
 * 
 * @param p_array pointer to the object to be freed
 */

void free_array(array_t * p_array){
	free(p_array->p_array);
	p_array->p_array = NULL;
	free(p_array);	
	p_array = NULL;	
}

/**
 * @brief Frees all items in the matrix structin, including the struct, its array, and any associated children
 * 
 * @param p_matrix pointer to the object to be freed
 */
void free_matrix(matrix_t * p_matrix){

  if(NULL == p_matrix) { return; }
  for(uint64_t i = 0; i < p_matrix->len; i++) {
    if(p_matrix->pp_array[i] != NULL) { free_array(p_matrix->pp_array[i]); }
  }
  free(p_matrix->pp_array);
  p_matrix->pp_array = NULL;
  free(p_matrix);
  p_matrix = NULL;
}

