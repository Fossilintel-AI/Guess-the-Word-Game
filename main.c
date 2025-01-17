#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


//// This is the structure responsible for creating a single linked list
typedef struct Node
{
    char *data;
    struct Node *next;
}Node;

/** Create a new node and initialize it with the provided data.
// - data: The data to be stored in the new node.
// Returns: A pointer to the newly created node. **/
Node *CreateNode(char *data)
{
    Node *NewNode = (Node*) malloc(sizeof(Node));
    if(NewNode == NULL)
    {
        printf("Failed stage of memory allocation\n");
        exit(1);
    }
    NewNode->data = strdup(data);
    NewNode->next = NULL;
    return NewNode;

}

/** Insert data into a node and add it to the linked list.
// - head: Pointer to the head of the linked list.
// - data: The data to be inserted into the node.
// Returns: The head of the updated linked list. **/
Node * insertNode(Node *head,char *data)
{
    if(head == NULL)
        return CreateNode(data);
    else
    {
        Node *current = head;
        while(current->next != NULL)
            current =current->next;
        current->next = CreateNode(data);
        return head;
    }
}

/** Read data from a text file and populate the linked list.
// - head: Pointer to the head of the linked list.
// - totalWord: Pointer to an integer that stores the total number of words read.
// Returns: The head of the linked list.**/
Node *ReadtextFile(Node *head,int *totalWord)
{
    //creating a file pointer variable
    FILE  *file;

    //Opening the text file in a reading mode
    file = fopen("names.txt","r");

    //error handling
    //if we fail to open the file, then exit
    if(file == NULL)
    {
        printf("Error encountered while opening file");
        exit(1);
    }

    //Create a buffer to adjust the size
    char word[1024];

    while(fgets(word,sizeof(word),file)!= NULL)
    {
        size_t wordLength = strlen(word);
        if(wordLength > 0 &&word[wordLength-1]== '\n')
        {
            word[wordLength-1] = '\0';
        }
        head = insertNode(head,word);
        *totalWord +=1;
    }
    fclose(file);
    return head;

}


/** Write data to a text file.
//User can stop writing by typing Done.
 .**/
void writeFile()
{
    //char array to store the word added by user
    //I MIGHT CHANGE THIS TO BE THE LONGEST WORD'S LENGTH FROM THE DICTIONARY
    char AddingWords[1024];
    int boolen = 1;
    FILE *file;

    //Start by opening the file
    file = fopen("names.txt","a");

    //Important check if file doesn't open
    if(file == NULL)
    {
        printf("Error while opening file.\n");
        exit(1);
    }
    printf("\n");
    printf("Please Begin Adding Words,Type DONE when you done adding words: \n");
    while(boolen)
    {
        //Continue reading each line from the txt file to the buffer
        fgets(AddingWords,1024,stdin);

        //Condition to cut the process of writting in the text file
        if(strcmp(AddingWords,"DONE\n") == 0)
            boolen =0;
        //IGNORE ADDING SPACES IN THE LIST
        else if (strcmp(AddingWords,"\n") != 0)
            fputs(AddingWords,file);
    }

    //close the file
    fclose(file);

}
/** Function to free the memory utilized by the linked list.
// - head: Pointer to the head of the linked list.**/
void dispose(Node *head)
{
    while(head != NULL)
    {
        Node *temp= head;
        head = head->next;
        free(temp->data);
        free(temp);
    }
}

/** Function to print the linked list.
// - head: Pointer to the head of the linked list.
// - lines: The number of lines in the linked list.
// Returns: A 2D array of strings representing the linked list.**/
char**  StorageOflist(Node *head,int lines) {
    //Dynamic memory allocation for a 2D.
    char **WordArray = (char **)malloc(lines * sizeof(char *));
    if (WordArray == NULL) {
        perror("Memory allocation failed");
        exit(1);
    }

    for (int i = 0; i < lines; i++) {
        WordArray[i] = (char *)malloc(1024 * sizeof(char));
        if (WordArray[i] == NULL) {
            perror("Memory allocation failed");
            exit(1);
        }
    }


    struct Node *current = head;
    int i = 0;
    while (current != NULL) {
        WordArray[i] = current->data;
        current = current->next;
        i++;
    }
    return WordArray;
}

/** Function to print the introductory page of the game and ask if the user wants to add words.
// Returns: 1 if the user wants to add words, 0 if not.**/
int WelcomePage()
{
    char wordAdder;
    printf("Welcome to Guess The Name game\n");
    printf("Would You like to Add more words?\n");
    printf("Press 'Y' to Add or 'N' to Begin Playing: ");
    while( scanf(" %c",&wordAdder)== 1)
    {
        if(wordAdder != 'Y' && wordAdder != 'N')
            printf("Invalid Input, Please Enter a valid input: ");
        else
            break;
    }
    if (wordAdder == 'Y')
        return 1;
    else
        return 0;
}

