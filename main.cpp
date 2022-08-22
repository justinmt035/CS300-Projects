//Justin M Taylor
//Professor Atkison
//CS-300 Project 2
//8-14-2022

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

//This program takes a csv file as input and stores the data in a BST,
//then allows the user to interact with the BST. 

//structure for course info
struct Course {
	string courseId;
	string courseName;
	string coursePreReq1;
	string coursePreReq2;

	//constructors
	Course() {
	}

	Course(string courseId, string courseName, string coursePreReq1, string coursePreReq2) {
		this->courseId = courseId;
		this->courseName = courseName;
		this->coursePreReq1 = coursePreReq1;
		this->coursePreReq2 = coursePreReq2;
	}
};

//BST nodes
struct Node {
	Course course;
	Node* left;
	Node* right;

	Node() {
		left = nullptr;
		right = nullptr;
	}

	Node(Course someCourse) : Node() {
		this->course = someCourse;
	}
};

//BST functions
class BST {

private:
	Node* root;

	void addNode(Node* node, Course someCourse);
	Node* removeNode(Node* node, string courseId);
	void inOrder(Node* node);
	void preOrder(Node* node);
	void postOrder(Node* node);
	void bstDestructor(Node* node);

public:
	BST();
	virtual ~BST();
	void Insert(Course someCourse);
	void Remove(string courseName);
	void InOrder();
	void PreOrder();
	void PostOrder();
	void Search(string courseId);
};

BST::BST() {
	root = nullptr;
}

BST::~BST() {
	bstDestructor(root);
}

void BST::Insert(Course someCourse) {
	if (root == nullptr) {
		root = new Node(someCourse);
	}
	else {
		this->addNode(root, someCourse);
	}
}

void BST::Remove(string courseId) {
	this->removeNode(root, courseId);
}

void BST::InOrder() {
	this->inOrder(root);
}

void BST::PreOrder() {
	this->preOrder(root);
}

void BST::PostOrder() {
	this->postOrder(root);
}

void BST::Search(string courseId) {
	Node* currNode = root;

	while (currNode != nullptr) {
		if (currNode->course.courseId.compare(courseId) == 0) {
			cout << endl;
			cout << currNode->course.courseId << ": "
				 << currNode->course.courseName << " | Prerequisites: ";
			//check for prereqs
			if (currNode->course.coursePreReq1 == "") {
				cout << "None" << endl;
			}
			else {
				cout << currNode->course.coursePreReq1 << " "
					 << currNode->course.coursePreReq2 << " " << endl;
			}
			cout << endl;
		}

		//traverse
		if (courseId.compare(currNode->course.courseId) < 0) {
			currNode = currNode->left;
		}
		else {
			currNode = currNode->right;
		}
	}
}

void BST::addNode(Node* node, Course someCourse) {
	if (node->course.courseId.compare(someCourse.courseId) > 0) {
		if (node->left == nullptr) {
			node->left = new Node(someCourse);
		}
		else {
			this->addNode(node->left, someCourse);
		}
	}
	else {
		if (node->right == nullptr) {
			node->right = new Node(someCourse);
		}
		else {
			this->addNode(node->right, someCourse);
		}
	}
}

Node* BST::removeNode(Node* node, string courseId) {
	while (node != nullptr) {
		if (courseId.compare(node->course.courseId) < 0) {
			node->left = removeNode(node->left, courseId);
		}
		else if (courseId.compare(node->course.courseId) > 0) {
			node->right = removeNode(node->right, courseId);
		}
		else {
			if (node->left == nullptr && node->right == nullptr) {
				delete node;
				node = nullptr;
			}
			else if (node->left != nullptr && node->right == nullptr) {
				Node* temp = node;
				node = node->right;
				delete temp;
			}
			else {
				Node* temp = node->right;
				while (temp->left != nullptr) {
					temp = temp->left;
				}
				node->course = temp->course;
				node->right = removeNode(node->right, temp->course.courseId);
			}
		}
		return node;
	}
}

