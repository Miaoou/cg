#include <iostream>
#include <iomanip>

using namespace std;

unsigned int threes(unsigned int i, unsigned int j) {
    return 1;
}

unsigned int plouf(unsigned int i, unsigned int j) {
    auto threes_val = threes( i, j );
    threes_val = threes_val >> ( ( i + j ) % 32 );
    return threes_val;
    //return 0b01000000000000000000000000000000;
}

int main() {
    unsigned int size;
    cin >> size;

    unsigned int* b = new unsigned int[ size / 16 ];
    unsigned int* a = new unsigned int[ size / 16 ];

    for( int i = 0; i < size / 16; i++ ) { // Read size / 16 integers to a
        cin >> hex >> b[ i ];
    }

    for( int i = 0; i < size / 16; i++ ) { // Read size / 16 integers to a
        a[ i ] = 0;
    }


//    for( int i = 0; i < size; i++ ) { // Read size / 16 integers to a
//        for( int j = 0; j < size; j++ ) { // Read size / 16 integers to a
    auto i = 31u, j = 31u;
    auto idx = ( i + j ) / 32;
    // return 0b01100110011101111110001000000001;
    auto b31 = b[ idx ];
    auto b30 = b31 ^ plouf(i, j);
//        }
//    }



    for( int i = 0; i < size / 16; i++ ) {
        if( i > 0 ) {
            cout << ' ';
        }
        cout << setfill( '0' ) << setw( 8 ) << hex << b[ i ]; // print result
    }
    cout << endl;
}
