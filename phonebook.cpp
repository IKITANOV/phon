# phon
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <clocale> 
#include <windows.h> 
using namespace std;


struct Contact {
    char name[100];
    char phone[100];
    char email[100];
};

// проты функций
void initPhoneBook(Contact*** phoneBook, int* capacity, int* size);
void freePhoneBook(Contact*** phoneBook, int size);
void addContact(Contact*** phoneBook, int* capacity, int* size);
void deleteContact(Contact*** phoneBook, int* size);
void searchContact(Contact** phoneBook, int size);
void editContact(Contact*** phoneBook, int size);
void displayAll(Contact** phoneBook, int size);
void saveToFile(Contact** phoneBook, int size);
void loadFromFile(Contact*** phoneBook, int* capacity, int* size);
void displayMenu();

int main() {
    
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "Russian");

    Contact** phoneBook = nullptr; // динам массив контактов
    int capacity = 0;             // место массива
    int size = 0;                 // кол контактов

    // Инициализация и загрузка телефонной книги
    initPhoneBook(&phoneBook, &capacity, &size);
    loadFromFile(&phoneBook, &capacity, &size);

    int choice = 0;
    while (true) {
        displayMenu(); //  менюгка
        if (!(cin >> choice)) {
            cout << "Некорректный ввод! Введите число\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore(10000, '\n'); // очистка буфера

        switch (choice) {
        case 1:
            addContact(&phoneBook, &capacity, &size); // добавить контакт
            break;
        case 2:
            deleteContact(&phoneBook, &size); // удалить контакт
            break;
        case 3:
            searchContact(phoneBook, size); // поиск контакта
            break;
        case 4:
            editContact(&phoneBook, size); // редактировать контакт
            break;
        case 5:
            displayAll(phoneBook, size); // показать все контакты
            break;
        case 6:
            cout << "Выход...\n"; // выход
            freePhoneBook(&phoneBook, size);
            return 0;
        default:
            cout << "Неверный выбор! Попробуйте снова\n";
        }
    }

    return 0; // невозможно
}

// иницилиазция телефонной книги
void initPhoneBook(Contact*** phoneBook, int* capacity, int* size) {
    *capacity = 10; //  вместимость
    *size = 0;      // начальный размер
    *phoneBook = new Contact * [*capacity];
    for (int i = 0; i < *capacity; i++) {
        (*phoneBook)[i] = new Contact;
        (*phoneBook)[i]->name[0] = '\0';
        (*phoneBook)[i]->phone[0] = '\0';
        (*phoneBook)[i]->email[0] = '\0';
    }
}

// свобождение сторейджа
void freePhoneBook(Contact*** phoneBook, int size) {
    if (*phoneBook == nullptr) return;
    for (int i = 0; i < size; i++) {
        delete (*phoneBook)[i]; 
    }
    delete[] * phoneBook; а
		* phoneBook = nullptr; // предотвращение утечек памяти
}

// добавление контакта
void addContact(Contact*** phoneBook, int* capacity, int* size) {
    if (*size >= *capacity) {
        // увеличение массива
        int newCapacity = *capacity * 2;
        Contact** temp = new Contact * [newCapacity];
        for (int i = 0; i < newCapacity; i++) {
            temp[i] = new Contact;
            temp[i]->name[0] = '\0';
            temp[i]->phone[0] = '\0';
            temp[i]->email[0] = '\0';
            if (i < *size) {
                strcpy_s(temp[i]->name, sizeof(temp[i]->name), (*phoneBook)[i]->name);
                strcpy_s(temp[i]->phone, sizeof(temp[i]->phone), (*phoneBook)[i]->phone);
                strcpy_s(temp[i]->email, sizeof(temp[i]->email), (*phoneBook)[i]->email);
            }
        }
        freePhoneBook(phoneBook, *size);
        *phoneBook = temp;
        *capacity = newCapacity;
    }

    string name, phone, email;
    cout << "Введите имя: ";
    getline(cin, name);
    if (name.empty()) {
        cout << "Имя не может быть пустым!\n";
        return;
    }
    cout << "Введите телефон: ";
    getline(cin, phone);
    cout << "Введите email: ";
    getline(cin, email);

    // копирование в контакт с использованием безопасной функции
    strncpy_s((*phoneBook)[*size]->name, sizeof((*phoneBook)[*size]->name), name.c_str(), _TRUNCATE);
    strncpy_s((*phoneBook)[*size]->phone, sizeof((*phoneBook)[*size]->phone), phone.c_str(), _TRUNCATE);
    strncpy_s((*phoneBook)[*size]->email, sizeof((*phoneBook)[*size]->email), email.c_str(), _TRUNCATE);
    (*phoneBook)[*size]->name[99] = '\0';
    (*phoneBook)[*size]->phone[99] = '\0';
    (*phoneBook)[*size]->email[99] = '\0';
    (*size)++;
    cout << "Контакт успешно добавлен!\n";
    saveToFile(*phoneBook, *size);
}

