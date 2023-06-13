#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//struct definition
typedef struct Point {
    double x;
    double y;
}Point;

//declare function definitions
void loadFile(Point * arr);
int checkSide(Point pOne, Point pTwo, Point pThree);
double distance(Point pOne, Point pTwo, Point pThree);
void quickHull(Point * arr, Point * extreme, int length, Point pOne, Point pTwo, int side);
void saveToArr(Point * extreme, Point temp);
void sortArr(Point * extreme, int length);
int cmpPoint(const void * a, const void * b);
void sortArr(Point * extreme, int length);
void shortestPath(Point * extreme, int length);

int main(){
    //declare and initialize clock counters
    clock_t start, end;
    //declare and initialize variables for array and array size
    int length =  30000;
    int numExtreme = 0;
    Point * arr = calloc(length, sizeof(Point));
    Point * extreme = calloc(150, sizeof(Point));
    //load array with file contents
    loadFile(arr);
    //get smallest and largest x values
    int min_x = 0, max_x = 0;
    for (int i = 0; i < length; i++){
        if (arr[i].x < arr[min_x].x)
            min_x = i;
        if (arr[i].x > arr[max_x].x)
            max_x = i;
    }
    //start timer
    start = clock();
    //get extreme points
    quickHull(arr, extreme, length, arr[min_x], arr[max_x], 1);
    quickHull(arr, extreme, length, arr[min_x], arr[max_x], -1);
    //end timer
    end = clock();
    //get num of extreme points
    for(int i = 0; i < 150; i++){
        if(extreme[i].x == 0 && extreme[i].y == 0 ){
            break;
        }
        numExtreme++;
    }
    //sort extreme points in correct order
    sortArr(extreme, numExtreme);
    //display extreme points
    printf("EXTREME POINTS IN THE HULL:\n");
    for(int i = 0; i < numExtreme; i++){
        printf("%lf %lf\n", extreme[i].x, extreme[i].y);
    }
    printf("Number of Extreme Points: %d\n", numExtreme);
    //calculate shortest path of 2 points
    shortestPath(extreme, numExtreme);
     //calculate time in seconds
    double time = ((double)end - start)/CLOCKS_PER_SEC;
    //print execution time
    printf("Hull Execution Time: %f seconds\n", time);
    //free memory
    free(arr);
    free(extreme);
    return 0;
}//end main

/*
loadFile: Reads from a file and loads an array with the file contents
In: Point * arr
Out: N/A
Post: Loads an array with file contents
*/
void loadFile(Point * arr){
    //declare and initialize variables
    char * fName = "data_A2_Q2.txt";
    char * fileLine = malloc(sizeof(char) * 500);
    int count = 0;

    //declare file pointer
    FILE *inFile = NULL;

    //open file for reading
    inFile = fopen(fName, "r");

    //if file is empty
    if(inFile == NULL){ 
        //print error message
        printf("File error.\n");
        //free memory
        free(fileLine);
        //end function call
        return ;
    } else {
        //loop through file
        int index;
        while(fgets(fileLine, 500, inFile) != NULL){
            index = 0;
            fileLine[strlen(fileLine) - 1] = '\0';
            //seperate string into numbers
            //get first number
            char * token;
            token = strtok(fileLine, " ");

            //loop until token equals NULL
            while(token != NULL){
                if(strcmp(token, " ") == 0){
                    continue;
                } 
                //add num to array
                double temp = atof(token);
                if(index == 0){
                    arr[count].x = temp;
                } else if(index == 1){
                    arr[count].y = temp;
                }
                //add 1 to index
                index++;
                //get next token
                token = strtok(NULL, " ");
            }//end inner while
            //add 1 to count
            count++;
        }//end outer while
    }

    //close file
    fclose(inFile);
    //free memory
    free(fileLine);
}//end loadFile

