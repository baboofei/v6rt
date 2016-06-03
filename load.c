// Test that fork fails gracefully.
// Tiny executable so that the limit can be filling the proc table.

#include "types.h"
#include "stat.h"
#include "user.h"

void
load(long it)
{
  while(it > 0)
  {
	long l = it^99;
	it += l;
	it -= l;
	--it;
  }
}

int
main(void)
{
  load(10^10);
  exit();
}
