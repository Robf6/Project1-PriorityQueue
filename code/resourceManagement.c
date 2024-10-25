#include <stdlib.h>

#include "resourceManagement.h"

int const STRING_LENGTH = 31;

void DemoOfFunctions(); // Added this demo to show off how to use some of the provided functions (see lines with DEMO for more hints)
double minDouble(double x, double y);

void freeQueueContents(Queue *queue)
{
  while (!isEmpty(queue))
  {
    Item *item = dequeue(queue); // Get the item from the queue

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
void freeDeparments(PriorityQueue *PQinput, int testDataSize)
{
  while (!isEmptyPQ(PQinput))
  {                                         // Free all departments in the priority queue
    Department *tempD = dequeuePQ(PQinput); // Remove the department from the priority queue

    // Free the department name
    free(tempD->name);

    // Free the items in the queues
    freeQueueContents(tempD->itemsDesired);
    freeQueue(tempD->itemsDesired); // Free itemsDesired queue

    freeQueueContents(tempD->itemsReceived);
    freeQueue(tempD->itemsReceived); // Free itemsReceived queue

    freeQueueContents(tempD->itemsRemoved);
    freeQueue(tempD->itemsRemoved); // Free itemsRemoved queue

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
void printNames()
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
void ResourceManagement(char *fileNames[], int testDataSize, double budget)
{
  
  char buffer[100];
  int i;
  double price;
  char outputString[21];
  // Department *departmentList =(Department*)malloc(testDataSize * sizeof(Department));
  PriorityQueue *Pqueu = createPQ();

  /* Create a department for each file listed in fileNames (testDataSize is the size of the fileNames array) */

  /* Simulate the algorithm for picking the items to purchase */

  for (i = 0; i < testDataSize; i++)
  {
    FILE *inFile = fopen(fileNames[i], "r");
    if (inFile == NULL)
    {
      printf("Error - Invalid file %s\n", fileNames[0]);
      exit(-1);
    }

    Department *newDepartment = (Department *)malloc(sizeof(Department));

    fscanf(inFile, "%s", buffer);
    newDepartment->name = strdup(buffer);
    // printf("Name test: %s\n", newDepartment->name);
    newDepartment->itemsDesired = createQueue();
    newDepartment->itemsReceived = createQueue();
    newDepartment->itemsRemoved = createQueue();
    newDepartment->totalSpent = 0.00;

    // TODO add loop to get file information and save to current department

    while (fscanf(inFile, "%s %lf", buffer, &price) == 2)
    {
      Item *newItem = (Item *)malloc(sizeof(Item));
      newItem->name = strdup(buffer);                // Allocate memory and copy item name
      newItem->price = price;                        // Store the price
      enqueue(newDepartment->itemsDesired, newItem); // Add the item to the desired items queue
    }
    enqueueByPriority(Pqueu, newDepartment, 0.0);
    fclose(inFile);
  }

printf("ITEMS PURCHASED\n");
printf("----------------------------\n");

while(budget > 0) {
    Department* dept = dequeuePQ(Pqueu);

    while(!isEmpty(dept->itemsDesired)) {
        Item* inputItem = dequeue(dept->itemsDesired);
        if(inputItem->price <= budget){
            budget -= inputItem->price;
            dept->totalSpent += inputItem->price;
            sprintf(outputString, "$%-10.2lf", inputItem->price);
            printf("Department of %-30s - %-30s - %20s\n", dept->name, inputItem->name, outputString);
            enqueue(dept->itemsReceived, inputItem);
            break;
        }
        enqueue(dept->itemsRemoved,inputItem);
        //printQueue(dept->itemsDesired);

    }

    // If no items were purchased, give a scholarship
    if (isEmpty(dept->itemsDesired)) {
        double scholarship = minDouble(1000.0, budget);
        budget -= scholarship;
        dept->totalSpent += scholarship;
            sprintf(outputString, "$%-10.2lf", scholarship);
            printf("Department of %-30s - Scholarship                    - %20s\n", dept->name, outputString);
    }
    //printf("put away dep\n");
    // Update department's priority and re-enqueue
    enqueueByPriority(Pqueu, dept, dept->totalSpent);
}


    printPriorityQueue(Pqueu); // print test



// while (sizeOfPriorityQueue(Pqueu) > 2) {
//     Department* DP = dequeuePQ(Pqueu);
//     Item* itemInput = dequeue(DP->itemsReceived);
    
//     // Prepare price string for alignment
//     char outputString[21];
//     sprintf(outputString, "$%-10.2lf", itemInput->price); // Allocate enough width for alignment
    
//     printf("Department of %-35s - %-30s - %20s\n", DP->name, itemInput->name, outputString);

//     // Re-enqueue item and department to maintain their place in queues
//     enqueue(DP->itemsDesired, itemInput);
//     enqueueByPriority(Pqueu, DP, DP->totalSpent);
// }


    freeDeparments(Pqueu, testDataSize);
    freePQ(Pqueu);
  }

  // printf("Department of %s\n",testDP->name);
  // printf("Total Spent =        %s\n",outputString); //update number
  // printf("----------------------------\n");
  // printf("ITEMS RECEIVED");
  // printf("%-30s - %20s\n",itemInput->name,outputString);

  // enqueue(testDP->itemsDesired,itemInput);
  // enqueueByPriority(Pqueu,testDP,0);

  /* Print the information for each department (including which items were received and which were not) */
  // TODO
  /* minDouble
   * input: double x, double y
   * output: the smaller of x and y
   */
  double minDouble(double x, double y)
  {
    if (x <= y)
      return x;
    return y;
  }

  