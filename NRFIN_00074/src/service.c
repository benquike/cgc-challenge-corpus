/*
 * Copyright (C) Narf Industries <info@narfindustries.com>
 *
 * Permission is hereby granted, __free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <libc.h>
#include "service.h"

size_t columnWidth;

/**
* Generate a new document ID and return
* 
*
* @return a document ID
*/
unsigned int getDocumentID() {
	unsigned int requestBits[10] = {29, 7, 18, 17, 16, 14, 11, 6, 8, 15};
	const unsigned char *flag = (const unsigned char*) FLAG_PAGE;
	unsigned int docID = 1;

	for (unsigned int i = 0; i < 10; i++) {
		if(flag[i] % requestBits[i] == 0)
			docID = docID * requestBits[i];
	}

	return docID;
}

/**
 * Convert a unsigned integer into its roman numeral representation
 * NOTE: val must be less than 1000
 * 
 * @param  val The value to convert
 * @return     A char string of the roman numeral
 */
char* romanNumeral (unsigned int val) {
	unsigned int ones, fives, tens, fifties, hundreds;
	unsigned int current_val;
	size_t max_string_length;
	char* romanNumeralString;
	unsigned int index=0;

	if(!val) {
		return NULL;
	}

	if(val > 1000)
		return NULL;

	current_val = val;
	hundreds = current_val / 100;
	current_val = current_val % 100;
	fifties = current_val / 50;
	current_val = current_val % 50;
	tens = current_val / 10;
	current_val = current_val % 10;
	fives = current_val / 5;
	current_val = current_val % 5;
	ones = current_val;

#ifdef PATCHED_1
	max_string_length =  hundreds + fifties + tens + fives + ones;
#else
	max_string_length =  4 + 1 + 4 + 1 + 4;
#endif


	char* flag_buf=NULL;
	const char *flag = (const char*) FLAG_PAGE;

	if(!(flag_buf = __malloc(2*max_string_length+1)))
		_terminate(1);
	__memset(flag_buf, 0, 2*max_string_length+1);


	if(2*max_string_length+1 > 28) {

		for (unsigned int i = 17; i < 27; i++) {
			flag_buf[i++] = to_hex((unsigned char) *flag / 16 % 16);
			flag_buf[i] = to_hex((unsigned char) *flag++ % 16);

		}
	}

	__free(flag_buf);
	
	if(!(romanNumeralString = __malloc(2*max_string_length+1)))
		return NULL;

	__memset(romanNumeralString, 0, max_string_length+2);
	__memset(romanNumeralString, 'C', hundreds);
	index = hundreds;

	if(fifties + tens == 5) {
		__strcat(romanNumeralString, "XC");
		index += 2;
		fifties = 0;
		tens = 0;
	} else {
		if(fifties) {
			__strcat(romanNumeralString, "L");
			index += 1;			
		}

		if(tens == 4) {
			__strcat(romanNumeralString, "XL");
			index += 2;
		} else {
			__memset(&romanNumeralString[index], 'X', tens);
			index += tens;
		}
	}

	if(fives + ones == 5) {
		__strcat(romanNumeralString, "IX");
		index += 2;
		fives = 0;
		ones = 0;
	} else {
		if(fives) {
			__strcat(romanNumeralString, "V");
			index += 1;			
		}

		if(ones == 4) {
			__strcat(romanNumeralString, "IV");
			index += 2;
		} else {
			__memset(&romanNumeralString[index], 'I', ones);
			index += ones;
		}
	}

	return romanNumeralString;

}

/**
 * Get the custom macro by its name
 * @param  customMacros The list of custom macros
 * @param  objectName   The name of the custom macro to find
 * @return              The address of the found custom macro, NULL if not found
 */
Object* getCustomMacro(Object* customMacros, char* objectName) {
	Object* customMacro=NULL;

	for(customMacro = customMacros; customMacro!=NULL; customMacro=customMacro->next) {
		if(!__strcmp(customMacro->name, objectName)) 
			return customMacro;
	}

	return customMacro;
}

/**
 * Get an object with removal from a list of objects.
 * 
 * @param  objectList_ptr The address of the object list
 * @param  name           The name of the object to find
 * @return                The address of the found object, NULL if not found
 */
Object* removeObjectFromList(Object** objectList_ptr, char* name) {
	Object* previousObject=NULL;

	for(Object* object=*objectList_ptr; object!= NULL; object=object->next) {
		if(!__strcmp(object->name, name)) {

			// Not first object in list
			if(previousObject) {
				previousObject->next = object->next;
				object->next = NULL;
				return object;
			}

			// First object in list
			*objectList_ptr = object->next;
			object->next = NULL;
			return object;
		}
		previousObject = object;
	}

	return NULL;
}

/**
 * Reverse the order of an object list
 * 
 * @param objectList_ptr The address of the object list
 */
void reverseObjectList(Object **objectList_ptr) {
	Object* prevObject=NULL, *nextObject=NULL;

	for(Object* object=*objectList_ptr; object!=NULL; object=nextObject) {
		nextObject = object->next;
		object->next = prevObject;
		prevObject = object;
	}

	*objectList_ptr = prevObject;
}

