#ifndef CBPP_UTILS_H
#define CBPP_UTILS_H

#include <cstdint>

#define CB_VAR_GETSET(_type, _pretty_name, _vname) private: _type _vname;\
public:\
void Set##_pretty_name ( _type new_value ) { _vname = new_value; }\
_type Get##_pretty_name () { return _vname; }

#define CB_VAR_GET(_type, _pretty_name, _vname) private: _type _vname;\
public:\
_type Get##_pretty_name () { return _vname; }

namespace cbpp {
    typedef float float_t;
    typedef int64_t id_t;
}

#endif