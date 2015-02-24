#include "main.h"

// Declare the Autonomous class
class Init {
private:
	// Field declarations
	unsigned int side;
	unsigned int index;
public:
	// Constant declarations
	static const unsigned int RED = 1;
	static const unsigned int BLUE = 2;
	// Method declarations
	Init(unsigned int s, unsigned int i) : side(s), index(i) { };
	void run(void);
};

// This function is in C++!
void Init::run(void) {
	printf("Ran script %d!\n", index);
	motorSet(1, 127);
	delay(1000);
	motorSet(1, 0);
}

// Important!
extern "C" {

void initializeIO()
{

}

void initialize()
{
	Init myInit(Init::RED, 4);
	myInit.run();
	// operator "new" and "delete" are broken so C++ isn't quite ready yet
}

}
// End extern "C"
