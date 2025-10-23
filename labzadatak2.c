#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struktura za osobu (čvor liste)
typedef struct person {
    char name[50];
    char surname[50];
    int birthYear;
    struct person* next;
} Person;

// Funkcija za unos nove osobe i vraća pokazivač na nju
Person* createPerson() {
    Person* p = (Person*)malloc(sizeof(Person));
    if (!p) return NULL;  // provjera alokacije memorije

    printf("Enter name: ");
    scanf(" %49s", p->name);
    printf("Enter surname: ");
    scanf(" %49s", p->surname);
    printf("Enter year of birth: ");
    scanf("%d", &p->birthYear);
    p->next = NULL;
    return p;
}

// Dodaje novu osobu na početak liste
void addToStart(Person** head) {
    Person* p = createPerson();
    p->next = *head;
    *head = p;
}

// Dodaje novu osobu na kraj liste
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

// Traži osobu po prezimenu i vraća pokazivač na nju (ako postoji)
Person* findBySurname(Person* head, char* surname) {
    while (head) {
        if (strcmp(head->surname, surname) == 0) return head;
        head = head->next;
    }
    return NULL;  // ako nije pronađena
}

// Briše osobu iz liste prema prezimenu
void deletePerson(Person** head, char* surname) {
    Person* temp = *head;
    Person* prev = NULL;

    // prolazi listu dok ne nađe traženo prezime
    while (temp && strcmp(temp->surname, surname) != 0) {
        prev = temp;
        temp = temp->next;
    }

    // ako osoba nije pronađena
    if (!temp) {
        printf("Person not found.\n");
        return;
    }

    // ako se briše prva osoba u listi
    if (!prev) *head = temp->next;
    else prev->next = temp->next;

    free(temp);  // oslobađa memoriju
    printf("Person deleted.\n");
}

int main() {
    Person* head = NULL;  // početak liste
    int choice;
    char surname[50];

    do {
        printf("\n1. Add to start\n2. Add to end\n3. Print list\n4. Find by surname\n5. Delete person\n0. Exit\nChoice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addToStart(&head);  // dodavanje na početak
                break;
            case 2:
                addToEnd(&head);    // dodavanje na kraj
                break;
            case 3:
                printList(head);    // ispis liste
                break;
            case 4: {
                printf("Enter surname: ");
                scanf(" %49s", surname);
                Person* p = findBySurname(head, surname);
                if (p) printf("%s %s (%d)\n", p->name, p->surname, p->birthYear);
                else printf("Person not found.\n");
                break;
            }
            case 5:
                printf("Enter surname to delete: ");
                scanf(" %49s", surname);
                deletePerson(&head, surname);  // brisanje
                break;
        }
    } while (choice != 0);

    // Oslobađanje cijele liste iz memorije
    while (head) {
        Person* t = head;
        head = head->next;
        free(t);
    }

    return 0;
}