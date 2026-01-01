#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_LENGTH 50
#define MAX_TRIES 6
#define MAX_PLAYERS 100
#define MAX_WORDS 100



struct HangMan {
    char word[MAX_WORD_LENGTH];
    char hint[MAX_WORD_LENGTH];
};

struct Player {
    char name[50];
    int score;
};



int loadWords(struct HangMan words[]);
void playGame(char playerName[]);
void drawHangman(int tries);
void displayWord(const char guessedWord[]);
void saveScore(char name[], int score);
void displayLeaderboard();
void searchPlayer(char searchName[]);



int main() {
    int choice;
    char playerName[50];

    srand(time(NULL));

    printf("Welcome to Hangman!\n");
    printf("Enter your name: ");
    scanf("%49s", playerName);

    do {
        printf("\n============================\n");
        printf("        HANGMAN MENU\n");
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
            case 3: {
                char searchName[50];
                printf("Enter player name to search: ");
                scanf("%49s", searchName);
                searchPlayer(searchName);
                break;
            }
            case 4:
                printf("Thanks for playing, %s!\n", playerName);
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 4);

    return 0;
}



int loadWords(struct HangMan words[]) {
    FILE *file = fopen("words.txt", "r");
    if (file == NULL) {
        printf("Error: words.txt not found!\n");
        exit(1);
    }

    int count = 0;
    while (count < MAX_WORDS &&
           fscanf(file, "%[^|]|%[^\n]\n",
                  words[count].word,
                  words[count].hint) == 2) {
        count++;
    }

    fclose(file);
    return count;
}



void playGame(char playerName[]) {
    struct HangMan wordList[MAX_WORDS];
    int totalWords = loadWords(wordList);

    int index = rand() % totalWords;
    char *secretWord = wordList[index].word;
    char *hint = wordList[index].hint;

    int length = strlen(secretWord);
    char guessedWord[MAX_WORD_LENGTH];
    bool usedLetters[26] = {false};
    int tries = 0;
    int score = 0;

    for (int i = 0; i < length; i++) {
        guessedWord[i] = '_';
    }
    guessedWord[length] = '\0';

    printf("\nHint: %s\n", hint);

    while (tries < MAX_TRIES) {
        displayWord(guessedWord);
        drawHangman(tries);

        char guess;
        printf("Enter a letter: ");
        scanf(" %c", &guess);
        guess = tolower(guess);

        if (!isalpha(guess)) {
            printf("Invalid input. Enter a letter.\n");
            continue;
        }

        if (usedLetters[guess - 'a']) {
            printf("Letter already used.\n");
            continue;
        }
        usedLetters[guess - 'a'] = true;

        bool found = false;
        for (int i = 0; i < length; i++) {
            if (secretWord[i] == guess) {
                guessedWord[i] = guess;
                found = true;
            }
        }

        if (found) {
            printf("Correct!\n");
            score += 10;
        } else {
            printf("Wrong guess!\n");
            tries++;
        }

        if (strcmp(secretWord, guessedWord) == 0) {
            printf("\nYou won! Word: %s\n", secretWord);
            printf("Score: %d\n", score);
            saveScore(playerName, score);
            return;
        }
    }

    drawHangman(tries);
    printf("\nGame Over! The word was: %s\n", secretWord);
    printf("Score: %d\n", score);
    saveScore(playerName, score);
}



void displayWord(const char guessedWord[]) {
    printf("\nWord: ");
    for (int i = 0; guessedWord[i] != '\0'; i++) {
        printf("%c ", guessedWord[i]);
    }
    printf("\n");
}



void drawHangman(int tries) {
    const char *stages[] = {
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
    if (!file) {
        printf("Error saving score!\n");
        return;
    }

    fprintf(file, "%s %d\n", name, score);
    fclose(file);
}



void displayLeaderboard() {
    FILE *file = fopen("leaderboard.txt", "r");
    if (!file) {
        printf("\nNo leaderboard data.\n");
        return;
    }

    struct Player players[MAX_PLAYERS];
    int count = 0;

    while (count < MAX_PLAYERS &&
           fscanf(file, "%s %d",
                  players[count].name,
                  &players[count].score) != EOF) {
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

    printf("\n----- LEADERBOARD -----\n");
    printf("%-20s %-10s\n", "Name", "Score");
    for (int i = 0; i < count; i++) {
        printf("%-20s %-10d\n", players[i].name, players[i].score);
    }
}



void searchPlayer(char searchName[]) {
    FILE *file = fopen("leaderboard.txt", "r");
    if (!file) {
        printf("Leaderboard empty.\n");
        return;
    }

    struct Player p;
    bool found = false;

    printf("\nSearch results for '%s':\n", searchName);
    while (fscanf(file, "%s %d", p.name, &p.score) != EOF) {
        if (strcmp(p.name, searchName) == 0) {
            printf("Score: %d\n", p.score);
            found = true;
        }
    }

    if (!found) {
        printf("No player found.\n");
    }

    fclose(file);
}
