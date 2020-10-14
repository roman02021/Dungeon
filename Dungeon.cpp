#define _CRT_SECURE_NO_WARNINGS
#define RAND_MAX 10


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>


int generateRandomNum(void);
int generateRandomEnemyMove(void);
void board(int treasure_x, int treasure_y,int player_x, int player_y, int enemy1_x, int enemy1_y, int enemy2_x, int enemy2_y,int moves, int score);
int playerMove(int* player_x, int* player_y);
void enemyMove(int* enemy_x, int* enemy_y, int treasure_x, int treasure_y, int enemy2_x, int enemy2_y);
void intro();
void newScore(FILE* score_file,int score);
int scoreCounter(int moves, int score);


void winMenu(FILE* score_file, int *score);
void lossMenu(FILE* score_file);
void viewScore(FILE* score_file);

int main(void) {
	int player_x, player_y, enemy1_x, enemy1_y, enemy2_x, enemy2_y, treasure_x, treasure_y;
	int moves = 0, score = 2000;
	FILE* score_file;
	//generovanie nahodnych start pozici pre kazdy element

	srand(time(0));
	//otvorenie suboru
	//ak subor neexistuje tak sa vytvori
	if ((score_file = fopen("score.txt", "r+")) == NULL) {
		score_file = fopen("score.txt", "w");
		fprintf(score_file, "Name\t\tScore\t\tDate\n");
		fclose(score_file);
		score_file = fopen("score.txt", "r+");
	};

	system("cls");
	intro();

	lossMenu(score_file);

	//while loop ktory sa stara o to aby hra nesla skoncit inak ako stlacenim 'q'
	while(1){
	system("cls");

	//generovanie nahodnych startovacich pozic hraca, pokladu a nepriatelov
	treasure_x = generateRandomNum();
	treasure_y = generateRandomNum();
	do {
		player_x = generateRandomNum();
		player_y = generateRandomNum();
	} while (player_x == treasure_x && player_y == treasure_y);
	

	do {
		enemy1_x = generateRandomNum();
		enemy1_y = generateRandomNum();
	} while ((enemy1_x == player_x && enemy1_y == player_y) || (enemy1_x == treasure_x && enemy1_y == treasure_y));
	do {
		enemy2_x = generateRandomNum();
		enemy2_y = generateRandomNum();
	} while ((enemy2_x == player_x && enemy2_y == player_y) || (enemy2_x == treasure_x && enemy2_y == treasure_y) || (enemy2_x == enemy1_x && enemy2_y == enemy1_y));
	
	board(treasure_x, treasure_y, player_x, player_y, enemy1_x, enemy1_y, enemy2_x, enemy2_y, moves, score);

	//while loop, ktory sa stara o chod hry
	while (1) {
		playerMove(&player_x, &player_y);
		moves++;

		system("cls");		//clearing terminal
		board(treasure_x, treasure_y, player_x, player_y, enemy1_x, enemy1_y, enemy2_x, enemy2_y, moves, score); //generating board

		if (player_x == enemy1_x && player_y == enemy1_y) {
			system("cls");
			printf("\n\033[1;31mYOU DIED\033[0m\n\n");
			if (scoreCounter(moves, score) > 0) {
				printf("SCORE: \033[1;32m%d\033[0m\n\n\033[0;36mMENU: \033[0m\nPress 'ENTER' to start new game\nPress 'M' to go back to the main menu\nPress 'H' to view high scores\nPress 'Q' to quit\n", scoreCounter(moves, score));
			}
			else {
				printf("SCORE: \033[1;31m%d\033[0m\n\n\033[0;36mMENU: \033[0m\nPress 'ENTER' to start new game\nPress 'M' to go back to the main menu\nPress 'H' to view high scores\nPress 'Q' to quit\n", scoreCounter(moves, score));
			}
			score = 2000;
			lossMenu(score_file);
			moves = 0;
			break;
		}
		else if (player_x == enemy2_x && player_y == enemy2_y) {
			system("cls");
			printf("\n\033[1;31mYOU DIED\033[0m\n\n");
			if (scoreCounter(moves, score) > 0) {
				printf("SCORE: \033[1;32m%d\033[0m\n\n\033[0;36mMENU: \033[0m\nPress 'ENTER' to start new game\nPress 'M' to go back to the main menu\nPress 'H' to view high scores\nPress 'Q' to quit\n", scoreCounter(moves, score));
			}
			else {
				printf("SCORE: \033[1;31m%d\033[0m\n\n\033[0;36mMENU: \033[0m\nPress 'ENTER' to start new game\nPress 'M' to go back to the main menu\nPress 'H' to view high scores\nPress 'Q' to quit\n", scoreCounter(moves, score));
			}

			score = 2000;
			lossMenu(score_file);
			moves = 0;
			break;
		}
		else if (treasure_x == player_x && treasure_y == player_y) {
			system("cls");
			printf("\033[1;32m\nYOU GOT THE TREASURE\033[0m\n\n");

			if (scoreCounter(moves, score) > 0) {
				printf("SCORE: \033[1;32m%d\033[0m\n\n\033[0;36mMENU: \033[0m\nPress 'C' to continue your current game \033[1;32m(+2000 points)\033[0m\nPress 'N' to save your score\nPress 'H' to view high scores\nPress 'ENTER' to start new game\nPress 'M' to go back to the main menu\nPress 'Q' to quit\n", scoreCounter(moves, score));
			}
			else {
				printf("SCORE: \033[1;31m%d\033[0m\n\n\033[0;36mMENU: \033[0m\nPress 'C' to continue your current game \033[1;32m(+2000 points)\033[0m\nPress 'N' to save your score\nPress 'H' to view high scores\nPress 'ENTER' to start new game\nPress 'M' to go back to the main menu\nPress 'Q' t quitt\n", scoreCounter(moves, score));
			}

			score = scoreCounter(moves, score);
			winMenu(score_file, &score);
			moves = 0;
			break;
		}
		enemyMove(&enemy1_x, &enemy1_y, treasure_x, treasure_y, enemy2_x, enemy2_y);

		system("cls");		//clearing terminal
		board(treasure_x, treasure_y, player_x, player_y, enemy1_x, enemy1_y, enemy2_x, enemy2_y, moves, score); //generating board

		if (enemy1_x == player_x && enemy1_y == player_y) {
			system("cls");
			printf("\n\033[1;31mYOU DIED\033[0m\n\n");
			if (scoreCounter(moves, score) > 0) {
				printf("SCORE: \033[1;32m%d\033[0m\n\n\033[0;36mMENU: \033[0m\nPress 'ENTER' to start new game\nPress 'M' to go back to the main menu\nPress 'H' to view high scores\nPress 'Q' to quit\n", scoreCounter(moves, score));
			}
			else {
				printf("SCORE: \033[1;31m%d\033[0m\n\n\033[0;36mMENU: \033[0m\nPress 'ENTER' to start new game\nPress 'M' to go back to the main menu\nPress 'H' to view high scores\nPress 'Q' to quit\n", scoreCounter(moves, score));
			}
			score = 2000;
			lossMenu(score_file);
			moves = 0;
			break;
		}

		enemyMove(&enemy2_x, &enemy2_y, treasure_x, treasure_y, enemy1_x, enemy1_y);

		system("cls");		//clearing terminal
		board(treasure_x, treasure_y, player_x, player_y, enemy1_x, enemy1_y, enemy2_x, enemy2_y, moves, score); //generating board

		if (enemy2_x == player_x && enemy2_y == player_y) {
			system("cls");
			printf("\n\033[1;31mYOU DIED\033[0m\n\n");
			if (scoreCounter(moves, score) > 0) {
				printf("SCORE: \033[1;32m%d\033[0m\n\n\033[0;36mMENU: \033[0m\nPress 'ENTER' to start new game\nPress 'M' to go back to the main menu\nPress 'H' to view high scores\nPress 'Q' to quit\n", scoreCounter(moves, score));
			}
			else {
				printf("SCORE: \033[1;31m%d\033[0m\n\n\033[0;36mMENU: \033[0m\nPress 'ENTER' to start new game\nPress 'M' to go back to the main menu\nPress 'H' to view high scores\nPress 'Q' to quit\n", scoreCounter(moves, score));
			}
			score = 2000;
			lossMenu(score_file);
			moves = 0;
			break;
		}
	}
	}
	return 0;

}

