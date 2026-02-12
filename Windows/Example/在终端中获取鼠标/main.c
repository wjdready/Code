#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>

void gotoxy(int x, int y);
void setcolor(WORD color);
void setForeGroundAndBackGroundColor(int ForeGroundColor, int BackGroundColor);
void clearscreen();
void drawpixel(unsigned char x, unsigned char y, unsigned char Color);
void drawpixel2(unsigned char x, unsigned char y, unsigned char Color, char character);
void drawcircle(int x, int y, int a, int b, int color);
void drawline(int x0, int y0, int x1, int y1, int color);
void drawfilledrectangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char bkcol);
void drawframe(unsigned x, unsigned y, unsigned sx, unsigned sy, unsigned char col, unsigned char col2, char text_[]);
void drawwindow(unsigned x, unsigned y, unsigned sx, unsigned sy, unsigned char col, unsigned char col2, unsigned char bkcol, char text_[]);

int main()
{
    gotoxy(1, 23);
    setcolor(7);
    clearscreen();

    printf("click anywhere in console window to write - hello world -\n\n\n\n\n\n\n\n\n\n\n\n\n"
           "Press Ctrl+C to Exit");

    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD InputRecord;
    DWORD Events;
    COORD coord;
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 25;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hout, &cci);
    SetConsoleMode(hin, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
    bool EXITGAME = false;
    int buttonX = 1, buttonY = 1;

    drawpixel(buttonX, buttonY, 1);
    gotoxy(buttonX + 2, buttonY);
    setcolor(3);
    printf("<----- a button \n");

    while (!EXITGAME)
    {

        ReadConsoleInput(hin, &InputRecord, 1, &Events);

        switch (InputRecord.EventType)
        {
        case KEY_EVENT: // keyboard input

            switch (InputRecord.Event.KeyEvent.wVirtualKeyCode)
            {
            case VK_ESCAPE:
                EXITGAME = TRUE;
                break;

            case VK_SPACE:

                break;

            case VK_RETURN:

                break;

            case VK_LEFT:
                // left key   move player left
                printf("VK_LEFT   = %d\n", InputRecord.Event.KeyEvent.wVirtualKeyCode);

                break;

            case VK_RIGHT:
                // right key   move player right
                printf("VK_RIGHT   = %d\n", InputRecord.Event.KeyEvent.wVirtualKeyCode);

                break;

            case VK_UP:
                // up key   move player up
                printf("VK_UP   = %d\n", InputRecord.Event.KeyEvent.wVirtualKeyCode);

                break;

            case VK_DOWN:
                // up key   move player down
                printf("VK_DOWN   = %d\n", InputRecord.Event.KeyEvent.wVirtualKeyCode);

                break;

            } // switch

            //---------------------------------------------------------------------------------
            break;

        case MOUSE_EVENT: // mouse input

            if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
            {
                coord.X = InputRecord.Event.MouseEvent.dwMousePosition.X;
                coord.Y = InputRecord.Event.MouseEvent.dwMousePosition.Y;
                SetConsoleCursorPosition(hout, coord);
                SetConsoleTextAttribute(hout, rand() % 7 + 9);

                if ((InputRecord.Event.MouseEvent.dwMousePosition.X == buttonX) &&
                    (InputRecord.Event.MouseEvent.dwMousePosition.Y == buttonY))
                {

                    clearscreen();
                    gotoxy(1, 1);
                    setcolor(7);
                    drawpixel(buttonX, buttonY, 1);
                    setcolor(3);
                    printf(" mybutton was pressed \n");
                    setcolor(7);
                    Sleep(500);
                    drawpixel(buttonX, buttonY, 1);
                    gotoxy(buttonX + 2, buttonY);
                    setcolor(3);
                    printf("<----- a button      \n");
                }

                printf("Hello world at %d x %d", InputRecord.Event.MouseEvent.dwMousePosition.X,
                    InputRecord.Event.MouseEvent.dwMousePosition.Y);
                
            } // mouse

            break;

        case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing
            ;
            break;

        case FOCUS_EVENT: // disregard focus events

        case MENU_EVENT: // disregard menu events

            break;

        default:
            printf("Unknown event type \n");
            break;
        }

        FlushConsoleInputBuffer(hin);
    }
    gotoxy(1, 23);
    setcolor(7);
    clearscreen();
    printf("\n");
    return 0;
}

void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    return;
}

//*****************************************************************************

void setcolor(WORD color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    return;
}

//*****************************************************************************

//
//     colors:
//     0 = Black
//     1 = Blue
//     2 = Green
//     3 = Cyan
//     4 = Red
//     5 = Magenta
//     6 = Yellow
//     7 = LightGray
//     8 = DarkGray
//     9 = LightBlue
//     10 = LightGreen
//     11 = LightCyan
//     12 = LightRed
//     13 = LightMagenta
//     14 = LightYellow
//     15 = White

//

//*****************************************************************************

void setForeGroundAndBackGroundColor(int ForeGroundColor, int BackGroundColor)
{
    int color = 16 * BackGroundColor + ForeGroundColor;
    setcolor(color);
}

//*****************************************************************************

