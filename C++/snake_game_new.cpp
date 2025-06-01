#include <iostream>
#include <cstdlib> // For system()
#include <unistd.h> // For sleep()
#include <termios.h> // For non-blocking input
using namespace std;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;
bool gameOver;
const int height = 20;
const int width = 20;
int headX, headY, fruitX, fruitY, score;
int tailx[100], taily[100];
int tail_len;

// Function declarations
void setup();
void draw();
void input();
void logic();
char getch();

int main() {
    char start;
    cout << "\t-------------------------------" << endl;
    cout << "\t\t :Snake King:" << endl;
    cout << "\t-------------------------------" << endl;
    cout << "\tPress 's' to start: ";
    cin >> start;
    if (start == 's') {
        setup();
        while (!gameOver) {
            draw();
            input();
            logic();
            usleep(30000); // Sleep for 30 milliseconds
        }
    }
}

void setup() {
    gameOver = false;
    dir = STOP;
    headX = width / 2;
    headY = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    tail_len = 0;
}

void draw() {
    system("clear"); // Clear screen
    // Upper Border
    cout << "\t\t";
    for (int i = 0; i < width - 8; i++) {
        cout << "||";
    }
    cout << endl;

    // Snake, fruit, space, and side borders
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // Left border
            if (j == 0) {
                cout << "\t\t||";
            }
            // Snake head
            if (i == headY && j == headX) {
                cout << "O";
            }
            // Fruit
            else if (i == fruitY && j == fruitX) {
                cout << "*";
            }
            // Space and snake tail
            else {
                bool print = false;
                for (int k = 0; k < tail_len; k++) {
                    if (tailx[k] == j && taily[k] == i) {
                        cout << "o";
                        print = true;
                    }
                }
                if (!print) {
                    cout << " ";
                }
            }
            // Right border
            if (j == width - 1) {
                cout << "||";
            }
        }
        cout << endl;
    }
    // Lower Border
    cout << "\t\t";
    for (int i = 0; i < width - 8; i++) {
        cout << "||";
    }
    cout << endl;
    cout << "\t\t\tScore: " << score << endl;
}

char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcgetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
}

void input() {
    if (cin.peek() != EOF) { // Non-blocking input
        char key = getch();
        switch (key) {
            case 'a':
                dir = LEFT;
                break;
            case 'd':
                dir = RIGHT;
                break;
            case 'w':
                dir = UP;
                break;
            case 's':
                dir = DOWN;
                break;
            default:
                break;
        }
    }
}

void logic() {
    // Tail logic
    int prevx = tailx[0];
    int prevy = taily[0];
    int prev2x, prev2y;
    tailx[0] = headX;
    taily[0] = headY;
    for (int i = 1; i < tail_len; i++) {
        prev2x = tailx[i];
        prev2y = taily[i];
        tailx[i] = prevx;
        taily[i] = prevy;
        prevx = prev2x;
        prevy = prev2y;
    }

    // Direction logic
    switch (dir) {
        case LEFT:
            headX--;
            break;
        case RIGHT:
            headX++;
            break;
        case UP:
            headY--;
            break;
        case DOWN:
            headY++;
            break;
        default:
            break;
    }

    // Touch walls
    if (headX >= width) {
        headX = 0;
    } else if (headX < 0) {
        headX = width - 1;
    }
    if (headY >= height) {
        headY = 0;
    } else if (headY < 0) {
        headY = height - 1;
    }

    // Snake bite itself
    for (int i = 0; i < tail_len; i++) {
        if (tailx[i] == headX && taily[i] == headY) {
            gameOver = true;
        }
    }

    // Snake eats fruit
    if (headX == fruitX && headY == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        tail_len++;
    }
}
