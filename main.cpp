#include <iostream>
#include <fstream>
#include <iomanip>		// Manipulates the state of iostream objects

using namespace std;


/// The class to store data of different students.
/// There are 5 subjects: English, Maths, Chemistry, Physics, CS.
class Student
{
	int roll_no;
	char name[50];
	int eng_marks,
		math_marks,
		chem_marks,
		phy_marks,
		cs_marks;
	double average;
	char grade;

public:
	void get_data();
	void show_data() const;
	void calculate();
	int get_roll() const;
};

// To calculate the average and grade of the student
void Student::calculate()
{
	average = (eng_marks + math_marks + chem_marks + phy_marks + cs_marks) / 5.0;

	if (average >= 90)
		grade = 'A';
	else if (average >= 75)
		grade = 'B';
	else if (average >= 60)
		grade = 'C';
	else if (average >= 45)
		grade = 'D';
	else if (average >= 30)
		grade = 'E';
	else
		grade = 'F';
}

// To get the data of the student
void Student::get_data()
{
	cout << "\nRoll number    : ";
	cin >> roll_no;
	cout << "Student's Name : ";
	cin.ignore(); cin.getline(name, 50);
	cout << "\nMarks (out of 100)" << endl;
	cout << "English        : "; cin >> eng_marks;
	cout << "Maths          : "; cin >> math_marks;
	cout << "Chemistry      : "; cin >> chem_marks;
	cout << "Physics        : "; cin >> phy_marks;
	cout << "CS             : "; cin >> cs_marks;

	calculate();
}

// To display the data of the student
void Student::show_data() const
{
	cout << "\nRoll number    : " << roll_no;
	cout << "\nStudent's Name : " << name;
	cout << "\n\nMarks (out of 100)";
	cout << "\nEnglish        : " << eng_marks;
	cout << "\nMaths          : " << math_marks;
	cout << "\nChemistry      : " << chem_marks;
	cout << "\nPhysics        : " << phy_marks;
	cout << "\nCS             : " << cs_marks;
	cout << "\n\nAverage        : " << average;
	cout << "\nGrade          : " << grade;
}

// Returns roll number of the student
int Student::get_roll() const
{
	return roll_no;
}


// Function declarations
void create_student();
void display_student(int);
void display_all();
void delete_student(int);
void change_student(int);


// MAIN
int main()
{
	char choice = '0';
	cout << setprecision(2);

	do
	{
		char choice;
		int num;
		system("cls");

		cout << "\n1 - Create Student Record";
		cout << "\n2 - Search Student Record";
		cout << "\n3 - Display All Records";
		cout << "\n4 - Delete Student Record";
		cout << "\n5 - Modify Student Record";
		cout << "\n0 - Exit";
		cout << "\n\nChoose an option: "; cin >> choice;

		system("cls");

		switch (choice)
		{
		case '1':
			create_student();
			break;
		case '2':
			cout << "\n\nEnter the roll number: "; cin >> num;
			display_student(num);
			break;
		case '3':
			display_all();
			break;
		case '4':
			cout << "\n\nEnter the roll number: "; cin >> num;
			delete_student(num);
			break;
		case '5':
			cout << "\n\nEnter the roll number: "; cin >> num;
			change_student(num);
			break;
		case '0':
			cout << "Exiting...";
			exit(0);
		}
	} while (choice != 0);

	return 0;
}


// Write student details to file
void create_student()
{
	Student student;
	ofstream outfile;
	outfile.open("student.dat", ios::binary | ios::app);
	student.get_data();
	outfile.write(reinterpret_cast<char*> (&student), sizeof(student));
	outfile.close();
	cout << "\n\nStudent record has been created!";
	cin.ignore();
	cin.get();
}

// Read student details from the record
void display_all()
{
	Student student;
	ifstream infile;
	infile.open("student.dat", ios::binary);

	if (!infile)
	{
		cout << "File could not be loaded! Press any key to exit...";
		cin.ignore();
		cin.get();
		return;
	}

	cout << "\n\n\nDisplaying all records...\n\n";
	while (infile.read(reinterpret_cast<char*> (&student), sizeof(student)))
	{
		student.show_data();
		cout << endl << endl;
	}
	infile.close();
	cin.ignore();
	cin.get();
}

// Read student record based on roll number
void display_student(int n)
{
	Student student;
	ifstream infile;
	infile.open("student.dat", ios::binary);

	if (!infile)
	{
		cout << "File could not be loaded! Press any key to exit...";
		cin.ignore();
		cin.get();
		return;
	}

	bool flag = false;
	while (infile.read(reinterpret_cast<char*> (&student), sizeof(student)))
	{
		if (student.get_roll() == n)
		{
			student.show_data();
			flag = true;
		}
	}
	infile.close();

	if (!flag)
	{
		cout << "\n\nRecord does not exist...";
		cin.ignore();
		cin.get();
	}
}

// Modify record for the specified roll number
void change_student(int n)
{
	bool found = false;
	Student student;
	fstream f;
	f.open("student.dat", ios::binary | ios::in | ios::out);

	if (!f)
	{
		cout << "File could not be loaded! Press any key to exit...";
		cin.ignore();
		cin.get();
		return;
	}

	while (!f.eof() && !found)
	{
		f.read(reinterpret_cast<char*> (&student), sizeof(student));

		if (student.get_roll() == n)
		{
			student.show_data();
			cout << "\n\nEnter new details for the student..." << endl;
			student.get_data();

			int pos = (-1) * static_cast<int>(sizeof(student));
			f.seekp(pos, ios::cur);
			f.write(reinterpret_cast<char*> (&student), sizeof(student));
			cout << "\n\nRecord updated...";

			found = true;
		}
		f.close();
	}

	if (!found)
	{
		cout << "\n\nRecord not found...";
		cin.ignore();
		cin.get();
	}
}

// Delete record with a specified roll number
void delete_student(int n)
{
	Student student;

	ifstream infile;
	infile.open("student.dat", ios::binary);

	if (!infile)
	{
		cout << "File could not be loaded! Press any key to exit...";
		cin.ignore();
		cin.get();
		return;
	}

	ofstream outfile;
	outfile.open("Temp.dat", ios::out);
	infile.seekg(0, ios::beg);

	while (infile.read(reinterpret_cast<char*> (&student), sizeof(student)))
	{
		if (student.get_roll() != n)
		{
			outfile.write(reinterpret_cast<char*> (&student), sizeof(student));
		}
	}
	outfile.close();
	infile.close();

	remove("student.dat");
	rename("Temp.dat", "student.dat");
	cout << "\n\nRecord deleted...";
	cin.ignore();
	cin.get();
}