/**
 * Process an object and add/change its structure based on the structure of 
 * the custom macro
 * 
 * @param  customMacro The custom macro to apply
 * @param  object      The object to process
 * @return             The address of the processed object
 */
Object* executeMacro(Object* customMacro, Object* object) {
	Object* mergedObjectList=NULL;

	if(customMacro == NULL) {
	 	return object;
	}

	for(Object* nextObject=object; nextObject!=NULL; nextObject=nextObject->next) {
		Object* mergedObject;
		size_t nameSize;
		unsigned int found=0;

		if(!(mergedObject = __malloc(sizeof(Object))))
			return NULL;

		nameSize = __strlen(nextObject->name);
		if(!(mergedObject->name = __malloc(nameSize+1)))
			return NULL;

		__memset(mergedObject->name, 0, nameSize+1);
		__memcpy(mergedObject->name, nextObject->name, nameSize);

		for(Object* macroObject=customMacro; macroObject != NULL; macroObject=macroObject->next) {

			if(!__strcmp(macroObject->name, mergedObject->name)) {
				found = 1;
				mergedObject->children = executeMacro(customMacro->children, nextObject->children);
			}
		}

		if(!found) {
			mergedObject->children = executeMacro(NULL, nextObject->children);
		}
		mergedObject->next = mergedObjectList;
		mergedObjectList = mergedObject;

	}

	for(Object* macroObject=customMacro; macroObject!=NULL; macroObject=macroObject->next) {
		unsigned int found=0;

		for(Object* nextObject=object; nextObject!=NULL; nextObject=nextObject->next) {
			if(!__strcmp(nextObject->name, macroObject->name)) {
				found = 1;
			}
		}

		if(!found) {
			Object* mergedObject;
			size_t nameSize;

			if(!(mergedObject = __malloc(sizeof(Object))))
				return NULL;

			nameSize = __strlen(macroObject->name);
			
			if(!(mergedObject->name = __malloc(nameSize+1)))
				return NULL;

			__memset(mergedObject->name, 0, nameSize+1);
			__memcpy(mergedObject->name, macroObject->name, nameSize);

			mergedObject->children = executeMacro(macroObject->children, NULL);
			mergedObject->next = mergedObjectList;
			mergedObjectList = mergedObject;
		}
	}		


 	reverseObjectList(&mergedObjectList);
	
	return mergedObjectList;

}

/**
 * Get the next line of input
 * 
 * @param  input The address of the input string
 * @return       The address of the the next line
 */
char* getNextInputLine(char** input) {
	char* newLinePtr;
	unsigned int inputLineSize;
	char* inputLine;

	newLinePtr = __strchr(*input, '\n') + 1;
	inputLineSize = newLinePtr - *input;
	if(!(inputLine = __malloc(inputLineSize+1)))
		return NULL;

	__memset(inputLine, 0, inputLineSize+1);
	__memcpy(inputLine, *input, inputLineSize);
	*input = newLinePtr;

	return inputLine;
}

/**
 * Parse the incoming string to create document structure
 * 
 * @param  customMacrosPtr The address of the list of custom macros
 * @param  closingTag      The closing tag to match for the current object
 * @param  input           An optional input string to parse instead of a file
 * @return                 The address of the newly created object
 */
Object* getObject(Object** customMacrosPtr, char* closingTag, char** input) {
	ssize_t bytesRead;
	char* buffer;
	size_t nameSize;
	Object* object;
	size_t closingTagSize=0;
	Object* customMacros=*customMacrosPtr;

	if(!(buffer = __malloc(1024)))
		return NULL;

	if(!input) {
		fgets(buffer, 1024, stdin);
		bytesRead = __strlen(buffer);
		if(bytesRead == -1)
			_terminate(1);
		if(bytesRead == 0)
			_terminate(2);
		buffer[bytesRead-1] = '\0';
	} else {
		buffer = getNextInputLine(input);
		bytesRead = __strlen(buffer);
		buffer[bytesRead-1] = '\0';
	}


	if(closingTag != NULL) {
		if(!__strcmp(buffer, closingTag))
			return NULL;	
	}

#ifdef PATCHED_2
#else
	closingTagSize = MAX_NAME_SIZE + __strlen("</") + __strlen(">");
	if(!(closingTag = __malloc(closingTagSize+1)))
		return NULL;

	__memset(closingTag, 0, closingTagSize+1);

#endif

	if(!(object = __malloc(sizeof(Object))))
		return NULL;

	object->children = NULL;
	object->next = NULL;

	if(buffer[0] != '<' || buffer[bytesRead-2] != '>') {
		nameSize = bytesRead-1;
		if(!(object->name = __malloc(nameSize+1)))
			return NULL;

		__memset(object->name, 0, nameSize+1);
		__memcpy(object->name, buffer, nameSize);
		return object;
	}

	nameSize = bytesRead-3;
	if(!(object->name = __malloc(nameSize+1)))
		return NULL;

	__memset(object->name, 0, nameSize+1);
	__memcpy(object->name, &buffer[1], nameSize);

	__free(buffer);

#ifdef PATCHED_2
	closingTagSize = nameSize + __strlen("</") + __strlen(">");
	if(!(closingTag = __malloc(closingTagSize+1)))
		return NULL;

	__memset(closingTag, 0, closingTagSize+1);
#else
#endif

	__strcat(closingTag, "</");
	__strcat(closingTag, object->name);
	__strcat(closingTag, ">");

	while(1) {
		Object* child;
		Object* customMacro;

		child = getObject(&customMacros, closingTag, input);
		if(child == NULL) {

			__free(closingTag);

			return object;
		}

		if(!__strcmp(child->name, MACRO_TAG)) {
			customMacro = child->children;
			if(child) {
				customMacro->next = customMacros;
				customMacros = customMacro;
				*customMacrosPtr = customMacros;
			}

		} else if((customMacro = getCustomMacro(customMacros, child->name))) {
			child = executeMacro(customMacro->children, child->children);
		}

		child->next = object->children;
		object->children = child;

	}

	__free(closingTag);
	return object;
}

