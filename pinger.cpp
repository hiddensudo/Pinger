#include "Pinger.h"

#include <signal.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>

int main() {
    srand(time(NULL)); // Why does it here?
    Pinger& pinger = Pinger::getInstance();
    pinger.ping();
}