/** Function to randomly choose a number within a specified range.
// - max: The upper limit for random number generation.
// Returns: The randomly generated number.**/
unsigned int randomizer(int max)
{
    // random generator from 0 to max
    unsigned int randomNum = arc4random_uniform(max);
    return randomNum;
}

/** Function to randomly select a word of a specific length from the array.
// - length: The desired length of the word.
// - array: The 2D array containing the words.
// Returns: A pointer to the selected word.**/
char* wordSelection(int length, char **array)
{
    //Getting the random length
    unsigned int indexOfWord = (unsigned int)randomizer(length);

    //Selecting the word in indexOfword from the array
    char* TempWord = array[indexOfWord];
    size_t L = strlen(TempWord);

    // Allocate memory for a character array (string)
    char* value = (char*)malloc(L + 1); // +1 for the null terminator

    if (value == NULL) {
        // Handle memory allocation failure
        return NULL;
    }

    // Copy the string into the 'value' variable
    strcpy(value, TempWord);
    return value;
}

/** Function to select the number of misses allowed for each game round.
// Returns: The selected difficulty level.**/
int Difficulty()
{
    //Declaration of the input variable
    int n;
    int flag = 0; // Initialize a flag to track if the input is valid
    //This is the user manual
    printf("Stage Difficulty selection Menu:\n");
    printf("0. 25 number of miss\n"
           "1. 20 number of miss\n"
           "2. 15 number of miss\n"
           "3. 10 number of miss\n"
           "4. 5 number of miss\n");
    printf("Please select Difficulty stage: ");

    //This while loop is responsible for making sure the user enters int only.
    while (!flag) {
        if (scanf(" %d", &n) == 1) {
            flag = 1; // Input is valid, exit the loop
        } else {
            printf("Invalid input\n");
            printf("Stage Difficulty selection Menu:\n");
            printf("0. 25 number of miss\n"
                   "1. 20 number of miss\n"
                   "2. 15 number of miss\n"
                   "3. 10 number of miss\n"
                   "4. 5 number of miss\n");
            printf("Please select Difficulty stage: ");
            while (getchar() != '\n'); // Clear the input buffer
        }
    }
    if( n ==0)
        return 25;
    else if( n==1)
        return 20;
    else if (n ==2)
        return 15;
    else if (n ==3)
        return 10;
    else if (n ==4)
        return 5;
    else
        return 10;

}

/** Function to print the game's score results.
// - missedC: The number of missed characters.
// - wordsize: The size of the word.
// - time: The time taken by the player to complete the game.
// - win: Whether the player won (1) or lost (0).**/
void ScoreBoard(int missedC,int wordsize,double time,int win)
{
    //Calculating the sore of the user
    int baseScore;
    baseScore = 100 - (missedC/(wordsize*2.0))*100;
    printf("\n------------------------\n");
    printf("|   Game Scoreboard    |\n");
    printf("------------------------\n");
    printf("| Status: %s\n", win ? "Win" : "Lose");
    printf("| Time:   %f seconds\n", time);
    printf("| Missed: %d characters\n", missedC);
    printf("| Score:  %d\n", baseScore);
    printf("------------------------\n");

}


