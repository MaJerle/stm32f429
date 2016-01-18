/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2015/05/library-58-dynamic-strings-on-stm32f4xx
 * @version v1.1
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   String library for STM32F4xx devices
 *	
@verbatim
   ----------------------------------------------------------------------
    Copyright (C) Tilen Majerle, 2015
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
     
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
@endverbatim
 */
#ifndef TM_STRING_H
#define TM_STRING_H 110

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_STRING
 * @brief    String library for STM32F4xx devices - http://stm32f4-discovery.com/2015/05/library-58-dynamic-strings-on-stm32f4xx
 * @{
 *
 * This library allows you to dynamically operate with strings in HEAP memory.
 * For this purpose, you need to make sure that your HEAP memory is big enough.
 *
 * \par Increase HEAP memory
 *
 * If you use Keil uVision, then you can set HEAP memory by simply open startup file (*.s) and increase variable "Heap_Size".
 *
 * For other compilers, check it's manual.
 * 
 * \par Changelog
 *
@verbatim
 Version 1.1
  - May 23, 2015
  - Added support for replacing string
 
 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F4xx
 - STM32F4xx RCC
 - defines.h
@endverbatim
 */

#include "stm32f4xx.h"
#include "defines.h"

#include "string.h"
#include "stdlib.h"

/**
 * @defgroup TM_STRING_Macros
 * @brief    Library defines
 * @{
 */

/* Memory allocation function */
#ifndef LIB_ALLOC_FUNC
#define LIB_ALLOC_FUNC    malloc
#endif

/* Memory free function */
#ifndef LIB_FREE_FUNC
#define LIB_FREE_FUNC     free
#endif

/**
 * @}
 */
 
/**
 * @defgroup TM_STRING_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  Main string structure
 */
typedef struct {
	char** Strings;    /*!< Pointer to pointers to strings */
	uint32_t Count;    /*!< Number of string elementsz */
	uint32_t Size;     /*!< Number of all allocated pointers for strings */
} TM_STRING_t;

/**
 * @}
 */

/**
 * @defgroup TM_STRING_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Creates and allocated string structure and memory for pointers for desired number of strings
 * @note   Function uses @ref malloc() to allocate main structure and all pointers depending on count variable,
 *            so make sure that you have enough HEAP memory reserved
 * @param  count: Number of strings you will use. Set to 1, if you don't know how many of them will be used.
 *            It's recommended that you select the number which is greater or equal to count of all strings
 * @retval Pointer to allocated @ref TM_STRING_t structure or NULL of malloc() fails
 */
TM_STRING_t* TM_STRING_Create(uint16_t count);

/**
 * @brief  Adds new string to main string structure
 * @note   Function uses @ref malloc() to allocate memory for string where it will copy it.
 *            Memory size for allocation is string length + 1
 * @param  *String: Pointer to @ref TM_STRING_t structure
 * @param  *str: Pointer to string to be added to string
 * @retval String position in strings array
 */
uint16_t TM_STRING_AddString(TM_STRING_t* String, char* str);

/**
 * @brief  Replaces already added string with new string
 * @note   If new string is larger than string before, new memory is allocated and old is free,
 *         but if new string length is smaller than old, only new string is copied to already allocateed memory
 * @param  *String: Pointer to @ref TM_STRING_t structure
 * @param  pos: Position in array where to replace string
 * @param  *str: Pointer to new string which will be applied to memory
 * @retval Pointer to @ref TM_STRING_t structure
 */
TM_STRING_t* TM_STRING_ReplaceString(TM_STRING_t* String, uint16_t pos, char* str);

/**
 * @brief  Deletes string from strings array
 * @param  *String: Pointer to @ref TM_STRING_t structure
 * @param  pos: Position number in string array which string will be deleted.
 *            This number can be a value between 0 and number of strings - 1
 * @retval Pointer to @ref TM_STRING_t structure
 */
TM_STRING_t* TM_STRING_DeleteString(TM_STRING_t* String, uint16_t pos);

/**
 * @brief  Gets pointer to string at desired position
 * @param  *String: Pointer to @ref TM_STRING_t structure
 * @param  pos: Position number in string array which string pointer will be returned.
 *            This number can be a value between 0 and number of strings - 1
 * @retval Pointer to string at desired position in string array
 */
char* TM_STRING_GetString(TM_STRING_t* String, uint16_t pos);

/**
 * @brief  Free all. It will free all strings, all pointers to strings and also main string structure
 * @note   Use @ref TM_STRING_DeleteString() to delete only one string if needed
 * @param  *String: Pointer to @ref TM_STRING_t structure
 * @retval None
 */
void TM_STRING_FreeAll(TM_STRING_t* String);

/**
 * @brief  Gets number of strings in a string array
 * @param  *String: Pointer to @ref TM_STRING_t structure
 * @retval Number of strings in array
 * @note   Defined as macro for faster execution
 */
#define TM_STRING_GetCount(str)    ((str)->Count)

/**
 * @brief  Free main structure and pointer to all pointers.
 * @note   This is not meant for public use unless you know what you are doing.
 *            It will deallocate structure, but all strings will stay untouched in heap memory!
 *            Use @ref TM_STRING_FreeAll() instead.
 * @param  *String: Pointer to @ref TM_STRING_t structure
 */
void TM_STRING_Free(TM_STRING_t* String);

/**
 * @}
 */
 
/**
 * @}
 */
 
/**
 * @}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
