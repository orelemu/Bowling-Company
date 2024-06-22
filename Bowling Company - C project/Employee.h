#pragma once

#define MIN_ID 1
#define MAX_ID 9999

typedef enum {
	eCEO, eManager, eCashier, eKitchen, eMaintenance, eNofEmpTypes
} eEmpType;

static const char* EmpTypeStr[eNofEmpTypes]
= { "CEO", "Manager", "Cashier", "Kitchen", "Maintenance" };

static const char* EmpTypeStr[eNofEmpTypes];

typedef struct {
	char* empName;
	int			empID;
	double		salary;
	eEmpType	empType;
} Employee;

int			initEmployee(Employee* pEmployee, Employee** pEmployeeArr, int numOfElements);
void		initCEOEmployee(Employee* pEmployee);
void		initManagerEmployee(Employee* pEmployee);

eEmpType	getEmpType();
const char* getEmpTypeStr(int type);

int			getEmployeeID();
int			getEmployeeUniqueID(Employee** pEmployeeArr, int numOfElememnts);
int			isEmployeeUnique(int idToCheck, Employee** pEmployeeArr, int numOfElememnts);


double		getEmployeeSalary();

int			changeSalary(Employee* pEmp);
int			changeEmpType(Employee* pEmp);

int			compareEmployeesByID(int id1, int id2);
int			compareEmployeesBySalary(double salary1, double salary2);
int			compareEmployeesByEmpType(int type1, int type2);

void		printEmployee(const Employee* pEmployee);
void		printEmployeeV(void* val);

void		freeEmployee(Employee* pEmp);
void		freeEmployeeV(void* ppEmp);
