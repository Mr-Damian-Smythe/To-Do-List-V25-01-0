#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char title[256];
    struct tm deadline;
} Task;

typedef struct {
    Task *tasks;
    size_t size;
    size_t capacity;
} TaskList;

// Function declarations
void initTaskList(TaskList *list);
void addTask(TaskList *list, const char *title, const char *deadline);
void removeTask(TaskList *list, size_t index);
void modifyTask(TaskList *list, size_t index, const char *newTitle, const char *newDeadline);
void displayTasks(const TaskList *list);
void freeTaskList(TaskList *list);
int compareTasks(const void *a, const void *b);
void sortTasks(TaskList *list);

int main() {
    TaskList list;
    initTaskList(&list);

    printf("Simple To-Do App\n");
    printf("Commands:\n");
    printf("1 - Add Task\n");
    printf("2 - Remove Task\n");
    printf("3 - Modify Task\n");
    printf("4 - Display Tasks\n");
    printf("0 - Exit\n");

    int choice;
    char title[256], deadline[11];
    size_t index;

    while (1) {
        printf("\nEnter command: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter task title: ");
                scanf(" %[^\n]", title);
                printf("Enter deadline (yyyy-mm-dd): ");
                scanf("%s", deadline);
                addTask(&list, title, deadline);
                sortTasks(&list);
                break;
            case 2:
                printf("Enter task index to remove: ");
                scanf("%zu", &index);
                if (index < list.size) {
                    removeTask(&list, index);
                } else {
                    printf("Invalid index.\n");
                }
                break;
            case 3:
                printf("Enter task index to modify: ");
                scanf("%zu", &index);
                if (index < list.size) {
                    printf("Enter new title: ");
                    scanf(" %[^\n]", title);
                    printf("Enter new deadline (yyyy-mm-dd): ");
                    scanf("%s", deadline);
                    modifyTask(&list, index, title, deadline);
                    sortTasks(&list);
                } else {
                    printf("Invalid index.\n");
                }
                break;
            case 4:
                displayTasks(&list);
                break;
            case 0:
                freeTaskList(&list);
                printf("Exiting.\n");
                return 0;
            default:
                printf("Invalid command.\n");
        }
    }
}

void initTaskList(TaskList *list) {
    list->size = 0;
    list->capacity = 10;
    list->tasks = malloc(list->capacity * sizeof(Task));
    if (!list->tasks) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
}

void addTask(TaskList *list, const char *title, const char *deadline) {
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->tasks = realloc(list->tasks, list->capacity * sizeof(Task));
        if (!list->tasks) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }
    }
    strncpy(list->tasks[list->size].title, title, sizeof(list->tasks[list->size].title) - 1);
    strptime(deadline, "%Y-%m-%d", &list->tasks[list->size].deadline);
    list->size++;
}

void removeTask(TaskList *list, size_t index) {
    for (size_t i = index; i < list->size - 1; i++) {
        list->tasks[i] = list->tasks[i + 1];
    }
    list->size--;
}

void modifyTask(TaskList *list, size_t index, const char *newTitle, const char *newDeadline) {
    strncpy(list->tasks[index].title, newTitle, sizeof(list->tasks[index].title) - 1);
    strptime(newDeadline, "%Y-%m-%d", &list->tasks[index].deadline);
}

void displayTasks(const TaskList *list) {
    printf("\nTo-Do List:\n");
    for (size_t i = 0; i < list->size; i++) {
        char buffer[11];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", &list->tasks[i].deadline);
        printf("%zu. %s - %s\n", i, list->tasks[i].title, buffer);
    }
}

void freeTaskList(TaskList *list) {
    free(list->tasks);
}

int compareTasks(const void *a, const void *b) {
    const Task *taskA = a;
    const Task *taskB = b;
    return difftime(mktime((struct tm *)&taskA->deadline), mktime((struct tm *)&taskB->deadline));
}

void sortTasks(TaskList *list) {
    qsort(list->tasks, list->size, sizeof(Task), compareTasks);
}
