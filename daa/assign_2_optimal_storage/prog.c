/*
 * Author - Vaishnav Mhetre
 * Created at - Sunday, 8th April - 2:00 am
 *
 */

#include<stdio.h>

#define MAX 50          // Max array (haystack) size limit - any array


/*
 * To store the program with its identity
 *
 * @structure Program
 * @attribute char[] name
 * @attribute int length
 * @identifier Program
 *
 */
typedef struct Program {        // A structure for Program - name, length
    char name[MAX];
    int length;
} Program;


/*
 * Merging of Haystack elements - Merge Sort
 *
 * @function void merge
 * @param Program[] programs - Haystack of Programs for sorting
 * @param int low - Lower bound index
 * @param int mid - Center pivot index
 * @param int high - Upper bound index
 *
 */
void merge(Program programs[MAX], int low, int mid, int high) {
    int l1, l2, i;
    Program bkp_programs[MAX];

    for (l1 = low, l2 = mid + 1, i = low; l1 <= mid && l2 <= high; i++) {
        if (programs[l1].length <= programs[l2].length)
            bkp_programs[i] = programs[l1++];
        else
            bkp_programs[i] = programs[l2++];
    }

    while (l1 <= mid)
        bkp_programs[i++] = programs[l1++];

    while (l2 <= high)
        bkp_programs[i++] = programs[l2++];

    for (i = low; i <= high; i++)
        programs[i] = bkp_programs[i];
}

/*
 * Sorting of Haystack elements - Merge Sort
 *
 * @function void sort
 * @param Program[] program
 * @param int low - Lower bound index
 * @param int high - Upper bound index
 *
 */

void sort(Program programs[MAX], int low, int high) {
    int mid;

    if (low < high) {
        mid = (low + high) / 2;
        sort(programs, low, mid);
        sort(programs, mid + 1, high);
        merge(programs, low, mid, high);
    } else {
        return;
    }
}

/*
 * Storage of programs to tape storage (generalized)
 *
 * @function void store
 * @param Program[][] storage - Tape Storage
 * @param Program[] program - Haystack of Programs
 * @param int tapeCount - No. of tapes in storage
 * @param int progCount - No. of programs to be stored
 *
 */

void store(Program storage[MAX][MAX], Program programs[MAX], int tapeCount, int progCount) {

    int tapeIter = 0, storageColIter = 0;

    for (int i = 0; i < progCount; ++i) {
        storage[tapeIter][storageColIter] = programs[i];
        tapeIter = (tapeIter + 1) % tapeCount;
        if (tapeIter == 0) {
            storageColIter++;
        }
    }

};

/*
 * Storage of programs to tape storage (optimized)
 *
 * @function void store
 * @param Program[][] storage - Tape Storage
 * @param Program[] program - Haystack of Programs
 * @param int tapeCount - No. of tapes in storage
 * @param int progCount - No. of programs to be stored
 *
 */

void optimal_store(Program storage[MAX][MAX], Program programs[MAX], int tapeCount, int progCount) {

    sort(programs, 0, progCount - 1);

    store(storage, programs, tapeCount, progCount);

}

/*
 * View Tape Storage in user friendly format
 *
 * @function void view_storage
 * @param Program[][] storage
 * @param int tapeCount
 * @param int progCount
 */

void view_storage(Program storage[MAX][MAX], int tapeCount, int progCount){

    int tapeIter = 0, storageColIter = 0;

    /*
     * Storage display format
     *
     * tp   (tape_number)   [(tape_number)...]
     *      (program_name)  [(program_name)...]
     *      (program_name)  [(program_name)...]
     *
     */

    printf("tp\t");
    for (int i = 0; i < tapeCount; ++i) {
        printf("%d\t", i+1);
    }
    printf("\n\t");
    for (int i = 0; i < progCount; ++i) {
        printf("%s\t", storage[tapeIter][storageColIter].name);
        tapeIter = (tapeIter + 1) % tapeCount;
        if (tapeIter == 0) {
            printf("\n\t");
            storageColIter++;
        }
    }

}

/*
 * Get Retrieval time of a program from programs haystack
 *
 * @function int getRT
 * @param Program[] programs - Haystack of programs
 * @param int progIndex - Index of program in haystack
 * @return int rt - Retrieval Time
 *
 */

int getRT(Program programs[MAX], int progIndex) {

    int rt = 0;

    for (int i = 0; i <= progIndex; ++i) {
        for (int j = 0; j <= i; ++j) {
            rt += programs[j].length;
        }
    }

    return rt;
}

/*
 * Get Mean Retrieval time of programs in haystack
 *
 * @function int getMRT
 * @param Program[] programs - Haystack of programs
 * @param int progCount - Amount of programs in haystack
 * @return int mrt - Mean Retrieval Time
 *
 */

int getMRT(Program programs[MAX], int progCount) {
    return (getRT(programs, progCount - 1) / progCount);
}

/*
 * Sample structure of programs haystack
 *
    Program programs[] = {
         {"pg1", 5},                // Program instance - {(name), (length)}
         {"pg2", 10},
         {"pg3", 3},
    };
 *
 */

/*
 * Start of Execution
 */

int main() {

    Program programs[MAX], storage[MAX][MAX];
    int progCount, tapeCount;

    scanf("%d %d", &progCount, &tapeCount);

    for (int i = 0; i < progCount; ++i) {
        scanf("%s %d", programs[i].name, &programs[i].length);
    }

    printf("\n");
    printf("%d", getMRT(programs, progCount));

    printf("\n\n");
    for (int i = 0; i < progCount; ++i) {
        printf("{%s,%d} ", programs[i].name, programs[i].length);
    }

    sort(programs, 0, progCount-1);

    printf("\n\n");
    for (int i = 0; i < progCount; ++i) {
        printf("{%s,%d} ", programs[i].name, programs[i].length);
    }

    optimal_store(storage, programs, tapeCount, progCount);

    printf("\n\n");
    view_storage(storage, tapeCount, progCount);

    return 0;

}

/* INPUT FORMAT
 *
 * (program count) (tape count)
 * ((program name) (program length)) [((program name) (program length))...]
 *
 */

/*
 * OUTPUT FORMAT
 *
 * (mean retrieval time)
 *
 * (unsorted programs in haystack)
 *
 * (sorted programs in haystack)
 *
 * (optimally stored programs in tape)
 *
 */

/*
 * TAPE STORAGE DISPLAY FORMAT
 *
 * tp   (tape_number)   [(tape_number)...]
 *      (program_name)  [(program_name)...]
 *      (program_name)  [(program_name)...]
 *
 */

/*
 * INPUT
 *

3 2
pg1 5
pg2 10
pg3 3

 *
 */

/*
 * OUTPUT
 *

12

{pg1,5} {pg2,10} {pg3,3}

{pg3,3} {pg1,5} {pg2,10}

tp	1	2
	pg3	pg1
	pg2

 *
 */