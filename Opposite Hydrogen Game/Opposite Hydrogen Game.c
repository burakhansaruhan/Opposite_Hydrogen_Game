#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include <time.h>

#define MAX_USERS 10
#define MAX_SCORES 5

// kullanici
typedef struct {
    char *name;
    char *surname;
    char *username;
    char *password;
    int score;
} User;

// kullanici dizisi
User *users;
// en yuksek 5 skor
int highScores[MAX_SCORES];
int highScoreUsers[MAX_SCORES];

// fonksiyon prototipleri
void gameMenu(int index);
void createUser(int *userCount);
int loginUser(int *userCount);
void addScore(int newScore, int indis);
void displayHighScores();
void displayGameInfo();
int startGame(int a);
void printMatrix(char **matrix, int row, int col);
char** gameMatrix1();
char** gameMatrix2();
char** takeMatrix(int row, int col,FILE *map);
int rowHesaplama(FILE *map);
int colHesaplama(FILE *map);
int move (char **matrix, int row, int col);
int otoMove (char **matrix, int row, int col);
void readToFile(const char *fileName, User *users,int *userCount);

int main() {
	int choice;
	int userCount=0;
	User *users=(User*) malloc((userCount+1)*sizeof(User));
	
    do {
    	int index;
       
       	system("cls");
    	printf("\n--- MAIN MENU ---\n");
    	printf("1. Create User\n");
    	printf("2. Login\n");
    	printf("0. Exit the program\n\n");
       
        printf("Make your choice: ");
         while (scanf("%d", &choice) != 1) {
            printf("Invalid choice. Please try again:");
			while (getchar() != '\n');
        }
        switch (choice) {
            case 1:
				createUser(&userCount);
                break;
            case 2:
                index=loginUser(&userCount);
				gameMenu(index);
                break;
            case 0:
                printf("Exiting the program...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                sleep(1);
                break;
        }
    } while (choice != 0);
	
	readToFile("user_data.txt",users,&userCount);
    return 0;
}

void gameMenu(int index) {
	int ch;

    do {
    	system("cls");
    	printf("\n--- GAME MENU ---\n");
    	printf("1. Manual Play\n");
    	printf("2. Auto Play\n");
    	printf("3. View Highest Scores\n");
    	printf("4. View Game Information\n");
    	printf("0. Back to Main Menu\n\n");
        printf("Make your choice: ");
         while (scanf("%d", &ch) != 1) {
            printf("Invalid choice. Please try again:");
			while (getchar() != '\n');
        }
        switch (ch) {
            case 1:
                users[index].score=startGame(1);
				addScore(users[index].score, index);
                break;
            case 2:
                users[index].score=startGame(2);
				addScore(users[index].score, index);
                break;
            case 3:
                displayHighScores();
                break;
            case 4:
                displayGameInfo();
                break;
            case 0:
                printf("Exiting the program...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                sleep(1);
                break;
        }
    } while (ch != 0);
}

void createUser(int *userCount) {
	system("cls");
	int i;
    if (*userCount >= MAX_USERS) {
        printf("The maximum number of users has been reached. A new account cannot be created.\n");
        return;
    }

    User newUser;
    printf("\n---CREATING A USER---\n");
    printf("\nName: ");
    newUser.name = (char *)malloc(20 * sizeof(char)); // Gerekirse boyutu ayarla
    scanf("%s", newUser.name);

    printf("Surname: ");
    newUser.surname = (char *)malloc(20 * sizeof(char)); // Gerekirse boyutu ayarla
    scanf("%s", newUser.surname);

    printf("User Name: ");
    newUser.username = (char *)malloc(20 * sizeof(char)); // Gerekirse boyutu ayarla
    scanf("%s", newUser.username);
    

 // ayni kullanivi adi var ise
    for (i = 0; i < *userCount; i++) {
        if (strcmp(users[i].username, newUser.username) == 0) {
            printf("This username is already taken. Please choose another username.\n");
            sleep(1);
            i=99;
        }
    }
	if (i != 100){
	
    printf("Password: ");
    newUser.password = (char *)malloc(20 * sizeof(char));
    scanf("%s", newUser.password);
    
    users = (User *)realloc(users, (*userCount + 1) * sizeof(User));
    users[*userCount] = newUser;
    (*userCount)++;
    
    printf("\nUser created successfully!\n");
    sleep(2);
    }
}

int loginUser(int *userCount) {
	int i;
	int index;
	int success=0;
    char username[20];
    char password[20];

	while(success==0){
		system("cls");
		printf("\n---LOGIN---\n");
    printf("\nUser Name: ");
    scanf("%s", username);

    printf("Password: ");
    scanf("%s", password);
	
	i=0;
    while ( i < *userCount ) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            printf("\nLogin successful. Welcome, %s!\n", users[i].name);
            sleep(1);
            index=i;
           	i=99;
           	success=1;
        }
        i++;
    }
    if (i!=100){
    	printf("You have logged in incorrectly...");
    	sleep(1);
	}
}
	return index;
}

