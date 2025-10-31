#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <locale>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// Функция для установки русской локали
void setRussianLocale() {
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#else
    setlocale(LC_ALL, "ru_RU.UTF-8");
#endif
}

// Перечисление для типов коммунальных услуг
enum class UtilityType {
    ELECTRICITY,
    WATER,
    GAS,
    HEATING,
    TRASH_COLLECTION
};

// Вспомогательные функции для проверки ввода
string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    return (start == string::npos) ? "" : str.substr(start, end - start + 1);
}

// Простая проверка имени - только буквы, без пробелов и цифр
bool isValidName(const string& str) {
    if (str.empty()) return false;

    // Проверяем каждый символ
    for (char c : str) {
        // Если это пробел, цифра или специальный символ - невалидно
        if (c == ' ' ||
            c == '0' || c == '1' || c == '2' || c == '3' || c == '4' ||
            c == '5' || c == '6' || c == '7' || c == '8' || c == '9' ||
            c == '!' || c == '@' || c == '#' || c == '$' || c == '%' ||
            c == '^' || c == '&' || c == '*' || c == '(' || c == ')' ||
            c == '-' || c == '+' || c == '=' || c == '[' || c == ']' ||
            c == '{' || c == '}' || c == '|' || c == '\\' || c == '/' ||
            c == '<' || c == '>' || c == '?' || c == '.' || c == ',' ||
            c == ';' || c == ':' || c == '\'' || c == '\"') {
            return false;
        }
    }
    return true;
}

int inputInt(const string& prompt, int min = 0, int max = 100000) {
    int value;
    while (true) {
        cout << prompt;
        string input;
        getline(cin, input);
        input = trim(input);

        try {
            size_t pos;
            value = stoi(input, &pos);

            if (pos != input.length()) {
                cout << "Ошибка! Введите целое число без букв и символов." << endl;
                continue;
            }

            if (value < min || value > max) {
                cout << "Ошибка! Число должно быть от " << min << " до " << max << "." << endl;
                continue;
            }

            return value;

        }
        catch (const invalid_argument&) {
            cout << "Ошибка! Это не число. Введите целое число." << endl;
        }
        catch (const out_of_range&) {
            cout << "Ошибка! Число слишком большое." << endl;
        }
    }
}

double inputDouble(const string& prompt, double min = 0.01, double max = 10000.0) {
    double value;
    while (true) {
        cout << prompt;
        string input;
        getline(cin, input);
        input = trim(input);

        // Ограничиваем длину ввода чтобы избежать переполнения
        if (input.length() > 20) {
            cout << "Ошибка! Слишком длинное число." << endl;
            continue;
        }

        replace(input.begin(), input.end(), ',', '.');

        try {
            size_t pos;
            value = stod(input, &pos);

            if (pos != input.length()) {
                cout << "Ошибка! Введите число без лишних символов." << endl;
                continue;
            }

            if (value < min || value > max) {
                cout << "Ошибка! Число должно быть от " << min << " до " << max << "." << endl;
                continue;
            }

            if (min >= 0 && value < 0) {
                cout << "Ошибка! Число не может быть отрицательным." << endl;
                continue;
            }

            return value;

        }
        catch (const invalid_argument&) {
            cout << "Ошибка! Это не число. Введите число." << endl;
        }
        catch (const out_of_range&) {
            cout << "Ошибка! Число слишком большое." << endl;
        }
    }
}

string inputString(const string& prompt, bool checkName = false) {
    string value;
    while (true) {
        cout << prompt;
        getline(cin, value);
        value = trim(value);

        if (value.empty()) {
            cout << "Ошибка! Поле не может быть пустым." << endl;
            continue;
        }

        // Ограничение длины для всех полей
        if (value.length() > 100) {
            cout << "Ошибка! Слишком длинный ввод (максимум 100 символов)." << endl;
            continue;
        }

        if (checkName && !isValidName(value)) {
            cout << "Ошибка! Имя и фамилия должны содержать только буквы (без пробелов, цифр и специальных символов)." << endl;
            continue;
        }

        return value;
    }
}

