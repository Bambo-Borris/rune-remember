include(FetchContent)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 05690b963df9a62bd00d245447f0d440229e3732)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog
    GIT_TAG v1.10.0)
FetchContent_MakeAvailable(spdlog)
