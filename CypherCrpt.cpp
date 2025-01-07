#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <sstream>

using namespace std;

char* encrypt(const char* input, uint32_t key) {
    int len = strlen(input);
    char* output = new char[len + 1];

    for (int i = 0; i < len; ++i) {
        output[i] = input[i] ^ key;
    }

    output[len] = '\0';
    return output;
}

char* doubleEncrypt(const char* input, uint32_t firstKey, uint32_t secondKey) {
    char* firstLayer = encrypt(input, firstKey);
    char* secondLayer = encrypt(firstLayer, secondKey);
    delete[] firstLayer;
    return secondLayer;
}

vector<char*> readLinesFromFile(const char* filePath) {
    ifstream file(filePath);
    vector<char*> lines;

    if (!file.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return lines;
    }

    const size_t BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    while (file.getline(buffer, BUFFER_SIZE)) {
        char* line = new char[BUFFER_SIZE + 1];
        strcpy_s(line, BUFFER_SIZE, buffer);
        lines.push_back(line);
    }

    file.close();
    return lines;
}

void freeLines(vector<char*>& lines) {
    for (char* line : lines) {
        delete[] line;
    }
    lines.clear();
}

void writeLinesToFile(const char* filePath, const vector<char*>& lines) {
    ofstream file(filePath);

    if (!file.is_open()) {
        cerr << "Error: Could not open the file for writing!" << endl;
        return;
    }

    for (const char* line : lines) {
        file << line << endl;
    }

    file.close();
}

int main(int argc, char* argv[]) {
    if (argc == 5) {
        try {
            uint32_t firstKey = static_cast<uint32_t>(stoul(argv[3]));
            uint32_t secondKey = static_cast<uint32_t>(stoul(argv[4]));

            vector<char*> file = readLinesFromFile(argv[1]);
            vector<char*> output;

            for (const char* line : file) {
                output.push_back(doubleEncrypt(line, firstKey, secondKey));
            }

            writeLinesToFile(argv[2], output);

            // Clean up memory
            freeLines(file);
            freeLines(output);

        }
        catch (const invalid_argument& e) {
            cerr << "Error: Invalid key provided. Please provide numeric values." << endl;
        }
        catch (const out_of_range& e) {
            cerr << "Error: Key value out of range." << endl;
        }
    }
    else {
        cout << "Usage: program.exe <input_file> <output_file> <first_key> <second_key>" << endl;
    }

    return 0;
}
