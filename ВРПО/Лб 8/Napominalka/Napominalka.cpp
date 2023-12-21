#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <string>

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

private:
    vector<Reminder> reminders;
};

int main() {
    ReminderManager manager;

    // Пример использования:
    manager.loadFromFile("reminders.txt");

    int choice;
    do {
        cout << "1. Add Reminder\n";
        cout << "2. Edit Reminder\n";
        cout << "3. Delete Reminder\n";
        cout << "4. Show Reminders\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            Reminder reminder;
            cout << "Enter date and time (YYYY-MM-DD HH:MM): ";
            cin >> get_time(&reminder.datetime, "%Y-%m-%d %H:%M");
            cout << "Enter reminder text: ";
            cin.ignore();
            getline(cin, reminder.text);
            manager.addReminder(reminder);
            break;
        }
        case 2: {
            int index;
            cout << "Enter index of reminder to edit: ";
            cin >> index;
            Reminder reminder;
            cout << "Enter date and time (YYYY-MM-DD HH:MM): ";
            cin >> get_time(&reminder.datetime, "%Y-%m-%d %H:%M");
            cout << "Enter new reminder text: ";
            cin.ignore();
            getline(cin, reminder.text);
            manager.editReminder(index - 1, reminder);
            break;
        }
        case 3: {
            int index;
            cout << "Enter index of reminder to delete: ";
            cin >> index;
            manager.deleteReminder(index - 1);
            break;
        }
        case 4:
            manager.showReminders();
            break;
        }
    } while (choice != 0);

    manager.saveToFile("reminders.txt");

    return 0;
}