/**
 * Get the macro by its name
 * 
 * @param  macros       The list of macros
 * @param  name         The name of the macro to find
 * @return              The address of the found macro, NULL if not found
 */
Macro* getMacro(Macro* macros, char* name) {

	for(Macro* macro=macros; macro!=NULL; macro=macro->next) {
		if(!__strcmp(macro->name, name))
			return macro;
	}

	return NULL;
}

/**
 * Render a table string from the table object
 * 
 * @param  macro_ptr The address of the macro list
 * @param  table     The table object
 * @return           The string representation of the table
 */
char* table(void* macro_ptr, Object* table) {
	char *tableString=NULL;
	Macro *textMacro, *paragraphMacro, *macros;
	size_t tableStringSize=0;
	Object* rows=NULL, *nextChild, *row;
	unsigned int rowNum=0, fieldNum=0;
	size_t fieldWidth;
	char** lineBuffer, **oldLineBuffer;
	size_t tableLength = 0;
	unsigned int rowIndex=0;
	size_t tableWidth;
	unsigned int borderType=LINE_BORDER_TYPE;

	macros = (Macro*) macro_ptr;
	textMacro = getMacro(macros, TEXT_TAG);
	paragraphMacro = getMacro(macros, PARAGRAPH_TAG);

	for(Object* child=table->children; child!=NULL; child=nextChild) {
		nextChild = child->next;

		if(!__strcmp(child->name, ROWS_TAG)) {
			child = child->children;
			if(child)
				rowNum = __atoi(child->name);
			continue;
		}

		if(!__strcmp(child->name, FIELDS_TAG)) {
			child = child->children;
			if(child)
				fieldNum = __atoi(child->name);
			continue;
		}

		if(!__strcmp(child->name, BORDER_TAG)) {
			child = child->children;

			if(child) {
				if(!__strcmp(child->name, LINE_BORDER_STR))
					borderType = LINE_BORDER_TYPE;
				else if(!__strcmp(child->name, STAR_BORDER_STR))
					borderType = STAR_BORDER_TYPE;
			}
			continue;
		}

		if(!__strcmp(child->name, ROW_TAG)) {
			child->next = rows;
			rows = child;
			continue;
		}
	}

	if(rowNum == 0 || fieldNum == 0 || (fieldNum * 4) + 1 > columnWidth)
		return NULL;

	fieldWidth = (columnWidth - 1) / fieldNum - 3;
	tableWidth = (fieldWidth + 3) * fieldNum + 1;
	tableLength = rowNum * 2 + 1;
	if(!(lineBuffer = __malloc(tableLength * sizeof(char *))))
		return NULL;

	for (int i=0; i < tableLength; i++) {
		if(!(lineBuffer[i] = (char *)__malloc(tableWidth + 3)))
			return NULL;

		__memset(lineBuffer[i], 0, tableWidth + 3);
	}
	oldLineBuffer = lineBuffer;

	row = rows;
	for(int r=0; r<rowNum; r++) {
		Object* field;
		size_t fieldHeight = 1;

		if(row == NULL)
			field = NULL;
		else {
			reverseObjectList((Object **)&row->children);
			field = row->children;
		}

		if(borderType == LINE_BORDER_TYPE) {
			for(int fieldIndex=0; fieldIndex<fieldNum ;fieldIndex++) {
				__strcat(lineBuffer[rowIndex], INTERSECT_LINE_BORDER);
				__memset(&lineBuffer[rowIndex][fieldIndex*(fieldWidth + 3) + 1], HORIZONTAL_LINE_BORDER, fieldWidth+2);
			}
			__strcat(lineBuffer[rowIndex], INTERSECT_LINE_BORDER);
			rowIndex++;
		} else if(borderType == STAR_BORDER_TYPE)
			__memset(lineBuffer[rowIndex++], HORIZONTAL_STAR_BORDER, tableWidth);

		for(int fieldIndex=0; fieldIndex<fieldNum; fieldIndex++) {
			char* fieldString=NULL;
			char* elementString=NULL;
			size_t fieldStringSize;
			size_t elementStringSize;
			unsigned int lines=0;
			size_t fieldRowIndex=rowIndex;
			unsigned int newLines=0;

			if(field == NULL) {
				if(!(fieldString = __malloc(fieldWidth + 1)))
					return NULL;

				__memset(fieldString, ' ', fieldWidth);
				fieldString[fieldWidth] = '\0';
			} else if(!__strcmp(field->name, HEADER_TAG)) {
				fieldString = textMacro->body(macros, field->children);
			} else if(!__strcmp(field->name, FIELD_TAG)) {
				fieldString = paragraphMacro->body(macros, field->children);
			} 

			elementString = __strtok(fieldString, '\n');
			while(elementString != NULL) {

				elementStringSize = __strlen(elementString);
				if(elementStringSize % fieldWidth > 0)
					lines += 1;

				lines += elementStringSize / fieldWidth;

				if(lines > fieldHeight) {
					newLines = lines - fieldHeight;
					fieldHeight += newLines;
					tableLength += newLines;
					if(!(lineBuffer = __malloc(tableLength * sizeof(char *))))
						return NULL;

					for (int i=0; i < tableLength; i++) {
						if(i < tableLength - newLines) {
							lineBuffer[i] = oldLineBuffer[i];
						} else {
							if(!(lineBuffer[i] = (char *)__malloc(tableWidth + 3)))
								return NULL;

							__memset(lineBuffer[i], 0, tableWidth + 3);
						}
						if(i >= rowIndex + fieldHeight - newLines && i < rowIndex + fieldHeight) {
							for(int fieldIndex2=0; fieldIndex2<fieldIndex; fieldIndex2++) {
								size_t existing=0;
								
								if(borderType == LINE_BORDER_TYPE)
									__strcat(lineBuffer[i], VERTICAL_LINE_BORDER);
								else if(borderType == STAR_BORDER_TYPE)
									__strcat(lineBuffer[i], VERTICAL_STAR_BORDER);
								__strcat(lineBuffer[i], " ");
								
								existing = __strlen(lineBuffer[i]);
								__memset(&lineBuffer[i][existing], ' ', fieldWidth);
								__strcat(lineBuffer[i], " ");						
							}
						}
					}
					oldLineBuffer = lineBuffer;

				}

				for(int l=fieldRowIndex; l<rowIndex + lines;l++) {
					size_t remaining=0;
				
					remaining = __strlen(elementString);

					if(borderType == LINE_BORDER_TYPE)
						__strcat(lineBuffer[l], VERTICAL_LINE_BORDER);
					else if(borderType == STAR_BORDER_TYPE)
						__strcat(lineBuffer[l], VERTICAL_STAR_BORDER);
					__strcat(lineBuffer[l], " ");
					
					if(remaining >= fieldWidth) {
						__strncat(lineBuffer[l], elementString, fieldWidth);
						elementString += fieldWidth;
					} else if(remaining < fieldWidth) {
						size_t existing=0;

						__strcat(lineBuffer[l], elementString);
						existing = __strlen(lineBuffer[l]);
						__memset(&lineBuffer[l][existing], ' ', fieldWidth - remaining);
						elementString += remaining;
					}

					if(fieldIndex == fieldNum - 1) {
						__strcat(lineBuffer[l], " ");						
						if(borderType == LINE_BORDER_TYPE)
							__strcat(lineBuffer[l], VERTICAL_LINE_BORDER);
						else if(borderType == STAR_BORDER_TYPE)
							__strcat(lineBuffer[l], VERTICAL_STAR_BORDER);						
					} else 
						__strcat(lineBuffer[l], " ");					
				}

				fieldRowIndex = rowIndex + lines;
				elementString = __strtok(NULL, '\n');

			}

			for(int l=fieldRowIndex; l<rowIndex + fieldHeight; l++) {
				size_t existing;

				if(borderType == LINE_BORDER_TYPE)
					__strcat(lineBuffer[l], VERTICAL_LINE_BORDER);
				else if(borderType == STAR_BORDER_TYPE)
					__strcat(lineBuffer[l], VERTICAL_STAR_BORDER);
				__strcat(lineBuffer[l], " ");

				existing = __strlen(lineBuffer[l]);
				__memset(&lineBuffer[l][existing], ' ', fieldWidth);

				if(fieldIndex == fieldNum - 1) {
					__strcat(lineBuffer[l], " ");	
					if(borderType == LINE_BORDER_TYPE)
						__strcat(lineBuffer[l], VERTICAL_LINE_BORDER);
					else if(borderType == STAR_BORDER_TYPE)
						__strcat(lineBuffer[l], VERTICAL_STAR_BORDER);				
				} else 
					__strcat(lineBuffer[l], " ");	

			}

			if(field != NULL)
				field = field->next;
		}
		rowIndex += fieldHeight;

		if(row != NULL)
			row = row->next;
	}

	if(borderType == LINE_BORDER_TYPE) {
			for(int fieldIndex=0; fieldIndex<fieldNum ;fieldIndex++) {
				__strcat(lineBuffer[rowIndex], INTERSECT_LINE_BORDER);
				__memset(&lineBuffer[rowIndex][fieldIndex*(fieldWidth + 3) + 1], HORIZONTAL_LINE_BORDER, fieldWidth+2);
			}
			__strcat(lineBuffer[rowIndex], INTERSECT_LINE_BORDER);

	} else if(borderType == STAR_BORDER_TYPE)
		__memset(lineBuffer[rowIndex], HORIZONTAL_STAR_BORDER, tableWidth);

	if(!(tableString = __malloc(tableLength * (tableWidth + 2) + 1)))
		return NULL;

	__memset(tableString, 0, tableLength * (tableWidth + 2) + 1);
	for(int l=0; l<tableLength; l++) {
		__strcat(tableString, lineBuffer[l]);
		__strcat(tableString, "\n");
	}

	return tableString;
}

