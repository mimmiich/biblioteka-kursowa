#include <iostream> //input/output stream
#include <fstream> //file stream
#include <cstring> //for strings and arrays
#include <conio.h>
#include <iomanip>
using namespace std;
const int N = 100;
const char Filename[] = "Library.dat";
struct book { //struct - data type for grouping variables of mixed data types into a single unit
	int catnum; //catalogue number
	char title[30];
	char avtor[30];
	int year, br, zaem, nalich;
};


fstream fp; // fp- failow ukazatel, deklarirane na whod/izhoden failow potok
// da naprawq f koqto wzima list ot Lib (kakto e w enter)

int menu() {
	int ch;
	cout << "\n\n \t Menu" << endl;
	cout << "1. Append a new book" << endl;
	cout << "2. Append several books" << endl;
	cout << "3. Print all books" << endl;
	cout << "4. Borrow a book" << endl;
	cout << "5. Return book on title" << endl;
	cout << "6. Return book on catalog number" << endl;
	cout << "7. Query menu" << endl;
	cout << "8. Save library" << endl;
	cout << "9. Exit" << endl;
	do {
		cout << "Your choise is: ";
		cin >> ch;
	} while (ch < 1 || ch>9); // ako otgowarq na uslowieto, aka ako wyweda chislo>9 shte se powtori otnowo
	// samo "your choice is:", dokato ne wyweda chislo 0-9
	return(ch); // wryshta izbora na potrebitelq w programata
}


book input() {
	book a = { 0 }; //
	cout << "Catalog number: ";
	cin >> a.catnum; cin.ignore(); // ime_na_array.variable_ot_struct; cin.ignore removes the newline(left from cin) from the stream;
	cout << "Title: ";
	cin.getline(a.title, 30); // cin.getline wywejda niz i pozwolqwa space cin.getline(ime_na_string, razmer)
	cout << "Autor: ";
	cin.getline(a.avtor, 30);
	cout << "Year of publication: ";
	cin >> a.year;
	cout << "Number of books: ";
	cin >> a.br;
	do {
		cout << "Borrow number: ";
		cin >> a.zaem;
	} while (a.zaem<0 || a.zaem>a.br);
	cout << "Number presenting books: ";
	a.nalich = a.br - a.zaem;
	cout << a.nalich << endl;
	return (a);
}


int enter(book Lib[], int n) {
	int i, m;
	do {
		cout << "\n How many books to enter(max 5): ";
		cin >> m;
	} while (m < 0 || m>5);
	if (m + n < 100)
		for (i = n; i < n + m; i++) {
			cout << "Book" << i + 1 << endl;
			Lib[i] = input();
		}
	else
		cout << "There are no free places in array Lib" << endl;
	return(n + m);
}


void saveinfile(book Lib[], int n) {
	fp.open(Filename, ios::binary | ios::out);
	if (!fp) {
		cout << "Error in file" << endl;
		exit(1);
	}
	fp.write((char*)Lib, n * sizeof(book));
	fp.close();
}


void append() {
	book b;
	fp.open(Filename, ios::binary | ios::app);
	if (!fp) {
		cout << "Error in file" << endl;
		exit(1);
	}
	cout << "\n Append one new book to library" << endl;
	b = input();
	fp.write((char*)&b, sizeof(book));
	fp.close();
}


void output(book Lib[], int n) {
	int i, k = 0;
	cout << "\n Catalogue of books: " << endl;
	for (i = 0; i < n; i++) {
		cout << setiosflags(ios::left);
		cout << "\n" << i + 1 << "  " << "Cat.No."<< setw(5) << Lib[i].catnum << "Book: " << setw(17) << Lib[i].title;
		cout << "Autor: " << setw(17) << Lib[i].avtor << "Year: " << setw(7) << Lib[i].year << "Quantity: " << setw(4) << Lib[i].br;
		cout << "Borrowed: " << setw(4) << Lib[i].zaem << "Available: " <<setw(4) << Lib[i].nalich;
		k++;
		if (k % 5 == 0)
			cout << "\n"<<endl;
	}
}



int loadfile(book Lib[]) {
	long pos;
	int n = 0, i;
	book b;
	fp.open(Filename, ios::binary | ios::in);
	if (!fp) {
		cout << "\n File doesn't exist" << endl;
		return n;
	}
	fp.seekg(0l, ios::end);
	pos = fp.tellg();
	fp.close();
	n = pos / (sizeof(book));
	fp.open(Filename, ios::binary | ios::in);
	if (!fp) {
		cout << "\n Error in file" << endl;
		exit(1);
	}
	for (i = 0; i < n; i++) {
		fp.read((char*)&b, sizeof(book));
		Lib[i] = b;
	}
	fp.close();
	return n;
}



