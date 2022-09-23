#include "RuneRemember.hpp"
#include <SFML/GpuPreference.hpp>

SFML_DEFINE_DISCRETE_GPU_PREFERENCE

int main(int argc, char* argv[])
{
    _CRT_UNUSED(argc);
    _CRT_UNUSED(argv);

    RuneRemember rr;

    rr.run();
}