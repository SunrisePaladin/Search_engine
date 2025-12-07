#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <random>
#include <ctime>
#include "Headers/ConverterJSON.h"
#include "Headers/InvertedIndex.h"
#include "Headers/SearchServer.h"
#include "windows.h"

namespace fs = std::filesystem;

//extern void TestWord(InvertedIndex& index, const std::string& word);
extern void autotest();

//функция для форматирования текста
void PrintIndex(const std::map<std::string, std::vector<Entry>>& index) {
    std::cout << "\n--- Inverted Index Content ---" << std::endl;
    for (const auto& pair : index) {
        std::cout << "index[\"" << pair.first << "\"] = ";

        for (size_t i = 0; i < pair.second.size(); ++i) {
            // Печатаем {doc_id, count}
            std::cout << "{" << pair.second[i].doc_id << ", "
                      << pair.second[i].count << "}";

            // Добавляем запятую, если это не последний элемент
            if (i < pair.second.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "------------------------------" << std::endl;
}


void setup_test_environment() {
    fs::path db_path("../Database/"), json_path("../Json_documents/");

    // 1. Создадим директорию 'tmp_database', если нет источника для поиска
    if (!fs::exists(db_path)) {
        fs::create_directory("../Database");

        // Создадим файлы в tmp_database
        std::ofstream(db_path.string()+"file001.txt") << "это первый тестовый файл";
        std::ofstream(db_path.string()+"file002.txt") << "это второй тестовый файл";
    }

    if (!fs::exists(json_path)) fs::create_directory(json_path);

    // 2. Создадим пример config.json, если его ещё нет. Иначе открываем существующий
    if (!fs::exists(json_path.string()+"/config.json")) {
        json config_content = {
            {"config", {
                {"name", "SkillboxSearchEngine"},
                {"version", "1.0"},
                {"max_responses", 10}
            }},
            {"files", {
                db_path.string()+"file001.txt",
                db_path.string()+"file002.txt",
                db_path.string()+"file003.txt" // Несуществующий файл
            }}
        };
        std::ofstream(json_path.string()+"config.json") << std::setw(4) << config_content;
    }
    else {
        std::cout << "Файл конфигурации найден!" << std::endl;
    }

    // 3. Аналогично с конфигурацией создадим пример requests.json
    if (!fs::exists(json_path.string()+"requests.json")) {
        json requests_content = {
            {"requests", {
                "первый тестовый",
                "второй файл",
                "невозможный запрос"
            }}
        };
        std::ofstream(json_path.string()+"requests.json") << std::setw(4) << requests_content;
    }
    else {
        std::cout << "Файл запросов найден!" << std::endl;
    }
}

int main() {
    setlocale(LC_ALL, "russian"); // Clion не дружит с локалями, так что вывод в основном на английском
    srand(unsigned(time(NULL)));
    system("chcp 1251");
    SetConsoleOutputCP(CP_UTF8);

    setup_test_environment();
    std::cout << "\n--- Запуск теста ConverterJSON ---" << std::endl;

    try {
        // 1. Инициализация и загрузка конфигурации
        ConverterJSON converter("../Json_documents/", "../Database/"); //Стандартные пути
        std::vector<std::string> docs_content = converter.GetTextDocuments();
        std::vector<std::string> requests = converter.GetRequests();

        // 2. Индексация документов
        InvertedIndex index;
        index.UpdateDocumentBase(docs_content); // Запустит многопоточную индексацию

        // 3. Создание SearchServer
        std::cout << "\n--- ПОИСК ЗАПРОСОВ ---" << std::endl;
        SearchServer server(index);

        // 4. Запуск поиска
        std::vector<std::vector<RelativeIndex>> answers = server.search(requests);

        // 5. Запись результатов в answers.json
        converter.putAnswers(answers);

    } catch (const std::exception& e) {
        std::cerr << "\n--- КРИТИЧЕСКАЯ ОШИБКА ---" << std::endl;
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }

    autotest();

    system("pause");
    return 0;
}