void addScore(int newScore, int index) {
    int j;

    // yeni skorun eklenecegi index
    for (j = MAX_SCORES - 1; j > 0 && highScores[j - 1] < newScore; j--) {
        highScores[j] = highScores[j - 1];
        highScoreUsers[j] = highScoreUsers[j - 1];
    }

    highScores[j] = newScore;
    highScoreUsers[j] = index;
}

void displayHighScores() {
    system("cls");
    int i;
    printf("\n--- HIGHEST SCORES ---\n");

    for (i = 0; i < MAX_SCORES; i++) {
        if (highScores[i] > 0) {
            printf("%d. Score: %d - User: %s\n", i + 1, highScores[i], users[highScoreUsers[i]].username);
        } else {
            printf("%d. Score: 0 - User:\n", i + 1);
        }
    }

    printf("\nPress a key to return to the menu: \n");
    getch();
    system("cls");
}

void displayGameInfo() {
	system("cls");
    printf("\n--- GAME INFORMATION ---\n\n");
    printf("You can use arrow keys to control the game.\n");
    printf("'X' on the game screen represents the user.\n");
	printf("The 'C' on the game screen represents the exit.\n");
	printf("The 'G' on the game screen represents the entrance.\n");
	printf("The 'K' on the game screen represents the black hole.\n");
	printf("The '0' on the game screen represents the path.\n");
	printf("The '1' on the game screen represents the wall.\n");
	printf("The 'e' on the game screen represents e-.\n");
	printf("The 'E' on the game screen represents e+.\n");
	printf("The 'p' on the game screen represents P-.\n");
	printf("The 'P' on the game screen represents P+.\n");
	printf("When a particle comes together with its opposite-signed counterpart on the game screen, they annihilate each other.\n");
	printf("If you reach the black hole, you return to the main menu with 0 points.\n");
	printf("You earn 7 points for each particle you collect.\n");
	printf("If you generate opposite hydrogen at the exit of the maze, you earn 100 points.\n");
	printf("To be able to generate opposite hydrogen, you should have only P- and e+ particles at the exit of the maze.\n");
	printf("\n\nPress any key to return to the game menu: ");	
    getch();
}
int startGame(int a) {
	int i;
	int ch=0;
	int point;
	int row,col;
	char **matrix;
	system("cls");
	printf("Do you want to choose from ready maps or do you want to load them yourself?\n");
	printf("1)Ready map\n");
	printf("2)I want to load it myself\n");
	printf("Choice: ");
	
	while (scanf("%d", &ch) != 1) {
            printf("Invalid choice. Please try again:");
			while (getchar() != '\n');
        }
	
	while(ch!=1 && ch !=2){
		while (getchar() != '\n');
		printf("Please make a valid choice:");
		scanf("%d",&ch);
	}
	
	
	if (ch==1){
		int s;
		system("cls");
		printf("Which map would you like to play with?");
		
		matrix= gameMatrix1();
		row=7;
		col=8;
		printMatrix(matrix,row,col);
		
		printf("\n");
		
		matrix= gameMatrix2();
		row=10;
		col=9;
		printMatrix(matrix,row,col);
		
		printf("\nChoice:");
		while (scanf("%d", &s) != 1) {
            printf("Invalid choice. Please try again:");
			while (getchar() != '\n');
        }
        
        while(s!=1 && s !=2){
			while (getchar() != '\n');
			printf("Please make a valid choice:");
			scanf("%d",&s);
	}
		
		if (s==1){
		row=7;
		col=8;
		matrix= gameMatrix1();
		}
		if (s==2){
		row=10;
		col=9;
		matrix= gameMatrix2();
		}
		
	}
	
	if (ch==2){
	FILE *map;
	char fileName[20];
	int check=0;
		printf("\nThe map you will upload\nabc\ndef\nxyz\nklm\nmust be in the format.\n");
		printf("Enter the name of the text file you want to upload (example: filename.txt):");
		
		while (check==0){
			check=1;
		scanf("%s",fileName);
		map = fopen(fileName, "r");
		
		 if (map == NULL) {
                printf("The map could not be loaded. Please enter again.:");
                check=0;
            }
	}
		col=colHesaplama(map);
		row=rowHesaplama(map);
		matrix=takeMatrix(row,col,map);
		fclose(map);
	}
	
	printf("\n--- GAME STARTING---\n");	
	sleep(2);
    printMatrix(matrix,row,col);
    
    if(a==1)
	point=move(matrix,row,col);
	
	if(a==2)
	point=otoMove(matrix,row,col);
	
	for(i=0; i < row; i++){
		free(matrix[i]);
	}
	free(matrix);
	
	return point;
}

