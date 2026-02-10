#include <iostream>
#include "./SignalProducer.h"

void printfloat (float f) {
    printf("%f", f);
}

int main () {
    SignalProducer::ReadFile("./100_raw");
    SignalProducer::Start(printfloat);
}