//vypis hracej plochy(po kazdom pohybe hraca alebo nepriatela sa hracia plocha vytvori znova)
void board(int treasure_x, int treasure_y, int player_x, int player_y, int enemy1_x, int enemy1_y, int enemy2_x, int enemy2_y, int moves, int score) {

	printf("\033[1;31m----- DUNGEON -----\033[0m\n\n");

	if (scoreCounter(moves, score) > 0) {
		printf("SCORE: ");
		printf("\033[1;32m%d\033[0m\n", scoreCounter(moves, score));
	}
	else {
		printf("SCORE: ");
		printf("\033[1;31m%d\033[0m\n", scoreCounter(moves, score));
	}
	for (int row = 0; row < 10; row++) {
		
		for (int column = 0; column < 10; column++) {
			if (row == enemy1_y && column == enemy1_x) {
				printf("\033[0;31m");
				putchar('E');
				printf("\033[0m");
			}
			else if (row == enemy2_y && column == enemy2_x) {
				printf("\033[0;31m");
				putchar('E');
				printf("\033[0m");
			}
			else if (row == player_y && column == player_x) {
				printf("\033[1;34m");
				putchar('P');
				printf("\033[0m");
			}
			else if (row == treasure_y && column == treasure_x) {
				printf("\033[1;33m");
				putchar('T');
				printf("\033[0m");
			}
			else {
				putchar('x');
			}
			putchar(' ');
		}
		putchar('\n');
	}
}
//funkcia ktorou sa hrac hybe po hracej ploche
int playerMove(int *player_x, int *player_y) {

	char c;
	c = _getch();
		if (c == 'w' || c == 'W') {
			if (*player_y != 0) {
				return (*player_y)--;
			}
		}
		else if (c == 's' || c == 'S') {
			if (*player_y != 9) {
				return (*player_y)++;
			}
		}
		else if (c == 'd' || c == 'D') {
			if (*player_x != 9) {
				return (*player_x)++;
			}
		}
		else if (c == 'a' || c == 'A') {
			if (*player_x != 0) {
				return (*player_x)--;
			}
		}
		
}
//funkcie ktorou sa nepriatel hybe po hracej ploche
void enemyMove(int* enemy_x, int* enemy_y, int treasure_x, int treasure_y, int enemy2_x, int enemy2_y) {
	//printf("ENEMY_X: %d ENEMY_Y: %d\n", *enemy_x, *enemy_y);
		
		int original_x = *enemy_x, original_y = *enemy_y;

		
		while (((*enemy_x) == original_x) && ((*enemy_y) == original_y)) {
			int randomizer = generateRandomNum();	//premenna vdaka ktorej sa nepriatelia hybu viac nahodne
			int x_move = generateRandomEnemyMove(), y_move = generateRandomEnemyMove();
			//podmienky davaju pozor na to aby sa nepriatelia nepohli na miesto ineho nepriatela, pokladu alebo za hranice hracej plochy
			if ((((*enemy_x + x_move) != treasure_x && (*enemy_y + y_move) != treasure_y)) && (((*enemy_x + x_move) != enemy2_x) && (*enemy_y + y_move) != enemy2_y) && (*enemy_x + x_move) < 10 && (*enemy_y + y_move) < 10 && randomizer < 2) {
				*enemy_x += x_move;
				*enemy_y += y_move;
			}
			else if ((((*enemy_x - x_move) != treasure_x && (*enemy_y - y_move) != treasure_y)) && (((*enemy_x - x_move) != enemy2_x) && (*enemy_y - y_move) != enemy2_y) && (*enemy_x - x_move) > 0 && (*enemy_y - y_move) > 0 && randomizer < 3) {
				*(enemy_x) -= x_move;
				*(enemy_y) -= y_move;
			}
			else if ((((*enemy_x - x_move) != treasure_x && (*enemy_y + y_move) != treasure_y)) && (((*enemy_x - x_move) != enemy2_x) && (*enemy_y + y_move) != enemy2_y) && (*enemy_x - x_move) > 0 && (*enemy_y + y_move) < 10 && randomizer < 4) {
				*(enemy_x) -= x_move;
				*(enemy_y) += y_move;

			}
			else if ((((*enemy_x + x_move) != treasure_x && (*enemy_y - y_move) != treasure_y)) && (((*enemy_x + x_move) != enemy2_x) && (*enemy_y - y_move) != enemy2_y) && (*enemy_x + x_move) < 10 && (*enemy_y - y_move) > 0 && randomizer < 5) {
				*(enemy_x) += x_move;
				*(enemy_y) -= y_move;
			}
			else if ((((*enemy_x - x_move) != treasure_x) && (((*enemy_x - x_move) != enemy2_x)) && (*enemy_x - x_move) > 0) && randomizer < 6) {
				*(enemy_x) -= x_move;
			}
			else if ((((*enemy_x + x_move) != treasure_x) && (((*enemy_x + x_move) != enemy2_x)) && (*enemy_x + x_move) < 10) && randomizer < 7) {
				*(enemy_x) += x_move;
			}
			else if ((((*enemy_y - y_move) != treasure_y) && (((*enemy_y - y_move) != enemy2_y)) && (*enemy_y - y_move) > 0 ) && randomizer < 8) {
				*(enemy_y) -= x_move;
			}
			else if ((((*enemy_y + y_move) != treasure_y) && (((*enemy_y + x_move) != enemy2_x)) && (*enemy_x + x_move) < 10) && randomizer < 9) {
				*(enemy_y) += x_move;
			}
		}
			
		
		
}
//funkcia na generaciu nahodneho cisla medzi 0 a 9 vratane
int generateRandomNum(){
	int random = rand() % 10;
	return random;
}

