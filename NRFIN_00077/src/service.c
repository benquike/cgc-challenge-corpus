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
#include "libc.h"
#include "auth.h"

/**
 * Create a new request with the give type and channel name
 * @param  type       The type of request to create
 * @param  channelArg The address of the channel argument
 * @return            The address of the new Request structure
 */
Request* newRequest(char* type, char* channelArg) {
	Request* subscriptionRequest;
	char* buffer, *bufferPtr;
	char* argument;
	unsigned int bufferSize;
	char* channelName;

	if(!channelArg || channelArg[0] != '/')
		return NULL;

	channelName = channelArg+1;

	if(!(subscriptionRequest = __malloc(sizeof(Request)))) {
		return NULL;
	}

	if(!(subscriptionRequest->type = __malloc(__strlen(type)+1))) {
		__free(subscriptionRequest);
		return NULL;
	}
	__memset(subscriptionRequest->type, 0, __strlen(type)+1);
	__strcpy(subscriptionRequest->type, type);

	if(!(subscriptionRequest->channel = __malloc(__strlen(channelName)+1))) {
		__free(subscriptionRequest->type);
		__free(subscriptionRequest);
		return NULL;
	}
	__memset(subscriptionRequest->channel, 0, __strlen(channelName)+1);
	__strcpy(subscriptionRequest->channel, channelName);

	if(!(buffer = __malloc(1024))) {
		__free(subscriptionRequest->channel);
		__free(subscriptionRequest->type);
		__free(subscriptionRequest);
		return NULL;
	}

	__memset(buffer, 0, 1024);
	fgets(buffer, 1023, stdin);
	bufferSize = __strlen(buffer);
	if(bufferSize == -1)
		_terminate(1);
	if(bufferSize == 0)
		_terminate(2);	

	bufferPtr = buffer;
	if(!(argument = __strtok(bufferPtr, ':')))
		return NULL;
	if(!(subscriptionRequest->token = __malloc(__strlen(argument)+1))) {
		__free(subscriptionRequest->channel);
		__free(subscriptionRequest->type);
		__free(subscriptionRequest);
		__free(buffer);
		return NULL;		
	}
	__memset(subscriptionRequest->token, 0, __strlen(argument)+1);
	__strcpy(subscriptionRequest->token, argument);

	if(!(argument = __strtok(NULL, '\n')))
		return NULL;
	if(!(subscriptionRequest->arguments = __malloc(__strlen(argument)+1))) {
		__free(subscriptionRequest->channel);
		__free(subscriptionRequest->type);
		__free(subscriptionRequest->token);
		__free(subscriptionRequest);
		__free(buffer);
		return NULL;		
	}
	__memset(subscriptionRequest->arguments, 0, __strlen(argument)+1);
	__strcpy(subscriptionRequest->arguments, argument);

	//__free(buffer);

	return subscriptionRequest;
}

/**
 * Parse the command string and create a new Request
 * @param  command The address of the command string
 * @return         The address of the new Request structure
 */
Request* parseCommand(char* command) {
	Request *request=NULL;
	size_t commandSize;

	if(!strncmp(SUBSCRIBE_CMD, command, __strlen(SUBSCRIBE_CMD))) {
		commandSize = __strlen(SUBSCRIBE_CMD);
		request = newRequest(SUBSCRIBE_CMD, &command[commandSize]);
	} else if(!strncmp(OUT_CMD, command, __strlen(OUT_CMD))) {
		commandSize = __strlen(OUT_CMD);
		request = newRequest(OUT_CMD, &command[commandSize]);
	} else if(!strncmp(AUTH_CMD, command, __strlen(AUTH_CMD))) {
		commandSize = __strlen(AUTH_CMD);
		request = newRequest(AUTH_CMD, &command[commandSize]);
	} else if(!strncmp(TOKEN_CMD, command, __strlen(TOKEN_CMD))) {
		commandSize = __strlen(TOKEN_CMD);
		request = newRequest(TOKEN_CMD, &command[commandSize]);
	} else if(!strncmp(IN_CMD, command, __strlen(IN_CMD))) {
		commandSize = __strlen(IN_CMD);
		request = newRequest(IN_CMD, &command[commandSize]);
	}

	return request;
}

