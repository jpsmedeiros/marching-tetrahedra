#include <iostream>
#include <cstdlib>

using namespace std;

int main (int argc, char * argv[])
{
    cout << "argc: " << argc << "\n";
    for (int i = 0; i < argc; i++) {
        cout << "argv [" << i << "] " << argv[i] << "\n";
    }

    cout << atoi(argv[1]) * 10 << "\n";

    return 0;
}
