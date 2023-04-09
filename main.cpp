#include <iostream>
#include <string>
#include <algorithm>
#include <random>

void randomTaken(std::string matriz[3][3]){
    // Se acomodan de manera aleatoria los numeros
    std::random_device random;
    std::mt19937 generator(random());
    std::shuffle(&matriz[0][0], &matriz[0][0] + 9, generator);
}

int main() {
    std::string matriz[3][3] = {{" ", "1", "2"}, {"3", "4", "5"}, {"6", "7", "8"}};

    std::cout << "Matriz Inicializada: " << std::endl;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            std::cout << " [ "+matriz[i][j]+" ] " << " ";
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }
    
    randomTaken(matriz);
    std::cout << "Matriz reordenada: " << std::endl;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            std::cout << " [ "+matriz[i][j]+" ] " << " ";
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }
    return 0;
}
