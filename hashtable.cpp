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

void hashFunction(Student* newStud, Node* (&hashT)[], int size);
void rehash(Node* (&hashT)[], int size);
Student* addStudent();
//void printStudents(vector<Student*> studentVect);
//void deleteStudent(vector<Student*> &studentVect);
Student* generate(Node* hashT[], int size);

int main() {
  Node* hashTable[101];
  for (int i = 0; i < sizeof(hashTable)/sizeof(hashTable[0]); i++) {
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
    if (strcmp(action, "ADD") == 0) { //adding a new student to vector
      Student* newStud = addStudent();
      hashFunction(newStud, hashTable, sizeof(hashTable)/sizeof(hashTable[0]));
    } else if (strcmp(action, "PRINT") == 0) { //printing the vector
      //printStudents(studentVect);
    } else if (strcmp(action, "DELETE") == 0) { //deleting a student from the vector
      //deleteStudent(studentVect);
    } else if (strcmp(action, "GENERATE") == 0) {
      cout << "How many students would you like to generate?" << endl << "> ";
      int genStudents = 0;
      cin >> genStudents;
      cin.ignore();
      for (int i = 0; i < genStudents; i++) {
	Student* newStud = generate(hashTable, sizeof(hashTable)/sizeof(hashTable[0]));
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

void hashFunction(Student* newStud, Node* (&hashT)[], int size) { //map student id to key, then place in hash table
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
  cout << collisions;
  //if there is 4 nodes for one index, reshash data into new table
  if (collisions >= 3) {
    rehash(hashT, size);
  }
}

void rehash(Node* (&hashT)[], int size) {
  //brute force new prime # for array size?
  int newSize = 0;
  if (size < 211) {
    newSize = 211;
  } else if (size < 431) {
    newSize = 431;
  } else if (size < 877) {
    newSize = 877;
  }
  Node* newHash[newSize];
  for (int i = 0; i < newSize; i++) {
    newHash[i] = NULL;
  }
  for (int a = 0; a < size; a++) {
    if (hashT[a] != NULL) {
      hashFunction(hashT[a]->stud, newHash, newSize);
    }
  }
  hashT = newHash; //find out how to do this
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
/*  
void printStudents(vector<Student*> studentVect) { //go through vector and print all info
  vector<Student*>::iterator iter; //iteration code from https://stackoverflow.com/questions/23318875/iterating-through-a-vector-of-pointers
  for (iter = studentVect.begin(); iter < studentVect.end(); iter++) {
    cout << (*iter)->firstName << " " << (*iter)->lastName << ", " << (*iter)->studentID << ", " << (*iter)->gpa << endl;
  }
}

void deleteStudent(vector<Student*> &studentVect) { //delete student based on id, then DELETE DATA!!
  cout << "Enter the ID that you want to delete" << endl << "> ";
  int deleteID = 0;
  cin >> deleteID;
  cin.ignore();
  int count = 0;
  vector<Student*>::iterator iter;
  for (iter = studentVect.begin(); iter < studentVect.end(); iter++) {
    if (deleteID == (*iter)->studentID) {
      cout << (*iter)->firstName << " " << (*iter)->lastName << " was removed from the program" << endl;
      delete *iter;
      studentVect.erase(iter);
      return;
    }
  }
  cout << "Student ID not found" << endl;
}
*/
Student* generate(Node* hashT[], int size) {
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

  int tempid = 100000;
  int idKey = tempid % size;
  bool validid = false;
  while (validid == false) {
    if (hashT[idKey] == NULL) {
      validid = true;
    } else {
      //cout << hashT[idKey]->stud->firstName;
      tempid++;
      idKey = tempid % size;
      //cout << tempid % size << "< ";
      if (hashT[tempid % size] == NULL) cout << tempid;
    }
  }
  tempStudent->studentID = tempid; //RANDOMIZE THIS
  tempStudent->gpa = (float) (rand() % 40 + 1) / 10;
  //cout << tempStudent-> firstName << " " << tempStudent->lastName << ", " << tempStudent->studentID << ", " << fixed << setprecision(1) << tempStudent->gpa << endl;
  return tempStudent;
}
