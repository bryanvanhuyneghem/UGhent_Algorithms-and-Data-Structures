#ifndef BINTREE_H
#define BINTREE_H

#include <memory>
#include <vector>
#include <list>
#include <map>
#include <iostream>

class Node;

class BinaryTree : public std::unique_ptr<Node> {
public:
    using std::unique_ptr<Node>::unique_ptr;
    using std::unique_ptr<Node>::operator=;

    friend std::ostream &operator<<(std::ostream &os, const BinaryTree &tree);

    bool build(std::vector<std::string> &questions, std::list<std::string> &animals,
               std::vector<std::map<std::string, bool>> &answers);

    int height();

    double averageDepth();

    int numberOfLeaves();

    int numberOfSplits();

private:
    bool voegToe(std::vector<std::string> &questions, std::list<std::string> &animals,
                 std::vector<std::map<std::string, bool>> &answers);

    int totalDepth();
};

class Node {
public:
    std::string value;
    BinaryTree left, right;
    int level = 0; // used for printing method
};

bool BinaryTree::build(std::vector<std::string> &questions, std::list<std::string> &animals,
                       std::vector<std::map<std::string, bool>> &answers) {
    // Make root node for this tree
    *this = std::make_unique<Node>();
    return this->voegToe(questions, animals, answers);
}


bool BinaryTree::voegToe(std::vector<std::string> &questions, std::list<std::string> &animals,
                         std::vector<std::map<std::string, bool>> &answers) {
    if ((this->get()->level <= questions.size()) && animals.size() > 1) {
        // Vectors will contain the animals that are going left or right down the tree
        std::list<std::string> animals_left;
        std::list<std::string> animals_right;
        this->get()->value = questions[this->get()->level]; // Set the question (value) for this Node in the Binary Tree

        // Build a new Binary Tree node for both left & right nodes
        this->get()->left = std::make_unique<Node>();
        this->get()->right = std::make_unique<Node>();
        this->get()->left->level = this->get()->level + 1;
        this->get()->right->level = this->get()->level + 1;

        // Iterate all animals passed to this method and check the question of this node
        for (auto it = animals.begin(); it != animals.end(); it++) {
            if (answers[this->get()->level][*it]) {
                // Animal has to go down the left path of this node
                animals_left.push_back(*it);
            } else {
                // Animal has to go down the right path of this node
                animals_right.push_back(*it);
            }
        }

        // Recursively call this method again, passing on the animals down the appropriate node
        return this->get()->left.voegToe(questions, animals_left, answers) &&
                    this->get()->right.voegToe(questions, animals_right, answers);

    } else { // leaves
        // No more questions to add; move on to the animals
        if (animals.size() > 1) {
            // Conflicting animals
            std::cout << "Can not distinguish between ";
            for (auto it = animals.begin(); it != animals.end(); it++) {
                std::cout << *it << ", ";
            }
            std::cout << std::endl;
            return false;
        } else if (animals.size() == 1) { // set the animal to the leaf
            this->get()->value = animals.back();
        } else { // set leaves that won't have an animal assigned to them to null pointer
            *this = nullptr;
        }
        return true;
    }
}


int BinaryTree::height() {
    // Empty tree
    if (*this == nullptr) {
        return 0;
    } else {
        // the deepest point could be a left or right leaf; consider both
        int depth_left = this->get()->left.height();
        int depth_right = this->get()->right.height();

        if (depth_left > depth_right) {
            return depth_left + 1;
        } else {
            return depth_right + 1;
        }
    }
}

double BinaryTree::averageDepth() {
    if (*this == nullptr) {
        return 0;
    }
    return (double)this->totalDepth()/this->numberOfLeaves();
}

// Helper function for averageDepth
int BinaryTree::totalDepth(){
    if (*this == nullptr) {
        return 0;
    }
    if (this->get()->left == nullptr && this->get()->right == nullptr) {
        return this->get()->level;
    } else {
        if(this->get()->left != nullptr){
            this->get()->left->level = this->get()->level + 1;
        }
        if(this->get()->right != nullptr){
            this->get()->right->level = this->get()->level + 1;
        }
        return this->get()->left.totalDepth() + this->get()->right.totalDepth();
    }
}

int BinaryTree::numberOfLeaves() {
    if (*this == nullptr) {
        return 0;
    }
    if (this->get()->left == nullptr && this->get()->right == nullptr) {
        return 1;
    } else {
        return this->get()->left.numberOfLeaves() + this->get()->right.numberOfLeaves();
    }
}

int BinaryTree::numberOfSplits() {
    if (*this == nullptr) {
        return 0;
    }
    if (this->get()->left == nullptr && this->get()->right == nullptr) {
        return 0;
    } else {
        return 1 + this->get()->left.numberOfSplits() + this->get()->right.numberOfSplits();
    }
}

std::ostream &operator<<(std::ostream &os, const BinaryTree &tree) {
    if (tree != 0) {
        // write node
        if (tree->level == 0) {
            os << tree->value << std::endl;
        }
        if (tree->left) { // left node
            for (int i = 0; i < tree->level; i++) {
                os << "\t";
            }
            os << "--Y--> " << tree->left->value << "\n";
            operator<<(os, tree->left);
        }
        if (tree->right) { // right node
            for (int i = 0; i < tree->level; i++) {
                os << "\t";
            }
            os << "--N--> " << tree->right->value << "\n";
            operator<<(os, tree->right);
        }
    }
    return os;
}

#endif