int rowHesaplama(FILE *map){
    char ch;
    int row=1;
    fseek(map, 0, SEEK_SET);
    while ((ch = fgetc(map)) != EOF) {
        if (ch == '\n') {
            row++;
        }
    }
    
    return row;
}

int colHesaplama(FILE *map){
    char ch;
    int col=0;
    fseek(map, 0, SEEK_SET);
    while ((ch = fgetc(map)) != '\n' && ch != EOF) {
        col++;
	}
    
    return col;
}

char** gameMatrix1(){
	int i,j;
	
	printf("\n1) This map is a 7x8 map.\n");
	
	
	char **matrix;
	matrix = (char**) malloc(7*sizeof(char*));
    
    for(i=0;i<7;i++){
    	matrix[i] = (char*) malloc((8+1)*sizeof(char)); 
	}

	char matrixData[7][8] = {
        {'e', '0', 'P', '0', 'E', '0', '0', '1'},
        {'1', 'G', '0', 'p', '1', '0', 'K', 'E'},
        {'P', '0', '0', '0', 'e', '0', 'p', 'P'},
        {'1', '0', '1', 'e', '0', '0', '1', '0'},
        {'p', '0', '0', '0', '1', 'p', '0', 'e'},
        {'e', '0', '0', 'p', '0', 'C', '1', '0'},
        {'1', 'P', 'e', '0', 'E', '0', 'P', '1'}
    };
    // matrixi kopyalama
    for ( i = 0; i < 7; i++) {
        for ( j = 0; j < 8; j++) {
            matrix[i][j] = matrixData[i][j];
        }
    }

	return matrix;
}

