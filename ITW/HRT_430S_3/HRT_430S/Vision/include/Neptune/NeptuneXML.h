
#pragma  once

///////////////////////////////////////////////////////////////////////
//
//		Enumeration types and structures for XML node handling
//
///////////////////////////////////////////////////////////////////////
typedef enum _tagENeptuneNodeType
{
	NEPTUNE_NODE_TYPE_UKNOWN		= -1,
	NEPTUNE_NODE_TYPE_CATEGORY		= 0,
	NEPTUNE_NODE_TYPE_COMMAND,				// command type node
	NEPTUNE_NODE_TYPE_RAW,					// raw node
	NEPTUNE_NODE_TYPE_STRING,				// string node
	NEPTUNE_NODE_TYPE_ENUM,					// enumeration node
	NEPTUNE_NODE_TYPE_INT,					// int type node
	NEPTUNE_NODE_TYPE_FLOAT,				// float type node
	NEPTUNE_NODE_TYPE_BOOLEAN				// boolean type node
} ENeptuneNodeType;


typedef enum _tagENeptuneNodeAccessMode
{
	NEPTUNE_NODE_ACCESSMODE_NI			= 0,		// Not Implemented
	NEPTUNE_NODE_ACCESSMODE_NA			= 1,		// Not Available
	NEPTUNE_NODE_ACCESSMODE_WO			= 2,		// Write Only
	NEPTUNE_NODE_ACCESSMODE_RO			= 3,		// Read Only
	NEPTUNE_NODE_ACCESSMODE_RW			= 4,		// Read, Write
	NEPTUNE_NODE_ACCESSMODE_UNDEFINED	= 5			// undefined
} ENeptuneNodeAccessMode;


typedef enum _tagENeptuneNodeVisibility
{
	NEPTUNE_NODE_VISIBLE_UNKNOWN		= -1,
	NEPTUNE_NODE_VISIBLE_BEGINNER		= 0,		// beginner
	NEPTUNE_NODE_VISIBLE_EXPERT			= 1,		// expert
	NEPTUNE_NODE_VISIBLE_GURU			= 2			// guru
} ENeptuneNodeVisibility;


typedef struct _tagNEPTUNE_XML_NODE_LIST
{
	_uint32_t		nCount;												// number of nodes
	_char_t			pstrList[MAX_NODE_LIST_COUNT][MAX_XML_NODE_STRING_LENGTH];	// node list

	_tagNEPTUNE_XML_NODE_LIST()
	{
		nCount = MAX_NODE_LIST_COUNT;
		memset(pstrList, 0, sizeof(_char_t)*MAX_NODE_LIST_COUNT*MAX_XML_NODE_STRING_LENGTH);
	}
} NEPTUNE_XML_NODE_LIST, *PNEPTUNE_XML_NODE_LIST;


typedef struct _tagNEPTUNE_XML_ENUM_LIST
{
	_uint32_t		nCount;												// number of enumeration node string
	_char_t			pstrList[MAX_ENUM_LIST_COUNT][MAX_XML_NODE_STRING_LENGTH];		// enumeration node string list
	_uint32_t		nIndex;												// current index of the string list

	_tagNEPTUNE_XML_ENUM_LIST()
	{
		nCount = MAX_ENUM_LIST_COUNT;
		nIndex = 0;
		memset(pstrList, 0, sizeof(_char_t)*MAX_ENUM_LIST_COUNT*MAX_XML_NODE_STRING_LENGTH);
	}
} NEPTUNE_XML_ENUM_LIST, *PNEPTUNE_XML_ENUM_LIST;


typedef struct _tagNEPTUNE_XML_NODE_INFO
{
	ENeptuneNodeType			Type;			// node type(int, float, boolean, string, enumeration, command)
	ENeptuneNodeAccessMode		AccessMode;		// access mode
	ENeptuneNodeVisibility		Visibility;		// node visibility
	_bool8_t						bHasChild;	// has child ?
	_char_t						strDisplayName[MAX_STRING_LENGTH];	// node name
	_char_t						strTooltip[MAX_STRING_LENGTH];		// node tooltip
	_char_t						strDescription[MAX_STRING_LENGTH];	// node description

	_tagNEPTUNE_XML_NODE_INFO()
	{
		Type = NEPTUNE_NODE_TYPE_UKNOWN;
		AccessMode = NEPTUNE_NODE_ACCESSMODE_UNDEFINED;
		Visibility = NEPTUNE_NODE_VISIBLE_UNKNOWN;
		bHasChild = NEPTUNE_BOOL_FALSE;
		memset(strDisplayName, 0, sizeof(_char_t)*MAX_STRING_LENGTH);
		memset(strTooltip, 0, sizeof(_char_t)*MAX_STRING_LENGTH);
		memset(strDescription, 0, sizeof(_char_t)*MAX_STRING_LENGTH);
	}
} NEPTUNE_XML_NODE_INFO, *PNEPTUNE_XML_NODE_INFO;


typedef struct _tagNEPTUNE_XML_INT_VALUE_INFO {
	_int64_t					nValue;				// current value
	_int64_t					nMin;				// minimum value supported
	_int64_t					nMax;				// maximum value supported
	_int64_t					nInc;				// increment step
	ENeptuneNodeAccessMode		AccessMode; 		// access mode

	_tagNEPTUNE_XML_INT_VALUE_INFO()
	{
		nValue = 0;
		nMin = 0;
		nMax = 0;
		nInc = 0;
		AccessMode = NEPTUNE_NODE_ACCESSMODE_UNDEFINED;
	}
} NEPTUNE_XML_INT_VALUE_INFO, *PNEPTUNE_XML_INT_VALUE_INFO;


typedef struct _tagNEPTUNE_XML_FLOAT_VALUE_INFO {
	_double_t					dValue;			// current value
	_double_t					dMin;			// minimum
	_double_t					dMax;			// maximum
	_double_t					dInc;			// increment
	ENeptuneNodeAccessMode		AccessMode;

	_tagNEPTUNE_XML_FLOAT_VALUE_INFO()
	{
		dValue = 0;
		dMin = 0;
		dMax = 0;
		dInc = 0.1;
		AccessMode = NEPTUNE_NODE_ACCESSMODE_UNDEFINED;
	}
} NEPTUNE_XML_FLOAT_VALUE_INFO, *PNEPTUNE_XML_FLOAT_VALUE_INFO;


