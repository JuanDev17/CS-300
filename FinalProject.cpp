//============================================================================
// Name        : FinalProject.cpp
// Author      : Juan Aviles 
// Version     : 1.0
// Copyright   : Copyright � 2017 SNHU COCE
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

using namespace std;

const int GLOBAL_SLEEP_TIME = 5000; // milliseconds

struct Course {
    string courseId;
    string courseName;
    vector<string> preList;
};

class BinarySearchTree {
private:
    struct Node {
        Course course;
        Node* right;
        Node* left;

        Node() : left(nullptr), right(nullptr) {}
        Node(Course aCourse) : course(aCourse), left(nullptr), right(nullptr) {}
    };

    Node* root;
    void inOrder(Node* node);
    int size = 0;

public:
    BinarySearchTree();
    void InOrder();
    void Insert(Course aCourse);
    void Remove(string courseId);
    Course Search(string courseId);
    int Size();
};

BinarySearchTree::BinarySearchTree() {
    this->root = nullptr;
}

void BinarySearchTree::InOrder() {
    inOrder(root);
}

void BinarySearchTree::Insert(Course aCourse) {
    Node* currentNode = root;
    if (root == nullptr) {
        root = new Node(aCourse);
    } else {
        while (currentNode != nullptr) {
            if (aCourse.courseId < currentNode->course.courseId) {
                if (currentNode->left == nullptr) {
                    currentNode->left = new Node(aCourse);
                    break;
                } else {
                    currentNode = currentNode->left;
                }
            } else {
                if (currentNode->right == nullptr) {
                    currentNode->right = new Node(aCourse);
                    break;
                } else {
                    currentNode = currentNode->right;
                }
            }
        }
    }
    size++;
}

void BinarySearchTree::Remove(string courseId) {
    Node* par = nullptr;
    Node* curr = root;

    while (curr != nullptr) {
        if (curr->course.courseId == courseId) {
            if (curr->left == nullptr && curr->right == nullptr) {
                if (par == nullptr) root = nullptr;
                else if (par->left == curr) par->left = nullptr;
                else par->right = nullptr;
            } else if (curr->right == nullptr) {
                if (par == nullptr) root = curr->left;
                else if (par->left == curr) par->left = curr->left;
                else par->right = curr->left;
            } else if (curr->left == nullptr) {
                if (par == nullptr) root = curr->right;
                else if (par->left == curr) par->left = curr->right;
                else par->right = curr->right;
            } else {
                Node* suc = curr->right;
                while (suc->left != nullptr) suc = suc->left;
                Node successorData = Node(suc->course);
                Remove(suc->course.courseId);
                curr->course = successorData.course;
            }
            return;
        } else if (curr->course.courseId < courseId) {
            par = curr;
            curr = curr->right;
        } else {
            par = curr;
            curr = curr->left;
        }
    }
    cout << "\nValue not found" << endl;
}

Course BinarySearchTree::Search(string courseId) {
    Course aCourse;
    Node* currentNode = root;

    while (currentNode != nullptr) {
        if (currentNode->course.courseId == courseId) {
            return currentNode->course;
        } else if (courseId < currentNode->course.courseId) {
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }
    }
    return aCourse;
}

void BinarySearchTree::inOrder(Node* node) {
    if (node == nullptr) return;
    inOrder(node->left);
    cout << node->course.courseId << ", " << node->course.courseName << endl;
    inOrder(node->right);
}

int BinarySearchTree::Size() {
    return size;
}

vector<string> Split(string lineFeed) {
    char delim = ',';
    lineFeed += delim;
    vector<string> lineTokens;
    string temp = "";

    for (int i = 0; i < lineFeed.length(); i++) {
        if (lineFeed[i] == delim) {
            lineTokens.push_back(temp);
            temp = "";
            i++;
        }
        temp += lineFeed[i];
    }
    return lineTokens;
}

