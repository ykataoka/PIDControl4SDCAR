#include <iostream>
#include <cstdio>
#include <ctime>
#include <cstdlib> //_sleep()  --- just a function that waits a certain amount of milliseconds


int main() {
    std::clock_t start;
    double duration;

    start = std::clock();

    /* Your algorithm here */
    for(int i=0; i<1000000000; i++){
      if (i % 100000000 == 0){
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	std::cout << duration << std::endl;
      }
    }
     
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

    std::cout<<"printf: "<< duration <<'\n';
}
