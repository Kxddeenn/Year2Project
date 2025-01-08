#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "functions.h"

// Utility function to test parser
void testingParser(int arg1, char *arg2) {
    printf("The parser was called with arguments: %d and %s\n", arg1, arg2);
}

User* create_user(const char* name, const char* email); // int user_id is auto-generated to be unique
void add_friend(User* user1, User *user2); // users user1 and user2 are now friends
void delete_friend(User* user1, User *user2); // users user1 and user2 are no longer friends
void delete_user(User* user); // user is deleted
void print_users(); // prints all user names in ascending order
void change_user_name(User* user, char* new_name);
void change_user_email(User* user, char* new_email);
void print_friends(User* user); // prints user's friends in ascending order
User* search_user_by_name(const char* name);
User* search_user_by_email(const char* email);
User** mutual_friends(User* user1, User* user2); // returns an array of pointers to the mutual friends
void print_mutual_friends(User** friends); // prints mutual friends' user names in acsending order
Message* create_message(User* sender, User* receiver, const char* content); // int message_id is auto-generated to be unique
void print_message(Message* message);
void display_chat(User* user1, User* user2); // print messages in FIFO
Post* new_post(User* user, const char* content); // post id is auto-generated to be unique
void add_like(Post* post, User* user); // user is the individual who liked the post
void display_feed(User* user1);
User *createTree(User *root, User *newUser);
void print_message(Message *message);
User *search_user_by_email(const char *email);
User *search_user_by_name(const char *name);
User *createIDtree(User *root, User *newUser);

    // WRITE FUNCTIONS BELOW

int id = 0;
int globalID = 0;
User* root = NULL;
int MAXFRIENDS = 10;


// source (GeeksforGeeks) Majority of the helper functions

int getHeight (User* user){

    if(user == NULL){
        return 0;
    }

    return user->height;

}

int maxNum(int a, int b){
    return (a > b) ? a : b; 
}

int getBalanceFactor(User* user){

    if (user == NULL){
        return 0;
    }

    return getHeight(user->left) - getHeight(user->right);

}

// Implement all the different rotations for the AVL tree

