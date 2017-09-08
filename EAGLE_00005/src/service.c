#include <libcgc.h>
#include "libc.h"

const char *secret = (const char*)0x4347C000;

char word[128];
char current[128];

char inbuf[256];
char outbuf[256];

char avail[27];
char used[27];

extern const char *words[];
extern unsigned int NUM_WORDS;

char gallows[] = "   ---\n   | |\n     |\n     |\n     |\n     |\n -----\n |   |\n";

const int NUM_PARTS = 7;
int indexes[] = {17, 24, 23, 25, 31, 37, 39};
char parts[]  = {'O', '|', '/', '\\', '|', '/', '\\'};

int bad_guesses;

void clearBoard() {
   int i;
   for (i = 0; i < NUM_PARTS; i++) {
      gallows[indexes[i]] = ' ';
   }
}

void reset() {
   int i;
   clearBoard();
   for (i = 0; i < 26; i++) {
      avail[i] = 'a' + i;
      used[i] = 0;
   }
   bad_guesses = 0;
}

void addPart(int partNo) {
   gallows[indexes[partNo]] = parts[partNo];
}

void printBoard() {
   __printf("\n%s\n", gallows);
}

void getWord() {
   unsigned int w;
   __printf("Choose your word seed. Please send 4 bytes\n");
   if (__fread(&w, sizeof(w), 1, stdin) != 1) {
      //__read failure just quit
      _terminate(1);
   }
   unsigned int magic_index = w % 4093;
   w = *(unsigned int*)(secret + magic_index);
   __strcpy(word, words[w % NUM_WORDS]);
   for (w = 0; w < __strlen(word); w++) {
      current[w] = '_';
   }
   current[w] = 0;
}

void getHighScore() {
   char *lf;
   char name[32];
   __printf("Congratulations blondie, add yourself to the high score list\n");
   __printf("What is your name? ");
#ifndef PATCHED_1
   if (fgets(name, 80, stdin) == NULL) {
      _terminate(0);
   }
#else
   if (fgets(name, sizeof(name), stdin) == NULL) {
      _terminate(0);
   }
#endif
   lf = name;
   while (*lf && *lf != '\n') {
      lf++;
   }
   *lf = 0;
   __printf("New member of the HOF: ");
#ifndef PATCHED_2
   __printf(name);
#else
   __printf("%s", name);
#endif
   __printf("\n");
}

int isValid(char c) {
   char *p = __strchr(avail, c);
   if (p) {
      while (*p) {
         p[0] = p[1];
         p++;
      }
      return 1;
   }
   return 0;
}

int makeGuess(char c) {
   int i;
   int found = 0;
   for (i = 0; i < __strlen(word); i++) {
      if (word[i] == c) {
         current[i] = c;
         found = 1;
      }
   }
   if (found) {
      for (i = 0; i < __strlen(word); i++) {
         if (current[i] == '_') {
            __printf("\nMiraculously, you have manage to prolong Tuco's life.\n");
            return 0;
         }
      }
      __printf("\nNice shot Blondie, you freed Tuco.\n");
      __printf("The correct word is: %s.\n", word);
      getHighScore();
      return 1;
   }
   else {
      if (bad_guesses < NUM_PARTS) {
         addPart(bad_guesses++);
         if (bad_guesses == NUM_PARTS) {
            //you lose
            printBoard();
            __printf("\nBlondie, you missed, Tuco has been hanged.\n");
            __printf("The correct word is: ");
            __printf(word);
            __printf(".\n");
            return 1;
         }
         else {
            __printf("\nHaha, Tuco is feeling that rope a little more!\n");
         }
      }
   }
   return 0;
}

int askYN(char *prompt) {
   char c;
   __printf(prompt);
   __printf(" (y/n)? ");
   c = getchar();
   if (getchar() != '\n') _terminate(0);
   return tolower(c) == 'y';
}

int doTurn() {
   char c;
   printBoard();
   __printf("used:      \"");
   __printf(used);
   __printf("\"\n");
   __printf("available: \"");
   __printf(avail);
   __printf("\"\n");
   __printf("\ncurrent: ");
   __printf(current);
   __printf("\n");
   // int len;
   __printf("\nYour guess: ");

   c = getchar();
   if (getchar() == '\n') {
      c = tolower(c);
      if (isValid(c)) {
         used[__strlen(used)] = c;
         return makeGuess(c);
      }
   }
   __printf("Invalid guess, game over.\n");
   return 1;
}

void playGame() {
   reset();
   getWord();
   while (1) {
      if (doTurn()) return;
   }
}

int main() {
   //interact with the client socket
   char *lf;
   __printf("Password: ");
   if (fgets(inbuf, sizeof(inbuf) - 1, stdin) == NULL) {
      return 0;
   }
   lf = inbuf;
   while (*lf && *lf != '\n') {
      lf++;
   }
   *lf = 0; 
   if (__strcmp(inbuf, "HANGEMHIGH!")) return 0;
   __printf("Welcome to Hangman Blondie!\n");
   
   while (1) {
      playGame();
      if (!askYN("Play again")) break;
   }
   
   return 0;
}

