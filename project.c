#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define ENTER 13
#define BKSP 8
#define SPACE 32
#define TAB 9

#define FILENAME "NextFile.dat"
#define TEMPFILE "TempFile.dat"

void setcolor(int ForgC) {
    WORD wColor;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
}

struct item {
    char productname[40], productcomp[40];
    int productid;
    int price;
    int Qnt;
} st;

void wel_come(void);
void title(void);
void login(void);
void menu(void);
void deleteproduct(void);
void gotoxy(short x, short y);
void add_item(void);
void read_item(void);
void search_item(void);
void edit_item(void);

int main(void) {
    wel_come();
    login();
    return 0;
}

void gotoxy(short x, short y) {
    COORD pos = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void wel_come(void) {
    time_t t;
    time(&t);
    system("cls");
    printf("                                                                                                         \n");
    printf("---------------------------------------------------------------------------------------------------------\n");
    printf("\t\t\t\t\t%s", ctime(&t));
    printf("---------------------------------------------------------------------------------------------------------\n");
    printf("\n");
    printf("|\t\t\t\t==================================\t\t\t\t\t\t|\n");
    printf("|\t\t\t\t|\t     WELCOME TO \t |\t\t\t\t\t\t|\n");
    printf("|\t\t\t\t|\tPRODUCT MGMT SYSTEM\t |\t\t\t\t\t\t|\n");
    printf("|\t\t\t\t==================================\t\t\t\t\t\t|\n");
    printf("\nPress any key to continue.........\n");
    getch();
    system("cls");
}

void login(void) {
    int attempts = 0;
    char uname[10], pword[10], c;
    const char user[10] = "user";
    const char pass[10] = "pass";

    while (attempts < 3) {
        system("cls");
        printf("\n  ========================  LOGIN   ========================  ");
        printf("\n                        USERNAME: ");
        scanf("%s", uname);
        printf("\n                        PASSWORD: ");
        
        int i = 0;
        while (i < 10) {
            pword[i] = getch();
            c = pword[i];
            if (c == 13) break;
            else printf("*");
            i++;
        }
        pword[i] = '\0';

        if (strcmp(uname, user) == 0 && strcmp(pword, pass) == 0) {
            printf("\n\n\n       WELCOME TO PRODUCT MANAGEMENT SYSTEM !!!! LOGIN IS SUCCESSFUL");
            printf("\n\n\n\t\t\t\tPress any key to continue...");
            getch();
            system("cls");
            menu();
            return;
        } else {
            printf("\n        SORRY !!!!  LOGIN IS UNSUCCESSFUL");
            attempts++;
            getch();
        }
    }

    printf("\nSorry, you have entered the wrong username and password too many times!");
    getch();
    exit(0);
}

void menu(void) {
    int choice;
    while (1) {
        system("cls");
        printf("\n======================== Product Management System ========================\n");
        printf("\n\t\tPress <1> Add Products");
        printf("\n\t\tPress <2> Delete Products");
        printf("\n\t\tPress <3> Search Products");
        printf("\n\t\tPress <4> Read Items");
        printf("\n\t\tPress <5> Edit Items");
        printf("\n\t\tPress <6> Exit!");
        printf("\n\n\t\tEnter your choice[1-6]: ");
        scanf("%d", &choice);

        system("cls");

        switch (choice) {
            case 1:
                add_item();
                break;
            case 2:
                deleteproduct();
                break;
            case 3:
                search_item();
                break;
            case 4:
                read_item();
                break;
            case 5:
                edit_item();
                break;
            case 6:
                printf("System Exit\n");
                exit(0);
            default:
                printf("Invalid Choice! Try Again.\n");
                getch();
        }
    }
}

void add_item(void) {
    FILE *fp;
    char c;

    do {
        system("cls");
        printf("============ Enter Product Detail ============\n");
        int ID;
        fp = fopen(FILENAME, "a+");

        if (fp != NULL) {
            while (1) {
                printf("\nProduct Code: ");
                scanf("%d", &ID);
                int exists = 0;
                while (fscanf(fp, "%s %s %d %d %d", st.productname, st.productcomp, &st.price, &st.productid, &st.Qnt) != EOF) {
                    if (ID == st.productid) {
                        printf("\n\tTHE PRODUCT CODE ALREADY EXISTS.\n");
                        exists = 1;
                        break;
                    }
                }
                if (!exists) {
                    st.productid = ID;
                    break;
                }
                fseek(fp, 0, SEEK_SET);
            }
        } else {
            printf("\nProduct Code: ");
            scanf("%d", &st.productid);
        }

        while (1) {
            printf("\nProduct Name: ");
            fflush(stdin);
            fgets(st.productname, sizeof(st.productname), stdin);
            st.productname[strcspn(st.productname, "\n")] = 0; // remove newline character
            st.productname[0] = toupper(st.productname[0]);
            int valid = 1;
            for (int index = 0; index < strlen(st.productname); ++index) {
                if (!isalpha(st.productname[index])) {
                    valid = 0;
                    break;
                }
            }
            if (valid) break;
            printf("\n Name contains invalid characters. Please enter again.");
        }

        while (1) {
            printf("\nProduct Company: ");
            fflush(stdin);
            fgets(st.productcomp, sizeof(st.productcomp), stdin);
            st.productcomp[strcspn(st.productcomp, "\n")] = 0; // remove newline character
            st.productcomp[0] = toupper(st.productcomp[0]);
            int valid = 1;
            for (int index = 0; index < strlen(st.productcomp); ++index) {
                if (!isalpha(st.productcomp[index])) {
                    valid = 0;
                    break;
                }
            }
            if (valid) break;
            printf("\n Name contains invalid characters. Please enter again.");
        }

        while (1) {
            printf("\nPrice [10-5000] Rupees: ");
            scanf("%d", &st.price);
            if (st.price >= 10 && st.price <= 5000) break;
            printf("\n\tYou cannot enter the price outside the limit [10-5000]. Re-enter.");
        }

        while (1) {
            printf("\nQuantity [1-500]: ");
            scanf("%d", &st.Qnt);
            if (st.Qnt >= 1 && st.Qnt <= 500) break;
            printf("\n\tEnter quantity [1-500] only. Re-enter.");
        }

        fp = fopen(FILENAME, "a");
        fprintf(fp, "%s %s %d %d %d\n", st.productname, st.productcomp, st.price, st.productid, st.Qnt);
        fclose(fp);

        printf("\nPress 'Enter' to add more items or any other key to go to main menu.");
    } while ((c = getch()) == ENTER);

    menu();
}

void search_item(void) {
    char target[40];
    int found = 0;
    FILE *sfile = fopen(FILENAME, "r");

    if (sfile == NULL) {
        printf("\nNo records found.");
        getch();
        menu();
        return;
    }

    printf("\nEnter name to search: ");
    fflush(stdin);
    fgets(target, sizeof(target), stdin);
    target[strcspn(target, "\n")] = 0; // remove newline character
    target[0] = toupper(target[0]);

    while (!feof(sfile)) {
        fscanf(sfile, "%s %s %d %d %d", st.productname, st.productcomp, &st.price, &st.productid, &st.Qnt);
        if (strcmp(target, st.productname) == 0) {
            found = 1;
            break;
        }
    }

    if (found) {
        printf("\nRecord Found:\n");
        printf("Product Name: %s\n", st.productname);
        printf("Product Company: %s\n", st.productcomp);
        printf("Price: %d\n", st.price);
        printf("Product ID: %d\n", st.productid);
        printf("Quantity: %d\n", st.Qnt);
    } else {
        printf("\nNo record found.");
    }

    fclose(sfile);
    printf("\nPress any key to return to main menu.");
    getch();
    menu();
}

void deleteproduct(void) {
    char target[40];
    int found = 0;
    FILE *sfile = fopen(FILENAME, "r");
    FILE *tfile = fopen(TEMPFILE, "w");

    if (sfile == NULL) {
        printf("\nNo records found.");
        getch();
        menu();
        return;
    }

    printf("\nEnter product name to delete: ");
    fflush(stdin);
    fgets(target, sizeof(target), stdin);
    target[strcspn(target, "\n")] = 0; // remove newline character
    target[0] = toupper(target[0]);

    while (fscanf(sfile, "%s %s %d %d %d", st.productname, st.productcomp, &st.price, &st.productid, &st.Qnt) != EOF) {
        if (strcmp(target, st.productname) == 0) {
            found = 1;
        } else {
            fprintf(tfile, "%s %s %d %d %d\n", st.productname, st.productcomp, st.price, st.productid, st.Qnt);
        }
    }

    fclose(sfile);
    fclose(tfile);

    if (found) {
        remove(FILENAME);
        rename(TEMPFILE, FILENAME);
        printf("\nRecord deleted successfully.");
    } else {
        printf("\nRecord not found.");
    }

    printf("\nPress any key to return to main menu.");
    getch();
    menu();
}

void read_item(void) {
    FILE *f = fopen(FILENAME, "r");

    if (f == NULL) {
        printf("\nNo records found.");
        getch();
        menu();
        return;
    }

    printf("====================================================\n");
    printf("Product Name\tProduct Company\tPrice\tProduct ID\tQuantity\n");
    printf("====================================================\n");

    while (fscanf(f, "%s %s %d %d %d", st.productname, st.productcomp, &st.price, &st.productid, &st.Qnt) != EOF) {
        printf("%s\t%s\t%d\t%d\t%d\n", st.productname, st.productcomp, st.price, st.productid, st.Qnt);
    }

    fclose(f);
    printf("\nPress any key to return to main menu.");
    getch();
    menu();
}

void edit_item(void) {
    char target[40];
    int found = 0;
    FILE *sfile = fopen(FILENAME, "r");
    FILE *tfile = fopen(TEMPFILE, "w");

    if (sfile == NULL) {
        printf("\nNo records found.");
        getch();
        menu();
        return;
    }

    printf("\nEnter product name to edit: ");
    fflush(stdin);
    fgets(target, sizeof(target), stdin);
    target[strcspn(target, "\n")] = 0; // remove newline character
    target[0] = toupper(target[0]);

    while (fscanf(sfile, "%s %s %d %d %d", st.productname, st.productcomp, &st.price, &st.productid, &st.Qnt) != EOF) {
        if (strcmp(target, st.productname) == 0) {
            found = 1;
            printf("\nEditing Product ID: %d", st.productid);
            printf("\nProduct Name: ");
            fflush(stdin);
            fgets(st.productname, sizeof(st.productname), stdin);
            st.productname[strcspn(st.productname, "\n")] = 0; // remove newline character
            st.productname[0] = toupper(st.productname[0]);

            printf("\nProduct Company: ");
            fflush(stdin);
            fgets(st.productcomp, sizeof(st.productcomp), stdin);
            st.productcomp[strcspn(st.productcomp, "\n")] = 0; // remove newline character
            st.productcomp[0] = toupper(st.productcomp[0]);

            printf("\nPrice: ");
            scanf("%d", &st.price);
            printf("\nQuantity: ");
            scanf("%d", &st.Qnt);

            fprintf(tfile, "%s %s %d %d %d\n", st.productname, st.productcomp, st.price, st.productid, st.Qnt);
        } else {
            fprintf(tfile, "%s %s %d %d %d\n", st.productname, st.productcomp, st.price, st.productid, st.Qnt);
        }
    }

    fclose(sfile);
    fclose(tfile);

    if (found) {
        remove(FILENAME);
        rename(TEMPFILE, FILENAME);
        printf("\nRecord updated successfully.");
    } else {
        printf("\nRecord not found.");
    }

    printf("\nPress any key to return to main menu.");
    getch();
    menu();
}