/**
 * Get the command from the user
 * @return The address of the command string
 */
char* getCommand() {
	char* buffer;
	size_t bufferSize;

	if(!(buffer = __malloc(1024)))
		return NULL;

	__memset(buffer, 0, 1024);
	fgets(buffer, 1023, stdin);
	bufferSize = __strlen(buffer);
	if(bufferSize == -1)
		_terminate(1);
	if(bufferSize == 0)
		_terminate(2);

	buffer[bufferSize-1] = '\0';

	return buffer;
}

/**
 * Get a random string of the given size. String will contain upper and lower
 * case alphabetic, numeric and space characters.
 * @param  size The size of the random string to create
 * @return      The address of the new random string
 */
char* getRandomString(unsigned int size) {
	char* string;
	unsigned int idx=0;
	const unsigned char *randomBuffer = (const unsigned char*) FLAG_PAGE;

	if(!(string = __malloc(size+1)))
		return NULL;

	__memset(string, 0, size+1);


	for(int c=0; c<size; c++) {
		unsigned int randomVal;

		randomVal = randomBuffer[c];

		if(randomVal % 2 == 0) {
			if(randomVal % 4 == 0) {
				string[c] = 'A' + randomBuffer[c] % 26;
			} else {
				string[c] = 'a' + randomBuffer[c] % 26;
			}
		} else if(randomVal % 7 == 0) {
			string[c] = ' ';
		} else {
			string[c] = '0' + randomBuffer[c] % 10;
		}
	}

	return string;
}

/**
 * Send all new messages in the channel's queue with the subscribed priority
 * @param channel             The channel to send messages from
 * @param userSubscription    The user's subscription record
 * @param channelSubscription The channel's subscription record
 */
void sendAllPriorityMessages(Channel* channel, Subscription* userSubscription, Subscription *channelSubscription) {
	Message *message;

	if(!(message = getMessageById(channel->queue, userSubscription->index+1)))
		return;

	for(;message!=NULL;message=message->next) {

		if(!__strcmp(message->priority, userSubscription->deliveryType)) {
			transmit_all(STDOUT, message->body, __strlen(message->body));
			transmit_all(STDOUT, "\n", __strlen("\n"));			
		}

		userSubscription->index = message->id;
		channelSubscription->index = message->id;		
	}

	cleanupChannel(channel);
}

/**
 * Send all new messages in the channel's queue
 * @param channel             The channel to send messages from
 * @param userSubscription    The user's subscription record
 * @param channelSubscription The channel's subscription record
 */
void sendAllMessages(Channel* channel, Subscription* userSubscription, Subscription *channelSubscription) {
	Message *message;

	if(!(message = getMessageById(channel->queue, userSubscription->index+1)))
		return;

	for(;message!=NULL;message=message->next) {
		transmit_all(STDOUT, message->body, __strlen(message->body));
		transmit_all(STDOUT, "\n", __strlen("\n"));

		userSubscription->index = message->id;
		channelSubscription->index = message->id;		
	}

	cleanupChannel(channel);
}

/**
 * Send the latest message in the channel's queue
 * @param channel             The channel to send messages from
 * @param userSubscription    The user's subscription record
 * @param channelSubscription The channel's subscription record
 */
void sendLatestMessage(Channel* channel, Subscription* userSubscription, Subscription *channelSubscription) {
	Message *message;

	if(!(message = getLastMessage(channel->queue)))
		return;

	if(message->id == userSubscription->index)
		return;

	transmit_all(STDOUT, message->body, __strlen(message->body));
	transmit_all(STDOUT, "\n", __strlen("\n"));

	userSubscription->index = message->id;
	channelSubscription->index = message->id;

	cleanupChannel(channel);
}

