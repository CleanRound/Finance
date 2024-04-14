#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <string>
using namespace std;

// Перечисление категорий расходов
enum ExpenseCategory {
    FOOD,
    TRANSPORTATION,
    SHOPPING,
    ENTERTAINMENT,
    UTILITIES,
    OTHER
};

// Класс для управления счетом
class Account {
private:
    string name; // Имя счета
    double balance; // Баланс счета

public:
    Account(string name, double balance) : name(name), balance(balance) {}

    // Метод для внесения средств на счет
    void deposit(double amount) {
        balance += amount;
    }

    // Метод для снятия средств со счета
    void withdraw(double amount) {
        if (amount > balance) {
            cout << "Insufficient funds" << endl;
        }
        else {
            balance -= amount;
        }
    }

    // Метод для получения баланса счета
    double getBalance() const {
        return balance;
    }

    // Метод для получения имени счета
    string getName() const {
        return name;
    }
};

// Класс для представления расходов
class Expense {
private:
    double amount; // Сумма расхода
    ExpenseCategory category; // Категория расхода

public:
    Expense(double amount, ExpenseCategory category) : amount(amount), category(category) {}

    // Метод для получения суммы расхода
    double getAmount() const {
        return amount;
    }

    // Метод для получения категории расхода
    ExpenseCategory getCategory() const {
        return category;
    }

    // Перегрузка оператора < для сравнения расходов по сумме
    bool operator<(const Expense& other) const {
        return amount < other.amount;
    }
};

// Класс для управления финансами
class FinanceManager {
private:
    map<ExpenseCategory, double> categoryTotals; // Общие расходы по категориям
    vector<Expense> expenses; // Все расходы
    vector<Account> accounts; // Все счета

public:
    // Метод для добавления расхода
    void addExpense(Expense expense) {
        expenses.push_back(expense);
        categoryTotals[expense.getCategory()] += expense.getAmount();
    }

    // Метод для генерации отчета о расходах
    void generateReport() {
        cout << "----- Expense Report -----" << endl;
        cout << setw(20) << left << "Category" << setw(10) << "Amount" << endl;
        for (auto& entry : categoryTotals) {
            cout << setw(20) << left << getCategoryName(entry.first) << "$" << entry.second << endl;
        }
        cout << "--------------------------" << endl;
    }

    // Статический метод для сравнения расходов по сумме
    static bool compareExpenses(const Expense& e1, const Expense& e2) {
        return e1.getAmount() > e2.getAmount(); // Sort in descending order based on amount
    }

    // Метод для генерации топ 3 расходов
    void generateTop3Costs() {
        cout << "----- Top 3 Costs -----" << endl;
        // Копирование вектора расходов
        vector<Expense> sortedExpenses = expenses;
        // Сортировка расходов по убыванию суммы
        sort(sortedExpenses.begin(), sortedExpenses.end(), compareExpenses);
        int count = 0;
        // Вывод топ 3 расходов
        for (auto& expense : sortedExpenses) {
            cout << getCategoryName(expense.getCategory()) << ": $" << expense.getAmount() << endl;
            if (++count >= 3) break;
        }
        cout << "------------------------" << endl;
    }

    // Метод для генерации топ 3 категорий
    void generateTop3Categories() {
        cout << "----- Top 3 Categories -----" << endl;
        vector<pair<double, ExpenseCategory>> sortedExpenses;
        for (auto& entry : categoryTotals) {
            sortedExpenses.push_back({ entry.second, entry.first });
        }
        sort(sortedExpenses.rbegin(), sortedExpenses.rend()); // Сортировка по убыванию
        int count = 0;
        for (auto& entry : sortedExpenses) {
            cout << getCategoryName(entry.second) << ": $" << entry.first << endl;
            if (++count >= 3) break;
        }
        cout << "-----------------------------" << endl;
    }

    // Метод для сохранения отчета в файл
    void saveToFile(string filename) {
        ofstream file(filename + ".txt"); // Сохранение отчета в формате .txt
        if (file.is_open()) {
            // Сохранение отчета о расходах
            file << "----- Expense Report -----" << endl;
            file << setw(20) << left << "Category" << setw(10) << "Amount" << endl;
            for (auto& entry : categoryTotals) {
                file << setw(20) << left << getCategoryName(entry.first) << "$" << entry.second << endl;
            }
            file << "--------------------------" << endl;

            // Сохранение топ 3 категорий
            file << "----- Top 3 Categories -----" << endl;
            vector<pair<double, ExpenseCategory>> sortedExpenses;
            for (auto& entry : categoryTotals) {
                sortedExpenses.push_back({ entry.second, entry.first });
            }
            sort(sortedExpenses.rbegin(), sortedExpenses.rend()); // Сортировка по убыванию
            int count = 0;
            for (auto& entry : sortedExpenses) {
                file << getCategoryName(entry.second) << ": $" << entry.first << endl;
                if (++count >= 3) break;
            }
            file << "-----------------------------" << endl;

            // Сохранение топ 3 расходов
            file << "----- Top 3 Costs -----" << endl;
            // Создание вектора для всех расходов
            vector<pair<double, Expense>> allExpenses;
            for (auto& expense : expenses) {
                allExpenses.push_back({ expense.getAmount(), expense });
            }
            // Сортировка расходов по убыванию суммы
            sort(allExpenses.rbegin(), allExpenses.rend());
            count = 0;
            // Вывод топ 3 расходов
            for (auto& entry : allExpenses) {
                file << getCategoryName(entry.second.getCategory()) << ": $" << entry.first << endl;
                if (++count >= 3) break;
            }
            file << "------------------------" << endl;

            // Сохранение баланса всех карт
            file << "----- Card Balances -----" << endl;
            for (auto& account : accounts) {
                file << account.getName() << ": $" << account.getBalance() << endl;
            }
            file << "--------------------------" << endl;

            file.close();
            cout << "Report saved to " << filename << ".txt" << endl;
        }
        else {
            cout << "Unable to open file" << endl;
        }
    }


