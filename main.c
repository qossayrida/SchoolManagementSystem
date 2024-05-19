// QOSSAY RIDA 1211553      section: 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define SIZEDATA 30

// Rename node pointer for BST
typedef struct node* Node;
typedef struct node* BST;

// Rename node pointer for linked list
// This list will use to store name of classes in school
typedef struct nodeList* NodeList;
typedef struct nodeList* List;

// Functions to read & write to file
BST ReadFile();
void WriteFile(BST T);
void CapitalizeFirstChar(char str[]);

// This function for BST (use to store student)
BST CreateBST();
void PrintStudents(BST T);
Node Find(int Id, BST T);
Node FindMin(BST T);
void SearchCity (char City[], BST T);
void FindUniqueClass (List L ,BST T);
void SearchClass (char Class[], BST T);
BST Insert(int Id,char Name[],char Class[],char City [],char DateEnrollment [],BST T);
BST Delete(int Id,char Name[], BST T);
BST Update (Node Element, BST T);

// This function for Linked List (use to store name of classes)
List CreateList();
void InsertToList(char Data[],List L);
int IsExist (char Data[],List L);

// Node for BST will contain information for student
struct node{
    int Id;
    char Name [SIZEDATA];
    char Class [SIZEDATA];
    char City [SIZEDATA];
    char DateEnrollment [SIZEDATA];
    Node Left;
    Node Right;
};

// Node for linked list will contain Name of classes
struct nodeList {
    char Data [SIZEDATA];
    NodeList Next;
};

int main(){
    printf("*********** Welcome to our school app ***********\n");
    BST STUDENTS = ReadFile();  // Read all students in file and insert them to BST
    int operation = 0;

    while (operation != 8){
        printf("\nPlease choose operation:\n");     // Show menu for user
        printf("1- Insert new student\n");
        printf("2- Search for student\n");
        printf("3- Print all student\n");
        printf("4- Search for a city\n");
        printf("5- Print all student by Class\n");
        printf("6- Delete student\n");
        printf("7- Save all information in file\n");
        printf("8- Exit\n");
        scanf("%d",&operation);     // Read operation from user

        if(operation==1){   // Read information for new student and insert it to BST
            int Id ;
            char Name [SIZEDATA],Class[SIZEDATA], City [SIZEDATA], DateEnrollment [SIZEDATA]="";
            printf("Please enter Id for new student\n");    // Read Id from user
            scanf("%d",&Id);

            while (Find(Id,STUDENTS) != NULL){  // Check if any student has this Id
                printf("There is another student has this ID, please enter new ID\n");
                scanf("%d",&Id);
            }

            printf("Enter another information Name, Class, City, DateEnrollment\n(Each field in line)\n");
            while (getchar() != '\n');   // Clear the input buffer (Because scanf() remains '\n' in buffer )
            scanf("%[^\n]s",Name);  // Read line from user (Name)
            scanf("%s",Class);  // Read Class
            scanf("%s",City);   // Read Data
            scanf("%s",DateEnrollment); // Read Date Enrollment

            // Make the first letter of the name uppercase and the rest lowercase to facilitate comparison
            CapitalizeFirstChar(Name);
            CapitalizeFirstChar(City);

            STUDENTS = Insert(Id,Name,Class,City,DateEnrollment,STUDENTS);  // Insert new student
        }
        else if(operation==2){  // Update data for student
            int Id ;
            printf("Please enter Id for student\n");
            scanf("%d",&Id);    // Read Id for student want update her information
            Node T = Find(Id,STUDENTS);     // Search for this Id
            if(T != NULL)
                STUDENTS = Update(T,STUDENTS);  // If Id exist go to update
            else
                printf("No student has this ID\n");   // If Id doesn't exist tell user
        }
        else if(operation==3){  // List all Students
            printf("List of all students in school:\n");
            PrintStudents(STUDENTS);    // Inorder traversal for BST
        }
        else if(operation==4){  // List all students from Data entered by user
            printf("Please enter the name of city:\n");
            char City [SIZEDATA];
            scanf("%s",City);   // Read the name of Data from user
            SearchCity(City,STUDENTS);      // Inorder traversal for BST by Data
        }
        else if(operation==5){  // Print All Student By Classes
            List L = CreateList();  // Create Linked List to save name of class
            FindUniqueClass(L,STUDENTS);    // FindFromGraph all classes in BST
            NodeList temp = L->Next ;   // Declare pointer to visit all classes in Linked List

            while (temp!=NULL){     // Visit all node in Linked List
                printf("Student in Class: %s\n",temp->Data);
                SearchClass(temp->Data,STUDENTS);   // Print all students in this class
                temp = temp->Next;  // Increment class
            }
        }
        else if(operation==6){  // Delete student from BST
            printf("Please enter ID of student to delete it\n");
            int Id ;
            scanf("%d",&Id);    // Read Id for student you  want delete
            Node check = Find(Id,STUDENTS);
            while (check == NULL){  // Check if exist student has this Id
                printf("There is no student has this Id, enter a new Id\n");
                scanf("%d",&Id);    // If Not enter another Id
                check = Find(Id,STUDENTS);
            }
            STUDENTS = Delete(Id,check->Name,STUDENTS); // Delete this Student
            printf("Done\n");
        }
        else if(operation==7){
            FILE* fp = fopen("students.txt", "w");  // This Step to remove all data in file
            fclose(fp);
            WriteFile(STUDENTS);    // Print all student of BST in file
            printf("Done\n");
        }
        else if(operation==8)
            printf("Good Bay :)");
        else
            printf("You choose invalid operation please try again\n");  // Print if choose invalid operation
    }
}

