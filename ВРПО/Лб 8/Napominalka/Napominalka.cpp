#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <string>
#include <windows.h>

using namespace std;

struct Reminder {
    string text;
    tm datetime;
};

class ReminderManager {
public:
    void addReminder(const Reminder& reminder) {
        reminders.push_back(reminder);
    }

    void editReminder(int index, const Reminder& reminder) {
        if (index >= 0 && index < reminders.size()) {
            reminders[index] = reminder;
        }
    }

    void deleteReminder(int index) {
        if (index >= 0 && index < reminders.size()) {
            reminders.erase(reminders.begin() + index);
        }
    }

    void showReminders() const {
        for (size_t i = 0; i < reminders.size(); ++i) {
            cout << "[" << i + 1 << "] " << "Date: " << put_time(&reminders[i].datetime, "%Y-%m-%d %H:%M") << " - " << reminders[i].text << endl;
        }

        if (reminders.size() == 0) { cout << "У вас ещё нет ни одного напоминания.\n"; }

    }

    void saveToFile(const string& filename) const {
        ofstream file(filename);
        for (const auto& reminder : reminders) {
            file << put_time(&reminder.datetime, "%Y-%m-%d %H:%M") << " " << reminder.text << endl;
        }
    }

    void loadFromFile(const string& filename) {
        reminders.clear();
        ifstream file(filename);
        Reminder reminder;
        while (file >> get_time(&reminder.datetime, "%Y-%m-%d %H:%M")) {
            getline(file >> ws, reminder.text);
            reminders.push_back(reminder);
        }
    }

    size_t getRemindersCount() const {
        return reminders.size();
    }

    const vector<Reminder>& getReminders() const {
        return reminders;
    }

private:
    vector<Reminder> reminders;
};

int main() {
    setlocale(LC_ALL, "Rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    ReminderManager manager;

    manager.loadFromFile("reminders.txt");

    int choice;
    do {
        cout << "1. Добавить напоминание\n";
        cout << "2. Редактировать напоминание\n";
        cout << "3. Удалить напоминание\n";
        cout << "4. Показать все напоминания\n";
        cout << "0. Выход\n";
        cout << "Введите свой выбор: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            Reminder reminder;
            cout << "Введите дату и время (ГГГГ-ММ-ДД ЧЧ:ММ): ";
            if (!(cin >> get_time(&reminder.datetime, "%Y-%m-%d %H:%M"))) {
                cout << "Неверный ввод. Введите дату и время в правильном формате." << endl;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                break;
            }
            cout << "Введите содержимое напоминания: ";
            cin.ignore();
            getline(cin, reminder.text);
            manager.addReminder(reminder);
            break;
        }
        case 2: {
            int index;
            cout << "Выберите индекс напоминания для изменения: ";
            cin >> index;

            if (index < 1 || index > manager.getReminders().size()) {
                cout << "Неверный индекс напоминания или такого напоминания не существует. Пожалуйста, выберите корректный индекс." << endl;
                break;
            }

            Reminder reminder;
            cout << "Введите дату и время (ГГГГ-ММ-ДД ЧЧ:ММ): ";
            if (!(cin >> get_time(&reminder.datetime, "%Y-%m-%d %H:%M"))) {
                cout << "Неверный ввод. Введите дату и время в правильном формате." << endl;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                break;
            }
            cout << "Введите новый текст напоминания: ";
            cin.ignore();
            getline(cin, reminder.text);
            manager.editReminder(index - 1, reminder);
            break;
        }
        case 3: {
            int index;
            cout << "Введите индекс для удаления напоминания: ";
            cin >> index;

            if (index < 1 || index > manager.getReminders().size()) {
                cout << "Неверный индекс напоминания или такого напоминания не существует. Пожалуйста, выберите корректный индекс." << endl;
                break;
            }

            manager.deleteReminder(index - 1);
            break;
        }
        case 4:
            manager.showReminders();
            break;

        case 0:
            cout << "Программа завершена." << endl;
            break; 

        default:
                cout << "Неправильный выбор. Пожалуйста, выберите существующий вариант." << endl;
    }


    } while (choice != 0);

    manager.saveToFile("reminders.txt");

    return 0;
}