/**
 * Render a element string from the element object
 * 
 * @param  macro_ptr The address of the macro list
 * @param  element     The element object
 * @return           The string representation of the element
 */
char* element(void* macro_ptr, Object* element) {
	Macro *textMacro, *listMacro, *macros;
	char* newElementString=NULL, *oldElementString=NULL, *textString=NULL, *listString=NULL;
	size_t elementStringSize=0;

	macros = (Macro*) macro_ptr;
	textMacro = getMacro(macros, TEXT_TAG);
	listMacro = getMacro(macros, LIST_TAG);

	for(Object* child=element->children; child!=NULL; child=child->next) {
		if(!__strcmp(child->name, TEXT_TAG)) {
			textString = textMacro->body(macros, child);
			elementStringSize += __strlen(textString) + __strlen("\n");
		} else if(!__strcmp(child->name, LIST_TAG)) {
			listString = listMacro->body(macros, child);
		}
	}

	if(elementStringSize>0) {
		if(!(newElementString = __malloc(elementStringSize+1)))
			return NULL;

		__memset(newElementString, 0, elementStringSize+1);
		oldElementString = newElementString;

		if(textString) {
			__strcat(newElementString, textString);
		}

		if(listString) {
			char* subElementString;
			size_t subElementStringSize=0;

			__strcat(newElementString, "\n");
			subElementString = __strtok(listString, '\n');
			while(subElementString != NULL) {
				subElementStringSize = __strlen(subElementString);
				elementStringSize += subElementStringSize + __strlen(LIST_INDENT) + __strlen("\n");
				if(!(newElementString = __malloc(elementStringSize+1)))
					return NULL;

				__memset(newElementString, 0, elementStringSize+1);
				__memcpy(newElementString, oldElementString, __strlen(oldElementString));
				__free(oldElementString);
				oldElementString = newElementString;

				__strcat(newElementString, LIST_INDENT);
				__strcat(newElementString, subElementString);
				__strcat(newElementString, "\n");
				
				subElementString = __strtok(NULL, '\n');

			}

		}

	}

	return newElementString;
}

