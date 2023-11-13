#include "Pinger.h"

#include <signal.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>

int main() {
    srand(time(NULL));
    Pinger pinger;
    pinger.ping();
}