void loadCourses(string csvPath, BinarySearchTree* courseList) {
    ifstream inFS;
    string line;
    vector<string> stringTokens;

    inFS.open(csvPath);

    if (!inFS.is_open()) {
        cout << "Could not open file. Please check inputs. " << endl;
        return;
    }

    while (!inFS.eof()) {
        Course aCourse;
        getline(inFS, line);
        stringTokens = Split(line);

        if (stringTokens.size() < 2) {
            cout << "\nError. Skipping line." << endl;
        } else {
            aCourse.courseId = stringTokens.at(0);
            aCourse.courseName = stringTokens.at(1);
            for (unsigned int i = 2; i < stringTokens.size(); i++) {
                aCourse.preList.push_back(stringTokens.at(i));
            }
            courseList->Insert(aCourse);
        }
    }

    inFS.close();
}

void displayCourse(Course aCourse) {
    cout << aCourse.courseId << ", " << aCourse.courseName << endl;
    cout << "Prerequisites: ";

    if (aCourse.preList.empty()) {
        cout << "none" << endl;
    } else {
        for (unsigned int i = 0; i < aCourse.preList.size(); i++) {
            cout << aCourse.preList.at(i);
            if (aCourse.preList.size() > 1 && i < aCourse.preList.size() - 1) {
                cout << ", ";
            }
        }
    }

    cout << endl;
}

void convertCase(string &toConvert) {
    for (unsigned int i = 0; i < toConvert.length(); i++) {
        if (isalpha(toConvert[i])) {
            toConvert[i] = toupper(toConvert[i]);
        }
    }
}

// Cross-platform sleep
void sleepMs(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// Cross-platform clear screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main(int argc, char* argv[]) {
    string csvPath, aCourseKey;
    switch (argc) {
        case 2:
            csvPath = argv[1];
            break;
        case 3:
            csvPath = argv[1];
            aCourseKey = argv[2];
            break;
        default:
            csvPath = "FinalProject_Sample_input.csv";
    }

    BinarySearchTree* courseList = new BinarySearchTree();
    Course course;
    bool goodInput;
    int choice = 0;

    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses" << endl;
        cout << "  3. Find Course" << endl;
        cout << "  4. Remove Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";

        aCourseKey = "";
        string anyKey = " ";
        choice = 0;

        try {
            cin >> choice;
            if ((choice > 0 && choice < 5) || (choice == 9)) {
                goodInput = true;
            } else {
                goodInput = false;
                throw 1;
            }

            switch (choice) {
                case 1:
                    loadCourses(csvPath, courseList);
                    cout << courseList->Size() << " courses read" << endl;
                    sleepMs(GLOBAL_SLEEP_TIME);
                    break;

                case 2:
                    courseList->InOrder();
                    cout << "\nEnter 'y' to continue..." << endl;
                    cin >> anyKey;
                    break;

                case 3:
                    cout << "\nWhat course do you want to know about? " << endl;
                    cin >> aCourseKey;
                    convertCase(aCourseKey);
                    course = courseList->Search(aCourseKey);
                    if (!course.courseId.empty()) {
                        displayCourse(course);
                    } else {
                        cout << "\nCourse ID " << aCourseKey << " not found." << endl;
                    }
                    sleepMs(GLOBAL_SLEEP_TIME);
                    break;

                case 4:
                    cout << "\nWhat course do you want delete? " << endl;
                    cin >> aCourseKey;
                    convertCase(aCourseKey);
                    courseList->Remove(aCourseKey);
                    sleepMs(GLOBAL_SLEEP_TIME);
                    break;

                case 9:
                    break;

                default:
                    throw 2;
            }
        } catch (int err) {
            cout << "\nPlease check your input." << endl;
            sleepMs(GLOBAL_SLEEP_TIME);
        }

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        clearScreen();
    }

    cout << "Good bye." << endl;
    sleepMs(GLOBAL_SLEEP_TIME);

    return 0;
}