// Класс для хранения информации о жильце
class Resident {
private:
    int id;
    string firstName;
    string lastName;
    string address;
    int apartmentNumber;

public:
    Resident(int id, const string& firstName, const string& lastName,
        const string& address, int apartmentNumber)
        : id(id), firstName(firstName), lastName(lastName),
        address(address), apartmentNumber(apartmentNumber) {}

    int getId() const { return id; }
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    string getAddress() const { return address; }
    int getApartmentNumber() const { return apartmentNumber; }

    void displayInfo() const {
        cout << "ID: " << id << ", " << firstName << " " << lastName
            << ", Адрес: " << address << ", Кв. " << apartmentNumber << endl;
    }
};

// Класс для хранения тарифов
class Tariff {
private:
    UtilityType utilityType;
    double rate;
    string unit;

public:
    Tariff(UtilityType type, double rate, const string& unit)
        : utilityType(type), rate(rate), unit(unit) {}

    UtilityType getUtilityType() const { return utilityType; }
    double getRate() const { return rate; }
    string getUnit() const { return unit; }

    string getUtilityTypeName() const {
        switch (utilityType) {
        case UtilityType::ELECTRICITY: return "Электричество";
        case UtilityType::WATER: return "Вода";
        case UtilityType::GAS: return "Газ";
        case UtilityType::HEATING: return "Отопление";
        case UtilityType::TRASH_COLLECTION: return "Вывоз мусора";
        default: return "Неизвестно";
        }
    }

    void displayInfo() const {
        cout << getUtilityTypeName() << ": " << rate << " руб/" << unit << endl;
    }
};

// Класс для хранения информации о потреблении услуг
class Consumption {
private:
    shared_ptr<Resident> resident;
    UtilityType utilityType;
    double amount;

public:
    Consumption(shared_ptr<Resident> resident, UtilityType type, double amount)
        : resident(resident), utilityType(type), amount(amount) {}

    shared_ptr<Resident> getResident() const { return resident; }
    UtilityType getUtilityType() const { return utilityType; }
    double getAmount() const { return amount; }

    void displayInfo() const {
        cout << resident->getLastName() << " " << resident->getFirstName();
        cout << ", Услуга: ";
        switch (utilityType) {
        case UtilityType::ELECTRICITY: cout << "Электричество"; break;
        case UtilityType::WATER: cout << "Вода"; break;
        case UtilityType::GAS: cout << "Газ"; break;
        case UtilityType::HEATING: cout << "Отопление"; break;
        case UtilityType::TRASH_COLLECTION: cout << "Вывоз мусора"; break;
        }
        cout << ", Количество: " << amount << endl;
    }
};

// Класс ЖЭС (Singleton)
class HousingOffice {
private:
    static HousingOffice* instance;

    vector<shared_ptr<Resident>> residents;
    vector<shared_ptr<Tariff>> tariffs;
    vector<shared_ptr<Consumption>> consumptions;

    int nextResidentId = 1;

    HousingOffice() = default;

public:
    HousingOffice(const HousingOffice&) = delete;
    HousingOffice& operator=(const HousingOffice&) = delete;

    static HousingOffice* getInstance() {
        if (instance == nullptr) {
            instance = new HousingOffice();
        }
        return instance;
    }

    ~HousingOffice() {
        cout << "Деструктор HousingOffice вызван. Очистка ресурсов..." << endl;
        residents.clear();
        tariffs.clear();
        consumptions.clear();
    }

    void addResident(const string& firstName, const string& lastName,
        const string& address, int apartmentNumber) {
        auto resident = make_shared<Resident>(nextResidentId++, firstName, lastName, address, apartmentNumber);
        residents.push_back(resident);
        cout << "Жилец добавлен: ";
        resident->displayInfo();
    }

