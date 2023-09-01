#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <fstream>
using namespace std;

class Experimento {

private:
    int datos;
    vector<int> dimensiones;
public:
    Experimento(int datos_, vector<int>& dimensiones_) : datos(datos_), dimensiones(dimensiones_) {}

    double Dist_Euclidiana(vector<double>& p1, vector<double>& p2) {
        double sum = 0.0;
        for (size_t i = 0; i < p1.size(); ++i) {
            sum += pow(p1[i] - p2[i], 2);
        }
        return sqrt(sum);
    }

    double NumAleatorios() {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<double> dis(0.0, 1.0);
        return dis(gen);
    }

    void RealizarExperimento() {
        ofstream outFile("experimento.txt");
        for (int dimension : dimensiones) {
            vector<vector<double>> p(datos, vector<double>(dimension));

            for (int i = 0; i < datos; ++i) {
                for (int j = 0; j < dimension; ++j) {
                    p[i][j] = NumAleatorios();
                }
            }
            for (int i = 0; i < datos; ++i) {
                for (int j = i + 1; j < datos; ++j) {
                    double dist = Dist_Euclidiana(p[i], p[j]);
                    outFile << dist << "\n";
                }
            }
        }
        outFile.close();
    }

};

int main() {
    int datos = 100;
    vector<int> dimensiones = {5000}; //10, 50, 100, 500, 1000, 2000, 5000

    Experimento exp(datos, dimensiones);
    exp.RealizarExperimento();

    return 0;
}
