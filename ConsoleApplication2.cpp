/****************************************
* Имя:     Мусаев Артём                 *
* Задание: Работа с файлами             *
* Вариант: 3                            *
****************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

struct Book {
    string author;
    string title;
    int year;
    int count;
    double price;
};

vector<string> splitFields(const string& line) {
    vector<string> fields;
    size_t start = 0;
    while (true) {
        size_t separator = line.find(';', start);
        if (separator == string::npos) {
            fields.push_back(line.substr(start));
            break;
        }
        fields.push_back(line.substr(start, separator - start));
        start = separator + 1;
    }
    return fields;
}

bool readInt(const string& text, int& value) {
    istringstream parser(text);
    char extra;
    if (!(parser >> value)) {
        return false;
    }
    return !(parser >> extra);
}

bool readDouble(const string& text, double& value) {
    istringstream parser(text);
    char extra;
    if (!(parser >> value) || !isfinite(value)) {
        return false;
    }
    return !(parser >> extra);
}

bool parseBook(const string& line, Book& book) {
    vector<string> fields = splitFields(line);
    if (fields.size() != 5) {
        return false;
    }
    if (fields[0].empty() || fields[1].empty()) {
        return false;
    }
    book.author = fields[0];
    book.title = fields[1];
    if (!readInt(fields[2], book.year)) {
        return false;
    }
    if (!readInt(fields[3], book.count)) {
        return false;
    }
    if (!readDouble(fields[4], book.price)) {
        return false;
    }
    return true;
}

bool readBooks(const string& path, vector<Book>& books) {
    ifstream input(path);
    if (!input) {
        cout << "ERROR open\n";
        return false;
    }

    string line;
    int lineNumber = 0;
    while (getline(input, line)) {
        ++lineNumber;
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty()) {
            cout << "ERROR line " << lineNumber << "\n";
            return false;
        }
        Book book;
        if (!parseBook(line, book)) {
            cout << "ERROR line " << lineNumber << "\n";
            return false;
        }
        if (books.size() >= 100) {
            cout << "ERROR line " << lineNumber << "\n";
            return false;
        }
        books.push_back(book);
    }

    if (input.bad()) {
        cout << "ERROR read\n";
        return false;
    }
    return true;
}

void makeReport(const vector<Book>& books, const string& author,
    ostringstream& report) {
    double total = 0.0;
    int matched = 0;

    report << "Books by " << author << ":\n";
    for (int i = 0; i < static_cast<int>(books.size()); ++i) {
        if (books[i].author == author) {
            report << books[i].title << " (" << books[i].year << "), "
                << books[i].count << " pcs, " << books[i].price << "\n";
            total += books[i].count * books[i].price;
            ++matched;
        }
    }
    if (matched == 0) {
        report << "EMPTY\n";
    }
    report << "Total: " << total << "\n";

    if (books.empty()) {
        report << "Oldest: NONE\n";
    }
    else {
        int oldestIndex = 0;
        for (int i = 1; i < static_cast<int>(books.size()); ++i) {
            if (books[i].year < books[oldestIndex].year) {
                oldestIndex = i;
            }
        }
        report << "Oldest: " << books[oldestIndex].author << " - "
            << books[oldestIndex].title << " (" << books[oldestIndex].year
            << ")\n";
    }
}

bool writeReport(const string& path, const string& report) {
    ofstream output(path);
    if (!output) {
        return false;
    }
    output << report;
    output.flush();
    return static_cast<bool>(output);
}

int main() {
    string inputPath;
    string outputPath;
    string author;

    cin >> inputPath >> outputPath;
    getline(cin, author);
    if (!author.empty() && author[0] == ' ') {
        author.erase(0, 1);
    }

    vector<Book> books;
    if (!readBooks(inputPath, books)) {
        return 1;
    }

    ostringstream report;
    makeReport(books, author, report);

    if (!writeReport(outputPath, report.str())) {
        cout << "ERROR write\n";
        return 1;
    }

    cout << report.str();
    return 0;
}