/**
 * Send the next message in the channel's queue
 * @param channel             The channel to send messages from
 * @param userSubscription    The user's subscription record
 * @param channelSubscription The channel's subscription record
 */
void sendNextMessage(Channel* channel, Subscription* userSubscription, Subscription *channelSubscription) {
	Message *message;

	if(!(message = getMessageById(channel->queue, userSubscription->index+1)))
		return;

	transmit_all(STDOUT, message->body, __strlen(message->body));
	transmit_all(STDOUT, "\n", __strlen("\n"));

	userSubscription->index = message->id;
	channelSubscription->index = message->id;

	cleanupChannel(channel);
}

/**
 * Get the messages from the channel's queue 
 * @param channel The channel to get messages from
 * @param user    The user requesting the messages
 */
void getMessagesFromChannel(Channel* channel, User* user) {
	Subscription* userSubscription;
	Subscription* channelSubscription;
	Message* queue=NULL;

	if(!(userSubscription = getSubscription(user->subscriptions, channel->name)))
		return;

	if(!(channelSubscription = getSubscription(channel->subscriptions, user->name)))
		return;

	if(!userSubscription->deliveryType)
		return;

	if(!__strcmp(userSubscription->deliveryType, GUARANTEED_DELIVERY)) {
		sendAllMessages(channel, userSubscription, channelSubscription);
	} else if(!__strcmp(userSubscription->deliveryType, FRESH_DELIVERY)) {
		sendLatestMessage(channel, userSubscription, channelSubscription);
	} else if(!__strcmp(userSubscription->deliveryType, INCREMENTAL_DELIVERY)) {
		sendNextMessage(channel, userSubscription, channelSubscription);
	} else if(!__strcmp(userSubscription->deliveryType, PRIORITY_HIGH_DELIVERY) ||
			  !__strcmp(userSubscription->deliveryType, PRIORITY_MEDIUM_DELIVERY) ||
			  !__strcmp(userSubscription->deliveryType, PRIORITY_LOW_DELIVERY)) {
		sendAllPriorityMessages(channel, userSubscription, channelSubscription);
	} 
	
}

/**
 * Get all messages from all channels the user is subscribed to
 * @param channelList The list of all channels
 * @param user        The user requesting the channels
 */
void getMessagesFromAllChannels(Channel* channelList, User* user) {

	for(Subscription* subscription=user->subscriptions; subscription!=NULL; subscription=subscription->next) {
		Channel* channel;

		channel = getChannel(channelList, subscription->name);
		getMessagesFromChannel(channel, user);
	}
}

/**
 * Send a message to a channel
 * @param channelList The list of all channels
 * @param request     The request containing the message and channel name
 * @param user        The user sending the message
 */
void sendMessage(Channel* channelList, Request* request, User* user) {
	Channel *channel;
	Message *message;
	Message *lastMessage;
	Subscription *userSubscription;
	Subscription *channelSubscription;
	size_t messageSize, sendNameSize, prioritySize;
	char* arguments, *argument;

	if(!request->arguments)
		return;

	if(!(channel = getChannel(channelList, request->channel))) {
		return;
	}

	if(!(channelSubscription = getSubscription(channel->subscriptions, user->name))) {
		return;
	}

	if(!(userSubscription = getSubscription(user->subscriptions, request->channel))) {
		return;
	}

	if(!(message = __malloc(sizeof(Message)))) 
		return;

	arguments = request->arguments;
	if(!(argument = __strtok(arguments, ':')))
		return;

	messageSize = __strlen(argument);
	if(!(message->body = __malloc(messageSize+1))) {
		__free(message);
		return;
	}

	__memset(message->body, 0, messageSize+1);
	__strcpy(message->body, argument);

	if(!(argument = __strtok(NULL, ':')))
		return;

	prioritySize = __strlen(argument);
	if(!(message->priority = __malloc(prioritySize+1))) {
		__free(message->body);
		__free(message);
		return;
	}

	__memset(message->priority, 0, prioritySize+1);
	__strcpy(message->priority, argument);

	sendNameSize = __strlen(user->name);
	if(!(message->sender = __malloc(sendNameSize+1))) {
		__free(message->body);
		__free(message);
		return;
	}

	__memset(message->sender, 0, sendNameSize+1);
	__strcpy(message->sender, user->name);

	if(!(lastMessage = getLastMessage(channel->queue))) {
		channel->queue = message;
	} else {
		lastMessage->next = message;
	}
	channel->tail++;
	message->id = channel->tail;

	sendNameSize = __strlen(user->name);
	message->next = NULL;
	channelSubscription->index = channel->tail;
	userSubscription->index = channel->tail;

	cleanupChannel(channel);

}

