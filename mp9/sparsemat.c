#include "sparsemat.h"

#include <stdio.h>
#include <stdlib.h>

/*
First create helper function new_data which takes a row, col, and value and adds it to an already sorted linked list such that it stays sorted (basically set_tuples() function)
 - if value is zero and already exists in linked list, delete that node from the linked list, otherwise don't do anything
 - if needing to add at head ptr make sure to switch headptr location
 - otherwise iterate through the linked list until you get the node before the one you want to insert into
 - if row,col already exists and its not zero all you have to do is update the value
 - else create a node on the heap assign it with row, col, value, and insert it into linked list

Then write load_tuples() which reads from the file to set the headptr to the rows, cols, and inits the other variables
 - uses set_tuples() for each row, col, value in the input file
Then gv_tuples() which simply returns the value of a row,col if it exists in the linked list otherwise return 0
Then set_tuples which just calls the helper function.
Then save_tuples which goes through the linked list and writes the row, col, val to the file passed as parameter (also the headptr information)
Then add_tuples which follows the algorithm provided to add two tuples passed as parameters. Uses gv_tuples to get val information.
Skip mult_tuples
Then destroy_tuples which goes through the linked list with a temp ptr to connect each node's next and then delete the node until reached final node
 - also free headptr
*/


void new_data(sp_tuples** headpptr, int row, int col, double val){

    sp_tuples_node* cursor = (*headpptr)->tuples_head; // pointer to which node in the linked list we are looking at right now
    if (cursor == NULL || (row < (cursor->row)) || (row == (cursor->row) && col < (cursor->col) )){ // if linked list is empty or new node needs to be head node
        if (val == 0)
            return; // if zero then has to be empty linked list so just return

        sp_tuples_node* temp = (sp_tuples_node*) malloc(sizeof(sp_tuples_node)); // new node to be head node
        temp->row = row;
        temp->col = col;
        temp->value = val;
        temp->next = (*headpptr)->tuples_head; // init params and set the next to be where the head is currently pointing

        (*headpptr)->tuples_head = temp; // set head to new node
        (*headpptr)->nz++; //increment b/c new nonzero
        return; 
    }

    while (cursor->next != NULL && ( cursor->next->row < row  || (row == cursor->next->row &&  cursor->next->col < col ))) // keep going as long as don't reach end of list and reach a node 
        cursor = cursor->next;                                                                                              // with bigger row major order num
    

    if (cursor->next != NULL && row == cursor->next->row && col == cursor->next->col){ // at same position, update value
        if (val == 0){ // if same position and zero, then have to free the node and connect the two sides.
            sp_tuples_node* temp_ptr = cursor->next;
            cursor->next = cursor->next->next;
            free(temp_ptr);
            (*headpptr)->nz--; // removing node so decrement
        }
        else{
            cursor->next->value = val; // if same position and not zero, just have to update the value
        }
        return; // done with function
    }
    if (val == 0) // if not same position and zero, nothing to do 
        return;

    sp_tuples_node* temp = (sp_tuples_node*) malloc(sizeof(sp_tuples_node)); // if didn't go through any edge case then just insert the node normally
    temp->row = row;
    temp->col = col;
    temp->value = val;
    temp->next = cursor->next;
    cursor->next = temp;

    (*headpptr)->nz++;

}
sp_tuples * load_tuples(char* input_file)
{
    FILE* f = fopen(input_file, "r");
    if (f == NULL){
        fclose(f);
        return NULL; // can't access file
    }
    sp_tuples* headptr = (sp_tuples*) malloc(sizeof(sp_tuples)); // create head pointer
    fscanf(f, "%d %d\n", &headptr->m, &headptr->n); 
    headptr->nz = 0;
    headptr->tuples_head = NULL; // get rows, cols, and initalize other values
    
    int row, col;
    double val;
    int scannedVals = fscanf(f, "%d %d %lf\n", &row, &col, &val); // get the row, col, val of a new node to be added (or deleted)
    while (scannedVals == 3){ // as long as we are still reading 3 numbers keep going
        new_data(&headptr, row, col, val); // call helper function
        scannedVals = fscanf(f, "%d %d %lf\n", &row, &col, &val); // read new nums
    }
    fclose(f);
    return headptr; // close file and return head pointer of linked list
}

double gv_tuples(sp_tuples * mat_t,int row,int col)
{
    sp_tuples_node* cursor = mat_t->tuples_head;
    while (cursor != NULL){
        if (cursor->row == row && cursor->col == col){
            return cursor->value; // if row, col exists in the linked list then return it
        }
        cursor = cursor->next;    // otherwise go to next node until null then return 0
    }
    return (double) 0;
}

void set_tuples(sp_tuples * mat_t, int row, int col, double value)
{
    new_data(&mat_t, row, col, value); // exact same as helper function (accidentally made that first), just used double pointer for sp_tuples which is better
}

void save_tuples(char * file_name, sp_tuples * mat_t)
{
    FILE* f = fopen(file_name, "w");
    if (f == NULL){
        fclose(f);
        return; // can't access file
    }
    fprintf(f, "%d %d\n", mat_t->m, mat_t->n); // write row, col to the file
    
    sp_tuples_node* cursor = mat_t->tuples_head;
    while (cursor != NULL){
        fprintf(f, "%d %d %lf\n", cursor->row, cursor->col, cursor->value); // printing the linked list elements until get to tail
        cursor = cursor->next;
    }
    fclose(f);
    return;
}

sp_tuples * add_tuples(sp_tuples * matA, sp_tuples * matB){

    sp_tuples* matC = (sp_tuples*) malloc(sizeof(sp_tuples));
    matC->m = matA->m;
    matC->n = matA->n;
    matC->nz = 0;
    matC->tuples_head = NULL; // first create new linked list and initaize with rows, cols from other one and other default values

    sp_tuples_node* cursor = matA->tuples_head;
    double sum;
    while (cursor != NULL){
        sum = gv_tuples(matC, cursor->row, cursor->col) + cursor->value;
        new_data(&matC, cursor->row, cursor->col, sum); // add matC and matA, but matA is all zero so effectively just copying matA into matC
        cursor = cursor->next;
    }

    cursor = matB->tuples_head;
    while (cursor != NULL){
        sum = gv_tuples(matC, cursor->row, cursor->col) + cursor->value;
        new_data(&matC, cursor->row, cursor->col, sum); // add matC and matB, but matC actually contains something now
        cursor = cursor->next;
    }

    return matC;
}

sp_tuples * mult_tuples(sp_tuples * matA, sp_tuples * matB){ 
    //return retmat;
    return NULL;
}
	
void destroy_tuples(sp_tuples * mat_t){
    
    sp_tuples_node* cursor = mat_t->tuples_head;
    sp_tuples_node* temp;
    while (cursor != NULL){
        temp = cursor;
        cursor = cursor->next; // go through the linked list and delete it one by one, using the temp node so that the cursor can point 
        free(temp);            // to the next node that needs to be deleted
    }
    free(mat_t); // delete the head pointer struct
    return;
}  






