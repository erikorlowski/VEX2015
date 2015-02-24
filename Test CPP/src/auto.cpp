#include "AutoClasses.hpp"

// Declare the Autonomous class
class Autonomous {
private:
	// Field declarations
	unsigned int side;
	unsigned int index;
public:
	// Constant declarations
	static const unsigned int RED = 1;
	static const unsigned int BLUE = 2;
	// Method declarations
	Autonomous(unsigned int s, unsigned int i) : side(s), index(i) { };
	void run(void);
};

// This function is in C++!
void Autonomous::run(void) {
	printf("Ran script %d!\n", index);
	motorSet(1, 127);
	delay(1000);
	motorSet(1, 0);
}

// Important!
extern "C" {

void autonomous() {
	Autonomous myAuto(Autonomous::RED, 4);
	myAuto.run();
	// operator "new" and "delete" are broken so C++ isn't quite ready yet
}

}
// End extern "C"
