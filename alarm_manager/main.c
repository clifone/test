#include "input_processing/input_processing.h"
#include "utils/program_log.h"

int main(int argc, char* argv[])
{
    my_log("main, start", 1);
    int err = input_processing(argc, argv);
    return 0;
}
