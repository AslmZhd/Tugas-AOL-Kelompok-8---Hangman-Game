#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Score {
    char name[30];
    int score;
};

void menu();

void menu() {
    int choice;

    do {
        printf("\n====== HANGMAN GAME ======\n");
        printf("1. Play Game\n");
        printf("2. View High Scores\n");
        printf("3. Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                playGame();
                break;
            case 2:
                showScores();
                break;
            case 3:
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 3);
}
