#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "seamFinder.h"

struct qSeamFinder
{
	void destroy();
	int makeSeamMask();
};