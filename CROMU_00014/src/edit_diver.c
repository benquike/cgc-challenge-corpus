/*

Author: Steve Wood <swood@cromulence.co>

Copyright (c) 2014 Cromulence LLC

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
#include "service.h"
#include "stdlib.h"

int edit_diver(logbook_type *Info)  {

	char buffer[1024];
	size_t count;

	__printf("First Name");

	if (Info->diver.first_name[0]!= 0)
		__printf(" (@s)", Info->diver.first_name);

	__printf(": ");

	count=getline(buffer, sizeof(Info->diver.first_name));

	if (count > 0)
		__strncpy(Info->diver.first_name, buffer, sizeof(Info->diver.first_name));


	__printf("Last Name");

	if (Info->diver.last_name[0]!= 0)
		__printf(" (@s)", Info->diver.last_name);

	__printf(": ");

	count=getline(buffer, sizeof(Info->diver.last_name));

	if (count > 0)
		__strncpy(Info->diver.last_name, buffer, sizeof(Info->diver.last_name));


	__printf("Street");

	if (Info->diver.street[0]!= 0)
		__printf(" (@s)", Info->diver.street);

	__printf(": ");

	count=getline(buffer, sizeof(Info->diver.street));

	if (count > 0)
		__strncpy(Info->diver.street, buffer, sizeof(Info->diver.street));

		__printf("City");

	if (Info->diver.city[0]!= 0)
		__printf(" (@s)", Info->diver.city);

	__printf(": ");

	count=getline(buffer, sizeof(Info->diver.city));

	if (count > 0)
		__strncpy(Info->diver.city, buffer, sizeof(Info->diver.city));

		__printf("State");

	if (Info->diver.state[0]!= 0)
		__printf(" (@s)", Info->diver.state);

	__printf(": ");

	count=getline(buffer, sizeof(Info->diver.state));

	if (count > 0)
		__strncpy(Info->diver.state, buffer, sizeof(Info->diver.state));

		__printf("Zip Code");

	if (Info->diver.zip[0]!= 0)
		__printf(" (@s)", Info->diver.zip);

	__printf(": ");

	count=getline(buffer, sizeof(Info->diver.zip));

	if (count > 0)
		__strncpy(Info->diver.zip, buffer, sizeof(Info->diver.zip));

	__printf("Phone Number");

	if (Info->diver.phone[0]!= 0)
		__printf(" (@s)", Info->diver.phone);

	__printf(": ");

	count=getline(buffer, sizeof(Info->diver.phone));

	if (count > 0)
		__strncpy(Info->diver.phone, buffer, sizeof(Info->diver.phone));

	
	__printf("PADI Diver Number");

	if (Info->diver.padi_number[0]!= 0)
		__printf(" (@s)", Info->diver.padi_number);

	__printf(": ");

	count=getline(buffer, sizeof(Info->diver.padi_number));

	if (count > 0)
		__strncpy(Info->diver.padi_number, buffer, sizeof(Info->diver.padi_number));

	__printf("PADI Cert Date");

	if (Info->diver.cert_date[0]!= 0)
		__printf(" (@s)", Info->diver.cert_date);

	__printf(": ");

	count=getline(buffer, sizeof(Info->diver.cert_date));

	if (count > 0)
		__strncpy(Info->diver.cert_date, buffer, sizeof(Info->diver.cert_date));


	print_diver_info(Info);

return 0;
}


int print_diver_info(logbook_type *Info)  {

	__printf("\n");
	__printf("     Name: @s @s\n", Info->diver.first_name, Info->diver.last_name);
	__printf("  Address: @s\n", Info->diver.street);
	__printf("     City: @s\n", Info->diver.city);
	__printf("    State: @s\n", Info->diver.state);
	__printf(" Zip Code: @s\n", Info->diver.zip);
	__printf("    Phone: @s\n", Info->diver.phone);
	__printf(" PADI Num: @s\n", Info->diver.padi_number);
	__printf("Cert Date: @s\n", Info->diver.cert_date);
	__printf("\n");

	return 0;

}
