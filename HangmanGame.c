#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_LENGTH 50
#define MAX_TRIES 6

struct HangMan {
    char word[MAX_WORD_LENGTH];
    char hint[MAX_WORD_LENGTH];
};

struct Player {
    char name[50];
    int score;
};

// Function Prototypes
void saveScore(char name[], int score);
void displayLeaderboard();
void searchPlayer(char searchName[]);
void displayWord(const char secretWord[], const char guessedWord[]);
void drawHangman(int tries);
void playGame(char playerName[]);

int main() {
    int choice;
    char playerName[50];

    srand(time(NULL));

    printf("Welcome to Hangman!\n");
    printf("Enter your name: ");
    scanf("%s", playerName);

    do {
        printf("\n============================\n");
        printf("       HANGMAN MENU\n");
        printf("============================\n");
        printf("1. Play Game\n");
        printf("2. View Leaderboard\n");
        printf("3. Search Player\n");
        printf("4. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                playGame(playerName);
                break;
            case 2:
                displayLeaderboard();
                break;
            case 3:
                printf("Enter player name to search: ");
                char sName[50];
                scanf("%s", sName);
                searchPlayer(sName);
                break;
            case 4:
                printf("Thanks for playing, %s! Goodbye.\n", playerName);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);

    return 0;
}

void playGame(char playerName[]) {
    struct HangMan wordList[] = {
        {"pantai", "tempat berpasir di dekat laut"},
        {"miawaug", "Youtuber yang tidak pernah berkata kasar"},
        {"gajah", "hewan yang memiliki belalai"},
        {"beruang", "hewan yang memiliki uang"},
    };

    int wordIndex = rand() % 4;
    const char* secretWord = wordList[wordIndex].word;
    const char* hint = wordList[wordIndex].hint;

    int wordLength = strlen(secretWord);
    char guessedWord[MAX_WORD_LENGTH];
    bool guessedLetters[26] = {false};
    int tries = 0;

    // Initialize guessedWord with underscores
    for (int i = 0; i < wordLength; i++) {
        guessedWord[i] = '_';
    }
    guessedWord[wordLength] = '\0';

    printf("\nHint: %s\n", hint);

    while (tries < MAX_TRIES) {
        displayWord(secretWord, guessedWord);
        drawHangman(tries);

        char guess;
        printf("Enter a letter: ");
        scanf(" %c", &guess);
        guess = tolower(guess);

        if (guess < 'a' || guess > 'z') {
            printf("Please enter a valid letter.\n");
            continue;
        }

        if (guessedLetters[guess - 'a']) {
            printf("You've already guessed '%c'. Try again.\n", guess);
            continue;
        }
        guessedLetters[guess - 'a'] = true;

        bool found = false;
        for (int i = 0; i < wordLength; i++) {
            if (secretWord[i] == guess) {
                found = true;
                guessedWord[i] = guess;
            }
        }

        if (found) {
            printf("Good guess!\n");
        } else {
            printf("Sorry, '%c' is not in the word.\n", guess);
            tries++;
        }

        if (strcmp(secretWord, guessedWord) == 0) {
            printf("\nCongratulations! The word was: %s\n", secretWord);
            int finalScore = (MAX_TRIES - tries) * 10;
            printf("Score for this round: %d\n", finalScore);
            saveScore(playerName, finalScore);
            return;
        }
    }

    drawHangman(tries);
    printf("\nGame Over! The word was: %s\n", secretWord);
}

void displayWord(const char secretWord[], const char guessedWord[]) {
    printf("Word: ");
    for (int i = 0; guessedWord[i] != '\0'; i++) {
        printf("%c ", guessedWord[i]);
    }
    printf("\n");
}

void drawHangman(int tries) {
    const char* stages[] = {
        "  +---+\n  |   |\n      |\n      |\n      |\n      |\n=========",
        "  +---+\n  |   |\n  O   |\n      |\n      |\n      |\n=========",
        "  +---+\n  |   |\n  O   |\n  |   |\n      |\n      |\n=========",
        "  +---+\n  |   |\n  O   |\n /|   |\n      |\n      |\n=========",
        "  +---+\n  |   |\n  O   |\n /|\\  |\n      |\n      |\n=========",
        "  +---+\n  |   |\n  O   |\n /|\\  |\n /    |\n      |\n=========",
        "  +---+\n  |   |\n  O   |\n /|\\  |\n / \\  |\n      |\n========="
    };
    printf("%s\n", stages[tries]);
}

void saveScore(char name[], int score) {
    FILE *file = fopen("leaderboard.txt", "a");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(file, "%s %d\n", name, score);
    fclose(file);
    printf("Score saved to leaderboard!\n");
}

void displayLeaderboard() {
    FILE *file = fopen("leaderboard.txt", "r");
    if (file == NULL) {
        printf("\nNo scores found yet. Play a game first!\n");
        return;
    }

    struct Player players[MAX_PLAYERS];
    int count = 0;

    while (count < MAX_PLAYERS && fscanf(file, "%s %d", players[count].name, &players[count].score) != EOF) {
        count++;
    }
    fclose(file);

    if (count == 0) {
        printf("\nLeaderboard is empty.\n");
        return;
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (players[j].score < players[j + 1].score) {
                struct Player temp = players[j];
                players[j] = players[j + 1];
                players[j + 1] = temp;
            }
        }
    }

    printf("\n--- LEADERBOARD ---\n");
    printf("%-20s %-10s\n", "Player", "Score");
    for (int i = 0; i < count; i++) {
        printf("%-20s %-10d\n", players[i].name, players[i].score);
    }
}

void searchPlayer(char searchName[]) {
    FILE *file = fopen("leaderboard.txt", "r");
    if (file == NULL) {
        printf("Leaderboard empty.\n");
        return;
    }
    struct Player p;
    bool found = false;
    printf("\nResults for '%s':\n", searchName);
    while (fscanf(file, "%s %d", p.name, &p.score) != EOF) {
        if (strcmp(p.name, searchName) == 0) {
            printf("- Score: %d\n", p.score);
            found = true;
        }
    }
    if (!found) printf("No records found for this player.\n");
    fclose(file);
}