/*
quickHull: Finds the extreme points of a hull
In: Point * arr, Point * extreme, int length, Point pOne, Point pTwo, int side
Out: N/A
Post: Creates an array with extreme points
*/
void quickHull(Point * arr, Point * extreme, int length, Point pOne, Point pTwo, int side){
    //declare and initialize variables
    int index = -1;
    int dist = 0;
    //loop throuhgh array
    for(int i = 0; i < length; i++){
        //calculate distance of line
        double lineDist = distance(pOne, pTwo, arr[i]);
        //find point with max distance
        if(checkSide(pOne, pTwo, arr[i]) == side && lineDist > dist){
            index = i;
            dist = lineDist;
        }
    }
    //if extreme point
    if(index == -1){
        //save extreme point to array
        saveToArr(extreme, pOne);
        saveToArr(extreme, pTwo);
        return ;
    }
    //recursively call function
    quickHull(arr, extreme, length, arr[index], pOne, -checkSide(arr[index], pOne, pTwo));
    quickHull(arr, extreme, length, arr[index], pTwo, -checkSide(arr[index], pTwo, pOne));

}//end quickHull

/*
checkSide: Checks the side that the point lies on in regards to the line
In: Point pOne, Point pTwo, Point pThree
Out: 1, -1, or 0
Post: returns an integer value corresponding to the position in relation to the line
*/
int checkSide(Point pOne, Point pTwo, Point pThree){
    //calculate position
    int num = (pThree.y - pOne.y) * (pTwo.x - pOne.x) - (pTwo.y - pOne.y) * (pThree.x - pOne.x);
    //return value based on position
    if(num > 0){
        return 1;
    } else if(num < 0){
        return -1; 
    } else {
        return 0;
    }
}//end checkSide

/*
distance: Calculates the distance between 2 points
In: Point pOne, Point pTwo, Point pThree
Out: double
Post: returns the distance
*/
double distance(Point pOne, Point pTwo, Point pThree){
    //calculate the distance between the points
    double num = abs((pThree.y - pOne.y) * (pTwo.x - pOne.x) - (pTwo.y - pOne.y) * (pThree.x - pOne.x));
    return num;
}//end distance

/*
saveToArr: Removes duplicate points from array
In: Point * extreme, Point temp
Out: N/A
Post: Creates an array of extreme points with no duplicates
*/
void saveToArr(Point * extreme, Point temp){
    //declare and initialize variable
    int duplicate = 0;
    //loop through array
    for(int i = 0; i < 150; i++){
        //if point already exists
        if(extreme[i].x == temp.x && extreme[i].y == temp.y){
            duplicate = 1;
            break;
        }
    }//end for

    if(duplicate == 0){
        //loop through array
        for(int i = 0; i < 150; i++){
            //save point to array
            if(extreme[i].x == 0 && extreme[i].y == 0){
                extreme[i] = temp;
                break;
            }
        }//end for
    }
}//end saveToArr

/*
sortArr: Sorts an array of extreme points in the correct order
In: Point * extreme, int length
Out: N/A
Post: Sorted array of extreme points
*/
void sortArr(Point * extreme, int length){
    //create temporary array
    Point * tempArr = malloc(sizeof(Point) * length);
    //initialize temp array
    for(int i = 0; i < length; i++){
        tempArr[i] = extreme[i];
    }
    //initialize and declare left and right pointer
    int left = 0;
    int right = length - 1;
    //sort array by x values in increasing order
    qsort(tempArr, length, sizeof(Point), cmpPoint);
    //get point with smallest x
    Point pOne = tempArr[0];
    //get point with largest x
    Point pTwo = tempArr[length - 1];
    //loop through points
    for(int i = 0; i < length; i++){
        //create a line and check if above or below
        if((tempArr[i].x - pOne.x) * (pTwo.y - pOne.y) * (tempArr[i].y - pOne.y) * (pTwo.x - pOne.x) > 0){
            //swap point
            extreme[left] = tempArr[i];
            left++;
        } else {
            //swap point
            extreme[right] = tempArr[i];
            right--;
        }
    }//end for

}//end sortArr

/*
cmpPoint: Helper function for qsort
In: const void * a, const void * b
Out: 1, 0, or -1
Post: Returns 1, 0, or -1
*/
int cmpPoint(const void * a, const void * b){
    //cast as points
    Point * pOne = (Point*)a;
    Point * pTwo = (Point*)b;
    return (pOne->x > pTwo->x) - (pOne->x < pTwo->x);
}//end cmpPoint