    shared_ptr<Resident> findResidentByLastName(const string& lastName) {
        for (const auto& resident : residents) {
            if (resident->getLastName() == lastName) {
                return resident;
            }
        }
        return nullptr;
    }

    void addTariff(UtilityType type, double rate, const string& unit) {
        auto tariff = make_shared<Tariff>(type, rate, unit);
        tariffs.push_back(tariff);
        cout << "Тариф добавлен: ";
        tariff->displayInfo();
    }

    shared_ptr<Tariff> findTariffByType(UtilityType type) {
        for (const auto& tariff : tariffs) {
            if (tariff->getUtilityType() == type) {
                return tariff;
            }
        }
        return nullptr;
    }

    void addConsumption(const string& lastName, UtilityType type, double amount) {
        auto resident = findResidentByLastName(lastName);
        if (!resident) {
            cout << "Ошибка: Жилец с фамилией '" << lastName << "' не найден!" << endl;
            return;
        }

        auto consumption = make_shared<Consumption>(resident, type, amount);
        consumptions.push_back(consumption);
        cout << "Потребление добавлено: ";
        consumption->displayInfo();
    }

    double calculateResidentDebt(const string& lastName) {
        double totalDebt = 0.0;
        auto resident = findResidentByLastName(lastName);

        if (!resident) {
            cout << "Жилец с фамилией '" << lastName << "' не найден!" << endl;
            return 0.0;
        }

        cout << "Расчет долга для " << lastName << ":" << endl;

        for (const auto& consumption : consumptions) {
            if (consumption->getResident()->getLastName() == lastName) {
                auto tariff = findTariffByType(consumption->getUtilityType());
                if (tariff) {
                    double cost = consumption->getAmount() * tariff->getRate();
                    totalDebt += cost;

                    cout << "  " << tariff->getUtilityTypeName() << ": "
                        << consumption->getAmount() << " * " << tariff->getRate()
                        << " = " << cost << " руб." << endl;
                }
            }
        }

        return totalDebt;
    }

    double calculateTotalRevenue() {
        double totalRevenue = 0.0;

        for (const auto& consumption : consumptions) {
            auto tariff = findTariffByType(consumption->getUtilityType());
            if (tariff) {
                totalRevenue += consumption->getAmount() * tariff->getRate();
            }
        }

        return totalRevenue;
    }

    void displayAllResidents() {
        cout << "\n--- Список всех жильцов ---" << endl;
        if (residents.empty()) {
            cout << "Жильцы не найдены." << endl;
            return;
        }
        for (const auto& resident : residents) {
            resident->displayInfo();
        }
    }

    void displayAllTariffs() {
        cout << "\n--- Список всех тарифов ---" << endl;
        if (tariffs.empty()) {
            cout << "Тарифы не найдены." << endl;
            return;
        }
        for (const auto& tariff : tariffs) {
            tariff->displayInfo();
        }
    }

    void displayAllConsumptions() {
        cout << "\n--- Список всех потреблений ---" << endl;
        if (consumptions.empty()) {
            cout << "Потребления не найдены." << endl;
            return;
        }
        for (const auto& consumption : consumptions) {
            consumption->displayInfo();
        }
    }
};

HousingOffice* HousingOffice::instance = nullptr;

// Функции меню
UtilityType inputUtilityType() {
    cout << "Выберите тип услуги:" << endl;
    cout << "1 - Электричество" << endl;
    cout << "2 - Вода" << endl;
    cout << "3 - Газ" << endl;
    cout << "4 - Отопление" << endl;
    cout << "5 - Вывоз мусора" << endl;

    return static_cast<UtilityType>(inputInt("Ваш выбор (1-5): ", 1, 5) - 1);
}

void addResidentMenu(HousingOffice* office) {
    cout << "\n--- Добавление жильца ---" << endl;
    string firstName = inputString("Введите имя: ", true);
    string lastName = inputString("Введите фамилию: ", true);
    string address = inputString("Введите адрес: ");
    int apartment = inputInt("Введите номер квартиры: ", 1, 1000);

    office->addResident(firstName, lastName, address, apartment);
}

