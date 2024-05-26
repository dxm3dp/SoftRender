#ifndef _CPP_LANG_H_
#define _CPP_LANG_H_

#include <cassert>

#include <iostream>

#include <string>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

// must be C++11 or later
#if __cplusplus < 201103
#   error "C++ is too old"
#endif  // __cplusplus < 201103

// macro for convienient namespace
#define BEGIN_NAMESPACE(x) namespace x {
#define END_NAMESPACE(x) }
#define USING_NAMESPACE(x) using namespace x

#endif // _CPP_LANG_H_
