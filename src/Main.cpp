#include "RuneRemember.hpp"
#include <SFML/GpuPreference.hpp>

SFML_DEFINE_DISCRETE_GPU_PREFERENCE

int main(int argc, char* argv[])
{
    RuneRemember rr;
    rr.run();
}