void BST::inOrder(Node* node) {
	if (node != nullptr) {
		inOrder(node->left);
		cout << node->course.courseId << ": "
			<< node->course.courseName << " | Prerequisites: ";
		if (node->course.coursePreReq1 == "") {
			cout << "None" << endl;
		}
		else {
			cout << node->course.coursePreReq1 << " "
				<< node->course.coursePreReq2 << " " << endl;
		}
		inOrder(node->right);
	}

}
void BST::preOrder(Node* node) {
	if (node != nullptr) {
		cout << node->course.courseId << ": "
			<< node->course.courseName << " | "
			<< node->course.coursePreReq1 << endl;
		preOrder(node->left);
		preOrder(node->right);
	}
}

void BST::postOrder(Node* node) {
	if (node != nullptr) {
		postOrder(node->left);
		postOrder(node->right);
		cout << node->course.courseId << ": "
			<< node->course.courseName << " | "
			<< node->course.coursePreReq1 << endl;
	}
}

void BST::bstDestructor(Node* node) {
	if (node) {
		bstDestructor(node->left);
		bstDestructor(node->right);
		delete node;
	}
}

//main() functions
void displayCourse(Course course) {
	cout << course.courseId << ": "
		<< course.courseName << " | Prerequisites: ";
	if (course.coursePreReq1 == "") {
		cout << "None" << endl;
	}
	else {
		cout << course.coursePreReq1 << " "
			<< course.coursePreReq2 << " " << endl;
	}

	return;
}

int main(int argc, char* argv) {

	BST* bst;
	bst = new BST();
	string courseChoice;
	string csvPath = "Courses.csv";
	ifstream csvFile;
	string line;
	//uncomment for testing parser
	//vector<Course> coursesVector;

	//main menu
	int choice = 0;
	while (choice != 9) {
		cout << "Menu: " << endl;
		cout << "  1. Load Data Structure" << endl;
		cout << "  2. Print Course List" << endl;
		cout << "  3. Print Course" << endl;
		cout << "  9. Exit" << endl;
		cout << endl;
		cout << "Enter choice: ";
		cin >> choice;

		switch (choice) {
		case 1:
			//get the filename
			cout << "Enter a filename: ";
			cin >> courseChoice;

			//check if file open
			csvFile.open(courseChoice);
			if (!csvFile.is_open()) {
				cout << endl;
				cout << "Couldn't open the file!" << endl;
				cout << endl;
			}
			else {
				cout << endl;
				cout << "File opened successfully!" << endl;
				cout << endl;
			}

			//parse lines and store in vector
			while (getline(csvFile, line)) {

				string courseID;
				string courseName;
				string coursePreReq1;
				string coursePreReq2;

				stringstream inputString(line);
				getline(inputString, courseID, ',');
				getline(inputString, courseName, ',');
				//check for prereqs
				if (!inputString.fail()) {
					getline(inputString, coursePreReq1, ',');
					if (!inputString.fail()) {
						getline(inputString, coursePreReq2, ',');
					}
				}

				Course course(courseID, courseName, coursePreReq1, coursePreReq2);
				//uncomment for testing with vector
				//coursesVector.push_back(course); 
				bst->Insert(course);

				//clear the line
				line = "";
			}

			//uncomment for testing with vector
			//for (auto course : coursesVector) {
			//	displayCourse(course);
			//}
			//cout << endl;

			break;

		case 2:
			cout << endl;
			bst->InOrder();
			//bst->PreOrder();
			//bst->PostOrder();
			cout << endl;
			break;

		case 3:
			cout << endl;
			cout << "Enter a course ID: ";
			cin >> courseChoice;
			bst->Search(courseChoice);

		case 9:
			break;

		default:
			cout << endl;
			cout << "Not a valid input!" << endl;
			cout << endl;
		}
	}

	return 0;
}