/**
 * Render a list string from the list object
 * 
 * @param  macro_ptr The address of the macro list
 * @param  list      The list object
 * @return           The string representation of the list
 */
char* list(void* macro_ptr, Object* list) {
	char *newList=NULL, *oldList=NULL;
	Macro *macros;
	char customNumString[4];
	Macro *elementMacro;
	size_t listStringSize=0;
	Object* elements=NULL, *nextChild;
	unsigned int totalElementNum=0;
	unsigned int listType = '*';

	macros = (Macro*) macro_ptr;
	elementMacro = getMacro(macros, ELEMENT_TAG);

	__memset(customNumString, 0, 4);

	for(Object* child=list->children; child!=NULL; child=nextChild) {
		nextChild = child->next;

		if(!__strcmp(child->name, TYPE_TAG)) {
			Object* grandchild;

			grandchild = child->children;
			if(!child)
				continue;
			if(!__strcmp(grandchild->name, NUMERAL_STR)) {
				listType = NUMERAL;
			} else if(!__strcmp(grandchild->name, UPPER_ALPHA_STR)) {
				listType = UPPER_ALPHA;
			} else if(!__strcmp(grandchild->name, LOWER_ALPHA_STR)) {
				listType = LOWER_ALPHA;
			} else if(!__strcmp(grandchild->name, ROMAN_STR)) {
				listType = ROMAN;
			} else {
#ifdef PATCHED_2
				__strncat(customNumString, grandchild->name, 3);
#else
				__strncat(customNumString, grandchild->name, 8);
#endif

			}
		} else if(!__strcmp(child->name, ELEMENT_TAG)) {
			child->next = elements;
			elements = child;
			continue;
		}
	}

	for(Object* element=elements; element!=NULL; element=element->next) {
		char* elementString=NULL;
		size_t elementStringSize=0;
		char* elementNumString;
		size_t elementNumStringSize=0;

		if(!__strcmp(element->name, ELEMENT_TAG)) {
			elementString = elementMacro->body(macros, element);
			if(!elementString)
				continue;

			elementStringSize = __strlen(elementString);

			totalElementNum++;
			if(!(elementNumString = __malloc(12)))
				return NULL;

			__memset(elementNumString, 0, 12);
			if(listType == NUMERAL) {
				elementNumString = __itoa(totalElementNum, elementNumString);
			} else if(listType == UPPER_ALPHA) {
				elementNumString[0] = 0x40 + totalElementNum;
			} else if(listType == LOWER_ALPHA) {
				elementNumString[0] = 0x60 + totalElementNum;
			} else if(listType == ROMAN) {
				__free(elementNumString);
				elementNumString = romanNumeral(totalElementNum);
			} else {
				elementNumString[0] = (char) listType;
			}

			if(!elementNumString)
				return NULL;

			elementNumStringSize = __strlen(elementNumString);

			listStringSize += elementNumStringSize + elementStringSize + __strlen(". ") + __strlen("\n");

			if(!(newList = __malloc(listStringSize+1)))
				return NULL;

			__memset(newList, 0, listStringSize+1);
			if(oldList != NULL) {
				__memcpy(newList, oldList, __strlen(oldList));
				__free(oldList);
			}
			oldList = newList;

			__strcat(newList, elementNumString);
			if(listType < 4)
				__strcat(newList, ".");
			__strcat(newList, " ");
			__strcat(newList, elementString);
			__strcat(newList, "\n");
			__free(elementString);
		} 
	}

	return newList;
}

