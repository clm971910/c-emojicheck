#include <stdio.h>
#include <string.h>

#include "emojicheck.h"


int test_emoji( const char * str )
{
    printf( "%s\n", str );

    int slen = strlen( str );
    int pos  = 0;
    
    while ( pos < slen )
    {
        if ( 0 == check_emoji( str, slen, &pos ) )
            printf( "\tfind emoji in pos:%d\n", pos );
    }
    
    return 0;
}



int main( int argc, char * argv[] )
{
    test_emoji( "11213243445454546565" );
    test_emoji( "有房🏡，性格温顺可人！经过家庭会议👪，打算给乐乐招名" );

    test_emoji( "aaefasfcvdgrdtfggyty" );
    test_emoji( "!@@##$$%%%^^^&&&&***(()__++" );


    return 0;
}