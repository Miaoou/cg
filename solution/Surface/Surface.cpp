#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main()
{
    int L = 4;
    int H = 4;

    vector< string > rows
    { "####",
        "##O#",
        "#OO#",
        "####"
    };

    int N = 3;

    vector< pair< int, int > > coords{ { 0, 0 },{ 1, 2 },{ 2, 1 } };

    for( int i = 0; i < N; i++ ) {

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        cout << "answer" << endl;
    }

    return 0;
}