char** gameMatrix2(){
	int i,j;
	
	printf("\n2) This map is a 10x9 map.\n");
	
	
	char **matrix;
	matrix = (char**) malloc(10*sizeof(char*));
    
    for(i=0;i<10;i++){
    	matrix[i] = (char*) malloc((9+1)*sizeof(char)); 
	}

	    char matrixData[10][9] = {
        {'E', '0', '1', '0', 'e', 'P', '0', 'p', '1'},
        {'K', '0', 'E', '1', 'e', '0', '1', '0', '1'},
        {'e', 'P', '1', 'p', 'E', '0', '1', '0', '0'},
        {'0', 'K', '0', '1', 'e', '0', 'p', '1', '0'},
        {'0', 'p', '0', 'E', '0', 'K', 'P', '1', 'C'},
        {'K', 'P', 'e', '1', '0', '0', '1', '1', '0'},
        {'1', '0', '0', '0', 'E', '1', '0', 'K', '0'},
        {'1', '1', 'p', '0', '0', 'P', 'e', '0', '1'},
        {'0', 'G', '0', '1', '0', 'P', 'E', '0', 'e'},
        {'1', '0', 'P', '0', '1', 'p', 'K', 'E', '1'}
    };
    // matrixi kopyalama
    for ( i = 0; i < 10; i++) {
        for ( j = 0; j < 9; j++) {
            matrix[i][j] = matrixData[i][j];
        }
    }

	return matrix;
}

char** takeMatrix(int row, int col,FILE *map){
	int i,j;
	char **matrix;
	fseek(map, 0, SEEK_SET);
    char ch;
  
    matrix = (char**) malloc((row)*sizeof(char*));
    
    for(i=0;i<row;i++){
    	matrix[i] = (char*) malloc((col+1)*sizeof(char)); 
	}

        // matrixe karakterleri yaz
    for (i = 0; i < row; i++) {
        for (j = 0; j < col+1; j++) {
            char ch = fgetc(map);

            // Dosya sonuna gelindiyse veya bir hata olustuysa donguden cik
            if (feof(map) || ferror(map)) {
                break;
            }

            // matrixe karakteri ekle
            matrix[i][j] = ch;
        }
    }
	return matrix;
}

