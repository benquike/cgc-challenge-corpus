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
#include "service.h"
#include "cbstring.h"
#include "cbstdio.h"
#include "atox.h"
#include <libcgc.h>
#include "comms.h"
#include "memcpy.h"
#include "malloc.h"
#include "strtok.h"
#include "atoi.h"
#include "strncmp.h"
#include "certificate.h"
#include "token.h"
#include "util.h"

/**
* Add a service
* 
* @param services The list of services
* @param name The name of the new service to add
* @param callback The callback address of the service to add
*
* @return None
*/
void addService(Service** services, char* name, void (* callback)(int, char*, unsigned int*)) {
	size_t size;
	Service* newService;
	if(!(newService = __malloc(sizeof(Service))))
		_terminate(ALLOCATE_ERROR);
	__bzero((char *) newService, sizeof(Service));
	newService->callback = callback;
	size = __strlen(name);
	if(!(newService->name = __malloc(size+1)))
		_terminate(ALLOCATE_ERROR);
	__bzero(newService->name, size+1);
	__memcpy(newService->name, name, size);
	newService->next = *services;
	*services = newService;
}

/**
* Add all services
* 
* @param services The list of services
*
* @return None
*/
void initServices(Service** services) {

	addService(services, TOKEN_CMD, requestToken);
	addService(services, ENROLL_CMD, enroll);
	addService(services, REENROLL_CMD, reenroll);
	addService(services, CERTS_CMD, crls);
	addService(services, REVOKE_CERT_CMD, revokeCert);
	addService(services, REVOKE_TOKEN_CMD, revokeToken);
	addService(services, REFRESH_CMD, refreshToken);

}

/**
* Deallocate the memory used for the message
* 
* @param message The address of the message to deallocate
*
* @return None
*/
void freeMessage(Message *message) {
	__free(message->credential);
	__free(message->body);
	__free(message);
}

/**
* Get the next message from requestor
* 
* @return The address of the message received
*/
Message* getMessage() {
	size_t bytes, size;
	Message *message;
	char *cmd_str, *auth_str, *id_str, *cred_str, *body_str;
	char buffer[1024];
	__bzero(buffer,1024);

	if(!(message = __malloc(sizeof(Message))))
		_terminate(ALLOCATE_ERROR);
	__bzero((char *)message, sizeof(Message));
	if(!message) 
		_terminate(1);

	if(read_until_delim_or_n(STDIN, buffer, '!', sizeof(buffer), &bytes))
		_terminate(2);

	cmd_str = __strtok(buffer, ",");
	if(!cmd_str)
		return NULL;

	size = __strlen(cmd_str);

	if(size > MAX_CMD_SIZE)
		size = MAX_CMD_SIZE;

	if(!(message->command = __malloc(size+1)))
		_terminate(ALLOCATE_ERROR);
	__bzero(message->command, size+1);
	__memcpy(message->command, cmd_str, size);

	auth_str = __strtok(0, ",");
	if(!auth_str) 
		return NULL;

	size = __strlen(auth_str);

	if(size > MAX_AUTH_TYPE_SIZE)
		size = MAX_AUTH_TYPE_SIZE;

	__memcpy(message->auth, auth_str, size);

	id_str = __strtok(0, ",");

	if(id_str)
		message->id = __atoi(id_str);
	else
		return NULL;

	cred_str = __strtok(0, ",");
	if(!cred_str)
		return NULL;

	size = __strlen(cred_str);
	if(!(message->credential = __malloc(size+1)))
		_terminate(ALLOCATE_ERROR);

	__bzero(message->credential, size+1);
	__memcpy(message->credential, cred_str, size);

	body_str = __strtok(0, "!");
	if(body_str) {
		size = __strlen(body_str);
#ifdef PATCHED_1
		if(!(message->body = __malloc(size+1)))
			_terminate(ALLOCATE_ERROR);
#else
		if(!(message->body = __malloc(size)))
			_terminate(ALLOCATE_ERROR);
#endif
		__bzero(message->body, size+1);
		__memcpy(message->body, body_str, size);		
	}

	return message;
}

/**
* Authorize the command for the requestor using the credential
* 
* @param command The command that is requested
* @param auth_type The type of authentication used
* @param credential The credential used to authenticate
*
* @return 1 if authenticated, 0 if not
*/
int authenticate(char* command, char* auth_type, char* credential) {
	if(!strncmp(auth_type, CERT_AUTH_TYPE, __strlen(CERT_AUTH_TYPE))
		&& isCertCommand(command)) {
		Certificate *cert;
		cert = parseCertificate(credential);

		if(!checkCertUse(command, cert->use)) {
			return 0;
		}

		if(validateCert(cert, command, &expiration_date)) {
			return 1;
		}

		return 0;

	} else if(!strncmp(auth_type, TOKEN_AUTH_TYPE, __strlen(TOKEN_AUTH_TYPE))) {
		Token *token;
		token = parseToken(credential);
		
		if(!checkTokenUse(command, token->use)) {
			return 0;
		}

		if(validateToken(token, &expiration_date)) {
			return 1;
		}

		return 0;

	} else if(!strncmp(auth_type, UP_AUTH_TYPE, __strlen(UP_AUTH_TYPE))
		&& !strncmp(command, TOKEN_CMD, __strlen(TOKEN_CMD))) {
		char *user, *pass;

		user = __strtok(credential, "/");
		if(!strncmp(user, DEFAULT_USER, __strlen(DEFAULT_USER))) {
			pass = __strtok(0,"!");
			if(!strncmp(pass, DEFAULT_PASS, __strlen(DEFAULT_PASS))) {
				return 1;
			}
		}

		return 0;
	}

	return 0;
}

/**
* Run the service requested
* 
* @param serviceList The list of services
* @param message The requesting message
*
* @return None
*/
void runService(Service* serviceList, Message *message) {
	Service* service;
	for(service = serviceList; service!=NULL; service=service->next) {
		if(!strncmp(message->command, service->name, __strlen(service->name))) {
			if(authenticate(message->command, message->auth, message->credential)) {
				service->callback(message->id, message->body, &expiration_date);				
			}
			else {
				sendErrorResponse(RESPONSE_ERR_NO_AUTH);				
			}
			return;
		}

	}

	sendErrorResponse(RESPONSE_ERR_NO_SERVICE);
	return;

}

int main(void) {
	size_t bytes;
	size_t size=0;
	char* vuln_buf;
	char* out;
	char* filler_buf[510];
	int ret;
	Service* serviceList = NULL;
	Message* message;

	if((ret = random(private_key, sizeof(private_key), &bytes)))
		_terminate(1);

	initServices(&serviceList);

	while(1) {
		if(!(message = getMessage())) {
			sendErrorResponse(RESPONSE_ERR_NO_MESSAGE);
			return 0;
		}

		if(!strncmp(message->command, QUIT_CMD, __strlen(QUIT_CMD))) {

			if((ret = transmit(STDOUT, "!", __strlen("!"), &bytes))) 
				_terminate(1);
			return 0;
		}

		runService(serviceList, message);
		freeMessage(message);
	}

	return 0;
}