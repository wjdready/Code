#include <ncursesw/ncurses.h>
#include <stdio.h>

int main(void)
{
    // Initialize the window
    initscr();

    // Don't echo any keypresses
    noecho();

    static int ofs = 0;
    static char command[2048];
    addstr("\nsh# ");

    int ch;
    while ((ch = getch()) != ERR)
    {
        addch(ch);

        if (ch == '\n')
        {
            command[ofs] = '\0';
            printw("Run command: %s", command);
            ofs = 0;
            addstr("\nsh# ");
        }

        else if (ch == 0X7F) // 删除字符
        {
            if (ofs > 0)
            {
                // printw("\b \b"); // not work
                ofs--;
            }
        }

        else if (ofs < (sizeof(command) - 1))
        {
            command[ofs++] = ch;
        }

        refresh();
    }

    endwin();

    return 0;
}
