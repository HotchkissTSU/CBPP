#include <iostream>

#include <cbpp.h>

#ifdef main
#undef main
#endif

using namespace std;

int main(){
	cbpp::Matrix m(3);
	
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			cin>>m(j+1,i+1);
		}
	}
	
	cout<<m;
	
	cout<<"DET "<<m.Determinant()<<endl;
}