// Resistance.cpp

#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <algorithm>

using namespace std;

using dict_morse_t = map< char, string >;

struct Vertex
{
    Vertex( char val ) : _val{ val } {}
    char _val;
    map< char, shared_ptr< Vertex > > _children;
    int _idx{ 0 };
};

void
add_suffix( shared_ptr< Vertex >& root, string const& suffix )
{
    auto current_vertex = root;
    for( auto idx = 0; idx < suffix.size(); ++idx )
    {
        auto it = current_vertex->_children.find( suffix[ idx ] );
        if( it == end( current_vertex->_children ) )
        {
            auto& inserted = current_vertex->_children[ suffix[ idx ] ];
            inserted = make_shared< Vertex >( suffix[ idx ] );
            current_vertex = inserted;
        }
        else
            current_vertex = it->second;
    }

    current_vertex->_children[ '$' ] = make_shared< Vertex >( '$' );
}

void
add_word_trie( shared_ptr< Vertex >& root, string const& word )
{
    auto end_idx = word.size() - 1;
    if( end_idx == 0 ) // one character only
        add_suffix( root, word );
    else
        for( auto idx = (int)end_idx; idx >= 0; --idx )
            add_suffix( root, string( begin( word ) + idx, end( word ) ) );
}

shared_ptr< Vertex >
create_suffix_trie( string const& words_morse )
{
    auto root = make_shared< Vertex >( 'X' );

    add_word_trie( root, words_morse );

    return root;
}

vector< size_t >
find_occurences_in_trie( shared_ptr< Vertex > const& root_trie, string const& word_morse )
{
    vector< size_t > occurences_indexes;
    auto current_vertex = root_trie;

    for( auto c : word_morse )
    {
        auto& child = current_vertex->_children[ c ];
        if( !child )
            break;

        current_vertex = child;
    }

    if( current_vertex )
        occurences_indexes.push_back( current_vertex->_idx );

    return occurences_indexes;
}

vector< string >
convert_alpha_to_morse( dict_morse_t& dict_morse, vector< string > const& words_alpha )
{
    vector< string > words_morse;
    for( auto const& word_alpha : words_alpha )
    {
        string word_morse;
        for( char c : word_alpha )
            word_morse.append( dict_morse[ c ] );
        
        words_morse.push_back( move( word_morse ) );
    }

    return words_morse;
}

int
main()
{
    dict_morse_t dict_morse{
        { 'A', ".-" },
        { 'B', "-..." },
        { 'C', "-.-." },
        { 'D', "-.-." },
        { 'E', "." },
        { 'F', "..-." },
        { 'G', "--." },
        { 'H', "...." },
        { 'I', ".." },
        { 'J', ".---" },
        { 'K', "-.-" },
        { 'L', ".-.." },
        { 'M', "--" },
        { 'N', "-." },
        { 'O', "---" },
        { 'P', ".--." },
        { 'Q', "--.-" },
        { 'R', ".-." },
        { 'S', "..." },
        { 'T', "-" },
        { 'U', "..-" },
        { 'V', "...-" },
        { 'W', ".--" },
        { 'X', "-..-" },
        { 'Y', "-.--" },
        { 'Z', "--.." }
    };

    vector< string > words_alpha{
        { "HELL" },
        { "HELLO" },
        { "OWORLD" },
        { "WORLD" },
        { "TEST" }
    };

    string pattern{ { "......-...-..---.-----.-..-..-.." } };

    auto words_morse = convert_alpha_to_morse( dict_morse, words_alpha );

    auto root_pattern_trie = create_suffix_trie( pattern );

    vector< vector< size_t > > occ;
    for( auto const& word_morse : words_morse )
        occ.push_back( find_occurences_in_trie( root_pattern_trie, word_morse ) );
        
    return 0;
}
/*
 *......-...-..---.-----.-..-..-..
5
HELL
HELLO
OWORLD
WORLD
TEST

sortie: 2
 */
int main2()
{
    string L;
    cin >> L; cin.ignore();
    int N;
    cin >> N; cin.ignore();
    for (int i = 0; i < N; i++) {
        string W;
        cin >> W; cin.ignore();
    }

    // Write an action using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    cout << "answer" << endl;
}
