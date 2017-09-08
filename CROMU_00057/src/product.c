/*

Author: Steve Wood <swood@cromulence.com>

Copyright (c) 2015 Cromulence LLC

Permission is hereby granted, __free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include <libcgc.h>
#include "stdlib.h"
#include "service.h"
#include "stdio.h"
#include "string.h"
#include "commands.h"
#include "output_strings.h"
#include "malloc.h"

char *obf_strings(char *input_string);

int create_product( productDefType **database, void *command_data ) {

newProductMessageType *msg;
productDefType *newProduct;

	msg = (newProductMessageType *)command_data;

	// special case if this is the first product
	if (*database == 0) {

		*database = __calloc(sizeof(productDefType));

		if (*database == 0)
			_terminate(-1);

		newProduct = *database;

	}
	else {

		newProduct = *database;

		// add this new product to the end of the list
		while(newProduct->ID != msg->ID && newProduct->next != 0)
			newProduct = newProduct->next;

		// if this ID is already in the database, fail 
		if (newProduct->ID == msg->ID)
			return(-1);

		// otherwise add this new product to the end of the list
		newProduct->next = (productDefType *)__calloc(sizeof(productDefType));

		if (newProduct->next == 0)
			_terminate(-1);


		newProduct = newProduct->next;
	}

	newProduct->next = 0;
	newProduct->ID = msg->ID;
	newProduct->productBacklog = 0;
	newProduct->sprintList = 0;

	// allocate memory for the title
	newProduct->title = __calloc(__strlen((char *)&msg->title)+1);

	if (newProduct->title == 0)
		_terminate(-1);


	__strncpy(newProduct->title, (char *)&msg->title, __strlen(&msg->title));

	return 0;
}



int delete_product( productDefType **database, messageIDType *message ) {


productDefType *lastProduct, *tmpProduct;
sprintEntryType *sprintPtr, *tmpSprintPtr;
backlogItemType *PBIPtr, *tmpPBIPtr;


	// nothing to delete here, move along
	if (*database == 0)  {

		return(-1);
	}

	// is the matching entry the head of the linked list?
	if ((*database)->ID == message->ID) {

		tmpProduct = *database;
		*database = (*database)->next;

		// if there's a title, __free that memory
		if (tmpProduct->title)
			__free(tmpProduct->title);

		// now __free the Sprint list
		sprintPtr = tmpProduct->sprintList;

		while (sprintPtr!= 0) {

			PBIPtr = sprintPtr->sprintBacklogList;

			// first __free the backlog items tied to this Sprint
			while (PBIPtr != 0) {

				if (PBIPtr->description != 0)
					__free(PBIPtr->description);

				tmpPBIPtr = PBIPtr;
				PBIPtr = PBIPtr->next;

				__free(tmpPBIPtr);
			}

			// __free the memory for the sprint title
			if (sprintPtr->title != 0)
				__free(sprintPtr->title);


			// now __free the sprint memory itself
			tmpSprintPtr = sprintPtr;

			sprintPtr = sprintPtr->next;

			__free(tmpSprintPtr);

		}

		// now __free the PBI list
		PBIPtr = tmpProduct->productBacklog;

		while (PBIPtr != 0) {


			if (PBIPtr->description != 0)
				__free(PBIPtr->description);

			tmpPBIPtr = PBIPtr;
			PBIPtr = PBIPtr->next;

			__free(tmpPBIPtr);

		}

		// now __free the final object
		__free(tmpProduct);

		return(0);

	}

	// the entry to delete wasn't the head, so find it in the list
	lastProduct = *database;
	tmpProduct = (*database)->next;


	while (tmpProduct != 0 && tmpProduct->ID != message->ID  ) {

		lastProduct = tmpProduct;
		tmpProduct = tmpProduct->next;
	}

	// it wasn't found by the end of the list was reached
	if (tmpProduct == 0 ) {

		return(-1);
	}

	// __link around the entry to be deleted
	lastProduct->next = tmpProduct->next;

	// __free the title memory
	if (tmpProduct->title)
		__free(tmpProduct->title);


	// now __free the PBI
	PBIPtr = tmpProduct->productBacklog;

	while (PBIPtr != 0) {

		tmpPBIPtr = PBIPtr;

		if (PBIPtr->description != 0)
			__free(PBIPtr->description);

		PBIPtr = PBIPtr->next;

		__free(tmpPBIPtr);

	}

	// now __free the Sprints
	sprintPtr = tmpProduct->sprintList;

	while (sprintPtr!= 0) {

		PBIPtr = sprintPtr->sprintBacklogList;

		// first __free the backlog items tied to this Sprint
		while (PBIPtr != 0) {

			if (PBIPtr->description != 0)
				__free(PBIPtr->description);

			tmpPBIPtr = PBIPtr;
			PBIPtr = PBIPtr->next;

			__free(tmpPBIPtr);
		}

		// __free the memory for the sprint title
		if (sprintPtr->title != 0)
			__free(sprintPtr->title);


		// now __free the sprint memory itself
		tmpSprintPtr = sprintPtr;

		sprintPtr = sprintPtr->next;

		__free(tmpSprintPtr);

	}

	__free(tmpProduct);

	return 0;
}

int list_all_products( productDefType *database) {


	if (database == 0)
		return (-1);

	while (database != 0 ) {


		__printf(obf_strings(List_Products), database->title);

		database = database->next;

	}
	__printf("\n");

	return 0;

}


int list_product( productDefType *database, messageIDType *message) {

sprintEntryType *sprintPtr;
backlogItemType *PBIPtr;


	if (message->ID == 0) {

		list_all_products(database);
		return 0;

	}

	while(database != 0 && database->ID != message->ID )
		database = database->next;

	if (database == 0) {

		return(-1);
	}

	__printf("\n");
	__printf(obf_strings(Prod_Title), database->title);
	__printf(obf_strings(Prod_ID), database->ID);
	__printf("\n");

	PBIPtr = database->productBacklog;

	__printf(obf_strings(Prod_Backlog));
	__printf(obf_strings(Prod_Backlog2));
	
	while (PBIPtr != 0) {

		// __printf(obf_strings(Prod_Backlog3), PBIPtr->ID, PBIPtr->story_points, PBIPtr->description);
		__printf(obf_strings(Prod_Backlog3), PBIPtr->ID, PBIPtr->story_points);
		PBIPtr = PBIPtr->next;
	}

	__printf("\n");

	sprintPtr = database->sprintList;
	__printf(obf_strings(Sprints_Title));

	while (sprintPtr != 0) {

		__printf(obf_strings(Sprint_Entry), sprintPtr->ID, sprintPtr->title);

		PBIPtr = sprintPtr->sprintBacklogList;

		while (PBIPtr != 0) {
			// __printf(obf_strings(SBI_Entry), PBIPtr->ID, PBIPtr->story_points, PBIPtr->status, PBIPtr->description);
			__printf(obf_strings(SBI_Entry), PBIPtr->ID, PBIPtr->story_points, PBIPtr->status);

			PBIPtr = PBIPtr->next;
		}

		__printf("\n");
		sprintPtr = sprintPtr->next;

	}

	__printf("\n");
	return(0);

}




