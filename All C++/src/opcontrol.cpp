#include "main.h"

// Declare the Autonomous class
class OpControl {
private:
	// Field declarations
	unsigned int side;
	unsigned int index;
public:
	// Constant declarations
	static const unsigned int RED = 1;
	static const unsigned int BLUE = 2;
	// Method declarations
	OpControl(unsigned int s, unsigned int i) : side(s), index(i) { };
	void run(void);
};

// This function is in C++!
void OpControl::run(void) {
	printf("Ran script %d!\n", index);
	motorSet(1, 127);
	delay(1000);
	motorSet(1, 0);
}

// Important!
extern "C" {

void opControl() {
	OpControl myTeleop(OpControl::RED, 4);
	myTeleop.run();
	// operator "new" and "delete" are broken so C++ isn't quite ready yet
}

}
// End extern "C"