// NE MI WRYSHTA KNIGATA KATO ZAETA :(
void book_on_name(book Lib[], int n) {
	char bn[30];
	int flag = 0;
	int broi;
	int nov, i;
	cout << "\n Book's name: ";
	cin.ignore(); cin.getline(bn, 30);  //inache go skipva
	for (i = 0; i < n; i++)
		if (!strcmp(bn, Lib[i].title)) {
			cout << "Found" << endl;
			flag = 1;
			if (Lib[i].nalich == 0) 
				cout << "All books are borrowed"<<endl;
			else {
				do{ 
					cout << "How many would you like to borrow?"; cin >> broi;  Lib[i].nalich = Lib[i].nalich - broi; Lib[i].zaem = Lib[i].zaem + broi;
				if (Lib[i].nalich >= 0) {
					cout << "You are borrowing " << broi << " books." << endl;
					cout << Lib[i].nalich << " books left." << endl;
					saveinfile(Lib, n);
				}
				else {
					cout << "There are not that many books available." << endl;
				}
				} while (Lib[i].nalich < 0);
			}
		}
	if (!flag)
		cout << "No such book" << endl;
}


int return_book_name(book Lib[], int n) {
	char name[30];
	int flag = 0;
	int broi;
	int i;
	cout << "\n Book's name: ";
	cin.ignore();  cin.getline(name, 30);
	for (i = 0; i < n; i++)
		if (!strcmp(name, Lib[i].title)) {
			cout << "Found" << endl;
			flag = 1;
			if (Lib[i].zaem == 0)
				cout << "There are no borrowed books" << endl;
			else {
				do {
					cout << "How many books would you like to return? "; cin >> broi; Lib[i].zaem = Lib[i].zaem - broi;
					if ( Lib[i].zaem >= 0) {
						cout << "You are returning " << broi << " books." << endl;
						cout << Lib[i].zaem << " books left" << endl;
						saveinfile(Lib, n);
					}
					else {
						cout << "There are not that many books borrowed." << endl;
					}
				} while (Lib[i].zaem  < 0);
				Lib[i].nalich += broi; 
			}
		}
	if (!flag)
		cout << "No such book" << endl;
	return(Lib[i].zaem, Lib[i].nalich);

}


int return_book_catn(book Lib[], int n) {
	int flag = 0;
	int catn;
	int i, broi;
	cout << "\n Book's catalogue number: ";
	cin >> catn;
	for (i = 0; i < n; i++)
		if (catn == Lib[i].catnum ) { 
			cout << "Found" << endl;
			flag = 1;
			if (Lib[i].zaem == 0)
				cout << "There are no borrowed books" << endl;
			else {
				do {
					cout << "How many books would you like to return? "; cin >> broi; Lib[i].zaem = Lib[i].zaem - broi;
					if (Lib[i].zaem >= 0) {
						cout << "You are returning " << broi << " books." << endl;
						cout << Lib[i].zaem << " books left." << endl;
						saveinfile(Lib, n);
					}
					else {
						cout << "There are not that many books borrowed." << endl;
					}
				} while (Lib[i].zaem < 0);
				Lib[i].nalich += broi;
			}
		}
	if (!flag)
		cout << "No book with such catalogue number" << endl;
	return(Lib[i].zaem, Lib[i].nalich);

}


int auditmenu() {
	int ch; // Audit functions choice
	// Repeat until choice is one of the functions
	do {
		cout << "\n\n \t Query menu";
		cout << "\n1. Sort by autor's name";
		cout << "\n2. Sort by book's publication year";
		cout << "\n3. Print the most borrowed books";
		cout << "\n4. Print the least borrowed books";
		cout << "\n5. Exit";
		cout << "\nYour choice: ";
		cin >> ch; // Enter your choice
	} while (ch < 1 || ch>5);
	return (ch); // Return choice to claler function
}


void sortbyautorname(book Lib[], int n) {
	int i, j;
	char t[20];
		for (i = 1; i < n; i++) {
			for (j = 1; j < n; j++) {
				if (strcmp(Lib[j - 1].avtor, Lib[j].avtor) > 0) {
					strcpy_s(t, Lib[j - 1].avtor); // copies string1 (t) into string2 (Lib[j-1].avtor)
					strcpy_s(Lib[j - 1].avtor, Lib[j].avtor);
					strcpy_s(Lib[j].avtor, t);
				}
			}
		}
	cout << "\n Sorted by names in alphabetical order: \n";
	for (i = 0; i < n; i++) {
		cout << setiosflags(ios::left);
		cout <<"Cat.No."<<setw(5)<< Lib[i].catnum << "Autor: " <<setw(17)<< Lib[i].avtor << "Book: "<<setw(17) <<Lib[i].title<<"Year: "<<setw(7) <<Lib[i].year<<"Quantity: ";
		cout <<setw(4)<< Lib[i].br << "Borrowed: "<<setw(4) << Lib[i].zaem << "Available: "<<setw(4) << Lib[i].nalich<<endl;
	}
	_getch;
}

