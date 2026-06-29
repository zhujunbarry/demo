#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct LinkedList {
    Node *head;
    size_t size;
} LinkedList;

static Node *create_node(int value) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    node->value = value;
    node->next = NULL;
    return node;
}

static void list_init(LinkedList *list) {
    list->head = NULL;
    list->size = 0;
}

static bool list_push_front(LinkedList *list, int value) {
    Node *node = create_node(value);
    if (node == NULL) {
        return false;
    }
    node->next = list->head;
    list->head = node;
    list->size++;
    return true;
}

static bool list_push_back(LinkedList *list, int value) {
    Node *node = create_node(value);
    if (node == NULL) {
        return false;
    }

    if (list->head == NULL) {
        list->head = node;
        list->size++;
        return true;
    }

    Node *current = list->head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = node;
    list->size++;
    return true;
}

static Node *list_find(LinkedList *list, int value) {
    Node *current = list->head;
    while (current != NULL) {
        if (current->value == value) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

static bool list_delete_value(LinkedList *list, int value) {
    Node *current = list->head;
    Node *previous = NULL;

    while (current != NULL) {
        if (current->value == value) {
            if (previous == NULL) {
                list->head = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            list->size--;
            return true;
        }
        previous = current;
        current = current->next;
    }
    return false;
}

static void list_print(const LinkedList *list) {
    const Node *current = list->head;
    printf("LinkedList(size=%zu): ", list->size);
    while (current != NULL) {
        printf("%d", current->value);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf("\n");
}

static void list_free(LinkedList *list) {
    Node *current = list->head;
    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
    list->size = 0;
}

int main(void) {
    LinkedList list;
    list_init(&list);

    list_push_back(&list, 10);
    list_push_back(&list, 20);
    list_push_front(&list, 5);
    list_push_back(&list, 30);
    list_print(&list);

    Node *found = list_find(&list, 20);
    if (found != NULL) {
        printf("Find value 20: success\n");
    } else {
        printf("Find value 20: failed\n");
    }

    list_delete_value(&list, 5);
    list_delete_value(&list, 30);
    list_print(&list);

    list_free(&list);
    return 0;
}
