#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <random>
#include <ctime>
#include <cassert>
//#include <gtest/gtest.h>

#include "../Headers/InvertedIndex.h"
#include "../Headers/SearchServer.h"

struct RelativeIndex;
using namespace std;

void TestWord(InvertedIndex& index, const std::string& word) {
    std::cout << "Testing word: \"" << word << "\"" << std::endl;
    std::vector<Entry> entries = index.GetWordCount(word);

    if (entries.empty()) {
        std::cout << "  Word not found." << std::endl;
        return;
    }

    std::cout << "  Entries: ";
    for (size_t i = 0; i < entries.size(); ++i) {
        std::cout << "{doc_id: " << entries[i].doc_id
                  << ", count: " << entries[i].count << "}";
        if (i < entries.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
}