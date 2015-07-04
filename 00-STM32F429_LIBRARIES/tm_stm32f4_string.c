/**	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2015
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */
#include "tm_stm32f4_string.h"

TM_STRING_t* TM_STRING_Create(uint16_t size) {
	TM_STRING_t* String;

	/* Allocate memory */
	String = (TM_STRING_t *) LIB_ALLOC_FUNC(sizeof(TM_STRING_t));
	
	/* Check if allocated */
	if (String == NULL) {
		return NULL;
	}
	
	/* Allocate memory for number of counts */
	String->Strings = (char **) LIB_ALLOC_FUNC(size * sizeof(char *));
	
	/* Check if allocated */
	if (String->Strings == NULL) {
		return NULL;
	}
	
	/* Set default settings */
	String->Size = size;
	String->Count = 0;
	
	/* Return result */
	return String;
}

uint16_t TM_STRING_AddString(TM_STRING_t* String, char* str) {
	TM_STRING_t* ptr;
	char** tmp1;
	uint16_t i;
	
	/* Check input pointer */
	if (String == NULL) {
		return 0;
	}
	
	/* Check if memory available */
	if (String->Count >= String->Size) {
		/* Create new string with more memory */
		ptr = TM_STRING_Create(String->Size + 1);
		tmp1 = (char **) LIB_ALLOC_FUNC((String->Size + 1) * sizeof(char *));
		
		/* Check if allocated */
		if (ptr == NULL || tmp1 == NULL) {
			return 0;
		}
		
		/* New string is allocated, copy old pointers to new */
		/* Store old value */
		for (i = 0; i < String->Size; i++) {
			tmp1[i] = String->Strings[i];
		}
		
		/* Save new pointer location */
		String->Strings = ptr->Strings;
		
		/* Copy pointers from old to new */
		for (i = 0; i < String->Size; i++) {
			String->Strings[i] = tmp1[i];
		}
		
		/* Set new size */
		String->Size = ptr->Size;
		
		/* Set tmp old pointer to free */
		ptr->Strings = tmp1;
		
		/* Free tmp1 memory */
		free(tmp1);
		
		/* Free tmp structure */
		TM_STRING_Free(ptr);
	}
	
	/* Allocate memory for string */
	String->Strings[String->Count] = (char *) LIB_ALLOC_FUNC((strlen(str) + 1) * sizeof(char));
	
	/* Copy content to string */
	strcpy(String->Strings[String->Count], str);
	
	/* Increase count */
	String->Count++;
	
	/* Return pointer */
	return (String->Count - 1);
}

TM_STRING_t* TM_STRING_ReplaceString(TM_STRING_t* String, uint16_t pos, char* str) {
	char *tmp, *tmp1;
	
	/* Check input pointer */
	if (String == NULL) {
		return NULL;
	}
	
	/* Add string if necessary */
	if (pos >= String->Count) {
		/* Add to string */
		TM_STRING_AddString(String, str);
		
		/* Return string pointer */
		return String;
	}
	
	/* Check size */
	if (strlen(str) > strlen(String->Strings[pos])) {
		/* Allocate new memory */
		tmp = (char *) LIB_ALLOC_FUNC((strlen(str) + 1) * sizeof(char *));
		
		/* Check if allocated */
		if (tmp == NULL) {
			return String;
		}
		
		/* Get current string pointer */
		tmp1 = String->Strings[pos];
		
		/* Save new pointer */
		String->Strings[pos] = tmp;
		
		/* Copy string */
		strcpy(String->Strings[pos], str);
		
		/* Free old pointer */
		LIB_FREE_FUNC(tmp1);
	} else {
		/* Just replace in current memory */
		strcpy(String->Strings[pos], str);
	}
	
	/* Return pointer */
	return String;
}

TM_STRING_t* TM_STRING_DeleteString(TM_STRING_t* String, uint16_t pos) {
	uint16_t i;
	char* tmp;
	
	/* Check input pointer */
	if (String == NULL) {
		return NULL;
	}
	
	/* Check input parameter */
	if (String->Count <= pos) {
		return String;
	}
	
	/* Save pointer */
	tmp = String->Strings[pos];
	
	/* Copy pointers */
	for (i = pos; i < (String->Count - 1); i++) {
		/* Copy up */
		String->Strings[i] = String->Strings[i + 1];
	}
	
	/* Decrease count value */
	String->Count--;
	
	/* Deallocate */
	LIB_FREE_FUNC(tmp);
	
	/* Return pointer */
	return String;
}

char* TM_STRING_GetString(TM_STRING_t* String, uint16_t pos) {
	/* Check input pointer */
	if (String == NULL) {
		return NULL;
	}
	
	/* Check if memory available */
	if (String->Count > pos) {
		/* Return pointer to string */
		return String->Strings[pos];
	}
	
	/* Return NULL, no string available */
	return NULL;
}

void TM_STRING_Free(TM_STRING_t* String) {
	/* Check input pointer */
	if (String == NULL) {
		return;
	}
	
	/* Deallocate pointers */
	LIB_FREE_FUNC(String->Strings);
	
	/* Deallocate structure */
	LIB_FREE_FUNC(String);
}

void TM_STRING_FreeAll(TM_STRING_t* String) {
	uint16_t i;
	
	/* Check input pointer */
	if (String == NULL) {
		return;
	}
	
	/* Deallocate all string locations */
	for (i = 0; i < String->Count; i++) {
		/* Deallocate pointer by pointer */
		LIB_FREE_FUNC(String->Strings[i]);
	}
	
	/* Deallocate the rest */
	TM_STRING_Free(String);
}

