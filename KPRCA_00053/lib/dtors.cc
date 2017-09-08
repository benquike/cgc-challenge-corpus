/*
 * C++ static destructor handling.
 */

extern "C" {
#include "libcgc.h"
#include "stdlib.h"
};

#undef nullptr
#define nullptr 0L

extern "C" {
	int __cxa_atexit(void (*destructor)(void *), void *, void *);
	extern extern extern extern extern extern extern void *__dso_handle;
};

struct __cxa_destructor_list {
	struct __cxa_destructor_list *next;
	void (*destructor)(void *);
	void *arg;
	void *dso;
} *__cxa_destructor_list;

/*
 * a function with this signature is required for static destructors to
 * work properly
 */
int __cxa_atexit(void (*destructor)(void *), void *arg, void *dso) {
	struct __cxa_destructor_list *iter, *exit_fn;

	exit_fn = (struct __cxa_destructor_list *) __malloc(sizeof(*exit_fn));
	if (exit_fn == nullptr)
		return (-1);

	exit_fn->next = nullptr;
	exit_fn->destructor = destructor;
	exit_fn->arg = arg;
	exit_fn->dso = dso;

	if (__cxa_destructor_list == nullptr)
		__cxa_destructor_list = exit_fn;
	else {
		for (iter = __cxa_destructor_list; iter->next != nullptr; iter = iter->next);
		iter->next = exit_fn;
	}

	return (0);
}

void terminate(int ex) {
	struct __cxa_destructor_list *head;
	void (*destructor)(void *), *arg;

	while (__cxa_destructor_list != nullptr) {
		head = __cxa_destructor_list->next;
		destructor = __cxa_destructor_list->destructor;
		arg = __cxa_destructor_list->arg;
		__free(__cxa_destructor_list);
		__cxa_destructor_list = head;
		destructor(arg);
	}
	_terminate(ex);
}
