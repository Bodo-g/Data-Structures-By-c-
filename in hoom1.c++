#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>
#include <limits>

using namespace std;

const int MAX_SIZE = 100;

class Stack {
private:
    int arr[MAX_SIZE];
    int top;

public:
    Stack() {
        top = -1;
    }

    bool isEmpty() {
        return top == -1;
    }

    bool isFull() {
        return top == MAX_SIZE - 1;
    }

    void push(int value) {
        if (isFull()) {
            throw runtime_error("Stack overflow!");
        }
        top++;
        arr[top] = value;
    }

    int pop() {
        if (isEmpty()) {
            throw runtime_error("Stack underflow!");
        }
        int value = arr[top];
        top--;
        return value;
    }

    int peek() {
        if (isEmpty()) {
            throw runtime_error("Stack is empty!");
        }
        return arr[top];
    }
};

int precedence(char op) {
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

string infixToPostfix(const string &expression) {
    Stack st;
    string output = "";

    for (size_t i = 0; i < expression.length(); i++) {
        char c = expression[i];

        if (c == ' ') {
            continue;
        }

        if (isdigit(c)) {
            output += c;
            output += ' ';
        }
        else if (c == '(') {
            st.push(c);
        }
        else if (c == ')') {
            bool foundOpening = false;
            while (!st.isEmpty()) {
                int topChar = st.pop();
                if (topChar == '(') {
                    foundOpening = true;
                    break;
                }
                output += (char)topChar;
                output += ' ';
            }
            if (!foundOpening) {
                throw runtime_error("Error: Mismatched parentheses.");
            }
        }
        else if (isOperator(c)) {
            while (!st.isEmpty() && isOperator((char)st.peek()) &&
                   precedence((char)st.peek()) >= precedence(c)) {
                char op = (char)st.pop();
                output += op;
                output += ' ';
            }
            st.push(c);
        }
        else {
            throw runtime_error(string("Invalid character in expression: ") + c);
        }
    }

    while (!st.isEmpty()) {
        char op = (char)st.pop();
        if (op == '(' || op == ')') {
            throw runtime_error("Error: Mismatched parentheses.");
        }
        output += op;
        output += ' ';
    }

    return output;
}

int evaluatePostfix(const string &expression) {
    Stack st;

    for (size_t i = 0; i < expression.length(); i++) {
        char c = expression[i];

        if (c == ' ') {
            continue;
        }

        if (isdigit(c)) {
            int value = c - '0';
            st.push(value);
        }
        else if (isOperator(c)) {
            if (st.isEmpty()) {
                throw runtime_error("Error: Not enough operands.");
            }
            int right = st.pop();
            if (st.isEmpty()) {
                throw runtime_error("Error: Not enough operands.");
            }
            int left = st.pop();

            int result = 0;

            switch (c) {
                case '+':
                    result = left + right;
                    break;
                case '-':
                    result = left - right;
                    break;
                case '*':
                    result = left * right;
                    break;
                case '/':
                    if (right == 0) {
                        throw runtime_error("Error: Division by zero.");
                    }
                    result = left / right;
                    break;
            }

            st.push(result);
        }
        else {
            throw runtime_error(string("Invalid character in postfix expression: ") + c);
        }
    }

    if (st.isEmpty()) {
        throw runtime_error("Error: No result on stack.");
    }
    int finalResult = st.pop();

    if (!st.isEmpty()) {
        throw runtime_error("Error: Extra operands in expression.");
    }

    return finalResult;
}

int main() {
    try {
        cout << "Enter an infix expression (single-digit operands): ";
        string infix;

        if (cin.peek() == '\n') {
            cin.ignore();
        }

        getline(cin, infix);

        if (infix.empty()) {
            cout << "No expression entered. Exiting.\n";
        } else {
            string postfix = infixToPostfix(infix);
            int result = evaluatePostfix(postfix);

            cout << "Postfix expression: " << postfix << endl;
            cout << "Result: " << result << endl;
        }
    }
    catch (const exception &e) {
        cout << "Error: " << e.what() << endl;
    }

    cout << "\nPress Enter to exit...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    return 0;
}
