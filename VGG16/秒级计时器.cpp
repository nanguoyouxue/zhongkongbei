#include "stdafx.h"
#include <iostream>
#include <time.h>

using namespace std;

int main(){
  clock_t start = clock();

  clock_t end =(clock()- start)/CLOCKS_PER_SEC;

  cout<<"time comsumption is "<<end<<endl;
}