//funkcia na generaciu nahodneho cisla medzi 0 a 1 vratane
int generateRandomEnemyMove() {
	int random = rand() % 2;
	return random;
}

//funkcia na textove zobrazenie menu
void intro() {
	printf("\033[1;31m----- DUNGEON ----- \n\n\033[0m");
	printf("\033[0;36mCONTROLS: \033[0m\n");
	printf("Press 'A' to move LEFT\nPress 'W' to move UP\nPress 'D' to move RIGHT\nPress 'S' to move DOWN\n\n");
	printf("\033[0;36mGAMEPLAY: \033[0m\n");
	printf("Your character is the PLAYER");
	printf("\033[1;34m(P)\033[0m");
	printf(".\nAvoid ENEMIES");
	printf("\033[0;31m(E)\033[0m");
	printf(" and try to get the TREASURE");
	printf("\033[1;33m(T)\033[0m");
	printf(" in the least possible amount of moves.\nWhen you get treasure you can choose to end the game and save your score or continue your current game to achieve higher score.\n\n");
	printf("\033[0;36mMENU: \033[0m\n");
	printf("Press 'ENTER' To Start Game\n");
	printf("Press 'H' To View High Scores\n");
	printf("Press 'Q' To Quit\n");
}

//funkcia ktora caka na stlacenie klavesnice v menu v pripade ze hrac vyhral
void winMenu(FILE* score_file,int *score) {
	while (char c = _getch()) {
		if (c == 13) {
			(*score) = 2000;
			break;
		}
		if (c == 'h' || c == 'H') {
			system("cls");
			viewScore(score_file);
			printf("\n\033[0;36mMENU: \033[0m\nPress 'ENTER' to start the game\nPress 'M' to go back to the main menu\nPress 'Q' to quit\n");
		}
		if (c == 'm' || c == 'M') {
			system("cls");
			intro();
		}
		if (c == 'q' || c == 'Q') {
			exit(0);
		}
		if (c == 'n' || c == 'N') {
			newScore(score_file, *score);
			system("cls");
			printf("\033[1;32m\nSCORE SAVED\033[0m\n\n\033[0;36mMENU: \033[0m\nPress 'ENTER' to start the game\nPress 'M' to go back to the main menu\nPress 'H' to view high scores\nPress 'Q' to quit\n");
		}
		if (c == 'c' || c == 'C') {
			(*score) += 2000;
			break;
		}
	}
}
//funkcia ktora caka na stlacenie klavesnice v menu v kazdom pripade okrem toho kedy hrac vyhral
void lossMenu(FILE* score_file) {
	while (char c = _getch()) {
		if (c == 13) {
			break;
		}
		if (c == 'h' || c == 'H') {
			system("cls");
			viewScore(score_file);
			printf("\n\033[0;36mMENU: \033[0m\nPress 'ENTER' to start the game\nPress 'M' to go back to the main menu\nPress 'Q' to quit\n");
		}
		if (c == 'm' || c == 'M') {
			system("cls");
			intro();
		}
		if (c == 'q' || c == 'Q') {
			exit(0);
		}
	}
}
//funkcia na vypisanie skore zo suboru
void viewScore(FILE* score_file) {
	char c;
	fseek(score_file, 0, SEEK_SET);
	while ((c = getc(score_file)) != EOF) {
		putchar(c);
	}
}

//funkcia na vypocety skore
int scoreCounter(int moves,int score) {
	score -= 100 * moves;
	return score;
}

//funkcia na zapis noveho skore do suboru
void newScore(FILE* score_file, int score) {
	time_t t;
	time(&t);
	printf("\nEnter your username: ");
	char name[20];
	
	fgets(name, 20, stdin);
	name[strlen(name) - 1] = '\0';
	fseek(score_file, 0, SEEK_END);
	fprintf(score_file, "%s\t\t%d\t\t%s", name,score ,ctime(&t));
}
