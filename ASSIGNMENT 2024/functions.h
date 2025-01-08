// functions.h
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define MAXSIZE 49
#define MAXMESSAGE 257
#define MAXCHATLOG 51

typedef struct {
    // add attributes
    int id; 
    char name[MAXSIZE]; 
    char email[MAXSIZE];
    struct User *left;
    struct User *right; 
    int height; 
    // For friends list 
    // change maybe to another AVL tree storing IDs
    char** friendList;
    int friendAmount;
} User;

typedef struct {
    // add attributes
    int postID;
    char* poster;
    char* content;
    // Changed to ID to avoid user changing their names
    int* idPeople;
    int likes; 
} Post;

// Max heap 
typedef struct{
    Post** array;
    int size; 
    int capacity;
} PostDatabase;


// Messaging system 
typedef struct {
    // add attributes
    int messageID[MAXCHATLOG]; // unique message id
    char *sender[MAXCHATLOG]; // the sender for every message
    char *message[MAXCHATLOG]; // the actual content of the message
    int front;
    int rear;
    int user1Id;
    int user2Id;
    struct Message *next;
} Message;


// Create message algorithm
    // 1. sender, receiver, content
    // 2. Lookup a Chatlog in chats with the sender and receiver IDs being equal to sender and receiver
    // 3. Check if 
 
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
Post *searchDatabase(const char *content); // added to test for csv files

#endif