void printMatrix(char **matrix, int row, int col){
	int i,j;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    for ( i = 0; i < row; i++) {
        for ( j = 0; j < col; j++) {
            // Renkleri ayarla
            if (matrix[i][j] == 'C') {
                SetConsoleTextAttribute(hConsole, 5);  // mor renk kodu
            } else if (matrix[i][j] == 'K') {
                SetConsoleTextAttribute(hConsole, 4);  // kirmizi renk kodu
            } else if (matrix[i][j] == 'E') {
                SetConsoleTextAttribute(hConsole, 1);  // mavi renk kodu
            } else if (matrix[i][j] == 'e') {
                SetConsoleTextAttribute(hConsole, 3);  // acik mavi renk kodu
            } else if (matrix[i][j] == 'P') {
                SetConsoleTextAttribute(hConsole, 6);  // sari renk kodu
            } else if (matrix[i][j] == 'p') {
                SetConsoleTextAttribute(hConsole, 14); // acik sari renk kodu
            } else if (matrix[i][j] == '1') {
                SetConsoleTextAttribute(hConsole, 12); // acik kirmizi renk kodu
            } else if (matrix[i][j] == '0') {
                SetConsoleTextAttribute(hConsole, 8);  // gri renk kodu
            } else if (matrix[i][j] == 'X') {
                SetConsoleTextAttribute(hConsole, 2); // yesil renk kodu
            } else if (matrix[i][j] == 'G') {
                SetConsoleTextAttribute(hConsole, 2); // yesil renk kodu
            } else {
                SetConsoleTextAttribute(hConsole, 7);  // Normal renk beyaz
            }

            // matrixi yazdir
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }

    // renkleri sifirlsma
    SetConsoleTextAttribute(hConsole, 7);
}

int move (char **matrix, int row, int col){
	int timeLimit = 10;
	int passTime;
	int remainingTime=timeLimit;
	int i,j;
	int key;
	int flag=0;
	int currentRow,currentCol;
	int electron=0, antiElectron=0, proton=0, antiProton=0, antiHydrogen=0, point=1;
	
	for( i=0 ; i<row ; i++){
		
		for( j=0 ; j<col ; j++){
		
			if(matrix[i][j] == 'G'){
				currentRow=i;
				currentCol=j;
			}
		}
}
	time_t startTime = time(NULL);
    while ( flag==0 ) {
     system("cls");
        printMatrix(matrix,row,col);
        printf("\nCollected items:\nProton:%d\nAntiproton:%d\nElectron:%d\nAntielectron:%d\n\n",proton,antiProton,electron,antiElectron);
        printf("Remaining time:%d\n",remainingTime);
		printf("Enter the direction you want to go: ");
        key = getch();	
   
        if (key == 27) { // ESC 
            flag=1;
        }

        // yukari
        if (key == 72 && currentRow > 0) {
        	matrix[currentRow][currentCol]='0';
            currentRow--;
            
            if(matrix[currentRow][currentCol]=='1')
            	currentRow++;
            	
            if(matrix[currentRow][currentCol]=='E')
            	antiElectron++;
				
			if(matrix[currentRow][currentCol]=='e')
            	electron++;	
				
			if(matrix[currentRow][currentCol]=='P')
            	proton++;	
				
			if(matrix[currentRow][currentCol]=='p')
            	antiProton++;			
            	
        }
        
        // asagi
        else if (key == 80 && currentRow < row-1) {
        	matrix[currentRow][currentCol]='0';
            currentRow++;
            
            if(matrix[currentRow][currentCol]=='1')
            	currentRow--;
            	
            if(matrix[currentRow][currentCol]=='E')
            	antiElectron++;
				
			if(matrix[currentRow][currentCol]=='e')
            	electron++;	
				
			if(matrix[currentRow][currentCol]=='P')
            	proton++;	
				
			if(matrix[currentRow][currentCol]=='p')
            	antiProton++;
        }
        
        // sol
        else if (key == 75 && currentCol > 0) {
        	matrix[currentRow][currentCol]='0';
            currentCol--;
            
            if(matrix[currentRow][currentCol]=='1')
            	currentCol++;
            	
            if(matrix[currentRow][currentCol]=='E')
            	antiElectron++;
				
			if(matrix[currentRow][currentCol]=='e')
            	electron++;	
				
			if(matrix[currentRow][currentCol]=='P')
            	proton++;	
				
			if(matrix[currentRow][currentCol]=='p')
            	antiProton++;
        }
        
        // sag
        else if (key == 77 && currentCol < col-1) {
        	matrix[currentRow][currentCol]='0';
            currentCol++;
            
            if(matrix[currentRow][currentCol]=='1')
            	currentCol--;
            	
            if(matrix[currentRow][currentCol]=='E')
            	antiElectron++;
				
			if(matrix[currentRow][currentCol]=='e')
            	electron++;	
				
			if(matrix[currentRow][currentCol]=='P')
            	proton++;	
				
			if(matrix[currentRow][currentCol]=='p')
            	antiProton++;
        }

		if(matrix[currentRow][currentCol]=='C'){
			flag=2;
		}
		
		if(matrix[currentRow][currentCol]=='K'){
			flag=3;
		}

		matrix[currentRow][currentCol]='X';
		
		time_t currentTime = time(NULL);
		passTime = difftime(currentTime, startTime);
		remainingTime= timeLimit-passTime;
		if (passTime >= timeLimit) {
            printf("\nThe time is up! Game over.\n");
            printf("Earned points: 1 \n");
            flag=1;
            sleep(1);
        }
        
		printf("\n");
    }
    
    if (flag==1){
    	printf("RETURNING TO GAME MENU...");
	}
    
    if(flag==2){	//cikisa ulasilirsa
	
	printf("\nYOU HAVE REACHED THE EXIT...\n");
	printf("\nYOUR POINTS IS BEING CALCULATED...\n");
	sleep(2);
	
	point=(electron+antiElectron+proton+antiProton)*7;
	
    if (proton<antiProton){
    	antiProton=abs(antiProton-proton);
    	proton=0;
	}
    else {
    	proton=abs(antiProton-proton);
    	antiProton=0;
	}
	
	if (electron<antiElectron){
    	antiElectron=abs(antiElectron-electron);
    	electron=0;
	}
    else {
    	electron=abs(antiElectron-electron);
    	antiElectron=0;
	}
	
	if (antiProton<antiElectron){
		antiHydrogen=antiProton;
		antiElectron=abs(antiElectron-antiProton);
		antiProton=0;
	}
	
	else {
		antiHydrogen=antiElectron;
		antiProton=abs(antiProton-antiElectron);
		antiElectron=0;
	}
	
	point+=antiHydrogen*100;
	
	system("cls");
	printf("\nNumber of Antihydrogen Produced: %d\n\n",antiHydrogen);
	printf("Particles remaining after production:\nProton: %d\nAntiproton: %d\nElectron: %d\nAntielectron: %d\n",proton,antiProton,electron,antiElectron);
	printf("\nEARNED POINTS: %d\n\n",point);
	printf("Press any key to return to the game menu:");
	getch();
	printf("\n\nRETURNING TO GAME MENU...");
	sleep(1);
	}
	
	if (flag==3){
		printf("\nYOU ARE LOST IN THE BLACK HOLE. RETURNING TO GAME MENU...");
	}
	
	sleep(2);
	system("cls");
	
	return point;
}

int otoMove (char **matrix, int row, int col){
	int timeLimit = 60;
	int passTime;
	int remainingTime=timeLimit;
	int i,j;
	int key;
	int flag=0;
	int currentRow,currentCol;
	int electron=0, antiElectron=0, proton=0, antiProton=0, antiHydrogen=0, point=1;
	int keyValues[] = {72, 80, 75, 77};
	int randomIndex,oppositeIndex=-1;
	srand(time(NULL));
	
	for( i=0 ; i<row ; i++){
		
		for( j=0 ; j<col ; j++){
		
			if(matrix[i][j] == 'G'){
				currentRow=i;
				currentCol=j;
			}
		}
}
	time_t startTime = time(NULL);
    while ( flag==0 ) {
     system("cls");
        printMatrix(matrix,row,col);
        printf("\nCollected items:\nProton:%d\nAntiproton:%d\nElectron:%d\nAntielectron:%d\n\n",proton,antiProton,electron,antiElectron);
        printf("Remaining Time:%d\n",remainingTime);
		printf("Enter the direction you want to go: ");
		do {
        randomIndex = rand() % 4;
    	} while (randomIndex == oppositeIndex);

        key = keyValues[randomIndex];
   
        if (key == 27) { // ESC 
            flag=1;
        }

        // yukari
        if (key == 72 && currentRow > 0) {
        	matrix[currentRow][currentCol]='0';
            currentRow--;
            
            if(matrix[currentRow][currentCol]=='1')
            	currentRow++;
            	
            if(matrix[currentRow][currentCol]=='E')
            	antiElectron++;
				
			if(matrix[currentRow][currentCol]=='e')
            	electron++;	
				
			if(matrix[currentRow][currentCol]=='P')
            	proton++;	
				
			if(matrix[currentRow][currentCol]=='p')
            	antiProton++;			
            	
        }
        
        // asagi
        else if (key == 80 && currentRow < row-1) {
        	matrix[currentRow][currentCol]='0';
            currentRow++;
            
            if(matrix[currentRow][currentCol]=='1')
            	currentRow--;
            	
            if(matrix[currentRow][currentCol]=='E')
            	antiElectron++;
				
			if(matrix[currentRow][currentCol]=='e')
            	electron++;	
				
			if(matrix[currentRow][currentCol]=='P')
            	proton++;	
				
			if(matrix[currentRow][currentCol]=='p')
            	antiProton++;
        }
        
        // sol
        else if (key == 75 && currentCol > 0) {
        	matrix[currentRow][currentCol]='0';
            currentCol--;
            
            if(matrix[currentRow][currentCol]=='1')
            	currentCol++;
            	
            if(matrix[currentRow][currentCol]=='E')
            	antiElectron++;
				
			if(matrix[currentRow][currentCol]=='e')
            	electron++;	
				
			if(matrix[currentRow][currentCol]=='P')
            	proton++;	
				
			if(matrix[currentRow][currentCol]=='p')
            	antiProton++;
        }
        
        // sag
        else if (key == 77 && currentCol < col-1) {
        	matrix[currentRow][currentCol]='0';
            currentCol++;
            
            if(matrix[currentRow][currentCol]=='1')
            	currentCol--;
            	
            if(matrix[currentRow][currentCol]=='E')
            	antiElectron++;
				
			if(matrix[currentRow][currentCol]=='e')
            	electron++;	
				
			if(matrix[currentRow][currentCol]=='P')
            	proton++;	
				
			if(matrix[currentRow][currentCol]=='p')
            	antiProton++;
        }

		if(matrix[currentRow][currentCol]=='C'){
			flag=2;
		}
		
		if(matrix[currentRow][currentCol]=='K'){
			flag=3;
		}

		matrix[currentRow][currentCol]='X';
		
		time_t currentTime = time(NULL);
		passTime = difftime(currentTime, startTime);
		remainingTime= timeLimit-passTime;
		if (passTime >= timeLimit) {
            printf("\nThe time is up! Game over.\n");
            printf("Earned point: 1 \n");
            flag=1;
            sleep(1);
        }
        
        if(randomIndex==0)
        	oppositeIndex=1;
        	
        if(randomIndex==1)
        	oppositeIndex=0;
        	
        if(randomIndex==2)
        	oppositeIndex=3;
        	
        if(randomIndex==3)
        	oppositeIndex=2;
        
		printf("\n");
		usleep(500000); // 0.5 saniye
    }
    
    if (flag==1){
    	printf("RETURNING TO GAME MENU...");
	}
    
    if(flag==2){	//cikisa ulasilirsa
	
	printf("\nYOU HAVE REACHED THE EXIT...\n");
	printf("\nYOUR POINT IS BEING CALCULATED...\n");
	sleep(2);
	
	point=(electron+antiElectron+proton+antiProton)*7;
	
    if (proton<antiProton){
    	antiProton=abs(antiProton-proton);
    	proton=0;
	}
    else {
    	proton=abs(antiProton-proton);
    	antiProton=0;
	}
	
	if (electron<antiElectron){
    	antiElectron=abs(antiElectron-electron);
    	electron=0;
	}
    else {
    	electron=abs(antiElectron-electron);
    	antiElectron=0;
	}
	
	if (antiProton<antiElectron){
		antiHydrogen=antiProton;
		antiElectron=abs(antiElectron-antiProton);
		antiProton=0;
	}
	
	else {
		antiHydrogen=antiElectron;
		antiProton=abs(antiProton-antiElectron);
		antiElectron=0;
	}
	
	point+=antiHydrogen*100;
	
	system("cls");
	printf("\nNumber of Antihydrogen Produced: %d\n\n",antiHydrogen);
	printf("Particles remaining after production:\nProton: %d\nAntiproton: %d\nElectron: %d\nAntielectron: %d\n",proton,antiProton,electron,antiElectron);
	printf("\nEARNED POINTS: %d\n\n",point);
	printf("Press any key to return to the game menu:");
	getch();
	printf("\n\nRETURNING TO THE GAME MENU...");
	sleep(1);
	}
	
	if (flag==3){
		printf("\nYOU ARE LOST IN THE BLACK HOLE. RETURNING TO GAME MENU...");
	}
	
	sleep(2);
	system("cls");
	
	return point;
}

void readToFile(const char *fileName,User *users,int *userCount) {
	int i;
    FILE *file = fopen(fileName, "wb"); // dosyayi binary modda aç

    if (file == NULL) {
        perror("File opening error");
        return;
    }

    
    for ( i = 0; i < *userCount; ++i) {
        // fwrite kullanarak structi dosyaya yazma
        fwrite(&users[i], sizeof(User), 1, file);
    }
	
	printf("\nUser information written to file.");
	
    fclose(file);
}