// удаление контакта
void deleteContact(Contact*** phoneBook, int* size) {
    string name;
    cout << "Введите имя для удаления: ";
    getline(cin, name);
    if (name.empty()) {
        cout << "Имя не может быть пустым!\n";
        return;
    }

    for (int i = 0; i < *size; i++) {
        if (name == (*phoneBook)[i]->name) {
            // сдвиг контактов
            for (int j = i; j < *size - 1; j++) {
                strcpy_s((*phoneBook)[j]->name, sizeof((*phoneBook)[j]->name), (*phoneBook)[j + 1]->name);
                strcpy_s((*phoneBook)[j]->phone, sizeof((*phoneBook)[j]->phone), (*phoneBook)[j + 1]->phone);
                strcpy_s((*phoneBook)[j]->email, sizeof((*phoneBook)[j]->email), (*phoneBook)[j + 1]->email);
            }
            (*size)--;
            cout << "Контакт успешно удалён!\n";
            saveToFile(*phoneBook, *size);
            return;
        }
    }
    cout << "Контакт не найден!\n";
}

// поиск контакта
void searchContact(Contact** phoneBook, int size) {
    string query;
    cout << "Введите имя или телефон для поиска: ";
    getline(cin, query);
    if (query.empty()) {
        cout << "Поисковый запрос не может быть пустым!\n";
        return;
    }

    bool found = false;
    for (int i = 0; i < size; i++) {
        if (string(phoneBook[i]->name).find(query) != string::npos ||
            string(phoneBook[i]->phone).find(query) != string::npos) {
            cout << "Имя: " << phoneBook[i]->name << "\nТелефон: " << phoneBook[i]->phone
                << "\nEmail: " << phoneBook[i]->email << "\n\n";
            found = true;
        }
    }
    if (!found) cout << "Контакты не найдены!\n";
                                                                                                    // не получаеться осуществить поиск окнтактов так как они не сохраняться в тхт файле
}

// редактирование контакта
void editContact(Contact*** phoneBook, int size) {
    string name;
    cout << "Введите имя контакта для редактирования: ";
    getline(cin, name);
    if (name.empty()) {
        cout << "Имя не может быть пустым!\n";
        return;
    }

    for (int i = 0; i < size; i++) {
        if (name == (*phoneBook)[i]->name) {
            string phone, email;
            cout << "Введите новый телефон: ";
            getline(cin, phone);
            cout << "Введите новый email: ";
            getline(cin, email);
            strncpy_s((*phoneBook)[i]->phone, sizeof((*phoneBook)[i]->phone), phone.c_str(), _TRUNCATE);
            strncpy_s((*phoneBook)[i]->email, sizeof((*phoneBook)[i]->email), email.c_str(), _TRUNCATE);
            (*phoneBook)[i]->phone[99] = '\0';
            (*phoneBook)[i]->email[99] = '\0';
            cout << "Контакт успешно обновлён!\n";
            saveToFile(*phoneBook, size);
            return;
        }
    }
    cout << "Контакт не найден!\n";
}

// отображение всех контактов
void displayAll(Contact** phoneBook, int size) {
    if (size == 0) {
        cout << "Телефонная книга пуста!\n";
        return;
    }
    cout << "Все контакты:\n";
    for (int i = 0; i < size; i++) {
        cout << "Имя: " << phoneBook[i]->name << "\nТелефон: " << phoneBook[i]->phone
            << "\nEmail: " << phoneBook[i]->email << "\n\n";
    }
}

