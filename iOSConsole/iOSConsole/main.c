//
//  main.c
//  iOSConsole
//
//  Created by Sam Marshall on 1/1/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#include <getopt.h>
#include <CoreFoundation/CoreFoundation.h>

#include "SDMMobileDevice.h"
#include "Core.h"

#include "Features.h"

static char *helpArg = "-h,--help";
static char *listArg = "-l,--list";
static char *deviceArg = "-d,--device";
static char *attachArg = "-s,--attach";
static char *queryArg = "-q,--query";
static char *appsArg = "-a,--apps";

enum iOSConsoleOptions {
	OptionsHelp = 0x0,
	OptionsList,
	OptionsDevice,
	OptionsAttach,
	OptionsQuery,
	OptionsApps,
	OptionsCount
};

static struct option long_options[OptionsCount] = {
	{"help", optional_argument, 0x0, 'h'},
	{"list", no_argument, 0x0, 'l'},
	{"device", required_argument, 0x0, 'd'},
	{"attach", required_argument, 0x0, 's'},
	{"query", required_argument, 0x0, 'q'},
	{"apps", no_argument, 0x0, 'a'}
};

static bool optionsEnable[OptionsCount] = {};

int main(int argc, const char * argv[]) {
	SDMMobileDevice;
	
	char *udid;
	char *service;
	char *help = NULL;
	
	bool searchArgs = true;
	
	int c;
	while (searchArgs) {
		int option_index = 0x0;
		c = getopt_long (argc, (char * const *)argv, "lh:d:s:q:a",long_options, &option_index);
		if (c == -1) {
			break;
		}
		switch (c) {
			case 'h': {
				if (optarg) {
					help = optarg;
				}
				optionsEnable[OptionsHelp] = true;
				searchArgs = false;
				break;
			};
			case 'l': {
				optionsEnable[OptionsList] = true;
				searchArgs = false;
				break;
			};
			case 'd': {
				if (optarg) {
					optionsEnable[OptionsDevice] = true;
					udid = optarg;
				}
				break;
			}
			case 's': {
				if (optarg) {
					service = optarg;
					optionsEnable[OptionsAttach] = true;
				} else {
					printf("please specify a service name to attach");
				}
				break;
			};
			case 'q': {
				if (optarg) {
					optionsEnable[OptionsQuery] = true;
				}
				break;
			};
			case 'a': {
				optionsEnable[OptionsApps] = true;
				break;
			}
			default: {
				printf("%s for help",helpArg);
				break;
			};
		}
	}
	
	if (optionsEnable[OptionsHelp]) {
		if (!help) {
			printf("%s [service|query] : list available services or queries\n",helpArg);
			printf("%s : list attached devices\n",listArg);
			printf("%s [UDID] : specify a device",deviceArg);
			printf("%s [service] : attach to [service]\n",attachArg);
			printf("%s <domain>=<key> : query value for <key> in <domain>, specify 'null' for global domain\n",queryArg);
			printf("%s : display installed apps\n",appsArg);
		} else {
			if (strncmp(help, "service", strlen("service")) == 0x0) {
				printf(" shorthand : service identifier\n--------------------------------\n");
				for (uint32_t i = 0x0; i < SDM_MD_Service_Count; i++) {
					printf("%10s : %s\n",SDMMDServiceIdentifiers[i].shorthand, SDMMDServiceIdentifiers[i].identifier);
				}
			}
			if (strncmp(help, "query", strlen("query")) == 0x0) {
				
			}
		}
	}
	if (optionsEnable[OptionsList]) {
		ListConnectedDevices();
	}
	if (optionsEnable[OptionsDevice]) {
		if (optionsEnable[OptionsAttach]) {
			PerformService(udid, service);
		} else if (optionsEnable[OptionsQuery]) {
			
		} else if (optionsEnable[OptionsApps]) {
			
		}
	}
	
    return 0;
}
