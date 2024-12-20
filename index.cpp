#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include <sstream>
#include <memory>
using namespace std;

class Transaction {
protected:
    double amount;
    string description;
    string date;

public:
    // Ham tao
    Transaction(double amt, const string& des) : amount(amt), description(des) {
        time_t now = time(0);
        date = ctime(&now);
    }
    // Xuat du lieu
    virtual void display() {
        cout << setw(15) << amount << setw(20) << description << setw(30) << date;
    }
    // Save Transaction
    virtual string serialize() const {
        return "Transaction," + to_string(amount) + "," + description + "," + date;
    }

    virtual ~Transaction() = default;

    void update(double amt, const string& des) {
        amount = amt;
        description = des;
    }
    double get_amount(){
        return amount;
    }

};

class Income : public Transaction {
public:
    Income(double amt, const string& des) : Transaction(amt, des) {}

    void display() override {
        cout << setw(15) << "Income";
        Transaction::display();
    }
    // Save Income
    string serialize() const override {
        return "Income," + to_string(amount) + "," + description + "," + date;
    }

};

class Expenditure : public Transaction {
public:
    Expenditure(double amt, const string& des) : Transaction(amt, des) {}

    void display() override {
        cout << setw(15) << "Expenditure";
        Transaction::display();
    }
    // Save Expenditure
    string serialize() const override {
        return "Expenditure," + to_string(amount) + "," + description + "," + date;
    }
};


class Investment {
protected:
    double amount;
    int duration;
    string date;

public:
    // Ham tao
    Investment(double amt, int dur) : amount(amt), duration(dur) {
        time_t now = time(0);
        date = ctime(&now);
    }

    virtual void display() {
        cout << setw(15) << amount << setw(15) << duration ;
    }

    virtual double maturityAmount() {
        return amount;
    }
    // Save Investment
    virtual string serialize() const {
        return "Investment," + to_string(amount) + "," + to_string(duration) + "," + date;
    }

    virtual ~Investment() = default;

    virtual void update(double amt, int dur) {
        amount = amt;
        duration = dur;
    }

    double get_amount(){
        return amount;
    }
};

class SIP : public Investment {
private:
    double monthly;

public:
    SIP(double amt, int dur, double monAmt) : Investment(amt, dur), monthly(monAmt) {}

    void display() override {
        cout << setw(15) << "SIP";
        Investment::display();
        cout << setw(30) << monthly << setw(30) << date << endl;
        // cout << setw(30) << date << setw(10) << monthly  << endl;

    }
    // lai suat gia lap
    double maturityAmount() override {
        double final = amount * pow(1 + (0.096 / 12), duration * 12);
        return final + (monthly * 12 * duration);
    }
    // Save SIP
    string serialize() const override {
        return "SIP," + to_string(amount) + "," + to_string(duration) + "," + date + "," + to_string(monthly);
    }
    void update(double amt, int dur, double monAmt) {
        Investment::update(amt, dur);
        monthly = monAmt;
    }
};

class FD : public Investment {
public:
    FD(double amt, int dur) : Investment(amt, dur) {}

    void display() override {
        cout << setw(15) << "FD";
        Investment::display();
        cout << setw(60) << date << endl;
    }
    //lai suat gia lap
    double maturityAmount() override {
        return amount * pow((1 + 0.071), duration);
    }
    //Save FD
    string serialize() const override {
        return "FD," + to_string(amount) + "," + to_string(duration) + "," + date;
    }
     virtual void update(double amt, int dur) {
        amount = amt;
        duration = dur;
    }
};

class FinancialPlan {
    double incomeGoal;
    double expenditureGoal;
    double investmentGoal;
    
public:
    FinancialPlan(double income = 0, double expenditure = 0, double investment = 0)
        : incomeGoal(income), expenditureGoal(expenditure), investmentGoal(investment) {}
    
    void setGoals(double income, double expenditure, double investment) {
        incomeGoal = income;
        expenditureGoal = expenditure;
        investmentGoal = investment;
        cout << "Financial goals set successfully.\n";
    }
    
    void displayGoals() const {
        cout << "\n-- Financial Goals --\n";
        cout << "Income Goal: " << incomeGoal << "\n";
        cout << "Expenditure Goal: " << expenditureGoal << "\n";
        cout << "Investment Goal: " << investmentGoal << "\n";
    }
    
    void checkProgress(double totalIncome, double totalExpenditure, double totalInvestment) const {
        cout << "\n-- Financial Goal Progress --\n";
        cout << "Income Goal: " << totalIncome << "/" << incomeGoal << "\n";
        cout << "Expenditure Goal: " << totalExpenditure << "/" << expenditureGoal << "\n";
        cout << "Investment Goal: " << totalInvestment << "/" << investmentGoal << "\n";
    }
};


