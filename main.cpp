#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <limits>

using namespace std;
struct Point {
    float x;
    float y;
    float z;

    Point(float a, float b, float c) : x(a), y(b), z(c) {}

    Point(int a, int b, int c) : x(static_cast<float>(a)), y(static_cast<float>(b)), z(static_cast<float>(c)) {}
};

class Octree {
private:
    Octree* children[8];
    Point* points;
    Point bottomLeft;
    float h;
    int nPoints;

public:
    Octree(float x, float y, float z, float h) : bottomLeft(x, y, z), h(h), nPoints(0) {
        points = nullptr;
        for (int i = 0; i < 8; ++i) {
            children[i] = nullptr;
        }
    }

    bool exist(const Point& p) {
        if (points != nullptr) {
            return (points->x == p.x && points->y == p.y && points->z == p.z);
        }
        int index = 0;
        if (p.x >= bottomLeft.x + h) index |= 1;
        if (p.y >= bottomLeft.y + h) index |= 2;
        if (p.z >= bottomLeft.z + h) index |= 4;
        if (children[index] != nullptr) {
            return children[index]->exist(p);
        }
        return false;
    }

    void insert(const Point& p) {
        if (exist(p)) {
            return;
        }
        if (nPoints == 0) {
            points = new Point(p);
            nPoints = 1;
            return;
        }
        if (h < 1.0f) {
            return;
        }
        int index = 0;
        if (p.x >= bottomLeft.x + h) index |= 1;
        if (p.y >= bottomLeft.y + h) index |= 2;
        if (p.z >= bottomLeft.z + h) index |= 4;
        if (children[index] == nullptr) {
            float newH = h / 2.0f;
            float newX = bottomLeft.x + (index & 1 ? newH : 0);
            float newY = bottomLeft.y + (index & 2 ? newH : 0);
            float newZ = bottomLeft.z + (index & 4 ? newH : 0);
            children[index] = new Octree(newX, newY, newZ, newH);
        }
        children[index]->insert(p);
    }

    Point find_closest(const Point& p, float radius) {
        Point closest(-1.0f, -1.0f, -1.0f);
        float minDistance = std::numeric_limits<float>::max();
        find_closest_recursive(p, radius, closest, minDistance);
        return closest;
    }

    void print_nodos(std::ofstream& outFile, int depth = 0) {
        for (int i = 0; i < 8; ++i) {
            if (children[i] != nullptr) {
                for (int j = 0; j < depth; ++j) {
                    outFile << "  ";
                }
                outFile << "Nodos Hojas" << i << ": ";
                outFile << "BottomLeft(" << children[i]->bottomLeft.x << ", " << children[i]->bottomLeft.y << ", " << children[i]->bottomLeft.z << "), ";
                outFile << "h = " << children[i]->h << std::endl;
                children[i]->print_nodos(outFile, depth + 1);
            }
        }
    }

private:
    void find_closest_recursive(const Point& p, float radius, Point& closest, float& minDistance) {
        if (nPoints == 0) {
            return;
        }
        float distance = sqrt((p.x - points->x) * (p.x - points->x) +
                              (p.y - points->y) * (p.y - points->y) +
                              (p.z - points->z) * (p.z - points->z));
        if (distance < minDistance) {
            closest = *points;
            minDistance = distance;
        }
        if (h < 1.0f) {
            return;
        }
        for (int i = 0; i < 8; ++i) {
            if (children[i] != nullptr) {
                float dx = std::max(0.0f, std::max(bottomLeft.x - p.x, p.x - (bottomLeft.x + h)));
                float dy = std::max(0.0f, std::max(bottomLeft.y - p.y, p.y - (bottomLeft.y + h)));
                float dz = std::max(0.0f, std::max(bottomLeft.z - p.z, p.z - (bottomLeft.z + h)));
                float closestPointDistance = sqrt(dx * dx + dy * dy + dz * dz);
                if (closestPointDistance <= minDistance + radius) {
                    children[i]->find_closest_recursive(p, radius, closest, minDistance);
                }
            }
        }
    }
};


int main() {
    Octree tree(0.0f, 0.0f, 0.0f, 1.0f);

    int i = 0;
        string myText;
        vector<float> v;
        ifstream MyReadFile("C:\\Users\\Hillary\\Downloads\\untitled18\\points.txt");

    while (getline(MyReadFile, myText)) {
        std::istringstream is(myText);
        float n;
        while (is >> n)
            v.push_back(n);
    }

    while (i < v.size()) {
        Point p(v[i], v[i + 1], v[i + 2]);
        tree.insert(p);
        i += 3;
    }

    MyReadFile.close();

    // Abre el archivo para escribir los nodos del Octree
        ofstream outFile("C:\\Users\\Hillary\\Downloads\\untitled18\\octree_nodes.txt");
    if (outFile.is_open()) {
        tree.print_nodos(outFile);
        outFile.close();
    }

    return 0;
}
