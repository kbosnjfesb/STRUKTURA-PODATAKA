#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Element;
typedef struct Element* Position;
typedef struct Element
{
    int coefficient;
    int exponent;
    Position next;
} Element;

int readFile(Position headPoly1, Position headPoly2, char* fileName);
int ParseStringInToList(Position headPoly, char* buffer);
Position CreateElement(int koificijent, int exponent);
int MergeAfter(Position CurrentElement, Position newElement);
int InsertSorted(Position headPoly, Position newElement);
int InsertAfter(Position CurrentElement, Position newElement);
int DeleteElement(Position CurrentElement);
int freeMemory(Position head);
int printPoly(char* polynomeName, Position first);
int addPoly(Position resultHead, Position firstElementPoly1, Position firstElementPoly2);
int multiplyPoly(Position resultHead, Position firstElementPoly1, Position firstElementPoly2);
int createAndInsertAfter(int coefficient, int exponent, Position current);

int main() {
    Element headPoly1 = { .coefficient = 0, .exponent = 0, .next = NULL };
    Element headPoly2 = { .coefficient = 0, .exponent = 0, .next = NULL };
    Element headPolyAdd = { .coefficient = 0, .exponent = 0, .next = NULL };
    Element headPolyMultiply = { .coefficient = 0, .exponent = 0, .next = NULL };
    char* fileName = "polynomes.txt";

    if (readFile(&headPoly1, &headPoly2, fileName) == 0) {
        printPoly("First polynome: ", headPoly1.next);
        printPoly("Second polynome: ", headPoly2.next);

        addPoly(&headPolyAdd, headPoly1.next, headPoly2.next);
        multiplyPoly(&headPolyMultiply, headPoly1.next, headPoly2.next);

        printPoly("Added polynome: ", headPolyAdd.next);
        printPoly("Multiplied polynome: ", headPolyMultiply.next);

        freeMemory(&headPoly1);
        freeMemory(&headPoly2);
        freeMemory(&headPolyAdd);
        freeMemory(&headPolyMultiply);
    }

    return 0;
}

int printPoly(char* polynomeName, Position first) {
    printf("%s", polynomeName);
    while (first != NULL) {
        if (first->coefficient >= 0)
            printf(" + %dx^%d", first->coefficient, first->exponent);
        else
            printf(" - %dx^%d", abs(first->coefficient), first->exponent);
        first = first->next;
    }
    printf("\n");
    return 1;
}

int addPoly(Position resultHead, Position firstElementPoly1, Position firstElementPoly2) {
    Position currentPoly1 = firstElementPoly1;
    Position currentPoly2 = firstElementPoly2;
    Position currentResult = resultHead;
    Position remainingPoly = NULL;

    while (currentPoly1 != NULL && currentPoly2 != NULL) {
        if (currentPoly1->exponent == currentPoly2->exponent) {
            createAndInsertAfter(currentPoly1->coefficient + currentPoly2->coefficient,
                currentPoly1->exponent, currentResult);
            currentPoly1 = currentPoly1->next;
            currentPoly2 = currentPoly2->next;
            currentResult = currentResult->next;
        }
        else if (currentPoly1->exponent > currentPoly2->exponent) {
            createAndInsertAfter(currentPoly1->coefficient, currentPoly1->exponent, currentResult);
            currentPoly1 = currentPoly1->next;
            currentResult = currentResult->next;
        }
        else {
            createAndInsertAfter(currentPoly2->coefficient, currentPoly2->exponent, currentResult);
            currentPoly2 = currentPoly2->next;
            currentResult = currentResult->next;
        }
    }

    remainingPoly = (currentPoly1 != NULL) ? currentPoly1 : currentPoly2;
    while (remainingPoly != NULL) {
        createAndInsertAfter(remainingPoly->coefficient, remainingPoly->exponent, currentResult);
        remainingPoly = remainingPoly->next;
        currentResult = currentResult->next;
    }

    return 1;
}

int multiplyPoly(Position resultHead, Position firstElementPoly1, Position firstElementPoly2) {
    if (firstElementPoly1 == NULL || firstElementPoly2 == NULL) return 0;

    for (Position p1 = firstElementPoly1; p1 != NULL; p1 = p1->next) {
        for (Position p2 = firstElementPoly2; p2 != NULL; p2 = p2->next) {
            Position newElement = CreateElement(p1->coefficient * p2->coefficient,
                p1->exponent + p2->exponent);
            InsertSorted(resultHead, newElement);
        }
    }
    return 0;
}

int freeMemory(Position head) {
    while (head->next != NULL)
        DeleteElement(head);
    return 1;
}

int readFile(Position headPoly1, Position headPoly2, char* fileName) {
    char buffer[100] = { 0 };
    FILE* filePointer = fopen(fileName, "r");
    if (filePointer == NULL) {
        puts("Error opening file!");
        return 1;
    }

    fgets(buffer, sizeof(buffer), filePointer);
    ParseStringInToList(headPoly1, buffer);
    fgets(buffer, sizeof(buffer), filePointer);
    ParseStringInToList(headPoly2, buffer);
    fclose(filePointer);
    return 0;
}

int ParseStringInToList(Position headPoly, char* buffer) {
    char* currentBuffer = buffer;
    int exponent = 0, koificijent = 0, NumberOfMouvedBytes = 0;
    while (strlen(currentBuffer) > 0) {
        int status = sscanf(currentBuffer, " %dx^%d %n", &koificijent, &exponent, &NumberOfMouvedBytes);
        if (status != 2) break;
        Position newElement = CreateElement(koificijent, exponent);
        InsertSorted(headPoly, newElement);
        currentBuffer += NumberOfMouvedBytes;
    }
    return 0;
}

Position CreateElement(int koificijent, int exponent) {
    Position newElement = (Position)malloc(sizeof(Element));
    if (!newElement) exit(EXIT_FAILURE);
    newElement->coefficient = koificijent;
    newElement->exponent = exponent;
    newElement->next = NULL;
    return newElement;
}

int InsertSorted(Position headPoly, Position newElement) {
    Position CurrentElement = headPoly;
    while (CurrentElement->next != NULL && CurrentElement->next->exponent > newElement->exponent)
        CurrentElement = CurrentElement->next;
    MergeAfter(CurrentElement, newElement);
    return 0;
}

int MergeAfter(Position CurrentElement, Position newElement) {
    if (CurrentElement->next == NULL || CurrentElement->next->exponent != newElement->exponent)
        InsertAfter(CurrentElement, newElement);
    else {
        CurrentElement->next->coefficient += newElement->coefficient;
        if (CurrentElement->next->coefficient == 0)
            DeleteElement(CurrentElement);
        free(newElement);
    }
    return 0;
}

int InsertAfter(Position CurrentElement, Position newElement) {
    newElement->next = CurrentElement->next;
    CurrentElement->next = newElement;
    return 0;
}

int DeleteElement(Position CurrentElement) {
    Position toDelete = CurrentElement->next;
    CurrentElement->next = toDelete->next;
    free(toDelete);
    return 0;
}

int createAndInsertAfter(int coefficient, int exponent, Position current) {
    Position newElement = CreateElement(coefficient, exponent);
    InsertAfter(current, newElement);
    return 0;
}