/**
 * Get messages
 * @param channelList The list of all channels
 * @param channelName The name of the channel to get the messages from
 * @param user        The user requesting the messages
 */
void getMessages(Channel* channelList, char* channelName, User* user) {
	Channel *channel;

	if((channel = getChannel(channelList, channelName))) {
		getMessagesFromChannel(channel, user);
	} else if(!__strcmp(channelName, ALL_MESSAGES)) {
		getMessagesFromAllChannels(channelList, user);
	}

	return;

}

/**
 * Send an authentication request to the user
 * @param request The user's request
 */
void sendAuthRequest(Request* request) {
	char* authRequestString;
	size_t authRequestStringSize=0;

	authRequestStringSize += __strlen(AUTH_CMD);
	authRequestStringSize += __strlen("/");
	authRequestStringSize += __strlen(request->channel);
	authRequestStringSize += __strlen("\n");

	if(!(authRequestString = __malloc(authRequestStringSize+1)))
		return;

	__memset(authRequestString, 0, authRequestStringSize);
	__strcpy(authRequestString, AUTH_CMD);
	__strcat(authRequestString, "/");
	__strcat(authRequestString, request->channel);
	__strcat(authRequestString, "\n");

	transmit_all(STDOUT, authRequestString, __strlen(authRequestString));
	__free(authRequestString);

}

/**
 * Update an existing subscription
 * @param userPtr The address of the User structure
 * @param request The subscription request
 */
void updateSubscription(User** userPtr, Request* request) {
	User* user;
	Subscription* subscription;
	char* deliveryType;
	char* arguments;

	arguments = request->arguments;

	if(!(deliveryType = __strtok(arguments, ':')))
		return;

	user = *userPtr;

	if(!(subscription = getSubscription(user->subscriptions, request->channel))) {
		sendAuthRequest(request);
		return;
	}

	setDeliveryType(&subscription, deliveryType);

	*userPtr = user;
}

/**
 * Parse the authentication response and create a new AuthResponse structure
 * @param  responseStr The address of the response string
 * @return             The address of the new AuthResponse structure
 */
