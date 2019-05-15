//
// Created by Nick Grout on 2019-05-15.
//

#ifndef CONFIG_HPP
#define CONFIG_HPP

#if defined(_WIN32)
#define __windows__
#elif defined(__linux__)
#elif defined(__APPLE__)
#define __apple__
#endif

#endif //CONFIG_HPP
