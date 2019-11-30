/*
 *  https://github.com/jamesbarlow/icmptunnel
 *
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2016 James Barlow-Bignell
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include "resolve.h"




int resolve(const char* hostname, uint32_t* address)
{
	/* try to interpret the hostname as an ip address. */
	*address = ntohl(inet_addr(hostname));
	/* if we don't have an ip address, look up the name in dns. */
	if (*address == INADDR_NONE) {		
		int ret;
		struct addrinfo hints, * result,  * result_pointer;;
		int rv;

		memset(&hints, 0, sizeof(struct addrinfo));//--- 必须先清零再使用
		hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_CANONNAME;
		hints.ai_protocol = 0;
		if ((rv = getaddrinfo(hostname, NULL, &hints, &result)) != 0)
		{
			
			return -1;
		}

		result_pointer = result;
		{
			char hostname[1025] = "";
			ret = getnameinfo(result_pointer->ai_addr, result_pointer->ai_addrlen, hostname, sizeof(hostname), NULL, 0, NI_NUMERICHOST);
			if (ret != 0)
			{
				return 1;
			}
			else
			{
				printf("IP: %s \n", hostname);
			}
		}
		freeaddrinfo(result);
		*address = ntohl(inet_addr(hostname));
	}

	return 0;
}