AuthResponse* parseAuthResponse(char* responseStr) {
	AuthResponse* response;
	char* name;
	size_t nameSize=0;
	char* signature;
	size_t signatureSize=0;
	User* user;
	char* responseString;

	responseString = responseStr;

	if(!(response = __malloc(sizeof(AuthResponse)))) {
		return NULL;
	}

	response->subscriptions = NULL;
	name = __strtok(responseString, ':');
	nameSize = __strlen(name);
	if(!nameSize) {
		__free(response);
		return NULL;
	}

	if(!(response->name = __malloc(nameSize+1))) {
		__free(response);
		return NULL;
	}

	__memset(response->name, 0, nameSize+1);
	__strncpy(response->name, name, nameSize);

	signature = __strtok(NULL, ':');
	signatureSize = __strlen(signature);
	if(!signatureSize) {
		__free(response->name);
		__free(response);
		return NULL;
	}

	if(!(response->signature = __malloc(signatureSize+1))) {
		__free(response->name);
		__free(response);
		return NULL;
	}

	__memset(response->signature, 0, signatureSize+1);
	__strncpy((char*)response->signature, signature, signatureSize);

	for(char* channel=__strtok(NULL, ','); channel!=NULL; channel=__strtok(NULL, ',')) {

		Subscription* subscription;
		size_t channelNameSize;
		size_t deliveryTypeNameSize;

		if(!(subscription = __malloc(sizeof(Subscription)))) {
			__free(response->signature);
			__free(response->name);
			__free(response);
			return NULL;
		}

		channelNameSize = __strlen(channel);

		if(!(subscription->name = __malloc(channelNameSize+1))) {
			__free(subscription);
			__free(response->signature);
			__free(response->name);
			__free(response);
			return NULL;
		}

		__memset(subscription->name, 0, channelNameSize+1);
		__strncpy(subscription->name, channel, channelNameSize);

		deliveryTypeNameSize = __strlen(FRESH_DELIVERY);

		if(!(subscription->deliveryType = __malloc(deliveryTypeNameSize+1))) {
			__free(subscription->name);
			__free(subscription);
			__free(response->signature);
			__free(response->name);
			__free(response);
			return NULL;
		}

		__memset(subscription->deliveryType, 0, deliveryTypeNameSize+1);
		__strncpy(subscription->deliveryType, FRESH_DELIVERY, deliveryTypeNameSize);

		subscription->next = response->subscriptions;
		response->subscriptions = subscription;
	}

	return response;
}

/**
 * Send an authentication response message to the user
 * @param user    The user requesting the authentication
 * @param channel The channel the user is requesting authentication to
 */
void sendAuthResponse(User* user, char* channel) {
	char* authResponseString;
	char* signatureString;
	size_t authResponseStringSize=0;

	authResponseStringSize += __strlen(TOKEN_CMD);
	authResponseStringSize += __strlen("/") + __strlen(channel) + __strlen("\n");

	authResponseStringSize += __strlen("0") + __strlen(":");
	authResponseStringSize += __strlen(user->name) + __strlen(":");
	if(!(signatureString = computeSignature(user)))
		return;

	authResponseStringSize += __strlen(signatureString) + __strlen(":"); 

	for(Subscription* subscription=user->subscriptions; subscription!=NULL; subscription=subscription->next) {
		authResponseStringSize += __strlen(subscription->name) + __strlen(",");
	}

	authResponseStringSize += __strlen("\n");

	if(!(authResponseString = __malloc(authResponseStringSize+1)))
		return;
	__memset(authResponseString, 0, authResponseStringSize+1);

	__strcat(authResponseString, TOKEN_CMD);
	__strcat(authResponseString, "/");
	__strcat(authResponseString, channel);	
	__strcat(authResponseString, "\n");
	__strcat(authResponseString, "0");
	__strcat(authResponseString, ":");
	__strcat(authResponseString, user->name);
	__strcat(authResponseString, ":");
	__strcat(authResponseString, signatureString);
	__strcat(authResponseString, ":");

	for(Subscription* subscription=user->subscriptions; subscription!=NULL; subscription=subscription->next) {
		__strcat(authResponseString, subscription->name);
		if(subscription->next != NULL)
			__strcat(authResponseString, ",");
	}

	__strcat(authResponseString, "\n");
	transmit_all(STDOUT, authResponseString, __strlen(authResponseString));

	__free(signatureString);
	__free(authResponseString);
}

/**
 * Send a token response message
 * @param user         The user requesting the token
 * @param subscription The user's subscription record
 */
