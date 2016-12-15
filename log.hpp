#ifndef LOG_HPP_
#define LOG_HPP_

namespace logging {
    extern bool verbose;
    inline void vout(std::string msg) {
        if(verbose)
            std::cout << msg << std::endl;
    }
    inline void verr(std::string msg) {
        if(verbose)
            std::cerr << msg << std::endl;
    }
}
#endif