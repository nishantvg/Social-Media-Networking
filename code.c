#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_USERS 100
#define MAX_POSTS 100

// User data structure
typedef struct {
    int id;
    char username[50];
    char password[50];
} User;

// Graph data structure (Adjacency List)
typedef struct Node {
    int userId;
    struct Node* next;
} Node;

// Graph data structure (For Networking)
typedef struct {
    User user;
    Node* friends;
} Graph;

//  Data structure for Post
typedef struct {
    int id;
    int authorId;
    char content[500];
} Post;

Graph socialNetwork[MAX_USERS];
Post posts[MAX_POSTS];
int userCount = 0;
int postCount = 0;
int currentUserIndex = -1;

// Function to find the user index by username
int findUserIndex(const char* username) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(socialNetwork[i].user.username, username) == 0) {
            return i;
        }
    }
    return -1; 
}

// Function to register a new user
void registerUser() {
    if (userCount < MAX_USERS) {
        User newUser;
        printf("Enter username: ");
        scanf("%s", newUser.username);
        printf("Enter password: ");
        scanf("%s", newUser.password);
        newUser.id = userCount;
        socialNetwork[userCount].user = newUser;
        socialNetwork[userCount].friends = NULL;
        userCount++;
        printf("Registration successful!\n");
    } else {
        printf("User limit reached!\n");
    }
}

// Function to log in a user
void login() {
    if (currentUserIndex != -1) {
        printf("You are already logged in as %s.\n", socialNetwork[currentUserIndex].user.username);
    } else {
        char username[50];
        char password[50];
        printf("Enter username: ");
        scanf("%s", username);
        printf("Enter password: ");
        scanf("%s", password);

        int userIndex = findUserIndex(username);

        //To store the user to the social network
        if (userIndex != -1 && strcmp(socialNetwork[userIndex].user.password, password) == 0) {
            currentUserIndex = userIndex;
            printf("Login successful!\n");
        } else {
            printf("Invalid username or password!\n");
        }
    }
}

// Function to add a friend
void addFriend() {
    if (currentUserIndex == -1) {
        printf("Please log in first!\n");
        return;
    }

    char friendUsername[50];
    printf("Enter the username of the friend you want to add: ");
    scanf("%s", friendUsername);

    int friendIndex = findUserIndex(friendUsername);

    if (friendIndex == -1) {
        printf("User not found.\n");
        return;
    }

    // Check if they are already friends
    Node* currentFriend = socialNetwork[currentUserIndex].friends;
    while (currentFriend != NULL) {
        if (currentFriend->userId == friendIndex) {
            printf("You are already friends with this user.\n");
            return;
        }
        currentFriend = currentFriend->next;
    }

    // Add friend to the adjacency list
    Node* newFriendNode = (Node*)malloc(sizeof(Node));
    newFriendNode->userId = friendIndex;
    newFriendNode->next = socialNetwork[currentUserIndex].friends;
    socialNetwork[currentUserIndex].friends = newFriendNode;

    // Add current user to the friend's adjacency list
    Node* currentUserNode = (Node*)malloc(sizeof(Node));
    currentUserNode->userId = currentUserIndex;
    currentUserNode->next = socialNetwork[friendIndex].friends;
    socialNetwork[friendIndex].friends = currentUserNode;

    printf("You are now friends with %s.\n", friendUsername);
}

// Function to view a user's list of friends
void viewFriends() {
    if (currentUserIndex == -1) {
        printf("Please log in first!\n");
        return;
    }

    printf("Friends of %s:\n", socialNetwork[currentUserIndex].user.username);
    Node* currentFriend = socialNetwork[currentUserIndex].friends;
    while (currentFriend != NULL) {
        printf("%s\n", socialNetwork[currentFriend->userId].user.username);
        currentFriend = currentFriend->next;
    }
}

// Function to post updates
void postUpdate() {
    if (currentUserIndex == -1) {
        printf("Please log in first!\n");
        return;
    }

    if (postCount < MAX_POSTS) {
        Post newPost;
        newPost.id = postCount;
        newPost.authorId = currentUserIndex;
        printf("Enter your post (max 500 characters): ");
        scanf(" %[^\n]", newPost.content);
        posts[postCount++] = newPost;
        printf("Post added!\n");
    } else {
        printf("Post limit reached!\n");
    }
}

// Function to view the post
void viewPost() {
    if (currentUserIndex == -1) {
        printf("Please log in first!\n");
        return;
    }

    printf("News Feed for %s:\n", socialNetwork[currentUserIndex].user.username);
    for (int i = postCount - 1; i >= 0; i--) {
        if (posts[i].authorId == currentUserIndex) {
            printf("You added-- %s\n", posts[i].content);
        } else {
            printf("%s -- %s\n", socialNetwork[posts[i].authorId].user.username, posts[i].content);
        }
    }
}

int main() {
    int choice;

    while (1) {
        printf("===========================================\n");
        printf("\tSocial Media Networking\n");
        printf("===========================================\n");
        printf("1. Register\n2. Login\n3. Add Friend\n4. Post Update\n5. View Post\n6. View Friends\n7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                login();
                break;
            case 3:
                addFriend();
                break;
            case 4:
                postUpdate();
                break;
            case 5:
                viewPost();
                break;
            case 6:
                viewFriends();
                break;
            case 7:
                return 0;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}