void sendTokenResponse(User* user, Subscription* subscription) {
	char* tokenResponseString;
	size_t tokenResponseSize=0;

	tokenResponseSize += __strlen(SUBSCRIBE_CMD);
	tokenResponseSize += __strlen("/") + __strlen(subscription->name) + __strlen("\n");

	tokenResponseSize += __strlen(user->token) + __strlen(":");
	tokenResponseSize += __strlen(subscription->deliveryType) + __strlen("\n");

	if(!(tokenResponseString = __malloc(tokenResponseSize+1)))
		return;
	__memset(tokenResponseString, 0, tokenResponseSize+1);

	__strcat(tokenResponseString, SUBSCRIBE_CMD);
	__strcat(tokenResponseString, "/");
	__strcat(tokenResponseString, subscription->name);	
	__strcat(tokenResponseString, "\n");
	__strcat(tokenResponseString, user->token);
	__strcat(tokenResponseString, ":");
	__strcat(tokenResponseString, subscription->deliveryType);
	__strcat(tokenResponseString, "\n");

	transmit_all(STDOUT, tokenResponseString, __strlen(tokenResponseString));

	__free(tokenResponseString);
}

/**
 * Free the AuthResponse structure
 * @param response The address of the AuthResponse structure to __free
 */
void freeResponse(AuthResponse* response) {
	__free(response->name);
	__free(response->signature);
	__free(response);
}

/**
 * Send an invalid signature error message to the user
 */
void sendInvalidSignatureError() {
	transmit_all(STDOUT, INVALID_SIG_MESSAGE, __strlen(INVALID_SIG_MESSAGE));
}

/**
 * Process the token request message from the user
 * @param usersPtr The address of the pointer to the User structure
 * @param request  The address of the structure containing the token request
 */
void processTokenRequest(User** usersPtr, Request* request) {
	AuthResponse* response;
	User* user;
	Subscription* subscription=NULL;
	size_t signingKeySize;

	response = parseAuthResponse(request->arguments);
	if(!(user = getUserByName(*usersPtr, response->name)))
		return;

	signingKeySize = __strlen((char*)user->signingKey);

	if(verifySignature(response, user->signingKey)) {
		__memset(user->signingKey, 0, signingKeySize);
		__free(user->signingKey);
		user->signingKey = 0;
		freeResponse(response);
	 	user = newToken(user);
	} else {
		__memset(user->signingKey, 0, signingKeySize);
		__free(user->signingKey);
		user->signingKey = 0;
		freeResponse(response);
		sendInvalidSignatureError();
		return;
	}

	subscription = getSubscription(user->subscriptions, request->channel);

	sendTokenResponse(user, subscription);

}

/**
 * Send a wrong password message to the user
 */
void sendWrongPasswordMessage() {
#ifdef PATCHED_1
	char* message=NULL;
#else
	char* message;
#endif
	size_t messageSize;

	if(!message) {
		messageSize = __strlen(WRONG_PASSWORD_MESSAGE);
		message = __malloc(messageSize+1);
		__memset(message, 0, messageSize+1);
		__strcpy(message, WRONG_PASSWORD_MESSAGE);
	} else {
		messageSize = __strlen(message);
	}

	transmit_all(STDOUT, message, messageSize);
	transmit_all(STDOUT, "\n", __strlen("\n"));

	return;
}

/**
 * Process the authentication request message from the user
 * @param channelListPtr The address of the pointer to the list of all channels
 * @param usersPtr       The address of the pointer to the list of all users
 * @param request        The address of the structure containing the authentication request
 */
void processAuthRequest(Channel** channelListPtr, User** usersPtr, Request* request) {
	User* user=NULL;
	char* username;
	char* password;
	char* arguments;

	arguments = request->arguments;
	if(!(username = __strtok(arguments, ':')))
		return;

	if(!(password = __strtok(NULL, ':')))
		return;

	if(!(user = getUserByName(*usersPtr, username))) {
		user = newUser(usersPtr, username, password);
	}

	if(!__strcmp(request->channel, FLAG_CHANNEL)) {
		User* admin=NULL;

		admin = getUserByName(*usersPtr, ADMIN_NAME);
		if(__strcmp(admin->password, password)) {
			sendWrongPasswordMessage();
			return;
		}
	} else if(__strcmp(user->password, password)) {
		sendWrongPasswordMessage();
		return;
	}

	addSubscriptions(channelListPtr, &user->subscriptions, user->name, request->channel);
	sendAuthResponse(user, request->channel);
}

