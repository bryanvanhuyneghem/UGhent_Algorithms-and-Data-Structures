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
#include <iomanip>
#include <limits.h>

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
    // dictionary
    std::map<std::string, int> dictionary;
    // loop over the vector to create the dictionary
    int uniqueID = 0;
    for(int i = 0; i < input.size(); i++){
        // loop over the strings in the set
        for(const auto& str : input[i]){
            // add the key-value pair to the dictionary if it doesn't already exist in the dictionary
            if(dictionary.find(str) == dictionary.end()){
                dictionary[str] = uniqueID;
                uniqueID++;
            }
        }
    }
    // loop over the vector to convert strings into unique ids, using the dictionary as a look-up
    for(int i = 0; i < input.size(); i++){
        for(const auto& str : input[i]){
            contents[i].emplace(dictionary[str]);
        }
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Replacing words with IDs took " << duration.count() << " ms" << std::endl;
    return contents;
}

// Replaces each word with its hash code
template <unsigned int (*hashfunction)(const std::string &)>
std::vector<std::set<unsigned int>> replaceWithHash(const std::vector<std::set<std::string>> &input)
{
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::set<unsigned int>> contents(input.size());

    // ToDo
    // You can use "hashfunction as a function here"
    // loop over the vector and sets and turn every string into a hashcode; add this hashcode to the contents set
    for(int i = 0; i < input.size(); i++){
        for(const auto& str : input[i]){
            unsigned int hashcode = hashfunction(str); // hash
            contents[i].emplace(hashcode);
        }
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Replacing words with hash values took " << duration.count() << " ms" << std::endl;
    return contents;
}

// finds the different collisions for a hashing function
template <unsigned int (*hashfunction)(const std::string &)>
void findCollisions(const std::vector<std::set<std::string>> &input)
{
    // ToDo
    int coll = 0; // count the amount of collisions to I can compare the hashing algorithms
    // dictionary to keep track of mapping between hashcodes and strings
    std::map<int, std::string> dictionary;
    // loop over the vector and sets and turn every string into a hashcode
    for(int i = 0; i < input.size(); i++){
        for(const auto& str : input[i]){
            int hashcode = hashfunction(str);
            // if the hashcode isn't in the dictionary yet, add it
            if(dictionary.find(hashcode) == dictionary.end()){
                dictionary[hashcode] = str; // add the string as a value to the hashcode key
            }
            // if the hashcode is in the dictionary, then check if the two strings are the same
            // if they are not, then we have found a collision
            else{
                if(dictionary[hashcode] != str){
                    // uncomment to see the collision output
                    //std::cout << "Collision found: " << "\"" << str << "\"" << " and " << "\"" << dictionary[hashcode] << "\"" << " both map to " << hashcode << std::endl;
                    coll++;
                }
            }
        }
    }
    std::cout << "The total amount of collisions is: " << coll << "." << std::endl;
}

// Calculates the Jaccard index for every pair of documents
template <typename T>
std::vector<std::priority_queue<std::pair<double, int>>> jaccard(const std::vector<std::set<T>> &contents)
{
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::priority_queue<std::pair<double, int>>> similarities(contents.size());

    // ToDo
    // time complexity O(n²)
    for(int i = 0; i < contents.size(); i++){
        std::priority_queue<std::pair<double, int>> prio_queue;
        for(int j = i+1; j < contents.size(); j++){
            std::set<T> intersection_set; // set of intersection in the numerator

            // establish the intersection set using the set_intersection function from the algorithm library
            std::set_intersection(contents[i].begin(), contents[i].end(), contents[j].begin(), contents[j].end(), std::inserter(intersection_set, intersection_set.begin()));

            // Jaccard Index using the formula provided in the assignment
            double jaccard_index = (double)intersection_set.size()/(contents[i].size() + contents[j].size() - intersection_set.size());
            // Add to priority queue
            prio_queue.push(std::make_pair(jaccard_index, j));
        }
        // Add to similarities vector
        similarities[i] = prio_queue;
    }

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
    // Keep track of the documents that have already been shown
    std::vector<int> freq_table(similarities.size());

    std::cout << std::left << std::setw(12) << "doc\t"
              << std::right << std::setw(3) << "Similar documents"
              << '\n';
    for(int i = 0; i < similarities.size(); i++){
        bool start = true; // The first time we loop through the while-loop, we want to print the
        freq_table[i] = 1; // update the frequency table to indicate that we have already seen this i value
        while(!similarities[i].empty() && freq_table[similarities[i].top().second] == 0 && similarities[i].top().first >= threshold){
            std::string str = ""; // used for the table layout
            // Print the document index only the first time
            if(start){
                str = std::to_string(i); // this string will have the document index the first time we loop through it.
                                         // Afterwards, it will be empty for every other iteration
                start = false;
            }
            std::cout << std::left << std::setw(12) << str << "\t";
            std::cout << std::right << std::setw(3) << similarities[i].top().second << " (" << similarities[i].top().first << ")\n";
            // update frequency table
            freq_table[similarities[i].top().second] = 1;
            // remove the pair from the priority queue
            similarities[i].pop();
        }
    }
}

// helper function that finds the biggest value in the sets; this is the c in my hashing functions
int biggest_number_in_sets(const std::vector<std::set<int>> &contents){
    int max = 0;
    for(int i = 0; i < contents.size(); i++){
        for(auto& x : contents[i]){
            if(x > max){
                max = x;
            }
        }
    }
    std::cout << "Max value: " << max << std::endl;
    return max;
}

// minhashing
std::vector<std::priority_queue<std::pair<double, int>>> minhashing(const std::vector<std::set<unsigned int>> &contents, int n)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::priority_queue<std::pair<double, int>>> similarities(contents.size());

    // ToDo
    unsigned int hash = 0;
    unsigned int a; // a in hashing function
    unsigned int b; // b in hashing function
    std::mt19937 eng{std::random_device{}()};
    std::uniform_int_distribution<> dis(0, 3345);

    // time complexity O(k*x*n²)
    for(int i = 0; i < contents.size(); i++){ // loop through the vector of sets
        std::priority_queue<std::pair<double, int>> prio_queue;
        for(int j = i+1; j < contents.size(); j++){ // loop through the vector of sets again to compare 2 sets
            int y = 0; // # of minhash matches
            for(int k = 0; k < n; k++){ // k hashing functions
                unsigned int smallest_hash_code_i = 3345; // initialize the smallest_hash_code for set i to the biggest number possible
                // generate random numbers for a and b
                a = dis(eng);
                b = dis(eng);

                // first document
                for(auto x : contents[i]){ // loop through the hash codes in the set, in other words, loop through the document
                    // hashing function
                    hash = a*x + b;
                    hash %= 3347; // the first prime bigger than 3345 (the biggest number in my sets); source: http://compoasso.free.fr/primelistweb/page/prime/liste_online_en.php
                    // keep track of the smallest hash code for this hashing function
                    if(hash < smallest_hash_code_i){
                        smallest_hash_code_i = hash;
                    }
                }

                // second document
                unsigned int smallest_hash_code_j = 3345; // initialize the smallest_hash_code for set j to the biggest number possible
                for(auto x : contents[j]){ // loop through the numbers in the set, in other words, loop through the document
                    // hashing function
                    hash = a*x + b;
                    hash %= 3347; // the first prime bigger than 3345 (the biggest number in my sets); source: http://compoasso.free.fr/primelistweb/page/prime/liste_online_en.php
                    // keep track of the smallest hash code for this hashing function
                    if(hash < smallest_hash_code_j){
                        smallest_hash_code_j = hash;
                    }
                }

                // Check if the minhashes of these two document are the same for this hash
                if(smallest_hash_code_i == smallest_hash_code_j){
                    y++; // match found
                }
            }
            // Estimate the Jaccard Index based on the amount of matches (y) and the number of hashing functions (n)
            double jaccard_index_estimation = (double)y/n;
            prio_queue.push(std::make_pair(jaccard_index_estimation, j));
        }

        // Add to similarities vector
        similarities[i] = prio_queue;
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "minhashing took " << duration.count() << " ms" << std::endl;
    return similarities;
}

int main()
{
    // Read the source code as sets of strings
    std::vector<std::set<std::string>> contents = readDatafiles("C:/Users/bryan/CLionProjects/hashing/quiz/");
    std::cout << contents.size() << " files"<< std::endl;

    // Replace strings with integers
    //std::vector<std::set<int>> contentsId = replaceWithUniqueId(contents);
    std::vector<std::set<unsigned int>> contentsId = replaceWithHash<good_hash>(contents);

    //region Print collisions
    // Check if a certain hash function results in collisions
    std::cout << "************************************************" << std::endl;
    std::cout << "worst_hash: ";
    findCollisions<worst_hash>(contents); // 15983
    std::cout << "bad_hash: ";
    findCollisions<bad_hash>(contents); // 12662
    std::cout << "other_bad_hash: ";
    findCollisions<other_bad_hash>(contents); // 1905
    std::cout << "horner_hash: ";
    findCollisions<horner_hash>(contents); // 10059
    std::cout << "good_hash: ";
    findCollisions<good_hash>(contents); // 0 -> good hash for our dataset
    std::cout << "jenkins_one_at_a_time_hash: ";
    findCollisions<jenkins_one_at_a_time_hash>(contents); // 0 -> good hash for our dataset
    std::cout << "djb2: ";
    findCollisions<djb2>(contents); // 7 -> good hash for our dataset
    std::cout << "sdbm: ";
    findCollisions<sdbm>(contents); // 0 -> good hash for our dataset
    std::cout << "lose_lose: ";
    findCollisions<lose_lose>(contents); // 1905
    std::cout << "rolling_hash_but_not_really: ";
    findCollisions<rolling_hash_but_not_really>(contents); // 0 -> good hash for our dataset
    std::cout << "************************************************" << std::endl;
    //endregion

    // Calculate the Jaccard similarity either on sets of strings or on sets of ints
    //std::vector<std::priority_queue<std::pair<double, int>>> similarities = jaccard<std::string>(contents); // Calculating the Jaccard index took 994 ms
    //std::vector<std::priority_queue<std::pair<double, int>>> similarities = jaccard<int>(contentsId); // Calculating the Jaccard index took 536 ms (with replaceWithUniqueId)
    //std::vector<std::priority_queue<std::pair<double, int>>> similarities = jaccard<unsigned int>(contentsId); // Calculating the Jaccard index took 601 ms (with replaceWithHash<good_hash>)

    // Biggest number in my sets, using the uniqueId for strings, is... 3345; prime slightly bigger than that is... 3347
    //biggest_number_in_sets(contentsId);


    // Estimate the Jaccard similarity using minhashing
    // Experiment for different values of k
    std::vector<int> different_k_values = {2,5,10,20,50,100,200};
    std::vector<std::priority_queue<std::pair<double, int>>> similarities;
    for(int i = 0; i < different_k_values.size(); i++){
        std::cout << "k: " << different_k_values[i] << std::endl;
        similarities = minhashing(contentsId, different_k_values[i]);
        // Show the results; uncomment to see results
        //showSummary(similarities, 0.75);
        std::cout << "************************************************" << std::endl;
    }

    //showSummary(similarities, 0.75);

    return 0;
}
