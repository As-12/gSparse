/* This is the second Demo showing how to use the library */

#include <Library/Utility/Swap.hpp>

int main()
{
    int a = 0;
    int b = 1;
    Library::Utility::Swap<int>(a,b);
    return 0;
}