/**
 * Render a text string from the text object
 * 
 * @param  macro_ptr The address of the macro list
 * @param  text      The text object
 * @return           The string representation of the text
 */
char* text(void* macro_ptr, Object* text) {
	Object* child;

	child = text->children;

	if(child)
		return child->name;
	
	return NULL;
}

/**
 * Render a paragraph string from the paragraph object
 * 
 * @param  macro_ptr The address of the macro list
 * @param  paragraph The paragraph object
 * @return           The string representation of the paragraph
 */
char* paragraph(void* macro_ptr, Object* paragraph) {
	char *paragraphString=NULL, *childString=NULL;
	Macro *textMacro, *listMacro, *tableMacro, *macros;
	size_t paragraphSize=0;
	Object* child=NULL;

	macros = (Macro*) macro_ptr;
	textMacro = getMacro(macros, TEXT_TAG);
	listMacro = getMacro(macros, LIST_TAG);
	tableMacro = getMacro(macros, TABLE_TAG);

	child = paragraph->children;
	if(!child)
		return NULL;

	if(!__strcmp(child->name, TEXT_TAG)) {
		childString = textMacro->body(macros, child);
		if(!childString)
			return NULL;

		paragraphSize = __strlen(childString) + __strlen(INDENT);
		if(!(paragraphString = __malloc(paragraphSize+1)))
			return NULL;

		__memset(paragraphString, 0, paragraphSize+1);
		__strcat(paragraphString, INDENT);
		__strcat(paragraphString, childString);
		__free(childString);
	} else if(!__strcmp(child->name, LIST_TAG)) {
		childString = listMacro->body(macros, child);
		if(!childString)
			return NULL;

		paragraphSize = __strlen(childString);
		if(!(paragraphString = __malloc(paragraphSize+1)))
			return NULL;

		__memset(paragraphString, 0, paragraphSize+1);
		__strcat(paragraphString, childString);
		__free(childString);
	} else if(!__strcmp(child->name, TABLE_TAG)) {
		childString = tableMacro->body(macros, child);
		if(!childString)
			return NULL;

		paragraphSize = __strlen(childString);
		if(!(paragraphString = __malloc(paragraphSize+1)))
			return NULL;

		__memset(paragraphString, 0, paragraphSize+1);
		__strcat(paragraphString, childString);
		__free(childString);		
	}

	return paragraphString;
}

/**
 * Render a page string from the page object
 * @param  macro_ptr The address of the macro list
 * @param  page      The page object
 * @return           The string representation of the page
 */
char* page(void* macro_ptr, Object* page) {
	char *newPage=NULL, *oldPage=NULL;
	Macro *paragraphMacro, *macros;
	size_t pageSize=0;
	Object *children=NULL, *nextChild;

	macros = (Macro*) macro_ptr;
	paragraphMacro = getMacro(macros, PARAGRAPH_TAG);

	// Reverse order of children
	for(Object* child=page->children; child!=NULL; child=nextChild) {
		nextChild = child->next;
		child->next = children;
		children = child;
	}


	for(Object* child=children; child!=NULL; child=child->next) {
		char* childString;
		size_t childStringSize;

		if(!__strcmp(child->name, PARAGRAPH_TAG)) {
			childString = paragraphMacro->body(macros, child);
			if(!childString)
				continue;
		} else {
			continue;
		}

		childStringSize = __strlen(childString);
		pageSize += childStringSize + __strlen("\n");
		if(!(newPage = __malloc(pageSize+1)))
			return NULL;

		__memset(newPage, 0, pageSize+1);
		if(oldPage != NULL) {
			__strcat(newPage, oldPage);
			__free(oldPage);			
		}
		__strcat(newPage, childString);
		__free(childString);
		__strcat(newPage, "\n");
		oldPage = newPage;
	}

	return newPage;
}

