
# Finance Manager System

## Overview 
The **Finance Manager System** is a C++ application designed to assist users in managing their finances. This system provides functionality for managing user accounts, tracking transactions, setting financial goals, and managing investments. The project is structured using object-oriented programming principles and focuses on modularity and scalability.

---

## Features 
- **User Management**: 
  - Login, register, and manage user accounts.
  - Save and load user data securely.
  
- **Finance Management**:
  - Add, update, and track transactions (income and expenditure).
  - Set and monitor financial goals.

- **Investment Management**:
  - Track investments including Fixed Deposits (FD) and Systematic Investment Plans (SIP).
  - Calculate maturity amounts for investments.

- **Integration**:
  - Connect with external APIs (e.g., Bank API) to send debt reminders.

---

## Class Structure 
The project follows a structured object-oriented design. Key components include:

### **Core Classes**
- **User**: Represents the user and links to the `FinanceManager`.
- **UserAccount**: Handles user registration, login, and data persistence.
- **FinanceManager**: Core functionality for managing transactions, financial goals, and investments.
- **Transaction**: Tracks individual transactions such as income and expenditures.
- **Investment**: Manages investments with specific types (`FD` and `SIP`).

### **Specialized Classes**
- **FinancialPlan**: Enables setting and tracking income, expenditure, and investment goals.
- **BankAPI**: Mock integration for external banking functionalities like sending reminders.

## UML Diagram 
Below is the UML diagram illustrating the design and relationships of the classes used in the system:

## Live Preview 
View the UML diagram and repository live: Repository Link

## License 
This project is open source and available under the MIT License

