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

class RelativeIndex;
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

void autotest() {
    // 1. Создаем базу документов
    std::vector<std::string> document_texts = {
        "milk sugar salt",                     // doc_id = 0
        "milk a milk b milk c milk d",         // doc_id = 1
        "salt water and sugar"                 // doc_id = 2
    };

    // 2. Создаем экземпляр класса
    InvertedIndex index;

    // 3. Обновляем базу (это запустит многопоточную индексацию)
    index.UpdateDocumentBase(document_texts);

    // 4. Тестируем новый интерфейс GetWordCount
    std::cout << "\n--- Testing GetWordCount ---" << std::endl;

    TestWord(index, "milk");   // Ожидается {0, 1}, {1, 4}
    TestWord(index, "sugar");  // Ожидается {0, 1}, {2, 1}
    TestWord(index, "a");      // Ожидается {1, 1}
    TestWord(index, "water");  // Ожидается {2, 1}
    TestWord(index, "banana"); // Ожидается "Word not found"

    // 5. Тестируем оператор == (для GTest)
    Entry e1(0, 1);
    Entry e2(0, 1);
    Entry e3(1, 4);

    if (e1 == e2) {
        std::cout << "\n(Test OK: e1 == e2)" << std::endl;
    }
    if (!(e1 == e3)) {
        std::cout << "(Test OK: e1 != e3)" << std::endl;
    }
}