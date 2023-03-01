#include <stdio.h>

#define JSON_X(...) #__VA_ARGS__

#define JSON_M(args, ...) JSON_X(__VA_ARGS__), args

#define JSON_S(...) JSON_M(number, __VA_ARGS__)

#define JSON_INT(x) %d

int main(int argc, char const *argv[])
{
    printf("Hello World\n");
    
    int number = 12;

    printf (
        JSON_S(
            <N>{
                "Class": "105",
                /* Flag is a number */
                "Flag": JSON_INT(number),
                "Value": 666
            }</N>
        )
    );

    return 0;
}
