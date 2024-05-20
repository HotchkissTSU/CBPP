#include <iostream>

#include "cbpp.h"

#ifdef main
#undef main
#endif

using namespace std;
using namespace cbpp;

int main(){
	Matrix a(3);
	
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			cin>>a(j,i);
		}
	}
	
	Matrix c = a.Inv();
	Matrix d = a*c;
	
	c.Print();
}