// сохранение контактов в файл
void saveToFile(Contact** phoneBook, int size) {
    string filePath = "C:\\Users\\nikit\\Dekstop\\phonebook.txt";
    ofstream outFile(filePath);
    if (!outFile.is_open()) {
        cout << "Ошибка открытия файла для записи: " << filePath << "\n";
        return;
    }
    for (int i = 0; i < size; i++) {
        outFile << phoneBook[i]->name << ";" << phoneBook[i]->phone << ";" << phoneBook[i]->email << "\n";
    }
    outFile.close();
    cout << "Контакты сохранены в файл: " << filePath << "\n";
}

// загрузка контактов из файла
void loadFromFile(Contact*** phoneBook, int* capacity, int* size) {
    string filePath = "C:\\Users\\nikit\\Dekstop\\phonebook.txt";
    ifstream inFile(filePath);
    if (!inFile.is_open()) {
        cout << "Файл телефонной книги не найден: " << filePath << ". Начинаем с нуля.\n";
        return;
    }

    string line;
    *size = 0;
    *capacity = 10;
    *phoneBook = new Contact * [*capacity];
    for (int i = 0; i < *capacity; i++) {
        (*phoneBook)[i] = new Contact;
        (*phoneBook)[i]->name[0] = '\0';
        (*phoneBook)[i]->phone[0] = '\0';
        (*phoneBook)[i]->email[0] = '\0';
    }

    while (getline(inFile, line)) {
        if (line.empty()) continue; // пропуск пустых строк
        if (*size >= *capacity) {
            int newCapacity = *capacity * 2;
            Contact** temp = new Contact * [newCapacity];
            for (int i = 0; i < newCapacity; i++) {
                temp[i] = new Contact;
                temp[i]->name[0] = '\0';
                temp[i]->phone[0] = '\0';
                temp[i]->email[0] = '\0';
                if (i < *size) {
                    strcpy_s(temp[i]->name, sizeof(temp[i]->name), (*phoneBook)[i]->name);
                    strcpy_s(temp[i]->phone, sizeof(temp[i]->phone), (*phoneBook)[i]->phone);
                    strcpy_s(temp[i]->email, sizeof(temp[i]->email), (*phoneBook)[i]->email);
                }
            }
            freePhoneBook(phoneBook, *size);
            *phoneBook = temp;
            *capacity = newCapacity;
        }

        size_t pos1 = line.find(';');
        size_t pos2 = line.find(';', pos1 + 1);
        if (pos1 != string::npos && pos2 != string::npos && pos1 < pos2) {
            string name = line.substr(0, pos1);
            string phone = line.substr(pos1 + 1, pos2 - pos1 - 1);
            string email = line.substr(pos2 + 1);
            if (!name.empty()) { // проверка, что имя не пустое
                strncpy_s((*phoneBook)[*size]->name, sizeof((*phoneBook)[*size]->name), name.c_str(), _TRUNCATE);
                strncpy_s((*phoneBook)[*size]->phone, sizeof((*phoneBook)[*size]->phone), phone.c_str(), _TRUNCATE);
                strncpy_s((*phoneBook)[*size]->email, sizeof((*phoneBook)[*size]->email), email.c_str(), _TRUNCATE);
                (*phoneBook)[*size]->name[99] = '\0';
                (*phoneBook)[*size]->phone[99] = '\0';
                (*phoneBook)[*size]->email[99] = '\0';
                (*size)++;
            }
        }
    }
    inFile.close();
    if (*size > 0) {
        cout << "Загружено " << *size << " контактов из файла: " << filePath << "\n";
    }
    else {
        cout << "В файле не найдено действительных контактов: " << filePath << "\n";
    }
}

// снова менюшка
void displayMenu() {
    cout << "\nМеню телефонной книги:\n";
    cout << "1. Добавить контакт\n";
    cout << "2. Удалить контакт\n";
    cout << "3. Поиск контакта\n";
    cout << "4. Редактировать контакт\n";
    cout << "5. Показать все контакты\n";
    cout << "6. Выход\n";
    cout << "Введите выбор: ";
}
