# Bowling Company (C)

## Project Description

This project is a management system for a bowling company, designed to handle branches, employees, lanes, and games. The system supports:

- **Branch Management**: Add, update, and remove branches.
- **Employee Management**: Manage employee records including adding, updating, and deleting employees.
- **Lane Management**: Track bowling lanes for each branch.
- **Game Management**: Record and monitor bowling games, including players and scores.

## Project Structure

The project consists of multiple source (`.c`) and header (`.h`) files, organized by module:

- `BowlingCompany.c` & `BowlingCompany.h`: Main files containing core system logic.
- `Branch.c` & `Branch.h`: Handles branch-related operations.
- `Employee.c` & `Employee.h`: Handles employee-related operations.
- `Lane.c` & `Lane.h`: Handles lane-related operations.
- `Game.c` & `Game.h`: Handles game-related operations.
- `Date.c` & `Date.h`: Date-handling utilities.
- `General.c` & `General.h`: General-purpose utility functions.
- `GeneralList.c` & `GeneralList.h`: Linked list implementation for dynamic data structures.

Additionally, the project includes file input/output modules:

- `CompanyBinFile.c` & `CompanyBinFile.h`: Binary file read/write functionality.
- `CompanyTextFile.c` & `CompanyTextFile.h`: Text file read/write functionality.

## Data Files

The project includes sample data files:

- `BowlingCompanyBin.bin` & `BowlingCompanyBin_out.bin`: Binary files containing data for branches, employees, lanes, and games.
- `BowlingCompanyText.txt` & `BowlingCompanyText_out.txt`: Text files with similar data.
