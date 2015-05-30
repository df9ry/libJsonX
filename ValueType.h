/*
 * ValueType.h
 *
 *  Created on: 30.05.2015
 *      Author: tania
 */

#ifndef VALUETYPE_H_
#define VALUETYPE_H_

/**
 * Class marker, to avoid RTTI.
 */
enum class ValueType {
	T_STRING,    //!< T_STRING  This is a json string
	T_BLOB,      //!< T_BLOB    This is a json blob (Extension)
	T_NULL,      //!< T_NULL    This is a null value
	T_BOOL,      //!< T_BOOL    This is a json boolean
	T_INT16,     //!< T_INT16   This is a json 16 bit signed integer
	T_UINT16,    //!< T_UINT16  This is a json 16 bit unsigned integer
	T_INT32,     //!< T_INT32   This is a json 32 bit signed integer
	T_UINT32,    //!< T_UINT32  This is a json 32 bit unsigned integer
	T_INT64,     //!< T_INT64   This is a json 64 bit signed integer
	T_UINT64,    //!< T_UINT64  This is a json 64 bit unsigned integer
	T_DOUBLE,    //!< T_DOUBLE  This is a json double real
	T_OBJECT,    //!< T_OBJECT  This is a json object
	T_ARRAY,     //!< T_ARRAY   This is a json array
};

#endif /* VALUETYPE_H_ */