/** Core game logic function. Allows the user to guess the letters of a word.
// - word: The target word for the game.
// Returns: 1 if the user wins, 0 if they lose.**/
int GauseName(char* word)
{
    //time varibales for start and end
    clock_t  start_timmer,end_timmer;
    double timeUsedByCPU;

    int solved= 0;
    //Alphabets from a-z which are used to keep track of already gauessed words
    char alpha[] ={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    size_t lengthOfWord = strlen(word);
    int countMissedChar = 0;

    //create a dynamic 1D array
    // Dynamically allocate memory for the array
    char *TempArray = (char *)malloc(lengthOfWord * sizeof(char));

    // Check if memory allocation was successful
    if (TempArray == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    //Initializing the array with -
    for(int i= 1;i<lengthOfWord;i++)
        TempArray[i] = '_';
    printf("\n");
    char c;

    //printing the initial state of the array
    int counter = Difficulty();
    for (int x = 0; x < lengthOfWord; x++)
        printf("%c ", TempArray[x]);


    //This line is not neccesary bit will be used for debugging purposes
    printf("\nPlease Enter the characters: ");

    //Statr the clock
    start_timmer = clock();

    //This whole loop is responsible for the game play
    while(scanf(" %c",&c)==1 && counter > 0) {
        int i = 0;
        int boolen = 0;
        int check= 0;
        while (i < lengthOfWord && c != '\n') {
            int temp = 0;

            char Word[100]; // Assuming a maximum word length of 100 characters

            // Copy the selected word to the Word variable
            strcpy(Word, word);

            if (c == Word[i]) {
                for (int j = 0; j < 26; j++) {
                    if (c == alpha[j])
                        temp = 1;


                }
                if (temp == 0)
                {
                    printf("Guessed letter has already been guessed\n");
                    check = 1;
                }

                else {
                    TempArray[i] = c;
                    boolen = 1;
                    solved += 1;
                }
            }
            else
                temp = -1;
            i++;

        }
        if(c != '\n') {
            for (int x = 0; x < lengthOfWord; x++)
                printf("%c ", TempArray[x]);
        }

        //This loop makes sure to clear the character from the alpha array and make it a space
        for(int jj = 0;jj<26;jj++)
        {
            if(c == alpha[jj])
                alpha[jj] = ' ';
        }

        //checking if the users has input a wrong character
        if (boolen == 0 && c != '\n'&& check !=1 )
        {
            printf("Misses: %c\n", c);
            counter --;
            countMissedChar +=1;
             printf("Please Enter the characters: ");
        }


        else if (boolen ==1){
            int isDone = 1;
            for (int x = 0; x < lengthOfWord; x++)
            {
                if(TempArray[x] == '_')
                {
                    isDone = 0;
                }

            }

            //Condtiton to check if the user has input all the required alphabets and the word is correct
            if(isDone)
            {
                end_timmer = clock();
                timeUsedByCPU= (double )(end_timmer-start_timmer)/ 38.00;
                //The scoreboard function is called to show the user results
                ScoreBoard(countMissedChar,lengthOfWord,timeUsedByCPU,1);
                free(TempArray);
                return 1;
            }
            //The User should enter characters as the iput is incorrect
            else
                printf("\nPlease Enter the characters: ");

        }
        else
            printf("\nPlease Enter the characters: ");

    }

    //stopping the time
    end_timmer = clock();

    //This part is responsible if the user has missed more than allowed characters
    timeUsedByCPU= (double )(end_timmer-start_timmer)/ 38.00;
    //The scoreboard function is called to show the user results
    ScoreBoard(countMissedChar,lengthOfWord,timeUsedByCPU,0);
    free(TempArray);
    return 0;

}

/** Function to randomly select a word of a specific length from the array.
// - lengthOfWord: The desired length of the word.
// - length: The number of words in the array.
// - array: The 2D array containing the words.
// Returns: A pointer to the selected word.**/
char* wordSearchOfLength(int lengthOfWord,int length,char **array)
{
    //A temporal array that will be used to store words of length = lengthOfWord
    char* wordofLength[10];
    int count = 0;
    int i = 0;

    while (count < 10 && i < length)
    {
        char* TempWord = array[i];
        size_t L = strlen(TempWord);

        if (L == (lengthOfWord+1))
        {
            // Allocate memory for the word and copy it
            wordofLength[count] = (char*)malloc(L + 1); // +1 for the null terminator
            strcpy(wordofLength[count], TempWord);
            count++;
        }
        i++;
    }

    return wordofLength[randomizer(10)];


}

int main() {

    char Play_Again;
    int size = 0;
    Node *head = NULL;
    do {
        //int Length = 0;
        char **WordList= NULL;

        //Checks if the user wants to add words or not
        if (WelcomePage()) {
            writeFile();
            head = ReadtextFile(head,&size);
            WordList =StorageOflist(head,size);

        }
        else
        {
            head = ReadtextFile(head,&size);
            WordList =StorageOflist(head,size);


        }



        char *Word = wordSelection(size, WordList);
        char randomLength;
        int winner;


        printf("----------------------------------------\n");
        printf("Do you want to chose Length of word?\n");
        printf("Press 'Y' to choose or any key for random selection: ");
        scanf(" %c", &randomLength);
        if (randomLength == 'Y') {
            int a;
            printf("Enter the length of Word(Minimum length == 2): ");
            scanf("%d", &a);
            char *WordofLength = wordSearchOfLength(a, size, WordList);
            printf("%s", WordofLength);
            winner = GauseName(WordofLength);
            free(WordofLength);
        } else {
            printf("%s", Word);
            winner = GauseName(Word);
        }


        if (winner)
            printf("\nCONGRATULATIONS!!!!!!!!!\n");
        else
            printf("\nSorry Try Again next Time.\n");

        printf("Do you want to Play again (Y/N): ");
        scanf(" %c", &Play_Again);
    } while (Play_Again == 'Y' || Play_Again == 'y');

    dispose(head);
    return 0;
}
