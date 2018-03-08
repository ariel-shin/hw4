#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct called Tweeter
struct Tweeter {
    char *name; //name of tweeter
    int tweetCount; //total number of tweets
};

//array of structs that holds up to 200000 Tweeters
struct Tweeter TweetCount[20000];

//initially used these websites but had to change from strtok to strsep
//https://ideone.com/l23He
//https://stackoverflow.com/questions/26695399/csv-file-input-in-c-using-structures

//helper function to get access to field at specified number
//e.g. helps get name at xth field
char* getfield(char* line, int num)
{
    char *token;
    while(num > 0)
    {
        token = strsep(&line, ",");
        num--;
    }
    return token;
}

//helper function to compare two Tweeters's
//by their tweetCount
int compare(const void *s1, const void *s2)
{
    struct Tweeter *t1 = (struct Tweeter *)s1;
    struct Tweeter *t2 = (struct Tweeter *)s2;
    
    if (t1->tweetCount <= t2->tweetCount)
    {
        return 1;
    }
    else return -1;
}

//main driver function
int main(int argc, char* argv[])
{
    //assuming user can only submit one filename
    if (argc > 2)
    {
        printf("ERROR: Submitted too many commandline args\n");
        exit (1);
    }
    
    FILE *fp; //creating a file pointer
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    //opens file for reading
    fp=fopen(argv[1], "r");
    
    //if file does not exist, returns error
    if (fp == NULL)
    {
        fprintf(stderr, "ERROR: Error opening file\n");
        exit (1);
    }
    
    //gets the first line of a valid file
    //stores the first line in a variable called "line"
    read = getline(&line, &len, fp);
    
    //if file is empty, returns error
    if (read == -1) {
        printf("ERROR: Empty file\n");
        exit (1);
    }
    
//grabbing header line and then finding which index contains "name"
    //create a counter to keep track of which field contains name
    int index_of_header = 1;
    char *token;
    
    //get the first token
    token = strtok(line, ",");
    
    //walk through other tokens
    while( token != NULL ) {
        if (strcmp(token, "\"name\"") == 0 || strcmp(token, "name") == 0)
        {
            //if token = "name" or name
            //then break from for loop so the index_of_header
            //has a correct count
            break;
        }
        
        //if name is not found, continuing walking through tokens
        token = strtok(NULL, ",");
        index_of_header++;
    }

//storing each xth element into something
//increasing count of the xth element if if already exists in our data structure
//return top 10
    
    //longest line is apparently ~400
    char entire_line[500];
    char* current_name;
    while (fgets(entire_line, 500, fp))
    {
        char* tmp = strdup(entire_line); //duplicate the line
        int found_name = 0; //set bool to 0
        current_name = getfield(tmp, index_of_header); //get the current_name using helper fcn
        int idx = 0; //set index for our struct to 0
        
        //while the field and the name of the Tweeter are not NULL
        while (TweetCount[idx].name != NULL && current_name != NULL) {
            //checks if the current_name already exists in the struct
            if (strcmp(current_name, TweetCount[idx].name) == 0) {
                //if current_name exists in struct
                //increase tweetCount
                TweetCount[idx].tweetCount++;
                //change bool to 1
                found_name = 1;
                break;
            } else {
                //if current_name does not exist in struct
                //increase index and continue checking other names in struct
                idx++;
            }
        }
        //if found_name is 0 then current_name does not match any tweeterName in struct
        if (found_name == 0) {
            //prevents segfault by ensuring current_name is not null
            if (current_name == NULL)
            {
                printf("Invalid data in file.\n");
                exit(EXIT_FAILURE);
            }
            //creates a duplicate of current_name and stores in struct
            TweetCount[idx].name = strdup(current_name);
            //initializes tweetCount for tweeterName to 1
            TweetCount[idx].tweetCount = 1;
        }
        //frees the line called tmp
        free(tmp);
    }
    
//close the file
    fclose(fp);
    if (line)
        free(line);
    
//sorting the array then getting top 10
    //sorts by using helper function and qsort
    qsort(TweetCount, 20000, sizeof(struct Tweeter), compare);
    
    //for loop to display top 10 names and counts
    for (int i = 0; i < 10; ++i)
    {
        //if name is null, does not print out anything
        //needed if file contains less than 10 names
        if (TweetCount[i].name != NULL)
        {
            printf("name: %s, count: %d\n", TweetCount[i].name, TweetCount[i].tweetCount);
        }
    }

    //exit at the end
    exit(EXIT_SUCCESS);
}
