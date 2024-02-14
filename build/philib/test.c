#include "phi.h"

int main()
{
	struct Phi* phi = phiCreate();
	phiDoString(phi, "import io io.print(\"Hello World!\")");
	phiDestroy(phi);
	return 0;
}
