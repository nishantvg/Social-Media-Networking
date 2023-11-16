#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_USERS 100
#define MAX_POSTS 100

// User data structure
typedef struct
{
    int id;
    char username[50];
    char password[50];
} User;

// Graph data structure (Adjacency List)
typedef struct Node
{
    int userId;
    struct Node *next;
} Node;

// Graph data structure (For Networking)
typedef struct
{
    User user;
    Node *friends;
} Graph;

//  Data structure for Post
typedef struct
{
    int id;
    int authorId;
    char content[500];
} Post;

Graph social_network[MAX_USERS];
Post posts[MAX_POSTS];
int user_count = 0;
int post_count = 0;
int user_index = -1;

// Function to find the user index by username
int find_index(const char *username)
{
    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(social_network[i].user.username, username) == 0)
        {
            return i;
        }
    }
    return -1;
}

// Function to register a new user
void register_user()
{
    if (user_count < MAX_USERS)
    {
        User new_user;
        printf("Enter username: ");
        scanf("%s", new_user.username);
        printf("Enter password: ");
        scanf("%s", new_user.password);
        new_user.id = user_count;
        social_network[user_count].user = new_user;
        social_network[user_count].friends = NULL;
        user_count++;
        printf("Registration successful!\n");
    }
    else
    {
        printf("User limit reached!\n");
    }
}

// Function to log in a user
void login()
{
    if (user_index != -1)
    {
        printf("You are already logged in as %s.\n", social_network[user_index].user.username);
    }
    else
    {
        char username[50];
        char password[50];
        printf("Enter username: ");
        scanf("%s", username);
        printf("Enter password: ");
        scanf("%s", password);

        int userIndex = find_index(username);

        // To store the user to the social network
        if (userIndex != -1 && strcmp(social_network[userIndex].user.password, password) == 0)
        {
            user_index = userIndex;
            printf("Login successful!\n");
        }
        else
        {
            printf("Invalid username or password!\n");
        }
    }
}

// Function to add a friend
void add_friend()
{
    if (user_index == -1)
    {
        printf("Please log in first!\n");
        return;
    }

    char friend_name[50];
    printf("Enter the username of the friend you want to add: ");
    scanf("%s", friend_name);

    int friendIndex = find_index(friend_name);

    if (friendIndex == -1)
    {
        printf("User not found.\n");
        return;
    }

    // Check if they are already friends
    Node *curr_friend = social_network[user_index].friends;
    while (curr_friend != NULL)
    {
        if (curr_friend->userId == friendIndex)
        {
            printf("You are already friends with this user.\n");
            return;
        }
        curr_friend = curr_friend->next;
    }

    // Add friend to the adjacency list
    Node *new_friend = (Node *)malloc(sizeof(Node));
    new_friend->userId = friendIndex;
    new_friend->next = social_network[user_index].friends;
    social_network[user_index].friends = new_friend;

    // Add current user to the friend's adjacency list
    Node *curr_node = (Node *)malloc(sizeof(Node));
    curr_node->userId = user_index;
    curr_node->next = social_network[friendIndex].friends;
    social_network[friendIndex].friends = curr_node;

    printf("You are now friends with %s.\n", friend_name);
}

// Function to view a user's list of friends
void view_friends()
{
    if (user_index == -1)
    {
        printf("Please log in first!\n");
        return;
    }

    printf("Friends of %s:\n", social_network[user_index].user.username);
    Node *curr_friend = social_network[user_index].friends;
    while (curr_friend != NULL)
    {
        printf("%s\n", social_network[curr_friend->userId].user.username);
        curr_friend = curr_friend->next;
    }
}

// Function to post updates
void add_post()
{
    if (user_index == -1)
    {
        printf("Please log in first!\n");
        return;
    }

    if (post_count < MAX_POSTS)
    {
        Post new_post;
        new_post.id = post_count;
        new_post.authorId = user_index;
        printf("Enter your post (max 500 characters): ");
        scanf(" %[^\n]", new_post.content);
        posts[post_count++] = new_post;
        printf("Post added!\n");
    }
    else
    {
        printf("Post limit reached!\n");
    }
}

// Function to view the post
void view_post()
{
    if (user_index == -1)
    {
        printf("Please log in first!\n");
        return;
    }

    printf("News Feed for %s:\n", social_network[user_index].user.username);
    for (int i = post_count - 1; i >= 0; i--)
    {
        if (posts[i].authorId == user_index)
        {
            printf("You added-- %s\n", posts[i].content);
        }
        else
        {
            printf("%s -- %s\n", social_network[posts[i].authorId].user.username, posts[i].content);
        }
    }
}

// Function to delete a post
void delete_post()
{
    if (user_index == -1)
    {
        printf("Please log in first!\n");
        return;
    }

    printf("Your Posts:\n");
    for (int i = post_count - 1; i >= 0; i--)
    {
        if (posts[i].authorId == user_index)
        {
            printf("%d. %s\n", posts[i].id, posts[i].content);
        }
    }

    int post_id;
    printf("Enter the ID of the post you want to delete: ");
    scanf("%d", &post_id);

    for (int i = post_count - 1; i >= 0; i--)
    {
        if (posts[i].id == post_id && posts[i].authorId == user_index)
        {
            // Delete the post by shifting remaining posts
            for (int j = i; j < post_count - 1; j++)
            {
                posts[j] = posts[j + 1];
            }
            post_count--;
            printf("Post deleted successfully!\n");
            return;
        }
    }

    printf("Invalid post ID or you don't have permission to delete this post.\n");
}

int main()
{
    int choice;

    while (1)
    {
        printf("===========================================\n");
        printf("\tSocial Media Networking\n");
        printf("===========================================\n");
        printf("1. Register\n2. Login\n3. Add Friend\n4. Update Post\n5. View Post\n6. View Friends\n7. Delet post\n8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            register_user();
            break;
        case 2:
            login();
            break;
        case 3:
            add_friend();
            break;
        case 4:
            add_post();
            break;
        case 5:
            view_post();
            break;
        case 6:
            view_friends();
            break;
        case 7:
            delete_post();
            break;
        case 8:
            return 0;
        default:
            printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}
