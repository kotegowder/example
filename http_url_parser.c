/*
 * $ Copyright Broadcom Corporation $
 */

/** @file
 *
 *
 */

#include "wiced.h"

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               Static Function Declarations
 ******************************************************/

/******************************************************
 *               Variable Definitions
 ******************************************************/
char *tests[] = {"Hello%20G%C3%BCnter", "%41+%41+%20+%41X", "asdf %%X", "asdf %X", "google.com+foo++boo", "Hello%20G%C3%BCnter%25%20%25+", "Hello%20G%C3%BCnter%kz", "Hello%20G%C3%BCnter%%", NULL};

/******************************************************
 *               Function Definitions
 ******************************************************/

static uint16_t http_server_remove_escaped_characters( char* output, uint16_t output_length, const char* input, uint16_t input_length )
{
    uint16_t bytes_copied;
    int a;

    for ( bytes_copied = 0; ( input_length > 0 ) && ( bytes_copied != output_length ); ++bytes_copied )
    {
        if ( *input == '%' )
        {
            ++input;
            --input_length;

            if ( ( input_length > 0 ) && ( *input == '%' ) )
            {
                return 0;
            }
            else
            {
                *output = 0;
                for ( a = 0; (a < 2) && (input_length > 0); ++a )
                {
                    *output = (char) ( *output << 4 );
                    if ( *input >= '0' && *input <= '9' )
                    {
                        *output = (char) ( *output + *input - '0' );
                    }
                    else if ( *input >= 'a' && *input <= 'f' )
                    {
                        *output = (char) ( *output + *input - 'a' + 10 );
                    }
                    else if ( *input >= 'A' && *input <= 'F' )
                    {
                        *output = (char) ( *output + *input - 'A' + 10 );
                    }
                    else
                    {
                        return 0;
                    }
                    --input_length;
                    if ( input_length > 0 )
                    {
                        ++input;
                    }
                }
                ++output;
            }
        }
        else if ( *input == '+' )
        {
            *output = ' ';
            --input_length;
            if ( input_length > 0 )
            {
                ++input;
                ++output;
            }
        }
        else
        {
            *output = *input;
            --input_length;
            if ( input_length > 0 )
            {
                ++input;
                ++output;
            }
        }
    }

    return bytes_copied;
}

void application_start(void)
{
    int i=0;
    char output[1024];
    uint16_t retval = 0;

    while (tests[i] != NULL) {
      printf("test[%d]: %s\n", i, tests[i]);
      memset((void *)output, 0x42, sizeof(output));
      retval = http_server_remove_escaped_characters(output, sizeof(output), tests[i], strlen(tests[i]));
      if ( retval != 0 )
      {
          output[retval] = '\0';
          printf( "output of http_server_remove_escaped_characters: %s, retval is %d\n", output, retval );
      }
      else
      {
          printf( "bad url\n" );
      }
      i++;
    }
    printf("\n\n");
}
