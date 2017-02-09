/* #define DEBUG 1 */

#ifdef __cplusplus 
extern "C" {
#endif

typedef struct vector {
    int size;          // Real size of allocated memory
    void **base;   // The buffer used to store the data
    int off_begin;     // Offset to the first element
    int off_end;       // Offset to the first empty slot at the end
} vector_t;

// Functions prototypes

/* TARGET: Create a new vector
   INPUT: Value of the size of the new vector
   OUTPUT: The vector created
*/
vector_t *vector_create(int size);

/* TARGET: Destroy a given vector
   INPUT: Vector to be destroyed
   OUTPUT: -
*/
void vector_destroy(vector_t *v, int free_contents);

/* TARGET: Fill a vector with elements of type void *
   INPUT: Vector that wants to fill & the elements
   OUTPUT: -
*/
void vector_fill(vector_t *v, void *e);

int vector_len(vector_t *v);
  
/* TARGET: Obtain a vector's element
   INPUT: Vector in which the element is contained & index of the possition
   OUTPUT: The element
*/
//void * vector_get(vector_t *v, int ind);
void * vector_get(vector_t *, int);


/* TARGET: Change a vector's element
   INPUT: Vector that has to be changed & index of the possition where the new element is going to be located & the element.
   OUTPUT: -
*/
void vector_set(vector_t *v, int idx, void *e);

/* TARGET: Add a new element at the end of the vector
   INPUT: Vector that has to be changed & the element that will be added at the end
   OUTPUT: -
*/
void vector_append(vector_t *v, void *e);

/* TARGET: Add a new element at the beggining of the vector
   INPUT: Vector that has to be changed & the element that will be added at the beggining
   OUTPUT: -
*/
void vector_prepend(vector_t *v, void *e);

#ifdef __cplusplus
}
#endif