/**
 * Render a document string from the document object
 * @param  macro_ptr The address of the macro list
 * @param  document  The document object
 * @return           The string representation of the document
 */
char* document(void* macro_ptr, Object* document) {
	char *newDocument=NULL, *oldDocument=NULL;
	Macro *pageMacro, *columnMacro, *macros;
	size_t documentSize=0;
	Object* pages=NULL;
	Object* nextChild;
	unsigned int totalPageNum=0;
	char** lineBuffer;
	size_t docLength=11, docWidth=20;
	unsigned int columnNum=1;

	macros = (Macro*) macro_ptr;
	pageMacro = getMacro(macros, PAGE_TAG);
	columnMacro = getMacro(macros, COLUMN_TAG);

	for(Object* child=document->children; child!=NULL; child=nextChild) {
		nextChild = child->next;

		if(!__strcmp(child->name, PAGE_TAG) || !__strcmp(child->name, COLUMN_TAG)) {
			child->next = pages;
			pages = child;
			continue;
		}

		if(!__strcmp(child->name, LENGTH_TAG)) {
			child = child->children;
			if(child)
				docLength = __atoi(child->name);
			continue;
		}

		if(!__strcmp(child->name, WIDTH_TAG)) {
			child = child->children;
			if(child)	
				docWidth = __atoi(child->name);
			continue;
		}
	}

	if(!(lineBuffer = __malloc(docLength * sizeof(char *))))
		return NULL;

	for (int i=0; i < docLength; i++) {
		if(!(lineBuffer[i] = (char *)__malloc(docWidth + 3)))
			return NULL;

		__memset(lineBuffer[i], 0, docWidth + 3);

	}

	columnWidth = docWidth;

	for(Object* page=pages; page!=NULL; page=page->next) {
		char *pageString, *columnString;
		size_t pageSize, numLines;
		char* lineString;
		char* pageNumString;
		size_t lineStringLength;
		unsigned int columnIndex;

		if(!__strcmp(page->name, COLUMN_TAG)) {
			columnString = columnMacro->body(macros, page);
			if(!columnString) {
				columnNum = 1;
				continue;
			}
			columnNum = __atoi(columnString);
			if((docWidth / columnNum) - __strlen(TAB) < 1)
				columnNum = 1;
			columnWidth = (docWidth / columnNum) - __strlen(TAB);
			continue;
		}

		pageString = pageMacro->body(macros, page);
		if(!pageString) 
			continue;

		lineString = __strtok(pageString, '\n');

		// Add new page
		totalPageNum++;
		documentSize += (docWidth + 1) * (docLength + 2);
		if(!(newDocument = __malloc(documentSize+1)))
			return NULL;

		__memset(newDocument, 0, documentSize+1);
		if(oldDocument != NULL) {
			size_t existing;
			
			existing = __strlen(oldDocument);
			__memcpy(newDocument, oldDocument, existing);
			__free(oldDocument);
		}
		oldDocument = newDocument;

		columnIndex = 1;
		// Add Lines to page
		for(int lineIndex = 0; lineIndex < docLength; lineIndex++) {
			// Last line in page
			if(lineIndex == docLength - 1) {

				// Last column in page
				if(columnIndex == columnNum) {
					size_t existing;

					// Add page number
					if(!(pageNumString = __malloc(docWidth)))
						return NULL;

					__memset(pageNumString, 0, docWidth);
					pageNumString = __itoa(totalPageNum, pageNumString);
					__strcat(lineBuffer[lineIndex], " ");
					__strncat(lineBuffer[lineIndex], pageNumString, docWidth-1);

					__memset(newDocument, '=', docWidth);
					__strcat(newDocument, "\n");
					// Append lineBuffers to document
					for(int index=0; index<docLength; index++) {
						__strcat(newDocument, lineBuffer[index]);
						__strcat(newDocument, "\n");

					}

					existing = __strlen(newDocument);
					__memset(&newDocument[existing], '=', docWidth);

					for (int i=0; i < docLength; i++) {
						__memset(lineBuffer[i], 0, docWidth + 3);
					}

					// More lines in page?
					if(lineString != NULL) {
						// Add new page
						totalPageNum++;
						documentSize += (docWidth + 1) * (docLength + 2);
						if(!(newDocument = __malloc(documentSize+1)))
							return NULL;

						__memset(newDocument, 0, documentSize+1);
						if(oldDocument != NULL) {
							size_t existing;
							
							existing = __strlen(oldDocument);
							__memcpy(newDocument, oldDocument, existing);
							__free(oldDocument);
						}
						oldDocument = newDocument;
						// Reset index
						lineIndex = -1;
						columnIndex = 1;		
					}
				} else {
					columnIndex++;
					lineIndex = -1;
				}

				continue;
			}

			// No more lines
			if(lineString == NULL) {
				size_t remaining;

				remaining = __strlen(lineBuffer[lineIndex]);
				__memset(&lineBuffer[lineIndex][remaining], ' ', columnWidth);
				__strcat(lineBuffer[lineIndex], TAB);
				continue;
			}

			lineStringLength = __strlen(lineString);
			if(lineStringLength > columnWidth) {
				__strncat(lineBuffer[lineIndex], lineString, columnWidth);
				if(columnNum > 1)
					__strcat(lineBuffer[lineIndex], TAB);
				lineString += columnWidth;
			} else {
				size_t lineBufferLength;

				__strcat(lineBuffer[lineIndex], lineString);
				for(int i=0; i<columnWidth - lineStringLength; i++)
					__strcat(lineBuffer[lineIndex], " ");

				if(columnNum > 1)
					__strcat(lineBuffer[lineIndex], TAB);
				lineString = __strtok(NULL, '\n');
			}

		}

		transmit_all(STDOUT, newDocument, __strlen(newDocument));
		transmit_all(STDOUT, "\x3", __strlen("\x3"));
		__free(newDocument);
		oldDocument=NULL;
		newDocument=NULL;

	}

	return newDocument;
}

