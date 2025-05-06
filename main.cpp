#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GREEN "\033[0;32m"
#define RESET "\033[0m"

typedef struct {
    char name[50];
    char size[20];
} Game;

void header() {
    printf("\e[1;1H\e[2J\n");
    printf("1 - View game Library\n");
    printf("2 - Uninstall Game\n");
    printf("3 - View Game Store\n");
    printf("4 - Install Game from Store\n");
    printf("5 - Sort Installed Games\n");
    printf("0 - Exit Program\n");
}

int is_game_installed(const char* file, const char* game_name) {
    FILE* filePointer = fopen(file, "r");
    if (filePointer == NULL) return 0;

    Game game;
    char line[100];
    while (fgets(line, sizeof(line), filePointer)) {
        sscanf(line, "%49[^,],%19[^\n]", game.name, game.size);
        if (strcmp(game.name, game_name) == 0) {
            fclose(filePointer);
            return 1;
        }
    }
    fclose(filePointer);
    return 0;
}

void display_library(const char file[]) {
    FILE* filePointer = fopen(file, "r");
    Game game;
    char line[100];

    printf("--------------\n");
    printf("Installed Games\n");
    printf("--------------\n");

    if (filePointer == NULL) {
        printf("The file cannot be accessed.\n");
    } else {
        while (fgets(line, sizeof(line), filePointer)) {
            sscanf(line, "%49[^,],%19[^\n]", game.name, game.size);
            printf("Game: %s | Size: " GREEN "%s GB" RESET "\n", game.name, game.size);
        }
        fclose(filePointer);
    }

    printf("--------------\n");
    printf("Press Enter to go Back\n");
    getchar();
    getchar();
}

void uninstall_game(const char* file) {
    FILE* filePointer = fopen(file, "r");
    if (!filePointer) {
        printf("Library cannot be accessed.\n");
        return;
    }

    Game games[100];
    int count = 0;
    char line[100];
    while (fgets(line, sizeof(line), filePointer)) {
        sscanf(line, "%49[^,],%19[^\n]", games[count].name, games[count].size);
        count++;
    }
    fclose(filePointer);

    if (count == 0) {
        printf("No games to uninstall.\n");
        printf("Press Enter to go Back\n");
        getchar();
        getchar();
        return;
    }

    printf("--------------\n");
    printf("Select Game to Uninstall\n");
    printf("--------------\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s | Size: " GREEN "%s GB" RESET "\n", i + 1, games[i].name, games[i].size);
    }

    printf("Enter number to uninstall: ");
    int choice;
    scanf("%d", &choice);
    getchar();

    if (choice < 1 || choice > count) {
        printf("Invalid selection.\n");
        return;
    }

    FILE* temp = fopen("temp.txt", "w");
    for (int i = 0; i < count; i++) {
        if (i != (choice - 1)) {
            fprintf(temp, "%s,%s\n", games[i].name, games[i].size);
        }
    }
    fclose(temp);
    remove(file);
    rename("temp.txt", file);
    printf("Game uninstalled successfully.\n");

    printf("Press Enter to go Back\n");
    getchar();
}

void show_available_games(const char* store_file) {
    FILE* filePointer = fopen(store_file, "r");
    Game game;
    char line[100];

    if (!filePointer) {
        printf("Game Store cannot be accessed.\n");
        return;
    }

    printf("--------------\n");
    printf("Available Games in Store\n");
    printf("--------------\n");

    while (fgets(line, sizeof(line), filePointer)) {
        sscanf(line, "%49[^,],%19[^\n]", game.name, game.size);
        printf("Game: %s | Size: " GREEN "%s GB" RESET "\n", game.name, game.size);
    }

    printf("--------------\n");
    fclose(filePointer);

    printf("Press Enter to go Back\n");
    getchar();
    getchar();
}

void install_game_from_store(const char* store_file, const char* install_file) {
    FILE* storePointer = fopen(store_file, "r");
    if (!storePointer) {
        printf("Could not open Game Store file.\n");
        return;
    }

    Game games[100];
    int count = 0;
    char line[100];

    while (fgets(line, sizeof(line), storePointer)) {
        sscanf(line, "%49[^,],%19[^\n]", games[count].name, games[count].size);
        count++;
    }
    fclose(storePointer);

    if (count == 0) {
        printf("No games available in store.\n");
        return;
    }

    printf("--------------\n");
    printf("Select Game to Install\n");
    printf("--------------\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s | Size: " GREEN "%s GB" RESET "\n", i + 1, games[i].name, games[i].size);
    }

    printf("Enter game number to install: ");
    int choice;
    scanf("%d", &choice);
    getchar();

    if (choice < 1 || choice > count) {
        printf("Invalid choice.\n");
        return;
    }

    if (is_game_installed(install_file, games[choice - 1].name)) {
        printf("Game is already installed.\n");
        return;
    }

    FILE* installPointer = fopen(install_file, "a");
    if (!installPointer) {
        printf("Could not open install file.\n");
        return;
    }

    fprintf(installPointer, "%s,%s\n", games[choice - 1].name, games[choice - 1].size);
    fclose(installPointer);
    printf("Game '%s' installed successfully.\n", games[choice - 1].name);

    printf("Press Enter to go Back\n");
    getchar();
}

int compare_by_name(const void* a, const void* b) {
    return strcmp(((Game*)a)->name, ((Game*)b)->name);
}

int compare_by_size(const void* a, const void* b) {
    return atoi(((Game*)a)->size) - atoi(((Game*)b)->size);
}

void sort_games(const char* file) {
    FILE* filePointer = fopen(file, "r");
    Game games[100];
    int count = 0;
    char line[100];

    if (!filePointer) {
        printf("Could not open file.\n");
        return;
    }

    while (fgets(line, sizeof(line), filePointer)) {
        sscanf(line, "%49[^,],%19[^\n]", games[count].name, games[count].size);
        count++;
    }
    fclose(filePointer);

    if (count == 0) {
        printf("No games to sort.\n");
        return;
    }

    int choice;
    printf("Sort by:\n1 - Name (A–Z)\n2 - Size (Smallest to Largest)\nEnter choice: ");
    scanf("%d", &choice);
    getchar();

    if (choice == 1) {
        qsort(games, count, sizeof(Game), compare_by_name);
    } else if (choice == 2) {
        qsort(games, count, sizeof(Game), compare_by_size);
    } else {
        printf("Invalid option.\n");
        return;
    }

    printf("--------------\n");
    printf("Sorted Games\n");
    printf("--------------\n");
    for (int i = 0; i < count; i++) {
        printf("Game: %s | Size: " GREEN "%s GB" RESET "\n", games[i].name, games[i].size);
    }

    printf("--------------\n");
    printf("Press Enter to go Back\n");
    getchar();
}

void menu(int op) {
    switch (op) {
        case 1:
            display_library("file.txt");
            break;
        case 2:
            uninstall_game("file.txt");
            break;
        case 3:
            show_available_games("game_store.txt");
            break;
        case 4:
            install_game_from_store("game_store.txt", "file.txt");
            break;
        case 5:
            sort_games("file.txt");
            break;
        default:
            printf("EXIT\n");
    }
}

int main() {
    int option;
    do {
        header();
        printf("Enter option: ");
        scanf("%d", &option);
        menu(option);
    } while (option > 0 && option <= 5);
    return 0;
}
