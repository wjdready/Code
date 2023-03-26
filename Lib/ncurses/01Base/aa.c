#include <stdio.h>
#include <ncurses/ncurses.h>

int main(int argc, char const *argv[])
{
    // init screen and sets up screen
    initscr();

    // print to screen
    printw("Hello World");

    // refreshes the screen
    refresh();
    // pause the screen output
    getch();

    // deallocates memory and ends ncurses
    endwin();
    
    return 0;
}
