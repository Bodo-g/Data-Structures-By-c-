#include <iostream>
#include <string>
#include <cctype>   // عشان isdigit
#include <stdexcept> // عشان نستخدم exceptions
#include <limits>   // عشان numeric_limits

using namespace std;

// أقصى حجم للـ stack لو استخدمنا array
const int MAX_SIZE = 100;

// =========================
// Stack Implementation (Array)
// =========================
class Stack {
private:
    int arr[MAX_SIZE]; // مصفوفة لتخزين عناصر الستاك
    int top;           // مؤشر لأعلى عنصر (index)

public:
    Stack() {
        top = -1; // الستاك فاضي في البداية
    }

    // هل الستاك فاضي؟
    bool isEmpty() {
        return top == -1;
    }

    // هل الستاك مليان؟ (لأننا مستخدمين مصفوفة بحجم ثابت)
    bool isFull() {
        return top == MAX_SIZE - 1;
    }

    // إدخال عنصر أعلى الستاك
    void push(int value) {
        if (isFull()) {
            throw runtime_error("Stack overflow!"); // خطأ لو الستاك مليان
        }
        top++;
        arr[top] = value;
    }

    // إخراج العنصر الأعلى وإرجاعه
    int pop() {
        if (isEmpty()) {
            throw runtime_error("Stack underflow!"); // خطأ لو الستاك فاضي
        }
        int value = arr[top];
        top--;
        return value;
    }

    // إرجاع العنصر الأعلى بدون حذفه
    int peek() {
        if (isEmpty()) {
            throw runtime_error("Stack is empty!");
        }
        return arr[top];
    }
};

// =========================
// دوال مساعدة لـ infix → postfix
// =========================

// دالة ترجع أولوية الـ operator
int precedence(char op) {
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0; // أي حاجة تانية (مش أوبيراتور أساسي)
}

// هل الحرف ده operator؟
bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

// تحويل infix إلى postfix
string infixToPostfix(const string &expression) {
    Stack st;            // ستاك لخزن الـ operators
    string output = "";  // هنا هنجمع postfix

    for (size_t i = 0; i < expression.length(); i++) {
        char c = expression[i];

        // تجاهل المسافات لو المستخدم كتبها
        if (c == ' ') {
            continue;
        }

        // لو رقم (operand) - هنا افترضنا single-digit
        if (isdigit(c)) {
            // نضيفه مباشرة للـ output
            output += c;
            output += ' '; // مسافة بين كل رقم والتاني
        }
        // لو '(' نحطه في الستاك
        else if (c == '(') {
            st.push(c);
        }
        // لو ')' نطلع لحد ما نوصل لـ '('
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
        // لو operator
        else if (isOperator(c)) {
            // نطلع من الستاك أي operators لهم أولوية أعلى أو مساوية
            while (!st.isEmpty() && isOperator((char)st.peek()) &&
                   precedence((char)st.peek()) >= precedence(c)) {
                char op = (char)st.pop();
                output += op;
                output += ' ';
            }
            // بعد كده نحط الأوبيراتور الحالي
            st.push(c);
        }
        else {
            // حرف مش متوقع
            throw runtime_error(string("Invalid character in expression: ") + c);
        }
    }

    // بعد ما نخلص الـ expression نفضي الستاك
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

// =========================
// تقييم Postfix
// =========================

int evaluatePostfix(const string &expression) {
    Stack st; // ستاك للأرقام

    for (size_t i = 0; i < expression.length(); i++) {
        char c = expression[i];

        // تجاهل المسافات
        if (c == ' ') {
            continue;
        }

        // لو رقم، نحوله إلى int ونحطه في الستاك
        if (isdigit(c)) {
            int value = c - '0'; // تحويل char رقم لـ int
            st.push(value);
        }
        // لو operator نطلع رقمين من الستاك
        else if (isOperator(c)) {
            if (st.isEmpty()) {
                throw runtime_error("Error: Not enough operands.");
            }
            int right = st.pop(); // operand الثاني
            if (st.isEmpty()) {
                throw runtime_error("Error: Not enough operands.");
            }
            int left = st.pop();  // operand الأول

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

            // نحط النتيجة في الستاك
            st.push(result);
        }
        else {
            // حرف غير متوقع في postfix
            throw runtime_error(string("Invalid character in postfix expression: ") + c);
        }
    }

    // في الآخر لازم يكون عندنا قيمة واحدة بس في الستاك
    if (st.isEmpty()) {
        throw runtime_error("Error: No result on stack.");
    }
    int finalResult = st.pop();

    if (!st.isEmpty()) {
        throw runtime_error("Error: Extra operands in expression.");
    }

    return finalResult;
}

// =========================
// main program
// =========================
int main() {
    try {
        cout << "Enter an infix expression (single-digit operands): ";
        string infix;

        // لو فيه newline قديم في الـ input buffer نشيله
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

    // ⬇⬇ الجزء المهم عشان الـ CMD مايقفلش ⬇⬇
    cout << "\nPress Enter to exit...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    return 0;
}

