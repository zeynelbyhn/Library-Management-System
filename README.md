# Library Management System in C 📚

![C](https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c)
![CSV](https://img.shields.io/badge/Data-CSV_Based-success?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Completed-green?style=for-the-badge)

A robust and efficient **Library Management System** developed in **C**. This project demonstrates advanced file handling techniques by using CSV files as a relational database to manage books, authors, students, and borrowing transactions.

## 🎯 Key Features

* **Relational Data Management:** Manages complex relationships (Many-to-Many) between Books and Authors using `KitapYazar.csv`.
* **CRUD Operations:** Create, Read, Update, and Delete records for:
    * Books (`kitap.csv`)
    * Authors (`yazarlar.csv`)
    * Students (`ogrenciler.csv`)
* **Borrowing System:** Tracks borrowed books, due dates, and return status via `KitapOdunc.csv`.
* **Data Persistence:** All data is stored permanently in CSV files, ensuring no data loss after the program closes.
* **Dynamic Reporting:** Generates reports on borrowed books, student history, and library inventory.

## 🛠️ Technical Implementation

The project is built with a focus on low-level memory management and structured programming:

* **Structs & Pointers:** Used to define data models (Book, Student, Transaction) and manipulate data efficiently in memory.
* **File I/O:** Custom parsers to read/write CSV files, handling delimiters and newlines manually.
* **Memory Management:** Efficient use of buffers for string processing and record handling.
* **Algorithm Logic:** Implements search algorithms to link Student IDs with Book ISBNs.

## 📂 Database Structure (CSV)

The system operates on a file-based database architecture:

| File | Description |
| :--- | :--- |
| `kitap.csv` | Stores book details (ISBN, Title, Publisher). |
| `yazarlar.csv` | Stores author information. |
| `ogrenciler.csv` | Stores student details. |
| `KitapYazar.csv` | **Junction Table** linking Books and Authors (Relational Logic). |
| `KitapOdunc.csv` | Transaction logs for borrowing and returning books. |

## 🚀 How to Compile & Run

This project is compatible with GCC (GNU Compiler Collection).

1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/zeynelbyhn/Library-Management-System-C.git](https://github.com/zeynelbyhn/Library-Management-System-C.git)
    cd Library-Management-System-C
    ```

2.  **Compile the code:**
    ```bash
    gcc src/main.c -o library_system
    ```

3.  **Run the application:**
    * **Windows:** `library_system.exe`
    * **Linux/Mac:** `./library_system`

> **Note:** Ensure that the `data/` folder containing CSV files is in the same directory as the executable, or update the file paths in the source code.

## 🧠 Future Improvements
* Implement a GUI using GTK or Windows API.
* Add hashing for faster search operations.
* Migrate data storage to SQL (SQLite).

---

*Developed as a Computer Engineering project demonstrating C programming proficiency.*
