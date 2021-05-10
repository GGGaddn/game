#include <iostream>
#include <conio.h>//��� ���������� ��������� ������ ����������
#include <cmath>// ��� floor
#include <windows.h>//����������� ������� � �������, Sleep
#include <cstdlib>//��� srand(), rand()
#include <ctime>//����� ��� srand

const int width = 22;
const int height = 14;

char pixel[width][height];
int snake[width][height]{ 0 };

bool gameOver(false);
bool gameStart(false);

enum mover { STOP, LEFT, RIGHT, UP, DOWN };

void displayGraphics(int score) {
    //��������� ������� � 0,0
    COORD position;          // ���������� ����������� ���������
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);//��������� ����������� ���������� ������������ ������
    position.X = 0;         // ��������� ���������� X
    position.Y = 0;         // ��������� ���������� Y
    SetConsoleCursorPosition(hConsole, position);  // ����������� ������� �� �������� �����������

    //system("cls");
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++)
            std::cout << pixel[i][j];
        std::cout << std::endl;
    }
    //������� ����� ����������
    position.Y = height;
    SetConsoleCursorPosition(hConsole, position);
    for (int i = 0; i < 4; i++) {//���������� ����� � �����������
        for (int j = 0; j < width; j++)
            std::cout << " ";
        std::cout << "\n";
    }
    position.Y = height;
    SetConsoleCursorPosition(hConsole, position);
    std::cout << std::endl;
    std::cout << "SCORE " << score << "\n";
    //Sleep(sleep);
}

void displayParam(int x, int y, int xFruit, int yFruit, int snakeWidth) {
    std::cout << x << " " << y << "\n";
    std::cout << xFruit << " " << yFruit << " " << snakeWidth << "\n";
}

void startScreen() {
    //������� ���
    for (int j = 1; j < height - 1; j++)
        for (int i = 1; i < width - 1; i++)
            pixel[i][j] = ' ';
    //������� �����
    for (int i = 0; i < width; i++) {
        pixel[i][0] = '#';
        pixel[i][height - 1] = '#';
    }
    for (int j = 0; j < height; j++) {
        pixel[0][j] = '#';
        pixel[width - 1][j] = '#';
    }
    //����� � �������� �������
    int xCenter = floor(width / 2);
    int yCenter = floor(height) / 2;
    pixel[xCenter - 2][yCenter] = 'S';
    pixel[xCenter - 1][yCenter] = 'T';
    pixel[xCenter][yCenter] = 'A';
    pixel[xCenter + 1][yCenter] = 'R';
    pixel[xCenter + 2][yCenter] = 'T';
    displayGraphics(0);
    for (int i = xCenter - 2; i < xCenter + 3; i++)
        pixel[i][yCenter] = ' ';
    Sleep(3000);
}

