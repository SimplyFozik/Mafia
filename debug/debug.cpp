#include <iostream>
#include <string>

using namespace std;

int main()
{
    while (true)
    {
        string msg1;
        getline(std::cin, msg1);
        cout << "size: " << size(msg1) << endl;
    }
}