class BankAPI {
public:
    static void sendDebtReminder(const string& username, double amountDue) {
        cout << "Reminder: Dear " << username << ", you have an outstanding amount of " << amountDue << " Rs due.\n";
    }
};

class FinanceManager {
public:
    vector<Transaction*> transactions;
    vector<Investment*> investments;
    FinancialPlan plan;
    ~FinanceManager() {
        for (auto t : transactions) delete t;
        for (auto i : investments) delete i;
    }

    void addTransaction(Transaction* t) {
        // transactions.push_back(move(t));
        transactions.push_back(t);

    }

    void addInvestment(Investment* i) {
        investments.push_back(i);
        // investments.push_back(move(i));
    }


    void updateInvestment(int index, double amount, int duration, double monthly = 0) {
        if (index >= 0 && index < investments.size()) {
            // Handle SIP separately as it has a monthly amount
            SIP* sip = dynamic_cast<SIP*>(investments[index]);
            if (sip) {
                sip->update(amount, duration, monthly);
            } else {
                investments[index]->update(amount, duration);
            }
            cout << "Investment updated successfully.\n";
        } else {
            cout << "Invalid investment index.\n";
        }
    }

    void displayRecord(double balance) {
        cout << "-----------------------------------\n";
        cout << "|        Personal Finance         |\n";
        cout << "-----------------------------------\n";
        cout << "\n||--BALANCE--: " << balance << "||" << endl;

        cout << "\n--SAVINGS--: \n";
        cout << setw(15) << "Type" << setw(15) << "Amount" << setw(20) << "Description" << setw(30) << "Date" << endl;
        for (Transaction* t : transactions) {
            t->display();
        }

        cout << "\n--INVESTMENTS--\n";
        cout << setw(15) << "Type" << setw(15) << "Amount" << setw(15) << "Duration" << setw(30) << "Monthly amount invested" << setw(30) << "Date" << endl;
        for (Investment* i : investments) {
            i->display();
        }
    }
    void saveToFile(ofstream& outFile) {
        for (Transaction* t : transactions) {
            outFile << t->serialize() << endl;
        }
        for (Investment* i : investments) {
            outFile << i->serialize() << endl;
        }
    }
    int tcount(){
        return transactions.size();
    }
    int icount(){
        return investments.size();
    }
    void setFinancialGoals(double income, double expenditure, double investment) {
        plan.setGoals(income, expenditure, investment);
    }
    
    void checkGoalProgress() {
        double totalIncome = 0;
        double totalExpenditure = 0;
        double totalInvestment = 0;
        for (Transaction* t : transactions) {
            Income* income = dynamic_cast<Income*>(t);
            Expenditure* expenditure = dynamic_cast<Expenditure*>(t);
            if (income) totalIncome += income->get_amount();
            else if (expenditure) totalExpenditure += expenditure->get_amount();
        }

        for (Investment* i : investments) {
            totalInvestment += i->get_amount();
        }

        plan.checkProgress(totalIncome, totalExpenditure, totalInvestment);
    }
    void updateTransaction(int index, double amount, const string& description, double& balance) {
    if (index >= 0 && index < transactions.size()) {
        double oldAmount = transactions[index]->get_amount();
        transactions[index]->update(amount, description);
        if (dynamic_cast<Income*>(transactions[index])) {
            balance += (amount - oldAmount);  
        } 
        else if (dynamic_cast<Expenditure*>(transactions[index])) {
            balance -= (amount - oldAmount); 
        }

        cout << "Transaction updated successfully.\n";
    } else {
        cout << "Invalid transaction index.\n";
    }
}
};
   

class User {
protected:
    FinanceManager manager;
    double balance;
    string username;
public:

    User(double initialBalance, const string& user) : balance(initialBalance), username(user) {}
    double getBalance() const { return balance; }
    void setBalance(double newBalance) { balance = newBalance; }
    FinanceManager& getManager() { return manager; }