void movesSnake(int& x, int& y, int& snakeWidth, mover& moveSnake, int& score, int& xFruit, int& yFruit) {
    if (!gameStart) {
        //�������� ������ � ����� �� ��������� �����
        x = floor(width / 2);
        y = floor(height / 2);

        srand(static_cast<unsigned int>(time(0)));
        xFruit = rand() % (width - 2) + 1;
        yFruit = rand() % (height - 2) + 1;
        pixel[xFruit][yFruit] = 'F';
    }
    gameStart = true;
    pixel[x][y] = '0';//������ ����� ���� ������
    snake[x][y] = 1;
    //pixel[x][y]=' ';
    bool eatingFruit = false;//��� �������� �� ������ �������� ������ � ������� ���������
//���������� ������� ������� (���� ������)
    int fromTheKeyboard(1);
    if (_kbhit())
        fromTheKeyboard = _getch();
    //fromTheKeyboard = 'w';
    switch (fromTheKeyboard) {
    case 'a':
        if (moveSnake != RIGHT)//��������� �������� ������ �� 180 ��������
            moveSnake = LEFT;
        break;
    case 'd':
        if (moveSnake != LEFT)
            moveSnake = RIGHT;
        break;
    case 'w':
        if (moveSnake != DOWN)
            moveSnake = UP;
        break;
    case 's':
        if (moveSnake != UP)
            moveSnake = DOWN;
        break;
    case '0':
        moveSnake = STOP;
        gameOver = true;
        break;
    }
    switch (moveSnake) {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    case STOP:

        break;
    }
    //������������ �� ������ ������� ��� ������ �� �����
    if (x < 1)
        x = (width - 2) + x;
    if (x > (width - 2))
        x = x - (width - 2);
    if (y < 1)
        y = (height - 2) + y;
    if (y > (height - 2))
        y = y - (height - 2);
    //������������ ������ �������� ������
    if (x == xFruit && y == yFruit) {
        score += 10;
        snakeWidth++;
        eatingFruit = true;
    }
    pixel[x][y] = 'O';//������ ������ ������

    if (((snake[x][y]) > 0) && ((snake[x][y]) < snakeWidth))//����������� ���� ��� ������������ � ����� ������
        gameOver = true;

    /*�������� ������.
     � ������ snake �� ����������� �������� ��� ��� ���� ���� ����,
     ����, ��� ���� ���� - ���������� ������ ������� ����, ��� ������ ����� ���������� ����� 1,
     � ����� ��������� �����. �������� ����� ����� ��������� ������� � ����������� ��������
     ��������, ��� ����� ���� (snakeWidth).*/

    snake[x][y] = 1;//���������� ����� ������
    int numberOfEmptyCells = 0;//���������� ������ ������
    for (int j = 1; j < height - 1; j++) {
        for (int i = 1; i < width - 1; i++) {
            if ((i != x) || (j != y))
                if (snake[i][j] != 0) {
                    snake[i][j] = snake[i][j] + 1;
                    if (snake[i][j] > snakeWidth) {
                        pixel[i][j] = ' ';
                        snake[i][j] = 0;
                    }
                }
            if ((snake[i][j]) == 0)
                numberOfEmptyCells++;//������� ���������� ������ ������
        }
    }
    //��������� ����� ����� �� ��������� �����*
    if (eatingFruit) {
        int randomCellNumber = 0;
        srand(static_cast<unsigned int>(time(0)));
        randomCellNumber = rand() % numberOfEmptyCells;//������������ ��������� ������� ���������� ����� ������ ������, ���� ����� ��������� �����
        int sequenceNumberOfEmptyCells = 0;//���������� ����� ������ ������
        //�������� ����� � ������������ ��������� ������� ������ ������
        for (int j = 1; j < height - 1; j++)
            for (int i = 1; i < width - 1; i++)
                if ((snake[i][j]) == 0) {
                    if (sequenceNumberOfEmptyCells == randomCellNumber) {
                        xFruit = i;
                        yFruit = j;
                    }
                    sequenceNumberOfEmptyCells++;
                }
    }
    pixel[xFruit][yFruit] = 'Q';//*��������� ����� ����� �� ��������� �����
    /*if((snake[x][y])>1)//����������� ���� ��� ������������ � ����� ������
        gameOver=true;*/
}

void speed(int snakeWidth) {
    int sleep(200);//������� ��������
    if ((sleep - snakeWidth) < 0)
        Sleep(0);
    else
        Sleep(sleep - snakeWidth / 10 * 20);
}

int main()
{
    int snakeWidth(2);//��������� ����� ������
    int x(0), y(0), xFruit(0), yFruit(0), score(0);
    mover moveSnake(RIGHT);
    startScreen();
    while (!gameOver) {
        movesSnake(x, y, snakeWidth, moveSnake, score, xFruit, yFruit);
        displayGraphics(score);
        //displayParam(x, y, xFruit, yFruit, snakeWidth);
        speed(snakeWidth);
    }
    return 0;
}