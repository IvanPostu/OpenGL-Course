#include <iostream>
#include <cstdlib>

int main (int argc, char** argv)
{
  using namespace std;	
  int a = 6;
  a += rand();
	cout << "Hello1, the random number is: " << a << endl;

	return 0;
}