    void operations() {
        int choice = -1;
        while (choice != 0) {
            cout << "\n--OPTIONS--\n";
            cout << "1. Record INCOME\n";
            cout << "2. Record EXPENDITURE\n";
            cout << "3. Make Investment\n";
            cout << "4. Finance Information\n";
            cout << "5. Investment Information\n";
            cout << "6. Update Transaction\n";
            cout << "7. Update Investment\n";
            cout << "8. Set Financial Plan\n";
            cout << "9. Check Goal Progress\n";
            cout << "10. Send Debt Reminder\n";
            cout << "11. Update Balance\n";
            cout << "0. Exit\n";
            cout << "Enter choice : ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    double amt;
                    string desc;
                    cout << "Enter amount : ";
                    cin >> amt;
                    cout << "Enter description : ";
                    cin.ignore();
                    getline(cin, desc);
                    manager.addTransaction(new Income(amt, desc));
                    balance += amt;
                    break;
                }

                case 2: {
                    double amt;
                    string desc;
                    cout << "Enter amount: ";
                    cin >> amt;
                    if (balance - amt < 1000) {
                        cout << "Error: Balance cannot go below 1000." << endl;
                        continue;
                    }
                    cin.ignore();
                    cout << "Enter description: ";
                    getline(cin, desc);
                    manager.addTransaction(new Expenditure(amt, desc));
                    balance -= amt;
                    break;
                }

                case 3: {
                    makeInvestment();
                    break;
                }

                case 4: {
                    manager.displayRecord(balance);
                    break;
                }
                case 5: {
                    cout << "--MATURITY AMOUNTS--||\n";
                    for (int i = 0; i < manager.icount(); i++) {
                        Investment* inv = manager.investments[i];
                        cout << "\nInvestment " << i + 1 << " : " << inv->maturityAmount() << " Rs" << endl;
                        cout<<setw(15)<<"Type"<<setw(15)<<"Amount"<<setw(20)<<"Duration"<<setw(30)<<"Monthly amount invested"<<endl;
                        inv->display();
                    }
                    break;
                }
                case 6: {
                    if (manager.tcount() == 0) {
                        cout << "No transactions to update.\n";
                        break;
                    }
                    cout << "Available transactions:\n";
                    // Xuat transaction cho nguoi dung lua chon
                    cout << setw(5) << "Index" << setw(15) << "Type" << setw(15) << "Amount" << setw(20) << "Description" << setw(30) << "Date" << endl;
                    for (int i = 0; i < manager.tcount(); i++) {
                        cout << setw(5) << i + 1; 
                        manager.transactions[i]->display(); 
                        cout << endl;
                    }

                    int index;
                    double amount;
                    string description;

                    cout << "Enter transaction index to update: ";
                    cin >> index;

                    if (index < 1 || index > manager.tcount()) {
                        cout << "Invalid index.\n";
                        break;
                    }

                    cout << "Enter new amount: ";
                    cin >> amount;
                    cout << "Enter new description: ";
                    cin.ignore();
                    getline(cin, description);

                    manager.updateTransaction(index - 1, amount, description, balance);
                    break;
                }

                case 7: {
                    if (manager.icount() == 0) {
                        cout << "No investments to update.\n";
                        break;
                    }

                    cout << "Available investments:\n";
                    cout << setw(5) << "Index" << setw(15) << "Type" << setw(15) << "Amount" << setw(15) << "Duration" << setw(30) << "Monthly amount invested" << setw(30) << "Date" << endl;

                    for (int i = 0; i < manager.icount(); i++) {
                        cout << setw(5) << i + 1; // Display index starting from 1
                        manager.investments[i]->display(); // Display each investment
                        cout << endl;
                    }

                    int index, duration;
                    double amount, monthly = 0;

                    cout << "Enter investment index to update: ";
                    cin >> index;

                    if (index < 1 || index > manager.icount()) {
                        cout << "Invalid index.\n";
                        break;
                    }

                    cout << "Enter new amount: ";
                    cin >> amount;
                    cout << "Enter new duration: ";
                    cin >> duration;

                    // Check if it’s an SIP, and ask for monthly amount if so
                    if (dynamic_cast<SIP*>(manager.investments[index - 1])) {
                        cout << "Enter new monthly amount: ";
                        cin >> monthly;
                    }

                    manager.updateInvestment(index - 1, amount, duration, monthly);
                    break;
                }

                case 8: {
                    double incomeGoal, expenditureGoal, investmentGoal;
                    cout << "Set your monthly financial goals:\n";
                    cout << "Income Goal: ";
                    cin >> incomeGoal;
                    cout << "Expenditure Goal: ";
                    cin >> expenditureGoal;
                    cout << "Investment Goal: ";
                    cin >> investmentGoal;
                    manager.setFinancialGoals(incomeGoal, expenditureGoal, investmentGoal);
                    break;
                }
                case 9: {
                    manager.checkGoalProgress();
                    break;
                }
                case 10: {
                    double amountDue;
                    cout << "Enter amount due for reminder: ";
                    cin >> amountDue;
                    BankAPI::sendDebtReminder(username, amountDue);
                    break;
                }
                case 11: {
                    double update_balance;
                    cout << "New balance: "; cin >> update_balance;
                    balance = update_balance;
                }
                case 0:
                    break;
                default:
                    cout << "\nNo such option:(";
                    break;
            }
        }
    }
    void makeInvestment() {
        int sub = -1;
        while (sub != 0) {
            cout << "\nWhich one:\n";
            cout << "1. SIP\n";
            cout << "2. FD\n";
            cout << "0. Go back\n";
            cout << "Enter your choice : ";
            cin >> sub;

            switch (sub) {
                case 1: {
                    double amt, monthly;
                    int dur;
                    cout << "Enter amount : ";
                    cin >> amt;
                    if (balance - amt < 1000) {
                        cout << "ERROR : Min Balance=1000";
                        return;
                    }
                    cout << "Enter duration in yrs : ";
                    cin >> dur;
                    cout << "Enter monthly investment amount : ";
                    cin >> monthly;
                    manager.addInvestment(new SIP(amt, dur, monthly));
                    balance -= amt;
                    break;
                }

                case 2: {
                    double amt;
                    int dur;
                    cout << "Enter amount : ";
                    cin >> amt;
                    if (balance - amt < 1000) {
                        cout << "ERROR: Min Balance=1000";
                        return;
                    }
                    cout << "Enter duration in yrs : ";
                    cin >> dur;
                    manager.addInvestment(new FD(amt, dur));
                    balance -= amt;
                    break;
                }

                case 0:
                    break;

                default:
                    cout << "Invalid choice.";
                    break;
           }
        }
    }
};
class UserAccount {
    string username;
    string password;

public:
    bool login() {
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        ifstream file(username + ".txt");
        if (file.is_open()) {
            string storedPassword;
            getline(file, storedPassword);
            if (storedPassword == password) {
                cout << "Login successful.\n";
                return true;
            }
        }
        cout << "Login failed.\n";
        return false;
    }

