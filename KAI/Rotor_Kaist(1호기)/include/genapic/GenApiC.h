/*-----------------------------------------------------------------------------
    (c) 2009 by Basler Vision Technologies
    Section: Basler Components
    Project: PYLON
    Author:  AH, TK
  -----------------------------------------------------------------------------*/

/**
  \file
  \brief GenApi C bindings. 
*/

#ifndef GENAPI_GENAPIC_H_
#define GENAPI_GENAPIC_H_

#include <stddef.h>

#include <genapic/GenApiCDefines.h>
#include <genapic/GenApiCTypes.h>
#include <genapic/GenApiCEnums.h>
#include <genapic/GenApiCError.h>
#include <genapic/GenApiCModule.h>

#if defined GENAPIC_EXPORTS
[idl_quote( "typedef [public] long int32_t;" )];
#endif

#if defined GENAPIC_EXPORTS
[idl_quote( "typedef [public] byte _Bool;" )];
#endif

#if defined( GENAPIC_EXPORTS )
#   define GENAPIC_DECLARE_HANDLE(name) \
        [idl_quote ("typedef [public] long " #name ";")]; \
        struct name##_; typedef struct name##_ *name
#else
#   define GENAPIC_DECLARE_HANDLE(name) \
        struct name##_; typedef struct name##_ *name
#endif

/** 
  * \addtogroup genapi GenApi Interface
  * @{
  */

/** \brief A value for invalid / uninitialized handles. */
#ifdef __cplusplus
#   define GENAPIC_INVALID_HANDLE ( 0 ) // must match cGenApiInvalidHandle in the typelib
#else
#   define GENAPIC_INVALID_HANDLE ( (void*)0 )
#endif

GENAPIC_DECLARE_HANDLE( NODE_HANDLE );          /**< \brief A handle for a node object. */
GENAPIC_DECLARE_HANDLE( NODEMAP_HANDLE );       /**< \brief A handle for a node map object. */
GENAPIC_DECLARE_HANDLE( NODE_CALLBACK_HANDLE ); /**< \brief A handle for a node callback object. */

/** 
 * @}
 */


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef void GENAPIC_CC NodeCallbackFunction_t(NODE_HANDLE h);


/*
 * ----------------------------------------------------------------------------
 * Error handling
 * ----------------------------------------------------------------------------
 */

#ifndef GENAPIC_TLB_ONLY
GENAPIC_API GENAPIC_RESULT GENAPIC_CC GenApiGetLastErrorMessage( char* pBuf, size_t *pBufLen );
GENAPIC_API GENAPIC_RESULT GENAPIC_CC GenApiGetLastErrorDetail( char* pBuf, size_t *pBufLen );
#endif



IDL_ENTRY(GENAPIC_MODULE, "_GenApiNodeMapGetNode@12", "Find a node by its name.")
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiNodeMapGetNode( NODEMAP_HANDLE hMap, STRING_PAR const char* pName, RETVAL_PAR NODE_HANDLE* phNode );

IDL_ENTRY(GENAPIC_MODULE, "_GenApiNodeMapGetNumNodes@8", "Get the number of nodes contained in a node map.")
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiNodeMapGetNumNodes( NODEMAP_HANDLE hMap, RETVAL_PAR size_t *pValue);

IDL_ENTRY(GENAPIC_MODULE, "_GenApiNodeMapGetNodeByIndex@12", "Find a node by its index.")
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiNodeMapGetNodeByIndex( NODEMAP_HANDLE hMap, size_t index, RETVAL_PAR NODE_HANDLE* phNode );

#ifndef GENAPIC_TLB_ONLY
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiNodeMapPoll( NODEMAP_HANDLE hMap, int64_t timestamp );
#endif

IDL_ENTRY(GENAPIC_MODULE, "_GenApiNodeGetAccessMode@8", "Query node for allowed access modes.")
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiNodeGetAccessMode( NODE_HANDLE hNode, RETVAL_PAR EGenApiAccessMode *pAccessMode );

#ifndef GENAPIC_TLB_ONLY
GENAPIC_API GENAPIC_RESULT GENAPIC_CC
GenApiNodeGetName( NODE_HANDLE hNode, char *pBuf, size_t *pBufLen );
#endif

IDL_ENTRY(GENAPIC_MODULE, "_GenApiNodeGetNameSpace@8", "Return a node's name space.")
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiNodeGetNameSpace( NODE_HANDLE hNode, RETVAL_PAR EGenApiNameSpace *pNamespace );

IDL_ENTRY(GENAPIC_MODULE, "_GenApiNodeGetVisibility@8", "Return a node's visibility.")
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiNodeGetVisibility( NODE_HANDLE hNode, RETVAL_PAR EGenApiVisibility* pVisibility );

IDL_ENTRY(GENAPIC_MODULE, "_GenApiNodeInvalidateNode@4", "Mark node as possibly changed.")
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiNodeInvalidateNode( NODE_HANDLE hNode );

IDL_ENTRY(GENAPIC_MODULE, "_GenApiNodeGetCachingMode@8", "Query node for its caching mode.")
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiNodeGetCachingMode( NODE_HANDLE hNode, RETVAL_PAR EGenApiCachingMode* pCachingMode );

#ifndef GENAPIC_TLB_ONLY
GENAPIC_API GENAPIC_RESULT GENAPIC_CC
GenApiNodeGetToolTip( NODE_HANDLE hNode, char* pBuf, size_t *pBufLen );

GENAPIC_API GENAPIC_RESULT GENAPIC_CC
GenApiNodeGetDescription( NODE_HANDLE hNode, char* pBuf, size_t *pBufLen );

GENAPIC_API GENAPIC_RESULT GENAPIC_CC
GenApiNodeGetDisplayName( NODE_HANDLE hNode, char* pBuf, size_t *pBufLen );
#endif

IDL_ENTRY(GENAPIC_MODULE, "_GenApiNodeGetType@8", "Get node type.")
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiNodeGetType( NODE_HANDLE hNode, RETVAL_PAR EGenApiNodeType* pType );

#ifndef GENAPIC_TLB_ONLY
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiNodeGetPollingTime( NODE_HANDLE hNode, PINVOKE_RETVAL_PAR int64_t *pPollingTime );

GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE_DOC
GenApiNodeRegisterCallback( NODE_HANDLE hNode, NodeCallbackFunction_t *pCbFunction, PINVOKE_RETVAL_PAR NODE_CALLBACK_HANDLE *phCb );

GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiNodeDeregisterCallback( NODE_HANDLE hNode, NODE_CALLBACK_HANDLE hCb );
#endif



IDL_ENTRY(GENAPIC_MODULE, "_GenApiNodeImposeAccessMode@8", "Modify a node's accessibility.")
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiNodeImposeAccessMode( NODE_HANDLE hNode, EGenApiAccessMode imposedAccessMode );

IDL_ENTRY(GENAPIC_MODULE, "_GenApiNodeImposeVisibility@8", "Modify a node's visibility.")
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiNodeImposeVisibility( NODE_HANDLE hNode, EGenApiVisibility imposedVisibility );

/* IValue */
#ifndef GENAPIC_TLB_ONLY
GENAPIC_API GENAPIC_RESULT GENAPIC_CC
GenApiNodeToString( NODE_HANDLE hNode, char* pBuf, size_t *pBufLen );

GENAPIC_API GENAPIC_RESULT GENAPIC_CC
GenApiNodeToStringEx( NODE_HANDLE hNode, _Bool verify, char* pBuf, size_t *pBufLen );

GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiNodeFromStringEx( NODE_HANDLE hNode, _Bool verify, const char* pString );
#endif

IDL_ENTRY(GENAPIC_MODULE, "_GenApiNodeFromString@8", "Set a node's value from a text string with mandatory validation.")
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiNodeFromString( NODE_HANDLE hNode, STRING_PAR const char* pString );

/* IInteger */
#ifndef GENAPIC_TLB_ONLY
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiIntegerSetValue( NODE_HANDLE hNode, int64_t value );

GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiIntegerSetValueEx( NODE_HANDLE hNode, _Bool verify, int64_t value );

GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiIntegerGetValue( NODE_HANDLE hNode, PINVOKE_RETVAL_PAR int64_t* pValue );

GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiIntegerGetValueEx( NODE_HANDLE hNode, _Bool verify, PINVOKE_RETVAL_PAR int64_t* pValue );

GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiIntegerGetMin( NODE_HANDLE hNode, PINVOKE_RETVAL_PAR int64_t* pValue );

GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiIntegerGetMax( NODE_HANDLE hNode, PINVOKE_RETVAL_PAR int64_t* pValue );

GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiIntegerGetInc( NODE_HANDLE hNode, PINVOKE_RETVAL_PAR int64_t* pValue );
#endif

IDL_ENTRY(GENAPIC_MODULE, "_GenApiIntegerGetRepresentation@8", "Get a node's preferred representation.")
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiIntegerGetRepresentation( NODE_HANDLE hNode, RETVAL_PAR EGenApiRepresentation* pValue );

/* IBoolean */
#ifndef GENAPIC_TLB_ONLY
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiBooleanSetValue( NODE_HANDLE hNode, _Bool value );

GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiBooleanGetValue( NODE_HANDLE hNode, PINVOKE_RETVAL_PAR _Bool * pValue );
#endif

/* IFloat */
IDL_ENTRY(GENAPIC_MODULE, "_GenApiFloatSetValue@12", "Set a node's floating point value with mandatory validation.")
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiFloatSetValue( NODE_HANDLE hNode, double value );

#ifndef GENAPIC_TLB_ONLY
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiFloatSetValueEx( NODE_HANDLE hNode, _Bool verify, double value );
#endif

IDL_ENTRY(GENAPIC_MODULE, "_GenApiFloatGetValue@8", "Get a node's floating point value without validation.")
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiFloatGetValue( NODE_HANDLE hNode, RETVAL_PAR double* pValue );

#ifndef GENAPIC_TLB_ONLY
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiFloatGetValueEx( NODE_HANDLE hNode, _Bool verify, PINVOKE_RETVAL_PAR double* pValue );
#endif

IDL_ENTRY(GENAPIC_MODULE, "_GenApiFloatGetMin@8", "Get a node's minimum floating point value.")
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiFloatGetMin( NODE_HANDLE hNode, RETVAL_PAR double* pValue );

IDL_ENTRY(GENAPIC_MODULE, "_GenApiFloatGetMax@8", "Get a node's maximum floating point value.")
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiFloatGetMax( NODE_HANDLE hNode, RETVAL_PAR double* pValue );

IDL_ENTRY(GENAPIC_MODULE, "_GenApiFloatGetRepresentation@8", "Get a node's preferred representation.")
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiFloatGetRepresentation( NODE_HANDLE hNode, RETVAL_PAR EGenApiRepresentation* pValue );

#ifndef GENAPIC_TLB_ONLY
GENAPIC_API GENAPIC_RESULT GENAPIC_CC
GenApiFloatGetUnit( NODE_HANDLE hNode, char* pBuf, size_t* pBufLen );
#endif

/* ICommand */
IDL_ENTRY(GENAPIC_MODULE, "_GenApiCommandExecute@4", "Trigger the execution of a command.")
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiCommandExecute( NODE_HANDLE hNode );

#ifndef GENAPIC_TLB_ONLY
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiCommandIsDone( NODE_HANDLE hNode, PINVOKE_RETVAL_PAR _Bool * pValue );
#endif

/* IEnumeration */
IDL_ENTRY(GENAPIC_MODULE, "_GenApiEnumerationGetNumEntries@8", "Get the number of entries in an enumeration.")
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiEnumerationGetNumEntries( NODE_HANDLE hNode, RETVAL_PAR size_t* pValue );

IDL_ENTRY(GENAPIC_MODULE, "_GenApiEnumerationGetEntryByIndex@12", "Get an entry from an enumeration by index.")
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiEnumerationGetEntryByIndex( NODE_HANDLE hNode, size_t index, RETVAL_PAR NODE_HANDLE* pEntry );

IDL_ENTRY(GENAPIC_MODULE, "_GenApiEnumerationGetEntryByName@12", "Get an entry from an enumeration by name.")
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiEnumerationGetEntryByName( NODE_HANDLE hNode, STRING_PAR const char* pName, RETVAL_PAR NODE_HANDLE* pEntry );

/* IEnumEntry */
IDL_ENTRY(GENAPIC_MODULE, "_GenApiEnumerationEntryGetValue@8", "Returns the numerical index of an enumeration entry.")
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiEnumerationEntryGetValue( NODE_HANDLE hNode, RETVAL_PAR int* pValue );

#ifndef GENAPIC_TLB_ONLY
GENAPIC_API GENAPIC_RESULT GENAPIC_CC
GenApiEnumerationEntryGetSymbolic( NODE_HANDLE hNode, char* pBuf, size_t* pBufLen );
#endif

/* Convenience */
#ifndef GENAPIC_TLB_ONLY
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiNodeIsImplemented( NODE_HANDLE hNode, PINVOKE_RETVAL_PAR _Bool * pResult );

GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiNodeIsReadable( NODE_HANDLE hNode, PINVOKE_RETVAL_PAR _Bool * pResult );

GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiNodeIsWritable( NODE_HANDLE hNode, PINVOKE_RETVAL_PAR _Bool * pResult );

GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiNodeIsAvailable( NODE_HANDLE hNode, PINVOKE_RETVAL_PAR _Bool * pResult );
#endif

/* Category */
IDL_ENTRY(GENAPIC_MODULE, "_GenApiCategoryGetNumFeatures@8", "Returns the number of features in a category.")
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiCategoryGetNumFeatures( NODE_HANDLE hNode, RETVAL_PAR size_t* pValue );

IDL_ENTRY(GENAPIC_MODULE, "_GenApiCategoryGetFeatureByIndex@12", "Get a feature node from a category by its index.")
GENAPIC_API GENAPIC_RESULT GENAPIC_CC PINVOKE
GenApiCategoryGetFeatureByIndex( NODE_HANDLE hNode, size_t index, RETVAL_PAR NODE_HANDLE *phEntry );

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#include <genapic/GenApiC32BitMethods.h>

#endif
