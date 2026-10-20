#include <string>
#include "oop/include/main_oop.h"
#include "dod/include/main_dod.h"

int main() {
    std::string mode = "oop";
    if (mode == "oop") oop();
    else if (mode == "dod") dod();
    return 0;
}