void sortbyyear(book Lib[], int n) {
	int i, j, temp;
	for (i = 0; i < n; i++) {
		for (j = i + 1; j < n; j++) {
			if (Lib[i].year < Lib[j].year) {
				temp = Lib[i].year;
				Lib[i].year = Lib[j].year;
				Lib[j].year = temp;
			}
		}
	}
	cout << "\n Sorted by year of publication (descending order) " << endl;
	for (i = 0; i < n; i++) {
		cout << setiosflags(ios::left);
		cout << "Cat.No." << setw(5) << Lib[i].catnum << "Year: " << setw(7) << Lib[i].year << "Avtor: " << setw(17) << Lib[i].avtor;
		cout << "Book: "<<setw(17) << Lib[i].title << "Quantity: " <<setw(4);
		cout << Lib[i].br << "Borrowed: " <<setw(4)<< Lib[i].zaem << "Available: "<<setw(4) << Lib[i].nalich <<endl;
	}
}


void printmax(book Lib[], int n) {
	int i, j, temp;
	for (i = 0; i < n; i++) {
		for (j = i + 1; j < n; j++) {
			if (Lib[i].zaem < Lib[j].zaem) {
				temp = Lib[i].zaem;
				Lib[i].zaem = Lib[j].zaem;
				Lib[j].zaem = temp;
			}
		}
	}
	cout << "\n Sorted by most borrowed" << endl;
	for (i = 0; i < n; i++) {
		cout << setiosflags(ios::left);
		cout << "Cat.No." << setw(5) << Lib[i].catnum << "Borrowed: " << setw(4) << Lib[i].zaem << "Available: " << setw(4); 
		cout << Lib[i].nalich << "Quantity: "<<setw(4)<< Lib[i].br;
		cout << "Autor: " << setw(17)<<Lib[i].avtor << "Book: "<<setw(17) << Lib[i].title << "Year: "<<setw(7) << Lib[i].year << endl;
	}

}


void printmin(book Lib[], int n) {
	int i, j, temp;
	for (i = 0; i < n; i++) {
		for (j = i + 1; j < n; j++) {
			if (Lib[i].zaem > Lib[j].zaem) {
				temp = Lib[i].zaem;
				Lib[i].zaem = Lib[j].zaem;
				Lib[j].zaem = temp;
			}
		}
	}
	cout << "\n Sorted by least borrowed" << endl;
	for (i = 0; i < n; i++) {
		cout << setiosflags(ios::left);
		cout << "Cat.No." << setw(5) << Lib[i].catnum << "Borrowed: " << setw(4) << Lib[i].zaem << "Available: " << setw(4);
		cout << Lib[i].nalich << "Quantity: " << setw(4) << Lib[i].br;
		cout << "Autor: " << setw(17) << Lib[i].avtor << "Book: " << setw(17) << Lib[i].title << "Year: " << setw(7) << Lib[i].year << endl;
	}
}



int main() {
	int choice, n = 0;
	book Lib[N];
	char ans = 'y';
	// n=loadfile(Lib);
	do {
		choice = menu();
		switch (choice) {
		case 2: n = enter(Lib, n); saveinfile(Lib, n); break;
		case 1: do {
			append();
			n++;
			cout << "One more?[y/n]: " << endl; cin >> ans;
		} while (!(ans == 'N' || ans == 'n')); break;
		case 3: n = loadfile(Lib); output(Lib, n); break;
		case 4: n = loadfile(Lib); book_on_name(Lib, n); break;
		case 5: n = loadfile(Lib); return_book_name(Lib, n); break;
		case 6: n = loadfile(Lib);  return_book_catn(Lib, n); break;
		case 7:
			int auchoice; // Choice for audit menu functions
			do { // Repeat until choice is 5
				auchoice = auditmenu(); // Choose an audit menu function
				switch (auchoice) {
				case 1: sortbyautorname(Lib, n); break;
				case 2: sortbyyear(Lib, n); break; 
				case 3: printmax(Lib, n);  break; 
				case 4: printmin(Lib, n); break; 
				case 5: break; 
				}
			} while (auchoice != 5);
			break; 
		case 8: saveinfile(Lib, n); break;
		}
	} while (choice != 9);
}