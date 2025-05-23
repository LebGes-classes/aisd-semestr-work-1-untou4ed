#include "AVLtree.hpp"
#include <iostream>

int main() {
    AVLTree<int> tree;

    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    tree.insert(25);

    tree.printInOrder();  // 10 20 25 30 40 50

    std::cout << tree.contains(25) << std::endl; 
    std::cout << tree.contains(99) << std::endl;

    tree.remove(30);
    tree.printInOrder();  // 10 20 25 40 50

    tree.remove(999); // несуществующий элемент 
    tree.printInOrder();
    return 0;
}