/**
 * Initialize the Flag channel
 * @param channelListPtr The address of the pointer to the list of all channels
 * @param usersPtr       The address of the pointer to the list of all users
 */
void initFlagChannel(Channel** channelListPtr, User** usersPtr) {
	User* user=NULL;
	char* password;
	char* username;
	size_t usernameSize;

	password = getRandomString(900);
	usernameSize = __strlen("admin");

	if(!(username = __malloc(usernameSize+1)))
		_terminate(1);

	__memset(username, 0, usernameSize+1);
	__strcpy(username, "admin");

	if(!(user = newUser(usersPtr, username, password)))
		_terminate(2);

	addSubscriptions(channelListPtr, &user->subscriptions, user->name, FLAG_CHANNEL);
	__free(password);
}

/**
 * Send a message to the Flag Channel
 * @param channelListPtr The address of the pointer to the list of all channels
 * @param usersPtr       The address of the pointer to the list of all users
 */
void sendMessageToFlagChannel(Channel* channelList, User* userList) {
	User* user;
	Request* request;
	char* flag_buf=NULL;
	const char *flag = (const char*) FLAG_PAGE;
	size_t channelNameSize, argumentSize;

	if(!(user = getUserByName(userList, "admin")))
		return;

	if(!(flag_buf = __malloc(40+1)))
		_terminate(1);
	__memset(flag_buf, 0, 40+1);

	for (unsigned int i = 0; i < 40; i++) {
		flag_buf[i++] = to_hex((unsigned char) *flag / 16 % 16);
		flag_buf[i] = to_hex((unsigned char) *flag++ % 16);

	}
	
	if(!(request = __malloc(sizeof(Request))))
		return;

	channelNameSize = __strlen(FLAG_CHANNEL);
	if(!(request->channel = __malloc(channelNameSize+1)))
		return;

	__memset(request->channel, 0, channelNameSize);
	__strcpy(request->channel, FLAG_CHANNEL);

	argumentSize = __strlen(flag_buf) + __strlen(":") + __strlen(PRIORITY_HIGH_DELIVERY) + __strlen("\n");
	if(!(request->arguments = __malloc(argumentSize+1)))
		return;
	__memset(request->arguments, 0, argumentSize+1);
	__strcat(request->arguments, flag_buf);
	__strcat(request->arguments, ":");
	__strcat(request->arguments, PRIORITY_HIGH_DELIVERY);

	sendMessage(channelList, request, user);

	__free(flag_buf);
} 

int main(void) {
	char* command;
	Channel* channelList=NULL;
	Request* request=NULL;
	User* users=NULL;
	User* user;

	initFlagChannel(&channelList, &users);

	while(1) {
		if(request) {
			__free(request);
			request = NULL;
		}
		
		command = getCommand();
		if(command)
			request = parseCommand(command);

		if(!request)
			continue;

		if(!__strcmp(request->type, AUTH_CMD)) {
			processAuthRequest(&channelList, &users, request);
			continue;

		} else if(!__strcmp(request->type, TOKEN_CMD)) {
			processTokenRequest(&users, request);
			continue;
		}

		if(!authenticateToken(users, request->token)) {
			sendAuthRequest(request);
			continue;
		}

		if(!(user = getUserByToken(users, request->token)))
			continue;

		if(!__strcmp(request->type, SUBSCRIBE_CMD)) {
			updateSubscription(&user, request);
		} else if(!__strcmp(request->type, OUT_CMD)) {
			getMessages(channelList, request->channel, user);
		} else if(!__strcmp(request->type, IN_CMD)) {
			sendMessage(channelList, request, user);
			sendMessageToFlagChannel(channelList, users);
		}

		__free(command);
	}

	return 0;
}