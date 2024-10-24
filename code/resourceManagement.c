#include <stdlib.h>

#include "resourceManagement.h"

int const STRING_LENGTH = 31;

void DemoOfFunctions( ); //Added this demo to show off how to use some of the provided functions (see lines with DEMO for more hints)
double minDouble( double x, double y );


void freeQueueContents(Queue* queue) {
    while (!isEmpty(queue)) {
        Item* item = dequeue(queue);  // Get the item from the queue

        // Free the item's name (dynamically allocated by strdup)
        free(item->name);

        // Free the item itself
        free(item);
    }


}
/*freeDeparments
*  input: Deparment **inputDL, int testDataSize
*  output: none
*
*  takes list of deparments and frees the data in the struct
*/
void freeDeparments(PriorityQueue *PQinput, int testDataSize){
    while (!isEmptyPQ(PQinput)) {  // Free all departments in the priority queue
        Department *tempD = dequeuePQ(PQinput);  // Remove the department from the priority queue

        // Free the department name
        free(tempD->name);

        // Free the items in the queues
        freeQueueContents(tempD->itemsDesired);
        freeQueue(tempD->itemsDesired);  // Free itemsDesired queue

        freeQueueContents(tempD->itemsReceived);
        freeQueue(tempD->itemsReceived);  // Free itemsReceived queue

        freeQueueContents(tempD->itemsRemoved);
        freeQueue(tempD->itemsRemoved);  // Free itemsRemoved queue

        // Free the department struct itself
        free(tempD);
    }

}
/* printNames
 * input: none
 * output: none
 *
 * Prints names of the students who worked on this solution
 * REMINDER: You may only work with one other cs2123 student
 */
void printNames( )
{
  /* TODO : Fill in you and your partner's names (or N/A if you worked individually) */
  printf("\nThis solution was completed by:\n");
  printf("Robert Farrell and Braxton McMenamy\n");
  printf(" N/A\n\n");
}

/* TODO:
 * ResourceManagement
 * input: char* fileNames[], int testDataSize, double budget
 * output: none
 *
 * Simulates the algorithm from the pdf to determine what items are purchased
 * for the given budget and department item lists.
 * Print a summary of what each department received and did not receive.
 */
void ResourceManagement( char* fileNames[], int testDataSize, double budget )
{
  //DemoOfFunctions( ); //TODO: Feel free to remove this demo once you start coding your solution
  char buffer[100];
  int i;
  double price;
  //Department *departmentList =(Department*)malloc(testDataSize * sizeof(Department)); 
  PriorityQueue* Pqueu = createPQ();

  /* Create a department for each file listed in fileNames (testDataSize is the size of the fileNames array) */
    
  /* Simulate the algorithm for picking the items to purchase */

    for(i = 0; i < testDataSize;i ++){
      FILE *inFile = fopen(fileNames[i], "r");
      if(inFile == NULL){
        printf("Error - Invalid file %s\n", fileNames[0]);
        exit(-1);
      }

      Department *newDepartment = (Department*)malloc(sizeof(Department));

      fscanf(inFile, "%s", buffer);
      newDepartment->name = strdup(buffer);
      printf("Name test: %s\n", newDepartment->name);
      newDepartment->itemsDesired = createQueue();
      newDepartment->itemsReceived = createQueue();
      newDepartment->itemsRemoved  = createQueue();
      newDepartment->totalSpent = 0.00;

      //TODO add loop to get file information and save to current department

      while (fscanf(inFile, "%s %lf", buffer, &price) == 2) {
          Item* newItem = (Item*)malloc(sizeof(Item));
          newItem->name = strdup(buffer);  // Allocate memory and copy item name
          newItem->price = price;  // Store the price
          enqueue(newDepartment->itemsDesired, newItem);  // Add the item to the desired items queue
      }
      enqueueByPriority(Pqueu,newDepartment, 0.0 );
      fclose(inFile);
    }

    printPriorityQueue(Pqueu);

	/* Print the information for each department (including which items were received and which were not) */



    freeDeparments(Pqueu, testDataSize);
    freePQ(Pqueu);


} 



/* minDouble
 * input: double x, double y 
 * output: the smaller of x and y
 */
double minDouble( double x, double y ){
	if( x<=y )
		return x;
	return y;
}

/* DEMO
 * DemoOfFunctions
 * input: none
 * output: none
 *
 * Short demo of some of the provided functions.
 */
void DemoOfFunctions( )
{
  PriorityQueue *demoPQ = createPQ();
  Department* deptDemo  = (Department*)malloc( sizeof(Department) );
  
  deptDemo->name          = strdup( "DeptNameHere" );//DEMO: strdup creates a new string with given information copied into it
  deptDemo->totalSpent    = 13.37; //DEMO: all the numbers in the demo are just arbitrary values to help you spot them in the print out
  deptDemo->itemsDesired  = createQueue();  
  deptDemo->itemsReceived = createQueue();
  deptDemo->itemsRemoved  = NULL;  //DEMO: make sure you also create your other queues (only setting up 2 here to demo how that changes the print)
  
        Item* itemDemo  = (Item*)malloc( sizeof(Item) );
        itemDemo->name  = strdup( "ItemNameHere" );
        itemDemo->price = 3141.59;
        
        enqueue( deptDemo->itemsDesired, itemDemo );
  enqueueByPriority( demoPQ, deptDemo, 123.45 );
  
  printPriorityQueue( demoPQ );//DEMO: Useful function to see the contents of the PQ while testing (not used to create your tables)
  
  /*Department* temp =*/ getNextPQ( demoPQ );//DEMO: returns but doesn't remove the element from the PQ
  
  printPriorityQueue( demoPQ );//DEMO: Notice that the Demo Department is still in there
  
  Department* deptFreeDemo = dequeuePQ( demoPQ ); //DEMO: returns and removes the element from the PQ
  
  printPriorityQueue( demoPQ );//DEMO: Notice that the Demo Department is no longer in there
  
  Item* itemFreeDemo  = dequeue( deptFreeDemo->itemsDesired );
  
  freePQ( demoPQ ); //DEMO: Be sure to remove all of the elements from the PQ before freeing it
  
  free( itemFreeDemo->name );
  free( itemFreeDemo );
  
  free( deptFreeDemo->name );
  freeQueue( deptFreeDemo->itemsDesired );  //DEMO: Be sure to remove all of the elements from the Queue before freeing it
  freeQueue( deptFreeDemo->itemsReceived );
  free( deptFreeDemo );
} 