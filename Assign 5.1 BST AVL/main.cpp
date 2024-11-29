/*
* MS549 Data Structures and testing
* 11/28/24
* Jeff O'Hara
* In this program, we will be making a self-balancing Binary Search Tree (BST) as an AVL Tree
* The app will add, remove, caculate the maximum data stored, traverse, and find data or nodes with data.
* The program will read random, unsorted integers and also calculate performance analysis on the application.
* We will also be writing our data to a .CSV file to utilize in an Excel spreadsheet to analyze the data samples.
*/

// Libraries and header file
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <cmath> // For log function
#include <string> // For std::string
#include "BST.h"

using namespace std;
using namespace std::chrono;

// Function to generate prime numbers up to a given limit using Sieve of Eratosthenes
vector<int> generatePrimes(int limit) {
    vector<int> primes;
    if (limit >= 2) {
        vector<bool> isPrime(limit + 1, true);
        isPrime[0] = isPrime[1] = false;
        for (int num = 2; num <= limit; ++num) {
            if (isPrime[num]) {
                primes.push_back(num);
                if (num <= limit / num) { // Prevent integer overflow
                    for (int multiple = num * num; multiple <= limit; multiple += num) {
                        isPrime[multiple] = false;
                    }
                }
            }
        }
    }
    return primes;
}

int main() {
    // Informative message instead of using std::filesystem
    cout << "Program started. The current working directory depends on your environment." << endl;

    BST bst;

    // Test cases using prime numbers
    cout << "Adding prime numbers to BST:" << endl;
    int testPrimes[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };
    for (int prime : testPrimes) {
        bst.add(prime);
    }

    // Display the BST after adding prime numbers
    cout << "\nBST after adding prime numbers (inorder traversal):" << endl;
    bst.inorderTraverse();

    // Attempting to insert a duplicate prime number
    cout << "\nAttempting to insert duplicate prime number 13:" << endl;
    bst.add(13);

    // Removing a node with no children (leaf node)
    cout << "\nRemoving a leaf node (29):" << endl;
    bst.remove(29);
    bst.inorderTraverse();

    // Removing a node with one child
    cout << "\nRemoving a node with one child (19):" << endl;
    bst.remove(19);
    bst.inorderTraverse();

    // Removing a node with two children
    cout << "\nRemoving a node with two children (13):" << endl;
    bst.remove(13);
    bst.inorderTraverse();

    // Finding a prime number in the BST
    cout << "\nFinding prime number 17:" << endl;
    bool found = bst.find(17);
    if (found)
        cout << "Prime number 17 found." << endl;
    else
        cout << "Prime number 17 not found." << endl;

    // Finding the maximum prime number in the BST
    cout << "\nMaximum prime number in BST: " << bst.maximum() << endl;

    // Performance analysis using prime numbers
    const int sizes[] = { 100, 1000, 10000, 100000 };

    // Open the output file
    string outputPath = "bst_performance_primes.csv";
    ofstream outfile(outputPath);
    if (!outfile.is_open()) {
        cerr << "Error: Unable to open output file '" << outputPath << "'" << endl;
        return 1; // Exit the program if file can't be opened
    }
    outfile << "NumberOfPrimes,TimeMilliseconds" << endl;

    cout << "\nPerformance Analysis using Prime Numbers:" << endl;
    for (int n : sizes) {
        cout << "Starting performance analysis for n = " << n << endl;

        BST bst_perf;
        vector<int> primes;

        // Estimate an upper limit for generating at least n prime numbers
        // Using the Prime Number Theorem approximation: n ≈ limit / ln(limit)
        int limit = static_cast<int>(n * (log(n) + log(log(n))));

        // Generate prime numbers up to the estimated limit
        primes = generatePrimes(limit);

        // Ensure we have at least n prime numbers
        while (primes.size() < static_cast<size_t>(n)) {
            limit *= 2; // Increase limit and regenerate primes
            primes = generatePrimes(limit);
        }

        // Truncate the primes vector to have exactly n prime numbers
        primes.resize(n);

        // Measure the time taken to add prime numbers to the AVL tree
        auto start = high_resolution_clock::now();
        for (int val : primes) {
            bst_perf.add(val);
        }
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);

        cout << "Time taken to add " << n << " prime numbers: "
            << duration.count() << " milliseconds." << endl;
        outfile << n << "," << duration.count() << endl;

        cout << "Completed performance analysis for n = " << n << endl;
    }
    outfile.close();  // Close the output file

    // Check if file was written successfully
    if (outfile.fail()) {
        cerr << "Error: Failed to write to output file '" << outputPath << "'" << endl;
        return 1;
    }

    cout << "Performance data written to file: " << outputPath << endl;

    return 0;
}
