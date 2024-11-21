/*
    Header with some engine-wide utility macros
*/

#ifndef CBPP_UTILS_H
#define CBPP_UTILS_H

#include <cstdint>

//Creates a private class member with getter and setter functions
#define CB_VAR_GETSET(_type, _pretty_name, _vname) private: _type _vname;\
public:\
void Set##_pretty_name ( const _type& new_value ) { _vname = new_value; }\
_type Get##_pretty_name () { return _vname; }

//Creates a class member and a getter function for it
#define CB_VAR_GET(_type, _pretty_name, _vname) private: _type _vname;\
public:\
_type Get##_pretty_name () const { return _vname; }

//Getter for an already existing class member
#define CB_VAR_GETE(_type, _pretty_name, _vname) public:\
_type Get##_pretty_name () const { return _vname; }

//Return from a function if expr
#define CB_RETIF(ret_value, expr) if(expr) { return ret_value; }

/*
    Creates a static ID() method which returns the hash value of the marco argument
    Also creates a private field named m_cb_static_class_id
*/
#define CB_CLASS_UID(dt_name)\
    private constexpr static m_cb_static_class_id = cbpp::Hash(#dt_name);\
    public static hash_t ID() { return m_cb_static_class_id; }

namespace cbpp {
    /*
        The type to use in the engine`s CPU-side math (Vectors, Meshes etc.)
    */
    typedef float float_t;
}

#endif