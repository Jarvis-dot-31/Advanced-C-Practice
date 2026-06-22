#pragma once
#include<memory>
#include "impl.hpp"

struct pkt{
    int data;
};
inline auto qu=std::make_unique<SPSCqueue<pkt>>();