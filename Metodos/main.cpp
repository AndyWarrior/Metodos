//
//  Metodos Cuantitativos Proyecto Final
//
//  Mónica Lozano
//  Andrés López De León
//  Eliézer Galván
//

#include <iostream>
#include <math.h>

#define TOTAL_ITERATIONS  10000000

int clients = 0;
int iterations = 0;
double time_passed = 0.0;

using namespace std;

int main(int argc, const char * argv[]) {
    
    while (iterations <= TOTAL_ITERATIONS) {
        if (iterations % 222 == 0) {
            cout << "Departure time: " << iterations << " microseconds" << endl;
        }
        
        if (iterations % 41000 == 0) {
            cout << "Frame processing time: " << iterations << " microseconds" << endl;
        }
        
        if (iterations % 2000000 == 0) {
            cout << "New client arrives time: " << iterations << " microseconds" << endl;
        }
        
        iterations++;
    }
    return 0;
}