User* rightRotation(User* y){
    User* x = y->left;
    User* z = x->right; 

    x->right = y;
    y->left = z; 

    y->height = maxNum(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = maxNum(getHeight(x->left), getHeight(x->right)) + 1;

    return x; 
}

User* leftRotaton(User* x){

    User* y = x->right;
    User* z = y->left; 

    y->left = x; 
    x->right = z;

    y->height = maxNum(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = maxNum(getHeight(x->left), getHeight(x->right)) + 1;

    return y; 
}

User* minValue(User* newuser){

    User* curr = newuser;

    while (curr->left != NULL){
        curr = curr->left; 
    }

    return curr; 
}


// CREATE USER STUFF

User *create_user(const char *name, const char *email){
    User* newUser = (User*)malloc(sizeof(User));

    // Check to make sure that size is less than 49 characters.
    if (strlen(name) > 49){
        return 0;
    }

    if (strlen(email) > 49){
        return 0;
    }

    // Makes sure that's unique
    if (search_user_by_name(name)){
        return 0;
    }

    if (search_user_by_email(name)){
        return 0;
    }

    // Using strncpy to paste it into the char attributes of the node (remember that name and email are unique)
    strncpy(newUser->name, name, MAXSIZE);
    newUser->name[MAXSIZE-1] = '\0';

    strncpy(newUser->email, email, MAXSIZE);
    newUser->email[MAXSIZE-1] = '\0';

    // Implementing a  AVL binary search tree for better searching
    newUser->left = NULL;
    newUser->right = NULL;
    newUser->height = 1; 


    // Friendlist 

    newUser->friendList = (char**)malloc(sizeof(char*) * MAXFRIENDS);
    newUser->friendAmount = 0;

    //Generating a unique ID for each node

    if (globalID > 10000){
        // printf("Users have exceeded the maximum");
        free(newUser);
        return 0;
    }
    
    else{
        id += 1;
        globalID +=1;
    } 

    newUser->id = id;

    root = createTree(root, newUser);
    return newUser;
}


User* createTree(User *root, User* newUser){


    if (root == NULL){
        return newUser; 
    }

    User *left = root->left;
    User *right = root->right;

    if (strcmp(newUser->name, root->name) < 0){
        root->left = createTree(root->left, newUser);
    }

    else if (strcmp(newUser->name, root->name) > 0){
        root->right = createTree(root->right, newUser);
    }

    else{ 
        return root; // duplicate
    }

    root->height = 1 + maxNum(getHeight(root->left), getHeight(root->right));

    int balanceFactor = getBalanceFactor(root);

    // Case 1 : Left-left

    if (balanceFactor > 1 && strcmp(newUser->name, left->name) < 0){
        return rightRotation(root);
    }

    // Case 2 : Right-right

    if (balanceFactor < -1 && strcmp(newUser->name, right->name) > 0){
        return leftRotaton(root);
    }

    // Case 3 : Left-right 

    if (balanceFactor > 1 && strcmp(newUser->name, left->name) > 0){
        root->left = leftRotaton(left);
        return rightRotation(root);
    }

    // Case 4 : Right-left

    if (balanceFactor < -1 && strcmp(newUser->name, right->name) < 0){
        root->right = rightRotation(right);
        return leftRotaton(root);
    }

    return root; 

}

// Printing the Users

void printTree(User* root, int* count){

    if (root == NULL) return;

    printTree(root->left, count);

    // Need to use iteration instead in order to print out commas
    if (*count > 0){
        printf(",");
    }
    
    printf("%s", root->name);
    (*count)++;

    printTree(root->right, count);
    
}

void print_users(){
    int count = 0;
    printTree(root,&count);
    printf("\n");
}

// SEARCHING FUNCTIONS


// log n
User *search_user_by_name(const char *name){

    User *temp = root; 

    while(temp!= NULL){
        int tempValue = strcmp(name, temp->name);

        if (tempValue == 0){
           //  printf("Found user: %s\n", name);
            return temp;
        }

        else if (tempValue < 0 ){
            temp = temp->left;
        }

        else {
            temp = temp->right;
        }

    }

    // printf("Name could not be found.\n");
    return 0;

}

// o(n)
User *search_user_by_email(const char *email){
    User *temp = root;

    while (temp != NULL)
    {
        int tempValue = strcmp(email, temp->email);

        if (tempValue == 0){
            // printf("Found email: %s\n", email);
            return temp;
        }

        else if (tempValue < 0){
            temp = temp->left;
        }

        else{
            temp = temp->right;
        }
    }

    // printf("Email could not be found.\n");
    return 0;
}


User* deleteUserHelp(User* user, const char* name){

    User* temp; 

    // MUST FIND A WAY TO REALLOCATE THE IDs. done

    // Usual BST delete

    if(user==NULL){
        return NULL;
    }

    else if (strcmp(name, user->name) < 0){
        user->left = deleteUserHelp(user->left, name);
    }

    else if (strcmp(name, user->name) > 0 ){
        user->right = deleteUserHelp(user->right, name);
    }

    else{
       
        if (user->right && user->left){
            temp = minValue(user->right);
            strcpy(user->name, temp->name);
            user->right = deleteUserHelp(user->right, temp->name);
        }

        else{
            temp = user; 
            if(user->left == NULL){
                user = user->right; 
            }

            else if(user->right == NULL){
                user = user->left;
            }

            free(temp);

        }

    }

    // used different method bc left and right wasn't working bc it could be null so it would seg fault.
    // Rebalacing the tree

    int balanceFactor = getBalanceFactor(user);

    // Case 1 : Left-left

    if (balanceFactor > 1 && getBalanceFactor(user->left) >= 0)
    {
        return rightRotation(user);
    }

    // Case 2 : Right-right

    if (balanceFactor < -1 && getBalanceFactor(user->right) <= 0)
    {
        return leftRotaton(user);
    }

    // Case 3 : Left-right

    if (balanceFactor > 1 && getBalanceFactor(user->left) < 0)
    {
        user->left = leftRotaton(user->left);
        return rightRotation(user);
    }

    // Case 4 : Right-left

    if (balanceFactor < -1 && getBalanceFactor(user->right) > 0)
    {
        user->right = rightRotation(user->right);
        return leftRotaton(user);
    }

    return user; 

} 

// Go to lab for help 
void delete_user(User *user){ // must add a way for the ID to still be unique

    root = deleteUserHelp(root, user->name); 
    globalID -= 1;

}


// CHANGE USERNAME OR EMAIL (email or username already exists)

void change_user_name(User *user, char *new_name){
    
    if (search_user_by_name(new_name)){
        return;
    }
    
    else{
        strcpy(user->name, new_name);
    }

}

void change_user_email(User *user, char *new_email){
    
    if (search_user_by_email(new_email)){
        return;
    }
    
    else{
        strcpy(user->email, new_email);
    }
}


// MUTUAL FRIENDS

// Bubble sort for the array
void bubbleSort(User *user)
{

    int sizeofFriends = user->friendAmount;

    for (int i = 0; i < sizeofFriends - 1; i++)
    {
        for (int j = 0; j < sizeofFriends - i - 1; j++)
        {
            if (strcmp(user->friendList[j], user->friendList[j + 1]) > 0)
            {
                char *temp = user->friendList[j];
                user->friendList[j] = user->friendList[j + 1];
                user->friendList[j + 1] = temp;
            }
        }
    }
}

void add_friend(User *user1, User *user2){ // users user1 and user2 are now friends (thinking of storing IDs for this but I think the names work better)

    char *name1 = user1->name; 
    char *name2 = user2->name; 

    if (user1->friendAmount < MAXFRIENDS) {
        user1->friendList[user1->friendAmount] = (char*)malloc(MAXSIZE * sizeof(char));
        strcpy(user1->friendList[user1->friendAmount], name2);
        user1->friendAmount++;
        // printf("%s and %s are now friends. :)\n", user1->name, user2->name);
    }

    else{
        MAXFRIENDS *= 2;
        user1->friendList = (char**)realloc(user1->friendList, sizeof(char*) * MAXFRIENDS);
        add_friend(user1, user2);
    }

    if (user2->friendAmount < MAXFRIENDS) {
        user2->friendList[user2->friendAmount] = (char **)malloc(MAXSIZE * sizeof(char*));
        strcpy(user2->friendList[user2->friendAmount], name1);
        user2->friendAmount++;
    }

    else{
        MAXFRIENDS *= 2;
        user2->friendList = (char **)realloc(user2->friendList, sizeof(char *) * MAXFRIENDS);
        add_friend(user1, user2);
    }

    
    // Sorts the array using bubble sort so it can print ascending order
    bubbleSort(user1);
    bubbleSort(user2);


}

void delete_friend(User *user1, User *user2){  // users user1 and user2 are no longer friends

    char *friendRemove1 = user1->name; 
    char *friendRemove2 = user2->name; 

    int indexFriend = -1;

    for (int i = 0; i < user1->friendAmount; i++){
        if (strcmp(user1->friendList[i], friendRemove2) == 0 ){
            indexFriend = i;
            break;
        }
    }

    if (indexFriend != -1){
        for (int i = indexFriend; i < user1->friendAmount -1 ; i++){
            user1->friendList[i] = user1->friendList[i+1];
        }

        user1->friendAmount --; 
    }

    indexFriend = -1;

    for (int i = 0; i < user2->friendAmount; i++)
    {
        if (strcmp(user2->friendList[i], friendRemove1) == 0)
        {
            indexFriend = i;
            break;
        }
    }

    if (indexFriend != -1)
    {
        for (int i = indexFriend; i < user2->friendAmount - 1; i++)
        {
            user2->friendList[i] = user2->friendList[i + 1];
        }

        user2->friendAmount--;
    }

}


void print_friends(User *user){  // prints user's friends in ascending order 

    // printf("%s Friends List:", user->name);
    for (int i = 0; i < user->friendAmount; i++){
        if (i > 0){
            printf(",");
        }
        printf("%s", user->friendList[i]);
    
    }

    printf("\n");

}

User **mutual_friends(User *user1, User *user2){ // returns an array of pointers to the mutual friends

    char **mutualFriends = (char **)malloc(sizeof(char*) * MAXFRIENDS);
    int mutualIndex = 0;

    for (int i = 0 ; i < user1->friendAmount; i++){

        for (int j = 0 ; j < user2->friendAmount; j++){
            
            if (strcmp(user1->friendList[i], user2->friendList[j]) == 0){

                mutualFriends[mutualIndex] = user1->friendList[i];
                mutualIndex++;
            }

        }

    }

    if (mutualIndex == 0){
        mutualFriends[0] = NULL;
    }

    else{
    return mutualFriends;

    }
}


void print_mutual_friends(User **friends){  // prints mutual friends' user names in acsending order 

// need to create function to sort alphabetically. done 


if (friends[0]==NULL){
    // printf("No mutual friends :( \n");
    return;
}

else{

    for (int i = 0; i < 1; i++){
        if(i > 0){
            printf(",");
        }
        printf("%s", friends[i]);
    }
    printf("\n");
}

}

// MESSAGING SYSTEM <3

int messageID = 0;
Message* chatDatabase = NULL; // head of the database

void printChatList(Message* chatlog){

    Message* curr = chatlog;

    if (curr == NULL){
        return;
    }

    while(curr!= NULL){
        printf("%d , %d ->", curr->user1Id, curr->user2Id);
        curr = curr->next;
    }

}

int checkFriends(User* user1, User* user2){

    for(int i = 0 ; i < user1->friendAmount ; i++ ){

        if (strcmp(user1->friendList[i], user2->name) == 0){
            
            return 1;
        }
    }
    return 0; 

}

// QUEUE FUNCTIONALITY Circular :)

int isFull(Message *chatlog){
    return (chatlog->rear+1) % MAXCHATLOG == chatlog->front;
}

void enqueue(Message* chatlog, const char* addMessage, const char* sender){
    if (isFull(chatlog)){
        chatlog->front = (chatlog->front+1) % MAXCHATLOG;
        return;
    }

    chatlog->message[chatlog->rear] = strdup(addMessage);
    chatlog->sender[chatlog->rear] = strdup(sender);
    chatlog->messageID[chatlog->rear] = messageID;
    chatlog->rear = (chatlog->rear+1) % MAXCHATLOG;

    messageID++;
}

// Linked List

Message *createNode(User *user, User *user2)
{
    Message *temp = (Message *)malloc(sizeof(Message));
    temp->user1Id = user->id;
    temp->user2Id = user2->id;

    // Queue Init
    temp->front = 0;
    temp->rear = 0;

    for (int i = 0; i < MAXCHATLOG; i++)
    {
        temp->message[i] = NULL;
    }

    temp->next = NULL;

    for (int i = 0; i < MAXCHATLOG; i++)
    {
        temp->sender[i] = NULL;
    }

    for (int i = 0; i < MAXCHATLOG; i++)
    {
        temp->messageID[i] = NULL;
    }

    return temp;

}

void insertNode(Message **chatlog, User *user, User *user2){
    Message *curr = *chatlog;

    while (curr != NULL)
    {
        if (curr->user1Id == user || curr->user2Id == user)
        {
            return;
        }

        curr = curr->next;
    }

    Message *temp = createNode(user, user2);
    temp->next = *chatlog;
    *chatlog = temp;
}


void printQueue(Message* chatlog){

    for (int i = chatlog->front; i < chatlog->rear; i++){

        if(i > 0){
            printf(",");
        }
        printf("[%s:]%s", chatlog->sender[i], chatlog->message[i]);
    }
    printf("\n");
}

Message *create_message(User *sender, User *receiver, const char *content){

//  Messages are sent from one sender to one receiver with no functionality for group communi
// cation.
//  • Each message is at most 256 characters long. (done)
//  • The message ID fits in an integer type variable. (done)
//  • Each chat between 2 users will have at most 50 messages. Delete oldest message and keep
//  the 50 most recent messages.
//  • Users cannot delete messages.
//  • Only friends can message each other.
//  • No escape sequences will be in the messages.


// must check to see if the two users are friends first 
// let the user make the content 
// store it within chatlogs
// need to make unique message id for all messages

if(checkFriends(sender, receiver) == 0){
    printf("Not friends\n");
    return NULL;
}

// Check to make sure that content is less than 257
if (strnlen(content, MAXMESSAGE+1) > MAXMESSAGE)
{
    printf("Message is over 256 characters long\n");
    return 0;
}

Message* temp = chatDatabase;

if(temp == NULL){
   insertNode(&(chatDatabase), sender, receiver);
   temp = chatDatabase;
}

else {

    while(temp!=NULL){
        if ((temp->user1Id == sender->id && temp->user2Id == receiver->id) || (temp->user1Id == receiver->id && temp->user2Id == sender->id)){
            break;
        }
        temp = temp->next; 
    }

    if (temp == NULL){
        insertNode(&(chatDatabase), sender, receiver);
        temp = chatDatabase;
    }


}

enqueue(temp, content, sender->name);
return temp;

}

void print_message(Message *message){

    // Prints at the front of the queue :)
    printf("%s", message->message[message->front]);
    return; 
}

void display_chat(User *user1, User *user2){  // print messages in FIFO

    Message* curr = chatDatabase;

    while(curr!=NULL){
        if ((curr->user1Id == user1->id && curr->user2Id == user2->id) || (curr->user1Id == user2->id && curr->user2Id == user1->id)){
            break;
        }
        curr = curr->next; 
    }

    if (curr == NULL){
        return;
    }

    // at the right node 
    printQueue(curr);
} 


// POST SYSTEM HEAP STRUCTURE to have a priority queue

// HELPER FUNCTIONS

void doubleHeap(PostDatabase* data){
    data->capacity *= 2; 
    data->array = realloc(data->array, data->capacity * sizeof(Post*));
}

void initialDatabase(PostDatabase* data){
    data->array = malloc(10 * sizeof(Post*));
    data->size = 0;
    data->capacity = 10;
}

void insertPost(PostDatabase* data, Post* post){
    
    // checks to see if the size is full
    if (data->size == data->capacity){
        doubleHeap(data);
    }

    if (data->size >= 20){
        
        // need to remove the eldest post becased on the post id.
        int old = -1;
        int maxPost = -1;
        for (int i = 0; i < data->size; i++){
       
            if (data->array[i]->postID > maxPost){
            maxPost = data->array[i]->postID;
            old = i;
            }

        }
        if (old != -1){
            data->array[old] = post;
            heapify(data, old); // re-heapifies the heap since you removed
            return;
        }
    }

    data->array[data->size] = post;
    int i = data->size;
    data->size++;

    while (i > 0){
        int parent = (i - 1) / 2;

        if (data->array[i]->likes > data->array[parent]->likes ||
            (data->array[i]->likes == data->array[parent]->likes &&
             data->array[i]->postID < data->array[parent]->postID)) // again this check ensures likes are prioritized then recency
        {
            // swap for the current node (rebalancing the heap)
            Post *temp = data->array[i];
            data->array[i] = data->array[parent];
            data->array[parent] = temp;
            i = parent;
        }
        else{
            break;
        }
    }
}

void heapify(PostDatabase* data, int index){

    // typical calculations for the left and right child
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    //left child exist and its bigger than the root database
    // also added other if statements for the postIDs
    if (left < data->size && (data->array[left]->likes > data->array[largest]->likes 
    ||(data->array[left]->likes == data->array[largest]->likes && 
    data->array[left]->postID < data->array[largest]->postID))){

        largest = left;
    }

    // checks to see if right data is larger than prev if statement
    if (right < data->size &&
        (data->array[right]->likes > data->array[largest]->likes 
        ||(data->array[right]->likes == data->array[largest]->likes &&
        data->array[right]->postID < data->array[largest]->postID))){
    
        largest = right;
    }

    // swap continue until the largest number is in the root
    if (largest != index){
        Post *temp = data->array[index];
        data->array[index] = data->array[largest];
        data->array[largest] = temp;

        heapify(data, largest);
    }
}

void heapSort(PostDatabase *data){

    int origSize = data->size;

    // re-build heap
    for (int i = data->size / 2 - 1; i >= 0; i--){
        heapify(data, i);
    }

    for (int i = data->size - 1; i >= 0; i--){
        Post *temp = data->array[0];
        data->array[0] = data->array[i];
        data->array[i] = temp;
        data->size--;
        heapify(data, 0);
    }

    data->size = origSize;
}

int checkFriendsPost(Post* post, User* user1){

    // poster and user are the same person
    if (strcmp(post->poster, user1->name) == 0){
        return 1; 
    }

    else {
    
    // checks through the friends list 
    for (int i = 0; i < user1->friendAmount; i++){
        if (strcmp(user1->friendList[i], post->poster) == 0){
            return 1; 
        }
    }
    }
    return 0; 

}

// Global 
int postID = 0;
int maxFriendsPosts = 10;

PostDatabase* postGlolbal = NULL;

Post *new_post(User *user, const char *content){  // post id is auto-generated to be unique
// Design and implement a functionality to collect new posts made by users. Each post has
// a unique post ID, user (who posted the post), number of likes, and content

    if (postGlolbal == NULL){
        postGlolbal = malloc(sizeof(PostDatabase));
        initialDatabase(postGlolbal);
    }

    if (strlen(content) > 256){
        return 0;
    }

    Post* temp = malloc(sizeof(Post));
    temp->content = malloc(MAXMESSAGE * sizeof(char));
    temp->poster = malloc(MAXSIZE * sizeof(char));
    temp->idPeople = (int*)malloc(maxFriendsPosts * sizeof(int));
    strcpy(temp->content, content);
    temp->postID = postID; 
    postID++;
    strcpy(temp->poster, user->name);
    temp->likes = 0;

    insertPost(postGlolbal, temp);

    return temp; 
} 

void add_like(Post *post, User *user){  // individual who has liked the post

    // checks to see if they are friends or if user wants to like their own post
    if (!checkFriendsPost(post, user)){
        return; 
    }

    // checks to see if the user has already liked the post 
    if (post->idPeople != NULL){
        for (int i = 0; i < post->likes; i++){
            if (post->idPeople[i] == user->id){
                return;
            }
        }
    }

    // simple dynamic array for the posts (used earlier from friends)
    if (post->likes < maxFriendsPosts){
        post->idPeople[post->likes] = user->id;
        post->likes++;
    }

    else{
        // double the size if its full already
        maxFriendsPosts *= 2;
        post->idPeople[post->likes] = (int*)realloc(post->idPeople, sizeof(int) * maxFriendsPosts);
        add_like(post, user);
    }

    int index = -1;
    for (int i = 0; i < postGlolbal->size; i++){
        if (postGlolbal->array[i] == post){
            index = i;
            break;
        }
    }

    if (index == -1){
        return;
    }

    heapify(postGlolbal, index); // re-heapify the heap
}


void display_feed(User *user1){

    // Make a copy of the global array of all posts
    PostDatabase *temp2 = (PostDatabase *)malloc(sizeof(PostDatabase));
    temp2->size = postGlolbal->size;
    temp2->array = (Post **)malloc(postGlolbal->size * sizeof(Post *));
    temp2->capacity = postGlolbal->capacity;

    for (int i = 0; i < postGlolbal->size; i++){
        temp2->array[i] = postGlolbal->array[i];
    }

    heapSort(temp2);
    int count = 0;
    // variable for the commas thingy so it prints out correctly
    int j = 0;

    // prints out highest liked first, then oldest to youngest.
    for (int i = temp2->size - 1; i >= 0 && count < 20; i--){
        Post *temp = temp2->array[i];

        if (checkFriendsPost(temp, user1)){
            if (j > 0){
                printf(",");
            } 
            printf("[%s:]%s", temp->poster, temp->content);
            j++;
            count++;
        }
    }

    //printf("\n");
    free(temp2->array);
    free(temp2);
}

// function to test my liking system 
Post* searchDatabase(const char* content){

    if (postGlolbal == NULL){
        return NULL;
    }

    // checking for the strings to be exact for the main database
    for (int i = 0; i < postGlolbal->size; i++){
        if (strcmp(content, postGlolbal->array[i]->content) == 0){
            return postGlolbal->array[i];
        }
    }

    return NULL;
}