//****************************************************************************************
//                           Functions for Read & Write in file
//****************************************************************************************

// This function to read all student from file and store it at BST
BST ReadFile(){

    /*  Format data in the file
     *         ID                   each filed in different line
     *         Name
     *         Class
     *         Data
     *         Date Enrollment
     *         ---------------      space between each student
     */

    BST STUDENTS = CreateBST();     // Create BST to Save student on it
    FILE* fp = fopen("students.txt", "r");      // Declare file
    if (fp==NULL)
        return STUDENTS;

    int Id;     // Declare variable to save information of student on it before insertion
    char Name [SIZEDATA], Class [SIZEDATA],City [SIZEDATA], DateEnrollment [SIZEDATA],Trash[SIZEDATA];

    do{
        if (fscanf(fp,"%d",&Id)!=EOF){  // check if the file has new student
            getc(fp);       // Get rid of new line in the file
            fscanf(fp,"%[^\n]s",Name);   // read line from file without '\n'
            getc(fp);        // Get rid of new line in the file
            fscanf(fp,"%[^\n]s",Class);
            getc(fp);        // Get rid of new line in the file
            fscanf(fp,"%[^\n]s",City);
            getc(fp);        // Get rid of new line in the file
            fscanf(fp,"%[^\n]s",DateEnrollment);
            getc(fp);        // Get rid of new line in the file

            // Make the first letter of the name uppercase and the rest lowercase to facilitate comparison
            CapitalizeFirstChar(Name);
            CapitalizeFirstChar(City);

            // Insert Student to BST
            if (Find(Id,STUDENTS)==NULL)    // Make sure that no student has this ID
                STUDENTS = Insert(Id,Name,Class,City,DateEnrollment,STUDENTS);
            else
                // If there is a student who holds this ID, print an error
                printf("\nThere are two students with the same Id: %d\n",Id);
        }
    }while (fgets(Trash, sizeof(Trash), fp) != NULL);   // Get rid of (---------------) between two students

    fclose(fp);
    return STUDENTS ;
}

// Recursive function to print BST on file -Inorder traversal for BST
// Note: we can use Preorder or Postorder traversal in this function
// This is because when reading the file again, we maintain the balance of the tree
void WriteFile(BST T){
    FILE* fp = fopen("students.txt", "a");  // Declare the file to append students

    if (T == NULL)  // If head equal null go out
        return;

    WriteFile(T->Left);     // Check left subtree
    // Append information to file in required format
    fprintf(fp,"%d\n",T->Id);
    fprintf(fp,"%s\n",T->Name);
    fprintf(fp,"%s\n",T->Class);
    fprintf(fp,"%s\n",T->City);
    fprintf(fp,"%s\n",T->DateEnrollment);
    fprintf(fp,"---------------\n");
    fclose(fp);     // Close the file after append student
    WriteFile(T->Right);    // Check right subtree
}

void CapitalizeFirstChar(char str[]) {
    if (str[0] == '\0') // Handle empty string or NULL pointer
        return;

    str[0] = toupper(str[0]);  // Convert first character to uppercase

    for (int i = 1; str[i] != '\0'; i++)    // Convert remaining characters to lowercase
        str[i] = tolower(str[i]);
}

//****************************************************************************************
//                                  Functions for BST
//****************************************************************************************
/*              It is clear that the ID of the student is not repeated
 *              But the name of the student is repeated
 *              And we need to print the students in alphabetical order
 *              So we will create a composite key for this tree
 *              This key consists of the student's name and then his number
 *              By associating the name with the number like:
 *              Name: Qossay  Id: 1211553   the key for this student in tree: Qossay1211553
 */

// Create BST by make the value of root equal null and -Insert- function will handle
BST CreateBST() {
    return NULL;
}

