#include <string>

using namespace std;

class Process {
private:
    string name;
    char operation;
    int leftOperand;
    int rightOperand;
    int result;
    unsigned int estimatedTime;
    unsigned int elapsedTime;
    unsigned int programNumber;

public:
    void setName(string& name);
    string getName();

    void setOperation(char operation);
    char getOperation();

    void setLeftOperand(int leftOperand);
    int getLeftOperand();

    void setRightOperand(int rightOperand);
    int getRightOperand();

    int getResult();

    int run();
};
