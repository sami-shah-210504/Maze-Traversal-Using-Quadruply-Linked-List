#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class node {
public:
    int data;
    node* down;
    node* right;
    bool visitFlag;

    node(int value) : data(value), right(nullptr), down(nullptr), visitFlag(false) {}
    node() : data(0), right(nullptr), down(nullptr), visitFlag(false) {}
};

bool isVisited(node* currentNode) {
    return currentNode->visitFlag;
}

void markVisited(node* currentNode) {
    currentNode->visitFlag = true;
}

int numDigits(int data) {
    int count = 0;
    while (data > 0) {
        data /= 10;
        count++;
    }
    return count;
}

int digitSum(int data) {
    int sum = 0;
    while (data > 0) {
        sum += (data % 10);
        data /= 10;
    }
    return sum;
}

int mazeRows(node* head) {
    node* tempNode = head;
    int rowCount = 0;
    if (!head) { return rowCount; }
    while (tempNode) {
        tempNode = tempNode->down;
        rowCount++;
    }
    return rowCount;
}

int mazeCols(node* head) {
    node* tempNode = head;
    int colCount = 0;
    if (!head) { return colCount; }
    while (tempNode) {
        tempNode = tempNode->right;
        colCount++;
    }
    return colCount;
}

void visited(node* currentNode) {
    cout << currentNode->data << "  ";
}

void print(node* head) {
    if (!head) { return; }

    node* rowNode = head;
    while (rowNode) {
        node* tempNode = rowNode;
        while (tempNode) {
            visited(tempNode);
            tempNode = tempNode->right;
        }
        cout << endl;
        rowNode = rowNode->down;
    }
}

int clueRow(int data, node* head) {
    return (digitSum(data) % mazeRows(head)) + 1;
}

int clueColumn(int data) {
    return numDigits(data);
}

void eliteNode(node* elite) {
    if (!elite) {
        cout << "Elite node doesn't exist... :(" << endl;
    }
    else {
        cout << "Yaaaayyyyy! Elite node found! :D \nData in elite node: ";
        visited(elite);
        cout << endl;
    }
}

node* readMaze(const string& filename, int& totalRows, int& totalCols) {
    ifstream file(filename);
    string line;
    node* head = nullptr;
    node* prevRowStart = nullptr;

    totalRows = 0;
    totalCols = 0;

    while (getline(file, line)) {
        stringstream ss(line);
        string value;
        node* rowStart = nullptr;
        node* prevNode = nullptr;

        totalCols = 0;
        while (getline(ss, value, ',')) { // Reads file storing integer values by ignoring commas
            int num = stoi(value);
            node* newNode = new node(num);

            if (rowStart == nullptr) {
                rowStart = newNode;
            }

            if (prevNode != nullptr) {
                prevNode->right = newNode;
            }

            prevNode = newNode;
            totalCols++; // Adds a column based on value collected
        }

        if (prevRowStart != nullptr) {
            node* currentRow = rowStart; // Moves to next row
            node* prevRow = prevRowStart;

            while (prevRow && currentRow) {
                prevRow->down = currentRow; //Links the nodes in the row above the current row to the nodes in the current row
                prevRow = prevRow->right;
                currentRow = currentRow->right;
            }
        }

        prevRowStart = rowStart;

        if (!head) {
            head = rowStart;
        }

        totalRows++;
    }

    return head;
}

void traverseMaze(node* head, int totalRows, int totalCols) {
    node* currentNode = head;

    while (currentNode) {
        visited(currentNode);

        if (isVisited(currentNode)) {
            cout << "This node has already been visited! :/" << endl;
            break;
        }

        markVisited(currentNode);

        int nextRow = clueRow(currentNode->data, head);
        int nextCol = clueColumn(currentNode->data);

        if (nextRow >= 1 && nextRow <= totalRows && nextCol >= 1 && nextCol <= totalCols) {
            node* tempNode = head;

            for (int i = 1; i < nextRow && tempNode; i++) {
                tempNode = tempNode->down;
            }

            for (int j = 1; j < nextCol && tempNode; j++) {
                tempNode = tempNode->right;
            }

            currentNode = tempNode;
        }
        else {
            cout << "Out of bounds!" << endl;
            break;
        }

        if (currentNode && isVisited(currentNode)) {
            eliteNode(currentNode);
            break;
        }
    }
}

int main() {
    string filename = "maze.txt";
    int totalRows, totalCols;

    node* mazeHead = readMaze(filename, totalRows, totalCols);

    cout << "Maze structure:" << endl;
    print(mazeHead);

    traverseMaze(mazeHead, totalRows, totalCols);

    return 0;
}
