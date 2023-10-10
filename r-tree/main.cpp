#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "RTree.h"

using namespace std;

void print(const int& command, vector<vector<vector<pair<int, int>>>>& objects_n, string& output) {
    output.resize(0);
    output = to_string(command);

    switch (command) {
        case 0: // OBJECTS
            output += "|" + to_string(objects_n[0].size());
            for (auto& polygon : objects_n[0]) {
                output += "|" + to_string(polygon.size());

                for (auto& point : polygon) {
                    output += "|" + to_string(point.first) + "|" + to_string(point.second);
                }
            }
            break;

        case 1: // MBRS
            cout << "Number of (objects): " << to_string(objects_n.size()) << endl;
            output += "|" + to_string(objects_n.size());
            for (auto& objects : objects_n) {
                output += "|" + to_string(objects.size());
                cout << "Number of polygons: " << to_string(objects.size()) << endl;
                for (auto& polygon : objects) {
                    output += "|" + to_string(polygon.size());
                    cout << "Number of points: " << to_string(polygon.size()) << endl;
                    for (auto& point : polygon) {
                        output += "|" + to_string(point.first) + "|" + to_string(point.second);
                        cout << "   Point: " << to_string(point.first) << " | " << to_string(point.second) << endl;
                    }
                }
                cout << endl << "--------------------" << endl;
            }
            break;
        default:
            output += "|0";
            break;
    }

    output += "|END";
}

void print_pair(vector<pair<int, int>> output) {
    for (auto& x : output) {
        cout << " ( " << x.first << " , " << x.second << " ) ";
    }
}
void MBRs(const vector<vector<vector<pair<int, int>>>> &mbrs_n) {
    ofstream archivo("C:\\Users\\Hillary\\Downloads\\untitled20\\prueba.txt");
    ofstream niveles("C:\\Users\\Hillary\\Downloads\\untitled20\\prueba 2.txt");

    for (size_t level = 0; level < mbrs_n.size(); ++level) {
        const vector<vector<pair<int, int>>> &level_mbrs = mbrs_n[level];
        cout << "Level " << level << " MBRS:" << endl;

        for (size_t i = 0; i < level_mbrs.size(); ++i) {
            const vector<pair<int, int>> &mbr = level_mbrs[i];
            cout << "MBR " << i << ": ";

            for (size_t j = 0; j < mbr.size(); ++j) {
                archivo << mbr[j].first << " " << mbr[j].second << " ";
                if (j == 0) {
                    archivo << mbr[j].first << " " << mbr[j + 1].second << " ";
                } else if (j == 1) {
                    archivo << mbr[j].first << " " << mbr[j - 1].second << " ";
                    niveles << level << endl;
                }
                cout << "(" << mbr[j].first << "," << mbr[j].second << ")";
                if (j < mbr.size() - 1) {
                    cout << " - ";
                }
            }
            archivo << endl;
            cout << endl;
        }
    }

    archivo.close();
    niveles.close();
}

int main(int argc, char *argv[]) {
    vector<vector<pair<int, int>>> vpoints;

    float coord[50] = {
            770, 54, 690, 234, 914, 335, 689, 974, 875, 648,
            371, 420, 818, 199, 524, 915, 256, 702, 791, 182,
            300, 400, 500, 600, 700, 800, 100, 200, 300, 400,
            900, 100, 300, 700, 200, 600, 400, 800, 100, 500,
            300, 800, 700, 400, 200, 600, 100, 500, 300, 900
    };

    vector<pair<int, int>> points;
    for (int i = 0; i < 50; i += 2) {
        pair<int, int> A;
        A.first = coord[i];
        A.second = coord[i + 1];
        points.push_back(A);
    }

    for (unsigned int i = 0; i < 24; i += 3) {
        vector<pair<int, int>> sub1(&points[i], &points[i + 3]);
        vpoints.push_back(sub1);
    }

    RTree rtree;

    string output;
    vector<vector<pair<int, int>>> objects;
    vector<vector<vector<pair<int, int>>>> objects_n;

    for (auto &points : vpoints) {
        cout << "Inserting " << points.size() << " points: ";
        print_pair(points);
        Rect rect = rtree.MBR(points);
        rtree.Insert(rect.m_min, rect.m_max, points);
        cout << endl;
    }

    const int minCoords[2] = {10, 10}; // Coordenada mínima para la búsqueda
    const int maxCoords[2] = {1000, 1000}; // Coordenada máxima para la búsqueda

    // Llama a la función de búsqueda correctamente y almacena los resultados en 'result'
    vector<vector<pair<int, int>>> result;
    rtree.search(minCoords, maxCoords, rtree.m_root, result);

    // Itera a través de los resultados y los muestra en la consola
    for (size_t i = 0; i < result.size(); i++) {
        cout << "Resultado " << i + 1 << ":" << endl;
        for (size_t j = 0; j < result[i].size(); j++) {
            cout << "Coordenada " << j + 1 << ": (" << result[i][j].first << ", " << result[i][j].second << ")" << endl;
        }
        cout << endl;
    }

    rtree.getMBRs(objects_n);
    print(1, objects_n, output);


    cout << "INSERTING ONE MORE" << endl;
    vector<pair<int, int>> ad;
    ad.push_back({54, 12});
    ad.push_back({53, 4});

    cout << "Inserting " << ad.size() << " points: ";
    print_pair(ad);
    Rect rect = rtree.MBR(ad);
    rtree.Insert(rect.m_min, rect.m_max, ad);
    cout << endl;

    rtree.getMBRs(objects_n);
    print(1, objects_n, output);

    cout << "REMOVING LAST INSERTED" << endl;
    print_pair(ad);
    rtree.Remove(rect.m_min, rect.m_max, ad);
    cout << endl;

    rtree.getMBRs(objects_n);
    print(1, objects_n, output);
    MBRs(objects_n);

    return 0;
}
