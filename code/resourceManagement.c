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
void freeDeparment(Department *DPinput)
{                                       // Free all departments in the priority queue
 // Remove the department from the priority queue

    // Free the department name
    free(DPinput->name);

    // Free the items in the queues
    freeQueueContents(DPinput->itemsDesired);
    freeQueue(DPinput->itemsDesired); // Free itemsDesired queue

    freeQueueContents(DPinput->itemsReceived);
    freeQueue(DPinput->itemsReceived); // Free itemsReceived queue

    freeQueueContents(DPinput->itemsRemoved);
    freeQueue(DPinput->itemsRemoved); // Free itemsRemoved queue

    // Free the department struct itself
    free(DPinput);
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
  printf("Robert Farrell   Section: 005\n");
  printf("Braxton McMenamy Section: 006\n\n");
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
  double totalBudget = budget;

  PriorityQueue *Pqueu = createPQ();


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
    newDepartment->itemsDesired = createQueue();
    newDepartment->itemsReceived = createQueue();
    newDepartment->itemsRemoved = createQueue();
    newDepartment->totalSpent = 0.00;

    
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

    }

    // If no items were purchased, give a scholarship
    if (isEmpty(dept->itemsDesired)) {
        double scholarship = minDouble(1000.0, budget);
        budget -= scholarship;
        dept->totalSpent += scholarship;
        sprintf(outputString, "$%-10.2lf", scholarship);
        printf("Department of %-30s - Scholarship                    - %20s\n", dept->name, outputString);

        Item *newItem = (Item *)malloc(sizeof(Item));
        newItem->name = strdup("Scholarship");                // Allocate memory and copy item name
        newItem->price = scholarship;
        enqueue(dept->itemsReceived,newItem);                 //add sholarship to item recived
    }


    // Update department's priority and re-enqueue
    enqueueByPriority(Pqueu, dept, dept->totalSpent);
}

    printf("\n\n");



while (!isEmptyPQ(Pqueu)) {
    Department* DP = dequeuePQ(Pqueu);

    printf("Department of %s\n",DP->name);
    sprintf(outputString, "$%-10.2lf", DP->totalSpent);
    printf("Total Spent       = %s\n",outputString);
    printf("Percent of Budget = %.2lf%%\n",(DP->totalSpent/totalBudget)*100.0);
    printf("----------------------------\n");

    printf("ITEMS RECEIVED\n");
    while(!isEmpty(DP->itemsReceived)){
      Item* itemInput = dequeue(DP->itemsReceived);
      sprintf(outputString, "$%-10.2lf", itemInput->price);
      printf("%-30s - %-20s\n",itemInput->name,outputString);

      // Free the item's name (dynamically allocated by strdup)
      free(itemInput->name);

      // Free the item itself
      free(itemInput);
    }
    printf("\n\n");

    printf("ITEMS NOT RECEIVED\n");
    while(!isEmpty(DP->itemsRemoved)){
      Item* itemInput = dequeue(DP->itemsRemoved);
      sprintf(outputString, "$%-10.2lf", itemInput->price);
      printf("%-30s - %-20s\n",itemInput->name,outputString);
      // Free the item's name (dynamically allocated by strdup)
      free(itemInput->name);

      // Free the item itself
      free(itemInput);
    }
    printf("\n\n");
    

    freeDeparment(DP);
}



    freePQ(Pqueu);
  }



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