    void registerAccount() {
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        ofstream file(username + ".txt");
        if (file.is_open()) {
            file << password << endl;
            cout << "Registration successful.\n";
        } else {
            cout << "Error saving account.\n";
        }
    }

    void saveData(User& user) {
        ofstream file(username + "_data.txt");
        if (file.is_open()) {
            file << user.getBalance() << endl;  // Use getter
            user.getManager().saveToFile(file); // Access manager through getter
            file.close();
        }
    }

    void loadData(User& user) {
        ifstream file(username + "_data.txt");

        if (file.is_open()) {
            // Load the user's balance from the first line.
            double balance;
            file >> balance;
            user.setBalance(balance);  // Use setter
            file.ignore(); // To skip the newline after the balance

            string line;
            while (getline(file, line)) {
                istringstream stream(line);
                string type;
                getline(stream, type, ',');

                if (type == "Income") {
                    double amount;
                    string description, date;
                    stream >> amount;
                    stream.ignore(); // Skip comma
                    getline(stream, description, ',');
                    getline(stream, date);
                    user.getManager().addTransaction(new Income(amount, description));

                } else if (type == "Expenditure") {
                    double amount;
                    string description, date;
                    stream >> amount;
                    stream.ignore(); // Skip comma
                    getline(stream, description, ',');
                    getline(stream, date);
                    user.getManager().addTransaction(new Expenditure(amount, description));

                } else if (type == "SIP") {
                    double amount, monthly;
                    int duration;
                    string date;
                    stream >> amount >> duration >> monthly;
                    stream.ignore(); // Skip comma
                    getline(stream, date);
                    user.getManager().addInvestment(new SIP(amount, duration, monthly));

                } else if (type == "FD") {
                    double amount;
                    int duration;
                    string date;
                    stream >> amount >> duration;
                    stream.ignore(); // Skip comma
                    getline(stream, date);
                    user.getManager().addInvestment(new FD(amount, duration));
                }
            }
            file.close();
        } else {
            cout << "Error: Could not open file for user data.\n";
        }
    }

    string getUsername() const { return username; }
};


int main() {
    cout << "---Welcome to Finance Management System!---\n";
    
    UserAccount account;
    int choice;
    cout << "1. Register\n2. Login\nEnter choice: ";
    cin >> choice;

    if (choice == 1) {
        account.registerAccount();
    } else if (choice == 2) {
        if (account.login()) {
            User user(2000, account.getUsername());
            account.loadData(user);
            user.operations();
            account.saveData(user);
        }
    }

    return 0;
}
