#include "ActiveSide.h"
using namespace ljd;

ActiveSide::ActiveSide(const Triangle& t) 
{
	attribLeft = Attribution(t, 0);
	attribRight = Attribution(t, 1);
	attribwait = Attribution(t, 2);

	color = t.color;
}