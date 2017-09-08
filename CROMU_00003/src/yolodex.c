/*

Author: Dustin Fraze (df@cromulence.co)

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
#include <stdlib.h>

#define YOLO_ADD 0x40
#define YOLO_DEL 0x41
#define YOLO_EDIT 0x42
#define YOLO_SHOW 0x43
#define YOLO_LIST 0x44
#define YOLO_SORT 0x45
#define YOLO_EXIT 0x46

unsigned int cookie = 0;

typedef struct _contact {
	char first[32];
	char last[32];
	char phone[16];
	unsigned short officenum;
	char gender;
	char hacker;
	unsigned int cookie;
	int pad;
	struct _contact *prev;
	struct _contact *next;
} contact;

contact *head = NULL;

void put(char *str) {
	transmit(1, str, __strlen(str), 0);
}

int validateName(char *name)
{
	//let's make it title case.  And lets yell if its not.
	if(name[0] >= 'A' && name[0] <= 'Z')
		return 1;
	else
		return 0;
}

int validatePhone(char *phonenumber)
{
	int i;
	char *digit;

	digit = phonenumber;
	if(*digit++ != '(')
		return 0;
	for(i=0;i<3;i++,digit++)
	{
		if(*digit < '0' || *digit > '9')
			return 0;
	}
	if(*digit++ != ')')
		return 0;
	for(i=0;i<3;i++,digit++)
	{
		if(*digit < '0' || *digit > '9')
			return 0;
	}
	if(*digit++ != '-')
		return 0;
	for(i=0;i<4;i++,digit++)
		if(*digit < '0' || *digit > '9')
			return 0;
	return 1;
}

contact *createContact(char *first, char *last, char *phone, unsigned short officenum, char gender, char hacker)
{
	contact *ret = NULL;
	ret = (contact *)__calloc(1, sizeof(contact));
	if(ret == NULL)
	{
		__puts("Couldn't allocate contact.  Something's wrong.");
		_terminate(-1);
	}
	if(validateName(first)) {
		__memcpy(ret->first, first, __strlen(first));
	} else {
		__puts("Invalid first name.  Must begin with a capital letter.");
		__free(ret);
		ret = NULL;
		return NULL;
	}
	if(validateName(last)) {
		__memcpy(ret->last, last, __strlen(last));
	} else {
		__puts("Invalid last name.  Must begin with a capital letter.");
		__free(ret);
		ret = NULL;
		return NULL;
	}
	if(validatePhone(phone)) {
		__memcpy(ret->phone, phone, __strlen(phone));
	} else {
		__puts("Invalid phone number...must look like (nnn)nnn-nnnn");
		__free(ret);
		ret = NULL;
		return NULL;
	}
	ret->officenum = officenum;
	ret->gender = gender;
	ret->hacker = hacker;
	ret->cookie = cookie;
	ret->pad = 0;
	ret->next = NULL;
	ret->prev = NULL;
	return ret;
}

void insert(contact *toAdd)
{
	contact *tmp;
	if(head == NULL) {
		head = toAdd;
		return;
	}
	tmp = head;
	while(tmp->next != NULL) {
		tmp = tmp->next;
	}
	tmp->next = toAdd;
	toAdd->prev = tmp;
}

contact *remove(contact *toRemove)
{
	if((toRemove->next == NULL) && (toRemove->prev == NULL)) {
		head = NULL;
		return toRemove;
	}

	if(toRemove->prev == NULL) {
		head = toRemove->next;
		toRemove->next->prev = NULL;
	}
	else
		toRemove->prev->next = toRemove->next;

	if(toRemove->next == NULL)
		toRemove->prev->next = NULL;
	else
		toRemove->next->prev = toRemove->prev;

	toRemove->next = NULL;
	toRemove->prev = NULL;
	return toRemove;
}

void printGender(char gender)
{
	switch(gender)
	{
		case 'M':
			__puts("Male");
			break;
		case 'F':
			__puts("Female");
			break;
		case 'T':
			__puts("Trans-Gender");
			break;
		default:
			__puts("Other");
	}
}

void printContact(contact *toPrint)
{
	put("First Name: \t");
	__puts(toPrint->first);
	put("Last Name: \t");
	__puts(toPrint->last);
	put("Phone Number: \t");
	__puts(toPrint->phone);
}

void printList() {
	contact *ptr;
	ptr = head;
	if(head == NULL)
		__puts("Empty yolodex.");
	while(ptr != NULL)
	{
		__puts("========================================");
		printContact(ptr);
		ptr = ptr->next;
	}
	__puts("========================================");
}

void sortYolodex() {
	contact *ptr;
	contact *tmp;
	contact *first;
	contact *other;
	contact *sorted = NULL;
	int i;

	ptr = head;
	first = head;
	tmp = head;

	while(tmp != NULL)
	{
		while(ptr != NULL)
		{
			i = 0;
			while(first->last[i] && ptr->last[i])
			{
				if(first->last[i] > ptr->last[i]) {
					first = ptr;
					break;
				}
				if(first->last[i] == ptr->last[i])
					i++;
				else
					break;
			}
			if(!ptr->last[i] && first->last[i])
				first = ptr;
			ptr = ptr->next;
		}
		tmp = remove(first);
		if(sorted == NULL)
		{
			sorted = first;
		} else {
			other = sorted;
			while(other->next != NULL)
				other = other->next;
			other->next = tmp;
			tmp->prev = other;
		}
	first = head;
	ptr = head;
	tmp = head;
	}
	head = sorted;
}

void initYolodex() {
	head = createContact("Robert", "Morris", "(617)253-5982", 32972, 'M', 'y');
	insert(createContact("Chareth", "Cutestory", "(123)456-7890", 4242, 'M', 'n'));
	insert(createContact("Tim", "Berners-Lee", "(638)844-2074", 404, 'M','y'));
	insert(createContact("Pwnies", "McPwn", "(222)223-1337", 31337, 'F','y'));
	insert(createContact("Jenny", "Fakename", "(555)867-5309", 31337, 'F','n'));
}

int recvUntil(char *dst, int n, char delim)
{
	char *tmp;
	size_t gotBytes;
	tmp = dst;
	while(n--)
	{
		receive(STDIN, tmp, 1, &gotBytes);
		if(gotBytes == 0) {
			_terminate(-1);
		}
		if(*tmp == delim) {
			*tmp = 0;
			break;
		}
		tmp++;
	}
	return tmp-dst;
}

void promptAdd() {
	char first[32];
	char last[32];
	char phone[16];
	char officetmp[8];
	short officenum;
	char gender;
	char hacker;
	char tmp[2];

	contact *ptr;
	__memset(phone, 0, 16);
	__memset(officetmp, 0, 8);
	ptr = NULL;
	while(ptr == NULL) {
		put("First: ");
		recvUntil(first, 32, '\n');
		put("Last: ");
		recvUntil(last, 32, '\n');
		put("Phone Number: ");
		recvUntil(phone, 14, '\n');
		put("Office Number: ");
		__memset(officetmp, 0, 8);
		recvUntil(officetmp, 6, '\n');
		officenum = (unsigned short)__atoi(officetmp);
		put("Gender: ");
		recvUntil(tmp, 2, '\n');
		gender = tmp[0];
		hacker = '?';
		while(hacker == '?')
		{
			put("Hacker: ");
			recvUntil(tmp, 2, '\n');
			if(tmp[0] == 'n' || tmp[0] == 'y')
				hacker = tmp[0];
			else
				__puts("Please enter y or n for hacker.");
		}
		ptr = createContact(first, last, phone, officenum, gender, hacker);
	}
	if(ptr->cookie != cookie)
	{
		__puts("Contact corruption detected.");
		_terminate(-1);
	}
	insert(ptr);
}

contact *findContact() {
	char first[32];
	char last[32];
	contact *ptr;
	contact *ret = NULL;
	put("First: ");
	recvUntil(first, 32, '\n');
	put("Last: ");
	recvUntil(last, 32, '\n');

	ptr = head;
	while(ptr != NULL) {
		if(__strcmp(first, ptr->first) == 0) {
			if(__strcmp(last, ptr->last) == 0) {
				ret = ptr;
				break;
			}
		}
		ptr = ptr->next;
	}
	return ret;
}

void promptDel() {
	contact *ptr;

	ptr = findContact();
	if(ptr != NULL) {
		ptr = remove(ptr);
		__free(ptr);
		__puts("Contact removed.");
	} else {
		__puts("No such contact found.");
	}
}

void editUser(contact *ptr) {
	char data[32];
	char number[8];

	if(ptr != NULL) {
		__puts("Updating fields.  Send just a newline to keep old data.");
		put("New first name: ");
		recvUntil(data, 32, '\n');
		if(data[0] && validateName(data) == 0){
			__puts("Not a legal first name.  Stopping edit.");
			return;
		}
#ifdef PATCHED
		data[31] = 0;
#endif
		if(data[0] != 0) {
			__memset(ptr->first, 0, 32);
			__memcpy(ptr->first, data, __strlen(data));
		}
		put("New last name: ");
		recvUntil(data, 32, '\n');
#ifdef PATCHED
		data[31] = 0;
#endif

		if(data[0] && validateName(data) == 0) {
			__puts("Not a legal last name.  Stopping edit.");
			return;
		}
		if(data[0] != 0){
			__memset(ptr->last, 0, 32);
			__memcpy(ptr->last, data, __strlen(data));
		}
		put("New phone number: ");
		recvUntil(data, 14, '\n');
#ifdef PATCHED
		data[13] = 0;
#endif
		if(data[0] && validatePhone(data) == 0) {
			__puts("Not a legal phone number.  Stopping edit.");
			return;
		}
		if(data[0] != 0) {
			__memset(ptr->phone, 0, 16);
			__memcpy(ptr->phone, data, __strlen(data));
		}
		put("New office number: ");
		recvUntil(data, 6, '\n');
		if(data[0]) {
			__memcpy(number, data, 5);
			ptr->officenum = __atoi(number);
		}
		put("New gender: ");
		recvUntil(data, 2, '\n');
		if(data[0])
			ptr->gender = data[0];
		put("Is the user a hacker? ");
		recvUntil(data, 2, '\n');
		if(data[0])
			ptr->hacker = data[0];
	}
	if(ptr->cookie != cookie)
	{
		__puts("Contact corruption detected.");
		_terminate(-1);
	}

}

void promptEdit() {
	contact *ptr;
	char data[32];
	char number[8];

	__memset(number, 0, 8);
	ptr = findContact();
	if(ptr == NULL) {
		__puts("No such contact.");
		return;
	}
	editUser(ptr);
}

void promptShow() {
	contact *ptr;
	char office[8];
	char useraction[2];

	ptr = findContact();
	if(ptr == NULL) {
		__puts("No such contact.");
		return;
	}
	__memset(useraction, 0, 2);
	useraction[0] = 's';
	while(useraction[0] != 'q') {
		__memset(office, 0, 6);
		__puts("****************************************");
		put("First name:\t");
		__puts(ptr->first);
		put("Last name:\t");
		__puts(ptr->last);
		put("Phone num:\t");
		__puts(ptr->phone);
		__itoa(office, ptr->officenum, 6);
		put("Office Number:\t");
		__puts(office);
		put("Gender:\t\t");
		printGender(ptr->gender);
		if(ptr->hacker == 'y')
		{
			__puts("[\x1B[31m!\x1B[0m]This user known to be a hacker[\x1B[31m!\x1B[0m]");
		}
		__puts("****************************************");
		recvUntil(useraction, 2, '\n');
		switch(useraction[0]) {
			case 'q':
				break;
			case 'd':
				if(ptr->next == NULL && ptr->prev == NULL) {
					remove(ptr);
					__puts("No more contacts.  Leaving detail view.");
					useraction[0]='q';
				} else if (ptr->next == NULL) {
					ptr = ptr->prev;
					remove(ptr->next);
				} else {
					ptr = ptr->next;
					remove(ptr->prev);
				}
				break;
			case 'e':
				editUser(ptr);
				break;
			case 'n':
				if(ptr->next != NULL)
					ptr = ptr->next;
				else
					__puts("No next contact.");
				break;
			case 'p':
				if(ptr->prev != NULL)
					ptr = ptr->prev;
				else
					__puts("No previous contact.");
		}
	}
}

void initCookies() {
	size_t bytes;
	int got = 0;
	int shift = 0;

	contact *ptr;

	random(&cookie, 4, &bytes);
	if(bytes != 4)
	{
		__puts("Couldn't get 4 random bytes.  Panic!");
		_terminate(-1);
	}
	cookie |= 0x10204080;
}

void inputLoop() {
	char action[2];
	while(1)
	{
		__memset(action, 0, 2);
		recvUntil(action, 2, '\n');
		switch(action[0])
		{
			case YOLO_ADD:
				promptAdd();
				break;
			case YOLO_DEL:
				promptDel();
				break;
			case YOLO_EDIT:
				promptEdit();
				break;
			case YOLO_EXIT:
				__puts("Thank you for using the yolodex.");
				_terminate(0);
			case YOLO_LIST:
				printList();
				break;
			case YOLO_SORT:
				sortYolodex();
				break;
			case YOLO_SHOW:
				promptShow();
				break;
			default:
				__puts("Unrecognized Command.");
		}
	}
}

int main(void)
{
	initCookies();
	__puts("Welcome to the yolodex.");
	initYolodex();
	inputLoop();
	_terminate(0);
}