// Inorder traversal for BST
void PrintStudents(BST T){
    if (T == NULL)  // If head for tree or subtree equal null go out
        return;
    PrintStudents(T->Left);     // Check left subtree
    // Print all information for head
    printf("%d\t%s\t%s\t%s\t%s\n",T->Id,T->Name,T->Class,T->City,T->DateEnrollment);
    PrintStudents(T->Right);    // Check right subtree
}

// Inorder traversal for BST & just print information of node if node->Data equal entered city
void SearchCity (char City[], BST T){
    if( T == NULL)  // If head for tree or subtree equal null go out
        return ;

    SearchCity(City,T->Left);   // Check left subtree
    if (strcmp(City,T->City)==0)     // Print all information for head jest if head->Data equal Data
        printf("%d\t%s\t%s\t%s\t%s\n",T->Id,T->Name,T->Class,T->City,T->DateEnrollment);
    SearchCity(City,T->Right);   // Check right subtree
}

void SearchClass (char Class [], BST T){
    if( T == NULL)  // If head for tree or subtree equal null go out
        return ;

    SearchClass(Class,T->Left);     // Check left subtree
    if (strcmp(Class,T->Class)==0)  // Print all information for head jest if head->Class equal Class
        printf("%d\t%s\t%s\t%s\t%s\n",T->Id,T->Name,T->Class,T->City,T->DateEnrollment);
    SearchClass(Class,T->Right);     // Check right subtree
}

// Visited all node in tree and fills linked list name of classes in this BST
void FindUniqueClass (List L ,BST T){
    if( T == NULL)      // If head for tree or subtree equal null go out
        return ;

    FindUniqueClass(L,T->Left);      // Check left subtree
    if (!IsExist(T->Class,L))   // If this class doesn't exist in linked list add it
        InsertToList(T->Class,L);
    FindUniqueClass(L,T->Right);     // Check right subtree
}

// FindFromGraph student by there id As we mentioned, the Student ID is not a key we have composite key
Node Find(int Id, BST T){
    Node Answer = NULL; // Save the node we want to found it
    if( T == NULL)  // If head for tree or subtree equal null return null
        return NULL;

    Answer = Find(Id,T->Left);  // Check left subtree
    if (Answer==NULL)   // If student doesn't exist in left subtree go to check right subtree
        Answer = Find(Id,T->Right);

    if (Id==T->Id)  // If entered Id equal Id for head put this node as answer
        Answer = T;

    return Answer;  // return the answer
}

// FindFromGraph the node has minimum key
Node FindMin(BST T){
    if(T == NULL)   // If tree or subtree equal null return null
        return NULL;
    else if(T->Left == NULL)    // If reach far left return this node
        return T;
    else
        return FindMin(T->Left);    // Go to the far left
}

// This function to insert new node by key {Name,Id}
BST Insert(int Id,char Name[], char Class [],char City [],char DateEnrollment [],BST T){
    char Key1 [2*SIZEDATA] , Key2 [2*SIZEDATA] ;    // Declare two key for root and for node we want to insert
    if (T!=NULL){
        if (strcmp(Name,T->Name)==0){   // Set value for each key (concat name with id)
            sprintf(Key1, "%s%d",Name,Id);
            sprintf(Key2, "%s%d",T->Name,T->Id);
        }
        else{
            strcpy(Key1,Name);
            strcpy(Key2,T->Name);
        }
    }

    if(T == NULL){ // If head or subtree equal null
        T = (Node)malloc(sizeof(struct node));  // Create new node to insert
        if( T == NULL)  // Check if memory full
            printf("Out of space");
        else{
            // Set all information for student at node
            T->Id = Id;
            strcpy(T->Name,Name);
            strcpy(T->Class,Class);
            strcpy(T->City,City);
            strcpy(T->DateEnrollment,DateEnrollment);
            T->Left = T->Right = NULL;      // Left and right for new node pointed at null
        }
    }
    else if(strcmp(Key1,Key2)<0)    // If Key2 < Key1 go to insert at left subtree
        T->Left = Insert(Id,Name,Class,City,DateEnrollment,T->Left);
    else if(strcmp(Key1,Key2)>0)     // If Key2 > Key1 go to insert at right subtree
        T->Right = Insert(Id,Name,Class,City,DateEnrollment,T->Right );

    return T;   // Return the result
}