void addTariffMenu(HousingOffice* office) {
    cout << "\n--- Добавление тарифа ---" << endl;
    UtilityType type = inputUtilityType();
    double rate = inputDouble("Введите тариф (руб/ед.): ", 0.01, 1000.0);
    string unit = inputString("Введите единицу измерения: ");

    office->addTariff(type, rate, unit);
}

void addConsumptionMenu(HousingOffice* office) {
    cout << "\n--- Добавление потребления ---" << endl;
    string lastName = inputString("Введите фамилию жильца: ", true);
    UtilityType type = inputUtilityType();

    // Разные ограничения для разных типов услуг
    double maxAmount;
    switch (type) {
    case UtilityType::ELECTRICITY: maxAmount = 5000.0; break;
    case UtilityType::WATER: maxAmount = 1000.0; break;
    case UtilityType::GAS: maxAmount = 500.0; break;
    case UtilityType::HEATING: maxAmount = 100.0; break;
    case UtilityType::TRASH_COLLECTION: maxAmount = 50.0; break;
    default: maxAmount = 10000.0;
    }

    double amount = inputDouble("Введите количество потребления: ", 0.01, maxAmount);

    office->addConsumption(lastName, type, amount);
}

void calculateDebtMenu(HousingOffice* office) {
    cout << "\n--- Расчет долга жильца ---" << endl;
    string lastName = inputString("Введите фамилию жильца: ", true);

    double debt = office->calculateResidentDebt(lastName);
    cout << "Общий долг: " << fixed << setprecision(2) << debt << " руб." << endl;
}

void displayInfoMenu(HousingOffice* office) {
    int choice;
    do {
        cout << "\n--- Просмотр информации ---" << endl;
        cout << "1 - Показать всех жильцов" << endl;
        cout << "2 - Показать все тарифы" << endl;
        cout << "3 - Показать все потребления" << endl;
        cout << "4 - Рассчитать общую выручку" << endl;
        cout << "0 - Назад" << endl;

        choice = inputInt("Ваш выбор: ", 0, 4);

        switch (choice) {
        case 1:
            office->displayAllResidents();
            break;
        case 2:
            office->displayAllTariffs();
            break;
        case 3:
            office->displayAllConsumptions();
            break;
        case 4:
            cout << "Общая стоимость всех оказанных услуг: "
                << fixed << setprecision(2)
                << office->calculateTotalRevenue() << " руб." << endl;
            break;
        case 0:
            break;
        }
    } while (choice != 0);
}

void mainMenu() {
    HousingOffice* office = HousingOffice::getInstance();
    int choice;

    do {
        cout << "\n=== СИСТЕМА ЖЭС ===" << endl;
        cout << "1 - Добавить жильца" << endl;
        cout << "2 - Добавить тариф" << endl;
        cout << "3 - Добавить потребление услуги" << endl;
        cout << "4 - Рассчитать долг жильца" << endl;
        cout << "5 - Просмотр информации" << endl;
        cout << "0 - Выход" << endl;

        choice = inputInt("Ваш выбор: ", 0, 5);

        switch (choice) {
        case 1:
            addResidentMenu(office);
            break;
        case 2:
            addTariffMenu(office);
            break;
        case 3:
            addConsumptionMenu(office);
            break;
        case 4:
            calculateDebtMenu(office);
            break;
        case 5:
            displayInfoMenu(office);
            break;
        case 0:
            cout << "Выход из программы..." << endl;
            break;
        }
    } while (choice != 0);
}

int main() {
    setRussianLocale();

    cout << "Лабораторная работа: Система ЖЭС" << endl;
    cout << "Реализация на C++ с шаблоном Singleton" << endl;

    mainMenu();

    delete HousingOffice::getInstance();

    return 0;
}