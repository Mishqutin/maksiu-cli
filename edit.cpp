// A freakin' text editar!
// (c) Mishqutin 2019
//     under no-muthafuckin-license
#include <iostream>
#include <termios.h>
#include <stdio.h>
#include <string>

using namespace std;

// Configs etc.
struct colors_t {
	string bold = "\u001b[1m";
    string    r = "\u001b[0m";
    
    string left = "\u001b[1D";
} colors;

struct config_t {
    bool color = false;
    bool  nums = false;
} config;


// Functions.

int getch() {
    // Get single character.
    int ch = getchar();
    return ch;
}

// 27 - esc, 91or[ - arrow key:, ABCD
int escSeq[3] = {0, 0, 0};
int parseEscape(int ch) {
    // Parse escape code.
    if (ch == 27) {
        // Got Esc.
        escSeq[0] = 27;
        escSeq[1] = 0;
        escSeq[2] = 0;
        return 0;
    } else if (escSeq[0] == 27
           and escSeq[1] == 0
           and escSeq[2] == 0) {
        // Sequence = 
        //    Esc+<?>
        if (ch == '[') {
            // +[
            escSeq[1] = '[';
            return 0;
        } else {
            // + something else
            // Reset.
            escSeq[0] = 0;
            escSeq[1] = 0;
            escSeq[2] = 0;
            return ch;
        }
    } else if (escSeq[0] ==  27
           and escSeq[1] == '['
           and escSeq[2] ==  0) {
        // Sequence =
        //  Esc+[+<?>
        if (false) {
            // to-do
        } else {
            // +something else
            // Reset.
            escSeq[0] = 0;
            escSeq[1] = 0;
            escSeq[2] = 0;
            return ch;
        }
    }
}





int parseChar(int ch) {
    int c;
    // Esc seq section.
    if ( ch == 27
      or escSeq[0] == 27) {
        // Parse escape sequence.
        c = parseEscape(ch);
    // Text printing mides section.
    } else if (ch == 'B') {
        // Switch bold print.
        if (config.color) {
            c = 0;
            cout << colors.r;
            config.color = false;
        } else {
            c = 0;
            cout << colors.bold;
            config.color = true;
        }
    } else if (ch == 'N') {
        // Switch letters/numbers.
        config.nums = !config.nums;
    // Special characters section.
    } else if (ch == 127) {
        // Backspace.
        c = 0;
        cout << colors.left << " " << colors.left << flush;
    }
    // End section.
    else {
        // If none of the above apply
        // print character.
        c = ch;
    }
    return c;
}

void printChar(int ch) {
    if (ch == 0) return; // If got 0 don't print.
    if (config.nums)
        // Numbers mode.
        cout << ch << '-' << flush;
    else
        // Letters mode.
        cout << (char) ch << flush;
}


// Execution.
void init();
void sub();
void end();

int main() {
	
    cout << "Start" << endl;
    init();
    sub();
    end();
    cout << "Finish" << endl;
    
    return 0;
}

struct termios old, new_;
void init() {
    tcgetattr(0, &old);
    new_ = old;
    new_.c_lflag &= ~(ICANON | ECHO | IGNBRK);
    tcsetattr(TCSANOW, 0, &new_);

    cin.sync_with_stdio(false);
}

void sub() {
    int c;
    int cp;
    
    do {
        c = getch();
        cp = parseChar(c);
        printChar(cp);
    // Do untill got '.' or Ctrl+C.
    } while (c != '.' && c != 3);
}

void end() {
    tcsetattr(TCSANOW, 0, &old);
    cin.sync_with_stdio(true);
}

