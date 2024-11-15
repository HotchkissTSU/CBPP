#ifndef CBPP_UTILS_H
#define CBPP_UTILS_H

#include <cstdint>

#define CB_VAR_GETSET(_type, _pretty_name, _vname) private: _type _vname;\
public:\
void Set##_pretty_name ( _type new_value ) { _vname = new_value; }\
_type Get##_pretty_name () { return _vname; }

#define CB_VAR_GET(_type, _pretty_name, _vname) private: _type _vname;\
public:\
_type Get##_pretty_name () const { return _vname; }

#define CB_VAR_GETE(_type, _pretty_name, _vname) public:\
_type Get##_pretty_name () const { return _vname; }

#define CB_RETIF(ret_value, expr) if(expr) { return ret_value; }

namespace cbpp {
    /*
        The type to use in the engine`s CPU-side math (Vectors, Meshes etc.)
    */
    typedef float float_t;

    /*
        Thr type to store various IDs
    */
    typedef int64_t id_t;
}

#endif