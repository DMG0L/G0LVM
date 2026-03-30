#include <iostream>
#include <fstream>
#include <string>

// Коды операций (должны совпадать с вашей виртуальной машиной)
enum instructions {
    MOV = 0x01,
    ADD = 0x02,
    PUT = 0x03,
    HLT = 0x04,
    SUB = 0x05,
};

// Массив для хранения скомпилированной программы
int prog[64];

int main(int argc, char* argv[]) {
    // Проверяем, что переданы имена входного и выходного файлов
    if (argc != 3) {
        std::cout << "Использование: " << argv[0] << " <входной_файл.txt> <выходной_файл.gpg>\n";
        return 1;
    }

    std::string inputFileName = argv[1];
    std::string outputFileName = argv[2];

    std::ifstream infile(inputFileName);
    if (!infile) {
        std::cout << "Ошибка: не удалось открыть файл " << inputFileName << "\n";
        return 1;
    }

    int ip = 0; // Instruction Pointer (индекс в массиве prog)
    std::string line;

    // Читаем файл построчно
    while (std::getline(infile, line) && ip < 64) {
        // Удаляем все пробелы из строки для простоты обработки
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());

        // Пропускаем пустые строки
        if (line.empty()) continue;

        // Находим первую запятую
        size_t commaPos = line.find(',');
        if (commaPos == std::string::npos) continue; // Пропускаем строки без запятых

        // Извлекаем название команды (все символы до первой запятой)
        std::string cmd = line.substr(0, commaPos);

        int opcode = -1;
        // Преобразуем текст команды в код операции
        if (cmd == "MOV") opcode = MOV;
        else if (cmd == "ADD") opcode = ADD;
        else if (cmd == "PUT") opcode = PUT;
        else if (cmd == "HLT") opcode = HLT;
        else if (cmd == "SUB") opcode = SUB;

        // Если команда неизвестна или нет места в массиве - выходим
        if (opcode == -1 || ip >= 63) break;

        prog[ip++] = opcode;

        // Обрабатываем аргументы после первой запятой
        std::string argsPart = line.substr(commaPos + 1);

        // Разделяем аргументы по запятым
        size_t start = 0;
        size_t end = argsPart.find(',');

        while (end != std::string::npos && ip < 63) {
            std::string argStr = argsPart.substr(start, end - start);
            if (!argStr.empty()) {
                try {
                    int argValue = std::stoi(argStr);
                    prog[ip++] = argValue;
                }
                catch (...) {
                    // Если аргумент не число - просто пропускаем его
                }
            }
            start = end + 1;
            end = argsPart.find(',', start);
        }

        // Обрабатываем последний аргумент (после последней запятой)
        std::string lastArg = argsPart.substr(start);
        if (!lastArg.empty() && ip < 63) {
            try {
                int argValue = std::stoi(lastArg);
                prog[ip++] = argValue;
            }
            catch (...) {}
        }
    }

    infile.close();

    // Записываем скомпилированную программу в бинарный файл
    std::ofstream outfile(outputFileName, std::ios::binary);
    if (!outfile) {
        std::cout << "Ошибка: не удалось создать файл " << outputFileName << "\n";
        return 1;
    }

    outfile.write(reinterpret_cast<char*>(prog), sizeof(prog));
    outfile.close();

    std::cout << "Компиляция завершена. Записано " << ip << " инструкций.\n";

    return 0;
}