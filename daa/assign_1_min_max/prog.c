#include<stdio.h>

#define MAX 50              // Max limit of array (haystack) elements - any array

/*
 * Evaluation of min max value using
 * Divide and conquer strategy
 *
 * @function void min_max_eval
 * @param int elements[MAX] - haystack of elements to search for min max
 * @param int start - start index of haystack to start from
 * @param int end - end index of haystack to end at
 * @param int* min - minimum value to pass through recursion
 * @param int* max - maximum value to pass through recursion
 *
 */

void min_max_eval(int elements[MAX], int start, int end, int *min, int *max) {

    int tempmin, tempmax;   // To store result of any one halve after division for comparison
    int mid;                // Mid pivot for division of array

    if (start == end) {                             // Single element remaining
        *min = *max = elements[start];              // Set same as min and max
    } else if (start == end - 1) {                  // Only 2 elements remaining
        if (elements[start] > elements[end]) {      // Check element i-1 with i for min max and store same way
            *max = elements[start];
            *min = elements[end];
        } else {
            *max = elements[end];
            *min = elements[start];
        }
    } else {                                        // If more than 2 elements, divide strategy
        mid = (start + end) / 2;
        min_max_eval(elements, start, mid, min, max);               // Eval 1st half, conquer strategy
        min_max_eval(elements, mid + 1, end, &tempmin, &tempmax);   // Eval 2nd half, conquer strategy
        *max = (*max > tempmax) ? *max : tempmax;                   // Check both halves for max and assign respectively
        *min = (*min < tempmin) ? *min : tempmin;                   // Check both halves for min and assign respectively
    }

}

/*
 * Start of Execution
 *
 */

int main() {

    int elemCount, elements[MAX], iter, min, max;

    scanf("%d", &elemCount);                                // Accept element count

    for (iter = 0; iter < elemCount; iter++)                // Accept elements of given element count
        scanf("%d", &elements[iter]);

    min_max_eval(elements, 0, elemCount - 1, &min, &max);   // Eval min max

    printf("%d %d", min, max);                              // Print min max

    return 0;

}

/*
 * INPUT FORMAT
 *
 * (no. of elements in array)
 * (element),[(element)...]
 *
 */

/*
 * OUTPUT FORMAT
 *
 * (minimum element) (maximum element)
 *
 */

/* INPUT

 5
 2 6 4 7 8

 */

/*
 * OUTPUT

 2 8

 */