/*
shortestPath: Calculates the shortest path and distance from point One to point Two
In: Point * extreme, int length
Out: N/A
Post: Displays the shortest path, distance and number of points on path
*/
void shortestPath(Point * extreme, int length){
    //declare and initialize variables
    double xOfs1 = 0, yOfs1 = 0, xOfs2 = 0, yOfs2 =  0;
    double leftDist = 0, rightDist = 0;
    Point * leftPath = calloc(length, sizeof(Point));
    Point * rightPath = calloc(length, sizeof(Point));
    int pOneIndex = 0, pTwoIndex = 0;
    int nextPointsLeft = 0;
    int nextPointsRight = 0;
    int leftCount = 0, rightCount = 0;
    //prompt user for Point one and Point two
    printf("Enter the x coordinate of s1:\n");
    scanf("%lf", &xOfs1);
    printf("Enter the y coordinate of s1:\n");
    scanf("%lf", &yOfs1);
    printf("Enter the x coordinate of s2:\n");
    scanf("%lf", &xOfs2);
    printf("Enter the y coordinate of s2:\n");
    scanf("%lf", &yOfs2);
    printf("The shortest path around:\n");
    
    //find and save index of both points
    for(int i = 0; i < length; i++){
        if(xOfs1 == extreme[i].x && yOfs1 == extreme[i].y){
            pOneIndex = i;
        }
        if(xOfs2 == extreme[i].x && yOfs2 == extreme[i].y){
            pTwoIndex = i;
        }
    }//end for

    //save index in temp variables
    int tempOneIndex = pOneIndex;
    int tempTwoIndex = pTwoIndex;

    //loop through left path
    while(pOneIndex != pTwoIndex){
        //if out of array index, loop back
        if(pOneIndex == -1){
            pOneIndex = length - 1;
        }
        //get next point
        int nextP = pOneIndex - 1;
        if(pOneIndex == 0){
            nextP = length - 1;
        }
        //save point
        leftPath[leftCount] = extreme[pOneIndex];
        //calculate distance
        double dist = ((extreme[pOneIndex].x - extreme[nextP].x) * (extreme[pOneIndex].x - extreme[nextP].x)) + ((extreme[pOneIndex].y - extreme[nextP].y) * (extreme[pOneIndex].y - extreme[nextP].y));
        dist = sqrt(dist);
        //add to total distance
        leftDist += dist;
        //decremen and increment counters
        leftCount++;
        pOneIndex--;
        nextPointsLeft++;
    }//end while

    //save point
    leftPath[leftCount] = extreme[pTwoIndex];
    //increment counter
    leftCount++;
    nextPointsLeft++;

    //loop through right path
    while( tempOneIndex !=  tempTwoIndex){
        //if out of array index, loop back
        if( tempOneIndex == length){
            tempOneIndex = 0;
        }
        //get next point
        int nextP =  tempOneIndex + 1;
        if( tempOneIndex == length - 1){
            nextP = 0;
        }
        
        //save point
        rightPath[rightCount] = extreme[tempOneIndex];
        //calculate distance
        double dist = ((extreme[ tempOneIndex].x - extreme[nextP].x) * (extreme[ tempOneIndex].x - extreme[nextP].x)) + ((extreme[ tempOneIndex].y - extreme[nextP].y) * (extreme[ tempOneIndex].y - extreme[nextP].y));
        dist = sqrt(dist);
        //calculate total distance
        rightDist += dist;
        //increment and decrement counters
        tempOneIndex++;
        rightCount++;
        nextPointsRight++;
    }  
    
    //save point
    rightPath[rightCount] = extreme[pTwoIndex];
    //increment counter
    rightCount++;
    nextPointsRight++;

    //check which path is shorter
    if(leftDist < rightDist){
        //display path info
        for(int i = 0; i < nextPointsLeft; i++){
            printf("(%lf %lf)\n", leftPath[i].x, leftPath[i].y);
        }
        printf("The distance: %.1lf\n", leftDist);
        printf("The number of points: %d\n", nextPointsLeft);
    } else {
        //display path info
        for(int i = 0; i < rightCount; i++){
            printf("(%lf %lf)\n", rightPath[i].x, rightPath[i].y);
        }
        printf("The distance: %.1lf\n", rightDist);
        printf("The number of points: %d\n", nextPointsRight);
    }
}//end shortestPath
