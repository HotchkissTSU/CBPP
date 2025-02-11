#ifndef CBPP_UTILS_H
#define CBPP_UTILS_H

#include "cbpp/cb_hash.h"

#include <type_traits>

//Build a serverside library
#define CBPP_SERVER 0

//Build a clientside library
#define CBPP_CLIENT 1

/*
    === UTILITY MACROS ===
*/

//The byte size of a pointer on the current machine
#define CB_PSZ sizeof(void*)

/*
    Creates a getter, a setter and a private member
    + private member named [_name]
    + public void Set [_pretty_name] ( const [_type]& )
    + public const [_type]& Get [_pretty_name] () const noexcept
*/
#define CB_VAR_GETSET(_type, _pretty_name, _vname) private: _type _vname;\
public:\
void Set##_pretty_name ( const _type& new_value ) { _vname = new_value; }\
const _type& Get##_pretty_name () const noexcept { return _vname; } private:

/*
    Creates a getter and a setter for an already existing class member
    + public void Set [_pretty_name] ( const [_type]& )
    + public const [_type]& Get [_pretty_name] () const noexcept
*/
#define CB_VAR_GETSETE(_type, _pretty_name, _vname)\
public:\
void Set##_pretty_name ( const _type& new_value ) { _vname = new_value; }\
const _type& Get##_pretty_name () const noexcept { return _vname; } private:

/*
    Creates a getter and a class member
    + private member named [_name]
    + public const [_type]& Get [_pretty_name] () const noexcept
*/
#define CB_VAR_GET(_type, _pretty_name, _vname) private: _type _vname;\
public:\
const _type& Get##_pretty_name () const noexcept { return _vname; } private:

//Getter for an already existing class member
#define CB_VAR_GETE(_type, _pretty_name, _vname) public:\
_type Get##_pretty_name () const { return _vname; } private:

//Return from a function if expr
#define CB_RETIF(ret_value, expr) if(expr) { return ret_value; }

/*
    Generates a static ID for a class based on the macro argument
    + public static hash_t ID()
    + public static cchar* GetName()
    + private static hash_t m_cb_static_class_id 
*/
#define CB_CLASS_UID(dt_name)\
    private: constexpr static s_cb_iStaticIndex = cbpp::Hash(#dt_name);\
    public: static hash_t ID() { return s_cb_iStaticIndex; }\
    private:

/*
    Generates a static ID for a class
    + public static hash_t ID()
*/
#define CB_CLASS_UID_AUTO\
    public: static hash_t ID() {\
        constexpr static hash_t s_iHash = cbpp::Hash(__PRETTY_FUNCTION__); return s_iHash; }\
    private:

/*
    === ENGINE OPTIONS ===
*/

#define CBPP_VERSION_MAJOR 1
#define CBPP_VERSION_MINOR 0

//Paths
#define CB_PATH_LOGS "assets/logs/"
#define CB_PATH_SHADERS "assets/shaders/"

//Maximal name of the log file, not including the '.log' suffix and the path prefix
#define CB_MAX_LOGNAME 32

//Add this amount of characters to the final length of the result of the string concatenation operation
#define CBPP_STRING_OVERHEAD 32

#define CBPP_PRINT_FMTBUFF_SIZE 64

namespace cbpp {
    // The type to use in the engine`s CPU-side math (Vectors, Meshes etc.)
    typedef float float_t;
}

#endif
