#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <queue>
#include <chrono>
#include <random>
#include <algorithm>
#include <string>

#include "hashfunctions.h"

/*
Reads all the files in "folder". Assumes that they are named 0.py, 1.py, ...
Returns a vector with for each file a set of of tokens in that file. 
Tokens are unique words or bigrams (two sequential words).
*/
std::vector<std::set<std::string>> readDatafiles(const char *folder)
{
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::set<std::string>> contents;

    int i = 0;
    std::ifstream in(folder + std::to_string(i) + ".py");

    while (in)
    {
        std::string word;
        std::string previous = "";
        contents.push_back(std::set<std::string>());

        while (in)
        {
            in >> word;
            contents[i].insert(word);

            previous = previous + " " + word;
            contents[i].insert(previous);

            previous = word;
        }

        i++;
        in = std::ifstream(folder + std::to_string(i) + ".py");
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Reading the data files took " << duration.count() << " ms" << std::endl;

    return contents;
}

// Replaces each unique word with a unique id (0, 1, 2, ...)
std::vector<std::set<int>> replaceWithUniqueId(const std::vector<std::set<std::string>> &input)
{
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::set<int>> contents(input.size());

    // ToDo

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Replacing words with IDs took " << duration.count() << " ms" << std::endl;
    return contents;
}

// Replaces each word with its hash code
template <unsigned int (*hashfunction)(const std::string &)>
std::vector<std::set<int>> replaceWithHash(const std::vector<std::set<std::string>> &input)
{
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::set<int>> contents(input.size());

    // ToDo
    // You can use "hashfunction as a function here"
    int hashcode = hashfunction("test");

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Replacing words with hash values took " << duration.count() << " ms" << std::endl;
    return contents;
}

template <unsigned int (*hashfunction)(const std::string &)>
void findCollisions(const std::vector<std::set<std::string>> &input)
{
    // ToDo
}

template <typename T>
std::vector<std::priority_queue<std::pair<double, int>>> jaccard(const std::vector<std::set<T>> &contents)
{
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::priority_queue<std::pair<double, int>>> similarities(contents.size());

    // ToDo

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Calculating the Jaccard index took " << duration.count() << " ms" << std::endl;
    return similarities;
}

// Show for each document the most similar other documents. Each document only needs to occur once in the list.
// Ignore the document if the similarity score is lower than "threshold" 
void showSummary(std::vector<std::priority_queue<std::pair<double, int>>> &similarities, double threshold)
{
    // ToDo
}

std::vector<std::priority_queue<std::pair<double, int>>> minhashing(const std::vector<std::set<int>> &contents, int n)
{
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::priority_queue<std::pair<double, int>>> similarities(contents.size());

    // ToDo

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "minhashing took " << duration.count() << " ms" << std::endl;
    return similarities;
}

int main()
{
    // Read the source code as sets of strings
    std::vector<std::set<std::string>> contents = readDatafiles("src/quiz/");
    std::cout << contents.size() << " files"<< std::endl;

    // Replace strings with integeres
    std::vector<std::set<int>> contentsId = replaceWithUniqueId(contents);
    //std::vector<std::set<int>> contentsId = replaceWithHash<worst_hash>(contents);

    // Check if a certain hash function results in collisions
    //findCollisions<horner_hash>(contents);

    // Calculate the Jaccard similarity either on sets of strings or on sets of ints
    std::vector<std::priority_queue<std::pair<double, int>>> similarities = jaccard<std::string>(contents);
    //std::vector<std::priority_queue<std::pair<double, int>>> similarities = jaccard<int>(contentsId);

    // Estimate the Jaccard similarity using minhashing
    //std::vector<std::priority_queue<std::pair<double, int>>> similarities = minhashing(contentsId, 100);

    // Show the results
    showSummary(similarities, 0.75);

    return 0;
}
