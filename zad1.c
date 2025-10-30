#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_POINTS 50
#define FILE_NOT_OPENED -1

typedef struct {
    char name[30];
    char surname[30];
    int points;
} student_info;
//funkcije

int CountStud(const char* filename);                       // Broji koliko redaka ima u datoteci
int AddStudData(const char* filename, student_info* s, int n); // Učitava podatke o studentima
int PrintStudData(student_info* s, int n);                    // Ispisuje podatke i postotak


int main() {
    int student_number = 0; 
    student_info* students = NULL;

    // Broj redaka (studenata) u datoteci
    student_number = CountStud("studenti.txt");
    printf("Broj zapisanih studenata je %d.\n", student_number);

    if (student_number <= 0) {
        printf("Nema studenata ili se datoteka ne moze otvoriti.\n");
        return 1;
    }

    //  Dinamička alokacija memorije
    students = (student_info*)malloc(student_number * sizeof(student_info));
    if (students == NULL) {
        printf("Greska u alokaciji memorije!\n");
        return 1;
    }

    //  Učitavanje podataka
    if (AddStudData("studenti.txt", students, student_number) != 0) {
        printf("Greska pri ucitavanju podataka!\n");
        free(students);
        return 1;
    }

    // Ispis podataka
    PrintStudData(students, student_number);

   
    free(students);

    return 0;
}


// Funkcija koja broji broj redaka (studenata)

int CountStud(const char* filename) {
    int count = 0;
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("Problem s otvaranjem datoteke!\n");
        return FILE_NOT_OPENED;
    }

    char c;
    char lastChar = '\0';

    while ((c = fgetc(f)) != EOF) {
        if (c == '\n')
            count++;
        lastChar = c;
    }

    // Ako datoteka ne završava s '\n', dodaj još jedan redak
    if (lastChar != '\n' && lastChar != '\0')
        count++;

    fclose(f);
    return count;
}

// Funkcija koja učitava podatke o studentima

int AddStudData(const char* filename, student_info* students, int n) {
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("Problem s otvaranjem datoteke!\n");
        return FILE_NOT_OPENED;
    }

    for (int i = 0; i < n; i++) {
        fscanf(f, "%s %s %d", students[i].name, students[i].surname, &students[i].points);
    }

    fclose(f);
    return 0;
}


// Funkcija koja ispisuje podatke o studentima

int PrintStudData(student_info* s, int n) {
    if (s == NULL) {
        printf("Pogrešan pokazivač!\n");
        return -1;
    }

    printf("\n%-15s %-15s %-10s %-10s\n", "Ime", "Prezime", "Bodovi", "Postotak");
    printf("----------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        float relative = (float)s[i].points / MAX_POINTS * 100.0f;
        printf("%-15s %-15s %-10d %.2f%%\n", s[i].name, s[i].surname, s[i].points, relative);
    }

    return 0;
}