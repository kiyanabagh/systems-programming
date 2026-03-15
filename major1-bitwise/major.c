/*
Names: Joseph Smith || Kiyana Baghbadrani || Owen Erickson || Pablo Soliz
CSCE 3600.001 Group 13
Major 1 - Gitlab and Bitwise Operators
*/
#include "major.h"

#define MIN_VAL 1U
#define MAX_VAL 4294967295U

static uint32_t readValidNumber(void) //Read 32-Bit number in range
   {
      uint32_t num;
      while (1)
         {
            printf("Enter a 32-bit number (>= 1 and <= 4294967295, inclusively): ");
            if (scanf("%u", &num) == 1 && num >= MIN_VAL && num <= MAX_VAL)
            break;
            while (getchar() != '\n'); // bad input
         }
      return num;
   }

static unsigned int readRotateAmount(void) // Read rotate-right amount
   {
      unsigned int pos;
      while (1)
         {
            printf("Enter the number of positions to rotate-right the input (between 0 and 31, inclusively): ");
            if (scanf("%u", &pos) == 1 && pos <= 31)
            break;
            while (getchar() != '\n');
         }
      return pos;
   }

int main(void)
   {
      int choice;

      while (1)
         {  // Menu Display
            printf("\nEnter the menu option for the operation to perform:\n");
            printf("(1) Count Leading Zeroes\n");
            printf("(2) Endian Swap\n");
            printf("(3) Rotate-right\n");
            printf("(4) Parity\n");
            printf("(5) EXIT\n");
            printf("--> ");

            if (scanf("%d", &choice) != 1)
	       {
                  while (getchar() != '\n');
            	  printf("Error: Invalid option. Please try again.\n");
             	  continue;
               }
            if (choice == 5)
	       {  // Exit
            	  printf("Program terminating. Goodbye...\n");
            	  break;
               }
            if (choice < 1 || choice > 4)
	       {
            	  printf("Error: Invalid option. Please try again.\n");
            	  continue;
               }

            uint32_t num = readValidNumber();

            switch (choice)
	       {
            	  case 1: // CLZ
	             { /*
                	// CLZ
                        printf("The number of leading zeroes in %u is %u\n", num, zeros);
                	break;
			*/
            	     }
                  case 2: //Endian Swap
		     { /*
                	// Endian
                	printf("Endian swap of %u gives %u\n", num, swapped);
                	break;
			*/
            	     }
                  case 3: //Rotate Right
	             { /*
                	// Rotate Right
                	printf("%u rotated by %u position%s gives: %u\n", num, pos, (pos==1?"":"s"), rotated);
                        break;
			*/
            	     }
            	  case 4: //Parity
		     { /*
                	// Parity
                	printf("Parity of %u is %u\n", num, p);
                	break;
			*/
                     }
               }
         }
      return 0;
   }
