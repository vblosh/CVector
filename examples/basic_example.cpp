#include <iostream>
#include "cvector.hpp"

int main() {
    cvector::Vector<int> values;
    values.push_back(10);
    values.push_back(20);
    values.push_back(30);

    std::cout << "C++ vector: ";
    for (int value : values) {
        std::cout << value << ' ';
    }
    std::cout << '\n';

    CVector *raw = cvector_create(sizeof(int));
    if (raw == nullptr) {
        std::cerr << "Failed to create CVector\n";
        return 1;
    }

    int first = 40;
    int second = 50;
    if (cvector_push_back(raw, &first) != 0 ||
        cvector_push_back(raw, &second) != 0) {
        cvector_destroy(raw);
        std::cerr << "Failed to populate CVector\n";
        return 1;
    }

    // Ownership transfers to adopted. Do not use or destroy raw afterward.
    auto adopted = cvector::Vector<int>::adopt(raw);

    std::cout << "Adopted C vector: ";
    for (int value : adopted) {
        std::cout << value << ' ';
    }
    std::cout << '\n';

    return 0;
}