void clearscreen()
{
    COORD coordScreen = {0, 0};
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(hConsole, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
    SetConsoleCursorPosition(hConsole, coordScreen);
    return;
}

//*****************************************************************************

void drawpixel(unsigned char x, unsigned char y, unsigned char Color)
{
    setcolor(Color);
    gotoxy(x, y);
    printf("Û");
}
//*****************************************************************************

void drawpixel2(unsigned char x, unsigned char y, unsigned char Color, char character)
{
    setcolor(Color);
    gotoxy(x, y);
    printf("%c", character);
}

//*****************************************************************************

void drawcircle(int x, int y, int a, int b, int color)
{
    int wx, wy;
    int thresh;
    int asq = a * a;
    int bsq = b * b;
    int xa, ya;

    drawpixel(x, y + b, color);
    drawpixel(x, y - b, color);

    wx = 0;
    wy = b;
    xa = 0;
    ya = asq * 2 * b;
    thresh = asq / 4 - asq * b;

    for (;;)
    {
        thresh += xa + bsq;

        if (thresh >= 0)
        {
            ya -= asq * 2;
            thresh -= ya;
            wy--;
        }

        xa += bsq * 2;
        wx++;

        if (xa >= ya)
            break;

        drawpixel(x + wx, y - wy, color);
        drawpixel(x - wx, y - wy, color);
        drawpixel(x + wx, y + wy, color);
        drawpixel(x - wx, y + wy, color);
    }

    drawpixel(x + a, y, color);
    drawpixel(x - a, y, color);

    wx = a;
    wy = 0;
    xa = bsq * 2 * a;

    ya = 0;
    thresh = bsq / 4 - bsq * a;

    for (;;)
    {
        thresh += ya + asq;

        if (thresh >= 0)
        {
            xa -= bsq * 2;
            thresh = thresh - xa;
            wx--;
        }

        ya += asq * 2;
        wy++;

        if (ya > xa)
            break;

        drawpixel(x + wx, y - wy, color);
        drawpixel(x - wx, y - wy, color);
        drawpixel(x + wx, y + wy, color);
        drawpixel(x - wx, y + wy, color);
    }
}

//*****************************************************************************

void drawline(int x0, int y0, int x1, int y1, int color)
{
    int pix = color;
    int dy = y1 - y0;
    int dx = x1 - x0;
    int stepx, stepy;

    if (dy < 0)
    {
        dy = -dy;
        stepy = -1;
    }
    else
    {
        stepy = 1;
    }
    if (dx < 0)
    {
        dx = -dx;
        stepx = -1;
    }
    else
    {
        stepx = 1;
    }
    dy <<= 1; // dy is now 2*dy
    dx <<= 1; // dx is now 2*dx

    drawpixel(x0, y0, pix);
    if (dx > dy)
    {
        int fraction = dy - (dx >> 1); // same as 2*dy - dx
        while (x0 != x1)
        {
            if (fraction >= 0)
            {
                y0 += stepy;
                fraction -= dx; // same as fraction -= 2*dx
            }
            x0 += stepx;
            fraction += dy; // same as fraction -= 2*dy
            drawpixel(x0, y0, pix);
        }
    }
    else
    {
        int fraction = dx - (dy >> 1);
        while (y0 != y1)
        {
            if (fraction >= 0)
            {
                x0 += stepx;
                fraction -= dy;
            }
            y0 += stepy;
            fraction += dx;
            drawpixel(x0, y0, pix);
        }
    }
}

//*****************************************************************************

void drawframe(unsigned x, unsigned y, unsigned sx, unsigned sy, unsigned char col, unsigned char col2, char text_[])
{
    unsigned i, j, m;
    {

        m = (sx - x); // differential
        j = m / 8;    // adjust
        j = j - 1;    // more adjustment
        gotoxy(x, y);
        printf("É"); // Top left corner of drawframe
        gotoxy(sx, y);
        printf("»"); // Top right corner of drawframe
        gotoxy(x, sy);
        printf("È"); // Bottom left corner of drawframe
        gotoxy(sx, sy);
        printf("¼"); // Bottom right corner of drawframe

        for (i = x + 1; i < sx; i++)
        {
            gotoxy(i, y);
            printf("Í"); // Top horizontol line
            gotoxy(i, sy);
            printf("Í"); // Bottom Horizontal line
        }

        for (i = y + 1; i < sy; i++)
        {
            gotoxy(x, i);
            printf("º"); // Left Vertical line
            gotoxy(sx, i);
            printf("º"); // Right Vertical Line
        }

        gotoxy(x + j, y);
        printf(text_); // put Title
        gotoxy(1, 24);
    }
}

//*****************************************************************************

void drawfilledrectangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char bkcol)
{
    int x, y;
    setcolor(bkcol); // Set to color bkcol

    for (y = y1; y < y2; y++)
    { // Fill Y Region Loop
        for (x = x1; x < x2; x++)
        { // Fill X region Loop
            gotoxy(x, y);
            printf(" "); // Draw Solid space
        }
    }
}

//*****************************************************************************

void drawwindow(unsigned x, unsigned y, unsigned sx, unsigned sy,
                unsigned char col, unsigned char col2, unsigned char bkcol, char text_[])
{
    drawfilledrectangle(x, y, sx, sy, bkcol);
    drawframe(x, y, sx, sy, col, col2, text_);
}

//*****************************************************************************

void drawcolorpalette()
{
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            setForeGroundAndBackGroundColor(i, j);
            gotoxy(i * 4, j);
            printf("%d", (i * j) + 1);
        }
    }
}
