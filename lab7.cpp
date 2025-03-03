#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <locale>
#include <algorithm>

struct date {
    int day;
    char month[10];
    int year;
};

struct student {
    std::string fio;
    date birthDate;
    std::string specialization; // Направление подготовки
    std::string group;          // Номер группы
};

void inputStudentData(std::vector<student>& students) {
    student newStudent;
    std::cout << "Введите ФИО студента: ";
    std::getline(std::cin, newStudent.fio);

    std::cout << "Введите дату рождения (день, месяц, год): ";
    std::cin >> newStudent.birthDate.day >> newStudent.birthDate.month >> newStudent.birthDate.year;
    std::cin.ignore(); // Очистка буфера

    std::cout << "Введите направление подготовки: ";
    std::getline(std::cin, newStudent.specialization);

    std::cout << "Введите номер группы: ";
    std::getline(std::cin, newStudent.group);

    students.push_back(newStudent);
}

void displayStudents(const std::vector<student>& students) {
    for (const auto& s : students) {
        std::cout << "ФИО: " << s.fio
            << ", Дата рождения: " << s.birthDate.day << " " << s.birthDate.month << " " << s.birthDate.year
            << ", Направление подготовки: " << s.specialization
            << ", Номер группы: " << s.group << std::endl;
    }
}

void saveToFile(const std::vector<student>& students) {
    std::ofstream ofs("students.txt");
    for (const auto& s : students) {
        ofs << s.fio << ';'
            << s.birthDate.day << ';'
            << s.birthDate.month << ';'
            << s.birthDate.year << ';'
            << s.specialization << ';'
            << s.group << '\n';
    }
}

void loadFromFile(std::vector<student>& students) {
    std::ifstream ifs("students.txt");
    if (!ifs) {
        std::cerr << "Ошибка загрузки данных из файла!" << std::endl;
        return;
    }

    student newStudent;
    while (ifs.good()) {
        std::string line;
        std::getline(ifs, line);
        if (line.empty()) continue;

        size_t pos = 0;
        pos = line.find(';');
        newStudent.fio = line.substr(0, pos);
        line.erase(0, pos + 1);

        newStudent.birthDate.day = std::stoi(line.substr(0, line.find(';')));
        line.erase(0, line.find(';') + 1);

        pos = line.find(';');
        strcpy_s(newStudent.birthDate.month, sizeof(newStudent.birthDate.month), line.substr(0, pos).c_str());
        line.erase(0, pos + 1);

        newStudent.birthDate.year = std::stoi(line.substr(0, line.find(';')));
        line.erase(0, line.find(';') + 1);

        newStudent.specialization = line.substr(0, line.find(';')); // Направление подготовки
        line.erase(0, line.find(';') + 1);

        newStudent.group = line; // Номер группы
        students.push_back(newStudent);
    }
}

void searchBySpecialization(const std::vector<student>& students, const std::string& specialization) {
    bool found = false;
    for (const auto& s : students) {
        if (s.specialization == specialization) {
            std::cout << "ФИО: " << s.fio
                << ", Дата рождения: " << s.birthDate.day << " " << s.birthDate.month << " " << s.birthDate.year
                << ", Направление подготовки: " << s.specialization
                << ", Номер группы: " << s.group << std::endl;
            found = true;
        }
    }
    if (!found) {
        std::cout << "Студенты с направлением подготовки '" << specialization << "' не найдены." << std::endl;
    }
}


void searchByGroup(const std::vector<student>& students, const std::string& specialization, const std::string& group) {
    std::string formattedSpecialization = specialization;
    std::transform(formattedSpecialization.begin(), formattedSpecialization.end(), formattedSpecialization.begin(), ::tolower); // Приводим к нижнему регистру

    std::string formattedGroup = group;
    std::transform(formattedGroup.begin(), formattedGroup.end(), formattedGroup.begin(), ::tolower); // Приводим к нижнему регистру

    bool found = false;
    for (const auto& s : students) {
        std::string studentSpecialization = s.specialization;
        std::transform(studentSpecialization.begin(), studentSpecialization.end(), studentSpecialization.begin(), ::tolower); // Приводим к нижнему регистру

        std::string studentGroup = s.group;
        std::transform(studentGroup.begin(), studentGroup.end(), studentGroup.begin(), ::tolower); // Приводим к нижнему регистру

        if (studentSpecialization == formattedSpecialization && studentGroup == formattedGroup) {
            std::cout << "ФИО: " << s.fio
                << ", Дата рождения: " << s.birthDate.day << " " << s.birthDate.month << " " << s.birthDate.year
                << ", Направление подготовки: " << s.specialization
                << ", Номер группы: " << s.group << std::endl;
            found = true;
        }
    }
    if (!found) {
        std::cout << "Студенты с направлением '" << specialization << "' и номером группы '" << group << "' не найдены." << std::endl;
    }
}
bool isOlder(const student& s1, const student& s2) {
    if (s1.birthDate.year != s2.birthDate.year) {
        return s1.birthDate.year < s2.birthDate.year;
    }
    if (s1.birthDate.month != s2.birthDate.month) {
        return s1.birthDate.month < s2.birthDate.month;
    }
    return s1.birthDate.day < s2.birthDate.day;
}
student findOldestStudent(const std::vector<student>& students) {
    student oldest = students[0];
    for (const auto& student : students) {
        if (isOlder(student, oldest)) {
            oldest = student;
        }
    }
    return oldest;
}

student findYoungestStudent(const std::vector<student>& students) {
    student youngest = students[0];
    for (const auto& student : students) {
        if (!isOlder(student, youngest)) {
            youngest = student;
        }
    }
    return youngest;
}

int main() {
    setlocale(LC_ALL, "Russian");

    std::vector<student> students;
    char choice;

    loadFromFile(students);

    do {
        std::cout << "1. Ввести новые данные\n2. Показать данные\n3. Сохранить данные\n4. Поиск по направлению подготовки\n5. Поиск самого младшего студента\n6. Поиск самого старшего студента\n7. Поиск по номеру группы\n8. Выход\n";
        std::cin >> choice;
        std::cin.ignore(); // Очистка буфера

        switch (choice) {
        case '1':
            inputStudentData(students);
            break;
        case '2':
            displayStudents(students);
            break;
        case '3':
            saveToFile(students);
            break;
        case '4': {
            std::string specialization;
            std::cout << "Введите направление подготовки для поиска: ";
            std::getline(std::cin, specialization);
            searchBySpecialization(students, specialization);
            break;
        }
        case '5': {
            student youngest = findYoungestStudent(students);
            std::cout << "Самый младший студент: " << youngest.fio
                << ", Дата рождения: " << youngest.birthDate.day << "."
                << youngest.birthDate.month << "." << youngest.birthDate.year << std::endl;
            break;
        }
        case '6': {
            student oldest = findOldestStudent(students);
            std::cout << "Самый старший студент: " << oldest.fio
                << ", Дата рождения: " << oldest.birthDate.day << "."
                << oldest.birthDate.month << "." << oldest.birthDate.year << std::endl;
            break;
        }
        case '7': {
            std::string group;
            std::string specialization;
            std::cout << "Введите группу для поиска:\n";
            std::getline(std::cin, specialization);
            std::getline(std::cin, group);
            searchByGroup(students, specialization, group);
            break;
        }
        }
    } while (choice != '8');

    return 0;
}