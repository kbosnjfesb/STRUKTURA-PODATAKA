#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct person {
    char name[50];
    char surname[50];
    int birthYear;
    struct person* next;
} Person;

// Kreiramo novu osobu
Person* createPerson() {
    Person* p = (Person*)malloc(sizeof(Person));
    if (!p) return NULL;

    printf("Enter name: ");
    scanf(" %49s", p->name);
    printf("Enter surname: ");
    scanf(" %49s", p->surname);
    printf("Enter year of birth: ");
    scanf("%d", &p->birthYear);
    p->next = NULL;
    return p;
}

// Dodaje osobu na početak liste
void addToStart(Person** head) {
    Person* p = createPerson();
    p->next = *head;
    *head = p;
}

// Dodaje osobu na kraj liste
void addToEnd(Person** head) {
    Person* p = createPerson();
    if (*head == NULL) *head = p;
    else {
        Person* temp = *head;
        while (temp->next) temp = temp->next;
        temp->next = p;
    }
}

// Ispisuje sve osobe u listi
void printList(Person* head) {
    if (!head) {
        printf("List is empty.\n");
        return;
    }
    while (head) {
        printf("%s %s (%d)\n", head->name, head->surname, head->birthYear);
        head = head->next;
    }
}

// Traži osobu po prezimenu
Person* findBySurname(Person* head, char* surname) {
    while (head) {
        if (strcmp(head->surname, surname) == 0)
            return head;
        head = head->next;
    }
    return NULL;
}

// Briše osobu iz liste
void deletePerson(Person** head, char* surname) {
    Person* temp = *head;
    Person* prev = NULL;

    while (temp && strcmp(temp->surname, surname) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (!temp) {
        printf("Person not found.\n");
        return;
    }

    if (!prev) *head = temp->next;
    else prev->next = temp->next;

    free(temp);
    printf("Person deleted.\n");
}

int main() {
    Person* head = NULL; // početak liste
    int choice = -1;    
    char surname[50];

    
    while (choice != 0) {
        printf("\n1. Add to start\n2. Add to end\n3. Print list\n4. Find by surname\n5. Delete person\n0. Exit\nChoice: ");
        scanf("%d", &choice);

        if (choice == 1)
            addToStart(&head);
        else if (choice == 2)
            addToEnd(&head);
        else if (choice == 3)
            printList(head);
        else if (choice == 4) {
            printf("Enter surname: ");
            scanf(" %49s", surname);
            Person* p = findBySurname(head, surname);
            if (p) printf("%s %s (%d)\n", p->name, p->surname, p->birthYear);
            else printf("Person not found.\n");
        }
        else if (choice == 5) {
            printf("Enter surname to delete: ");
            scanf(" %49s", surname);
            deletePerson(&head, surname);
        }
        else if (choice != 0)
            printf("Invalid option!\n");
    }


    return 0;
}