    // Метод для добавления счета
    void addAccount(Account account) {
        accounts.push_back(account);
    }

    // Метод для вывода баланса всех карт
    void displayCardBalances() {
        cout << "----- Card Balances -----" << endl;
        for (auto& account : accounts) {
            cout << account.getName() << ": $" << account.getBalance() << endl;
        }
        cout << "--------------------------" << endl;
    }

    // Метод для пополнения карты
    void replenishCard(int index, double amount) {
        if (index >= 0 && index < accounts.size()) {
            accounts[index].deposit(amount);
            cout << "Replenished " << accounts[index].getName() << " with $" << amount << endl;
        }
        else {
            cout << "Invalid account index" << endl;
        }
    }

    // Метод для расходования денег по категории и карте
    void spendMoney(double amount, ExpenseCategory category, int cardIndex) {
        // Проверка на допустимость индекса карты
        if (cardIndex < 0 || cardIndex >= accounts.size()) {
            cout << "Invalid card index" << endl;
            return;
        }

        // Проверка на допустимость суммы расхода
        if (amount <= 0) {
            cout << "Invalid amount" << endl;
            return;
        }

        // Проверка на наличие достаточных средств на выбранной карте
        if (amount > accounts[cardIndex].getBalance()) {
            cout << "Insufficient funds in " << accounts[cardIndex].getName() << endl;
            return;
        }

        // Расходование средств с выбранной карты
        accounts[cardIndex].withdraw(amount);
        addExpense(Expense(amount, category));
        cout << "Expense of $" << amount << " spent from " << accounts[cardIndex].getName() << " under category " << getCategoryName(category) << endl;
    }

private:
    // Метод для получения названия категории по значению
    string getCategoryName(ExpenseCategory category) {
        switch (category) {
        case FOOD:
            return "Food";
        case TRANSPORTATION:
            return "Transportation";
        case SHOPPING:
            return "Shopping";
        case ENTERTAINMENT:
            return "Entertainment";
        case UTILITIES:
            return "Utilities";
        default:
            return "Other";
        }
    }
};

int main() {
    // Создание счетов
    Account wallet("Wallet", 1000.0);
    Account debitCard("Debit Card", 2000.0);
    Account creditCard("Credit Card", 5000.0);

    // Создание экземпляра управляющего класса финансами
    FinanceManager manager;
    manager.addAccount(wallet);
    manager.addAccount(debitCard);
    manager.addAccount(creditCard);

    // Вывод меню пользовательского интерфейса
    cout << "1. Add Expense" << endl;
    cout << "2. Generate Report" << endl;
    cout << "3. Generate Top 3 Costs" << endl;
    cout << "4. Generate Top 3 Categories" << endl;
    cout << "5. Save Report to File" << endl;
    cout << "6. Replenish Card" << endl;
    cout << "7. View Card Balances" << endl;
    cout << "8. Exit" << endl;

    int choice;
    do {
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // Очистка символа новой строки из входного потока
        switch (choice) {
        case 1: {
            double amount;
            cout << "Enter amount: $";
            cin >> amount;
            cin.ignore();

            int selectedCategory;
            cout << "Select Category:" << endl;
            cout << "0. Food" << endl;
            cout << "1. Transportation" << endl;
            cout << "2. Shopping" << endl;
            cout << "3. Entertainment" << endl;
            cout << "4. Utilities" << endl;
            cout << "Enter category number: ";
            string categoryInput;
            getline(cin, categoryInput);
            selectedCategory = stoi(categoryInput);
            ExpenseCategory category = static_cast<ExpenseCategory>(selectedCategory);

            int cardIndex;
            cout << "Select Card (0 - Wallet, 1 - Debit Card, 2 - Credit Card): ";
            cin >> cardIndex;
            cin.ignore();

            manager.spendMoney(amount, category, cardIndex);
            break;
        }

        case 2:
            manager.generateReport();
            break;
        case 3:
            manager.generateTop3Costs();
            break;
        case 4:
            manager.generateTop3Categories();
            break;
        case 5: {
            string filename;
            cout << "Enter filename: ";
            cin >> filename;
            cin.ignore();
            manager.saveToFile(filename);
            break;
        }
        case 6: {
            int cardIndex;
            double amount;
            cout << "Select Card (0 - Wallet, 1 - Debit Card, 2 - Credit Card): ";
            cin >> cardIndex;
            cin.ignore();
            cout << "Enter amount to replenish: $";
            cin >> amount;
            cin.ignore();
            manager.replenishCard(cardIndex, amount);
            break;
        }
        case 7:
            manager.displayCardBalances();
            break;
        case 8:
            cout << "Exiting program." << endl;
            break;
        default:
            cout << "Invalid choice. Please enter a number between 1 and 8." << endl;
            break;
        }
    } while (choice != 8);

    return 0;
}
