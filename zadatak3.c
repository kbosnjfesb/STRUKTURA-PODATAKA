#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAMESIZE 50

typedef struct Person
{
    char name[NAMESIZE];
    char surname[NAMESIZE];
    int age;
    struct Person* next;
} Person;

int IspisClana(Person* head);
int Ispisliste(Person* head);
int UpisiUlistu(Person* head);
Person* UnosElementa();
Person* DodajNaPocetak(Person* head);
Person* DodajNaKraj(Person* head);
int PronadiPoPrezimenu(Person* head);
Person* IzbrisiListu(Person* head);
Person* DodajNakon(Person* head);
Person* DodajIspred(Person* head);
Person* UcitajizDatoteke(Person* head);
int PocetniMenu();

int main()
{
    int odabir = PocetniMenu();
    Person* head = NULL;

    while (odabir >= 1 && odabir <= 9)
    {
        switch (odabir)
        {
        case 1: Ispisliste(head); break;
        case 2: head = DodajNaPocetak(head); break;
        case 3: head = DodajNaKraj(head); break;
        case 4: PronadiPoPrezimenu(head); break;
        case 5: head = IzbrisiListu(head); break;
        case 6: head = DodajNakon(head); break;
        case 7: head = DodajIspred(head); break;
        case 8: UpisiUlistu(head); break;
        case 9: head = UcitajizDatoteke(head); break;
        default:
            printf("Opcija nije u izboru.\n");
            break;
        }

        printf("\nUnesite novi broj (1–9) ili neki drugi broj za izlaz:\n");
        scanf("%d", &odabir);
    }

    return 0;
}

int IspisClana(Person* head)
{
    if (head == NULL)
        return 0;

    printf("%s %s %d\n", head->name, head->surname, head->age);
    return 1;
}

int Ispisliste(Person* head)
{
    if (head == NULL)
    {
        printf("Lista je prazna.\n");
        return 0;
    }

    while (head != NULL)
    {
        IspisClana(head);
        head = head->next;
    }
    return 1;
}

int UpisiUlistu(Person* head)
{
    FILE* FilePointer = fopen("popis.txt", "w");
    if (FilePointer == NULL)
    {
        printf("Nije moguće otvoriti datoteku.\n");
        return 0;
    }

    while (head != NULL)
    {
        fprintf(FilePointer, "%s %s %d\n", head->name, head->surname, head->age);
        head = head->next;
    }

    fclose(FilePointer);
    printf("Upis obavljen.\n");
    return 1;
}

Person* UnosElementa()
{
    Person* novaosoba = (Person*)malloc(sizeof(Person));
    if (novaosoba == NULL)
        return NULL;

    printf("Unesi ime: ");
    scanf("%s", novaosoba->name);
    printf("Unesi prezime: ");
    scanf("%s", novaosoba->surname);
    printf("Unesi godine: ");
    scanf("%d", &novaosoba->age);

    novaosoba->next = NULL;
    return novaosoba;
}

Person* DodajNaPocetak(Person* head)
{
    Person* novaosoba = UnosElementa();
    if (novaosoba == NULL)
        return head;

    novaosoba->next = head;
    return novaosoba;
}

Person* DodajNaKraj(Person* head)
{
    Person* novaosoba = UnosElementa();
    if (novaosoba == NULL)
        return head;

    if (head == NULL)
        return novaosoba;

    Person* current = head;
    while (current->next != NULL)
        current = current->next;

    current->next = novaosoba;
    return head;
}

int PronadiPoPrezimenu(Person* head)
{
    char wantedSurname[NAMESIZE];
    printf("Unesi prezime traženog člana: ");
    scanf("%s", wantedSurname);

    while (head != NULL)
    {
        if (strcmp(wantedSurname, head->surname) == 0)
        {
            IspisClana(head);
            return 1;
        }
        head = head->next;
    }

    printf("Član nije pronađen.\n");
    return 0;
}

Person* IzbrisiListu(Person* head)
{
    while (head != NULL)
    {
        Person* temp = head;
        head = head->next;
        free(temp);
    }

    printf("Lista izbrisana.\n");
    return NULL;
}

Person* DodajNakon(Person* head)
{
    if (head == NULL)
    {
        printf("Lista je prazna.\n");
        return head;
    }

    int pozicija;
    printf("Nakon kojeg člana (broj rednog mjesta) želite dodati novi? ");
    scanf("%d", &pozicija);

    Person* novaosoba = UnosElementa();
    if (novaosoba == NULL)
        return head;

    Person* current = head;
    for (int i = 1; i < pozicija; i++)
    {
        if (current->next == NULL)
        {
            printf("Mjesto izvan dosega liste.\n");
            free(novaosoba);
            return head;
        }
        current = current->next;
    }

    novaosoba->next = current->next;
    current->next = novaosoba;

    return head;
}

Person* DodajIspred(Person* head)
{
    if (head == NULL)
    {
        printf("Lista je prazna.\n");
        return head;
    }

    int pozicija;
    printf("Prije kojeg člana želite dodati novog? ");
    scanf("%d", &pozicija);

    if (pozicija == 1)
        return DodajNaPocetak(head);

    Person* novaosoba = UnosElementa();
    if (novaosoba == NULL)
        return head;

    Person* current = head;
    for (int i = 1; i < pozicija - 1; i++)
    {
        if (current->next == NULL)
        {
            printf("Mjesto izvan dosega liste.\n");
            free(novaosoba);
            return head;
        }
        current = current->next;
    }

    novaosoba->next = current->next;
    current->next = novaosoba;

    return head;
}

Person* UcitajizDatoteke(Person* head)
{
    FILE* FilePointer = fopen("popis.txt", "r");
    if (FilePointer == NULL)
    {
        printf("Datoteka nije pronađena.\n");
        return head;
    }

    char name[NAMESIZE], surname[NAMESIZE];
    int age;
    Person* kraj = NULL;

    while (fscanf(FilePointer, "%s %s %d", name, surname, &age) == 3)
    {
        Person* novaosoba = (Person*)malloc(sizeof(Person));
        if (novaosoba == NULL)
        {
            fclose(FilePointer);
            return head;
        }

        strcpy(novaosoba->name, name);
        strcpy(novaosoba->surname, surname);
        novaosoba->age = age;
        novaosoba->next = NULL;

        if (head == NULL)
        {
            head = novaosoba;
            kraj = novaosoba;
        }
        else
        {
            kraj->next = novaosoba;
            kraj = novaosoba;
        }
    }

    fclose(FilePointer);
    printf("Učitavanje obavljeno.\n");
    return head;
}

int PocetniMenu()
{
    int odabir;
    printf("----- IZBORNIK -----\n");
    printf("1. Ispisi listu\n");
    printf("2. Dodaj na pocetak\n");
    printf("3. Dodaj na kraj\n");
    printf("4. Pronadi po prezimenu\n");
    printf("5. Izbrisi listu\n");
    printf("6. Dodaj nakon clana\n");
    printf("7. Dodaj ispred clana\n");
    printf("8. Upisi listu u datoteku\n");
    printf("9. Ucitaj listu iz datoteke\n");
    printf("--------------------\n");
    printf("Odabir: ");
    scanf("%d", &odabir);
    return odabir;
}