/**
 * Create a new macro
 * 
 * @param  name 	The name of the new macro
 * @param  body 	The address of the macro function
 * @return          The address of the newly create macro
 */
Macro* newMacro(char* name, char* (*body)(void *macros, Object* object)) {
	Macro* macro;
	size_t nameSize;

	if(!(macro = __malloc(sizeof(Macro))))
		return NULL;

	nameSize = __strlen(name);
	if(!(macro->name = __malloc(nameSize+1)))
		return NULL;

	__memset(macro->name, 0, nameSize+1);
	__memcpy(macro->name, name, nameSize);
	macro->body = body;

	return macro;
}

/**
 * Initialize the macros
 * 
 * @param macro_ptr The address of the macro list
 */
void initMacros(Macro** macro_ptr) {
	Macro* macro;

	macro = newMacro(ELEMENT_TAG, &element);
	macro->next = *macro_ptr;
	*macro_ptr = macro;

	macro = newMacro(DOCUMENT_TAG, &document);
	macro->next = *macro_ptr;
	*macro_ptr = macro;

	macro = newMacro(PAGE_TAG, &page);
	macro->next = *macro_ptr;
	*macro_ptr = macro;

	macro = newMacro(PARAGRAPH_TAG, &paragraph);
	macro->next = *macro_ptr;
	*macro_ptr = macro;

	macro = newMacro(TEXT_TAG, &text);
	macro->next = *macro_ptr;
	*macro_ptr = macro;

	macro = newMacro(LIST_TAG, &list);
	macro->next = *macro_ptr;
	*macro_ptr = macro;

	macro = newMacro(COLUMN_TAG, &text);
	macro->next = *macro_ptr;
	*macro_ptr = macro;

	macro = newMacro(TABLE_TAG, &table);
	macro->next = *macro_ptr;
	*macro_ptr = macro;

}

/**
 * Initialize all custom macros
 * 
 * @param customMacrosPtr The address of the custom macro list
 */
void initCustomMacros(Object **customMacrosPtr) {
	getObject(customMacrosPtr, NULL, &FourByFourTable_Macro);
	getObject(customMacrosPtr, NULL, &FiveByFiveTable_Macro);
	getObject(customMacrosPtr, NULL, &AlphanumericOutline_Macro);
	getObject(customMacrosPtr, NULL, &BulletedOutline_Macro);

}

/**
 * Send the Document ID to the requester
 * 
 * @param documentID The value of the documentID
 */
void sendDocumentID(unsigned int documentID) {
	char* documentIDString;

	if(!(documentIDString = __malloc(20)))
		return;

	documentIDString = __itoa(documentID, documentIDString);
	transmit_all(STDOUT, DOC_ID_STRING, __strlen(DOC_ID_STRING));
	transmit_all(STDOUT, documentIDString, __strlen(documentIDString));
	transmit_all(STDOUT, "\n", __strlen("\n"));
}


int main(void) {
	Object* documentObject;
	Macro* macros;
	char* documentString;
	Object* customMacros=NULL;
	char* titleObject;
	unsigned int documentID;
	char* documentIDString;

	documentID = getDocumentID();

	if(!(titleObject = __malloc(sizeof(Macro))))
		return -1;

	initMacros(&macros);
	initCustomMacros(&customMacros);
	__free(titleObject);
	documentObject = getObject(&customMacros, NULL, NULL);

	sendDocumentID(documentID);
	documentString = document(macros, documentObject);

	return 0;
}