// Delete student from BST
BST Delete(int Id,char Name[], BST T){
    char Key1 [2*SIZEDATA] , Key2 [2*SIZEDATA] ;         // Declare two key for root and for node we want to insert
    if (strcmp(Name,T->Name)==0){   // Set value for each key (concat name with id)
        sprintf(Key1, "%s%d",Name,Id);
        sprintf(Key2, "%s%d",T->Name,T->Id);
    }
    else{
        strcpy(Key1,Name);
        strcpy(Key2,T->Name);
    }
    Node TmpCell;

    if(strcmp(Key1,Key2)<0)     // If Key2 < Key1 go to delete from left subtree
        T->Left = Delete(Id,Name,T->Left );
    else if(strcmp(Key1,Key2)>0)    // If Key2 > Key1 go to delete from right subtree
        T->Right = Delete(Id,Name,T->Right );
    else    // If Key2 equal Key1 Delete this node
        if (T->Left && T->Right) {  // If this node has two children
            TmpCell = FindMin(T->Right);    // FindFromGraph the minimum child in right subtree
            // Switch information between the node we want to delete with the minimum node on the right subtree
            T->Id = TmpCell->Id;
            strcpy(T->Name,TmpCell->Name);
            strcpy(T->Class,TmpCell->Class);
            strcpy(T->City,TmpCell->City);
            strcpy(T->DateEnrollment,TmpCell->DateEnrollment);
            T->Right = Delete(T->Id,T->Name,T->Right);  // Delete minimum node on the right subtree
        } else {
            TmpCell = T;
            if (T->Left == NULL)        //  If just has right subtree
                T = T->Right;       // Make the right subtree as head
            else if (T->Right == NULL)  //  If just has left subtree
                T = T->Left;        // Make the left subtree as head
            free(TmpCell);  // Delete node
        }

    return T;   // return the result
}

// Update value for given node
BST Update (Node Element, BST T){
    // Print information before update
    printf("%d\t%s\t%s\t%s\t%s\n",Element->Id,Element->Name,Element->Class,Element->City,Element->DateEnrollment);
    int edit=0 , newId;     // Declare edit to choose any field want to update
    char newData [SIZEDATA];
    while (edit != 6) {
        printf("\nChoose filed to edit\n");   // Choose the field
        printf("1- Edit ID\n");
        printf("2- Edit Name\n");
        printf("3- Edit Class\n");
        printf("4- Edit Data\n");
        printf("5- Edit Date Enrollment\n");
        printf("6- Exit\n");
        scanf("%d", &edit);

        if (edit==6){   // if choose exit break the loop
            printf("Done\n");
            break;
        }

        printf("Please enter new value:\n");    // Ask to enter new data
        if (edit == 1){     // If want to update Id
            scanf("%d", &newId);
            if (Find(newId,T)==NULL){   // Check if valid Id
                Element->Id = newId;    // Save new Id in the node
                strcpy(newData,Element->Name);  // Save name for this student to newData
                edit=2;     // Go to step two (remove student and add it again)
            }
            else
                printf("This Invalid Id\n");    // Print InValid Id
        }
        else { // If want to update another field
            scanf("%[^\n]s",newData);
            getchar();
        }

        if (edit == 2){     // If want to update name
            char Buffer1[SIZEDATA],Buffer2[SIZEDATA],Buffer3[SIZEDATA];
            // Save all information then delete the node
            newId=Element->Id;
            strcpy(Buffer1,Element->Class);
            strcpy(Buffer2,Element->City);
            strcpy(Buffer3,Element->DateEnrollment);
            T= Delete(Element->Id,Element->Name,T);    // Delete the node
            T= Insert(newId,newData,Buffer1,Buffer2,Buffer3,T);     // Insert student again
            Element = Find(newId,T);
        }
        else if (edit == 3)
            strcpy(Element->Class,newData); // Update Class
        else if (edit == 4)
            strcpy(Element->City,newData);  // Update Data
        else if (edit == 5)
            strcpy(Element->DateEnrollment,newData);    // Update Date Enrollment
        else
            printf("You choose invalid operation please try again\n");
    }
    return T ;  // Return the result after update
}

//****************************************************************************************
//                                  Functions for LinkedList
//****************************************************************************************
// We will use the linked list in this project to memorize the names of the classes in this school

List CreateList(){
    List L = (List)malloc(sizeof(struct nodeList)); // Create head for new linked list

    if(L == NULL)   // Handle if memory full
        printf("Out of memory!\n");

    L->Next = NULL; // Head pointed at null
    return L;
}

// Insert new Data in the linked list
void InsertToList(char Data[],List L){
    NodeList temp= (NodeList)malloc(sizeof(struct nodeList));   // Create new node to save data on it
    strcpy(temp->Data,Data);    // Save new data at node Created
    temp->Next = L->Next;   // Append new node to Linked List
    L->Next = temp;
}

// Check if entered data exist at linked list
int IsExist (char Data[],List L){
    NodeList temp = L->Next ;   // Pointer to visit all node

    while (temp != NULL && strcmp(Data,temp->Data)!=0)  // Check if entered data equal any data in linked list
        temp = temp->Next;  // Increment the pointer

    return (temp!=NULL);    // if temp equal null return 0 (that mean data doesn't exist)
}