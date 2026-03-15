#include <stdio.h>

int main(void){
    unsigned char input;

    printf("Enter an ASCII character: ");
    scanf("%c", &input);
    printf ("The ASCII value of a is:\n");
    
    printf("dec -- %d\n", input);
  //  printf("hexadecimal lowecase: %x\n", input);
 //   printf("hexadecimal uppercase: %X\n", input);
    printf("hex -- %X\n", input);

    printf("oct -- ");
    for (int i=7; i>=0; i--){
        int temp = (input >> i) & 1;
        printf("%d", temp);
    }
    printf("\n");

    return 0;
}
