PROJECT(LabelMapCreation) 

FIND_PACKAGE(ITK) 
IF(ITK_FOUND)
	INCLUDE(${ITK_USE_FILE}) 
ELSE(ITK_FOUND)
	MESSAGE(FATAL_ERROR "ITK not found. Please set ITK_DIR.") 
ENDIF(ITK_FOUND) 

LINK_LIBRARIES(${ITK_LIBRARIES})
ADD_EXECUTABLE(LabelMapCreation main.cxx)