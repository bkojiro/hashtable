#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <ctime>
#include <bits/stdc++.h>

using namespace std;

struct Student {
  char firstName[20];
  char lastName[20];
  int studentID = 0;
  float gpa = 0.0;
};

struct Node {
  Node* next = NULL;
  Student* stud = NULL;
};

void hashFunction(Student* newStud, Node** &hashT, int &size);
void rehash(Node** &hashT, int &size);
Student* addStudent();
void printStudents(Node** hashT, int size);
void deleteStudent(Node** &hashT, int size);
Student* generate(int &startID, Node** hashT, int size);

int main() {
  int size = 101;
  int startingID = 100000;
  Node** hashTable = new Node*[size];
  for (int i = 0; i < size; i++) {
    hashTable[i] = NULL;
  }
  
  srand(time(0));
  bool active = true;
  while (active) {
    cout << "Type ADD, PRINT, DELETE, GENERATE, or QUIT" << endl << "> ";
    //read in what user wants to do
    char action[10];
    cin.get(action, 11);
    cin.get();
    if (strcmp(action, "ADD") == 0) { //adding a new student to table
      Student* newStud = addStudent();
      hashFunction(newStud, hashTable, size);
    } else if (strcmp(action, "PRINT") == 0) { //printing the table
      printStudents(hashTable, size);
    } else if (strcmp(action, "DELETE") == 0) { //deleting a student from the table
      deleteStudent(hashTable, size);
    } else if (strcmp(action, "GENERATE") == 0) {
      cout << "How many students would you like to generate?" << endl << "> ";
      int genStudents = 0;
      cin >> genStudents;
      cin.ignore();
      for (int i = 0; i < genStudents; i++) {
	Student* newStud = generate(startingID, hashTable, size);
	hashFunction(newStud, hashTable, size);
      }
    } else if (strcmp(action, "QUIT") == 0) { //ending the program
      cout << "Program terminated";
      return 0;
    } else {
      cout << "Invalid input. Please check your capitalization and spelling" << endl;
    }
  }
  return 0;
}

void hashFunction(Student* newStud, Node** &hashT, int &size) { //map student id to key, then place in hash table
  int hashKey = newStud->studentID % size;
  Node* tempNode = new Node();
  tempNode->next = hashT[hashKey];
  tempNode->stud = newStud;
  hashT[hashKey] = tempNode;
  
  //check how many collisions have occured
  int collisions = 0;
  Node* current = hashT[hashKey];
  while (current->next != NULL) {
    collisions++;
    current = current->next;
  }
  //if there is 4 nodes for one index, reshash data into new table
  if (collisions >= 1) {
    rehash(hashT, size);
  }
}

void rehash(Node** &hashT, int &size) {
  cout << "NOTE: TABLE HAS BEEN REHASHED" << endl;
  int newSize = 2 * size - 1;
  Node** newHash = new Node*[newSize];
  for (int i = 0; i < newSize; i++) {
    newHash[i] = NULL;
  }
  for (int a = 0; a < size; a++) {
    Node* current = hashT[a];
    if (current != NULL) {
      hashFunction(current->stud, newHash, newSize);
      while (current->next != NULL) {
	current = current->next;
	hashFunction(current->stud, newHash, newSize);
      }
    }
  }
  for (int a = 0; a < size; a++) { //delete nodes
    Node* current = hashT[a];
    if (current != NULL) {
      Node* next = current->next;
      delete current;
      current = next;
    }
  }
  size = newSize;
  delete hashT;
  hashT = newHash;
}
    
Student* addStudent() {
  Student* newStudent = new Student();
  cout << "First Name:" << endl << "> ";
  cin.get(newStudent->firstName, 21);
  cin.get();
  cout << "Last Name:" << endl << "> ";
  cin.get(newStudent->lastName, 21);
  cin.get();
  cout << "Student ID" << endl << "> ";
  cin >> newStudent->studentID;
  cin.ignore();
  cout << "GPA" << endl << "> ";
  cin >> newStudent->gpa;
  cin.ignore();
  cout << newStudent->firstName << " " << newStudent->lastName << ", " << newStudent->studentID << ", " << fixed << setprecision(1) << newStudent->gpa << endl;
  return newStudent;
}
  
void printStudents(Node** hashT, int size) { //print all entries
  for (int a = 0; a < size; a++) {
    Node* current = hashT[a];
    while (current != NULL) {
      cout << current->stud->firstName << " " << current->stud->lastName << ", " << current->stud->studentID << ", " << fixed << setprecision(1) << current->stud->gpa << endl;
      current = current->next;
    }
  }
}

void deleteStudent(Node** &hashT, int size) { //find student based on id, then DELETE DATA!!
  cout << "Enter the ID that you want to delete" << endl << "> ";
  int deleteID = 0;
  cin >> deleteID;
  cin.ignore();
  int deleteKey = deleteID % size;
  Node* current = hashT[deleteKey];
  while (current != NULL) {
    if (deleteID == current->stud->studentID) {
      //if head is to be deleted, don't need to relink previous node
      Node* temp = current;
      current = current->next;
      delete temp;
      cout << "deleted";
    } else if (current->next != NULL) {
      if (deleteID == current->next->stud->studentID) {
	//if the next node is to be deleted
	Node* temp = current->next;
	current->next = current->next->next;
	delete temp;
      } else {
	current = current->next;
      }
    }
  }
}

Student* generate(int &startID, Node** hashT, int size) {
  ifstream first;
  ifstream last;
  first.open("firsts.txt");
  last.open("lasts.txt");
  int line_num = rand() % 50 + 1;
  int currentline = 0;
  string line;

  Student* tempStudent = new Student();

  while (!first.eof()) {
    currentline++;
    getline(first, line);
    if (currentline == line_num) break;
  }
  strcpy(tempStudent->firstName, line.c_str()); //https://www.geeksforgeeks.org/convert-string-char-array-cpp/
  currentline = 0;
  line_num = rand() % 50 + 1;
  while (!last.eof()) {
    currentline++;
    getline(last, line);
    if (currentline == line_num) break;
  }
  strcpy(tempStudent->lastName, line.c_str());

  /* this code was meant to make sure my id's key wasn't already in the table which i thought was pretty cool but defeats the purpose of collisions
  int tempid = 100000; //auto-set id to a 6 digit number
  int idKey = tempid % size;
  bool validid = false;
  while (validid == false) {
    if (hashT[idKey] == NULL) {
      validid = true;
    } else {
      tempid++;
      idKey = tempid % size;
    }
  }
  */

  tempStudent->studentID = startID;
  startID++;
  tempStudent->gpa = (float) (rand() % 40 + 1) / 10;
  //cout << tempStudent-> firstName << " " << tempStudent->lastName << ", " << tempStudent->studentID << ", " << fixed << setprecision(1) << tempStudent->gpa << endl;
  return tempStudent;
}
