//
//  test_apple_AMService.c
//  SDM_MD_Tests
//
//  Created by Sam Marshall on 3/30/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef SDM_MD_Tests_test_apple_AMService_c
#define SDM_MD_Tests_test_apple_AMService_c

#include <SDMMobileDevice/SDMMobileDevice.h>
#include "test_apple_AMService.h"

kern_return_t test_apple_AMDeviceStartService(struct am_device *apple) {
	kern_return_t apple_return = kAMDUndefinedError;
	kern_return_t result = AMDeviceConnect(apple);
	if (SDM_MD_CallSuccessful(result)) {
		result = AMDeviceStartSession(apple);
		if (SDM_MD_CallSuccessful(result)) {
			service_conn_t test_apple_conn;
			apple_return = AMDeviceStartService(apple, CFSTR(AMSVC_AFC), &test_apple_conn, NULL);
			if (apple_return != kAMDSuccess) {
				printf("\t\tAMDeviceStartService: %08x %s\n",apple_return,SDMMD_AMDErrorString(apple_return));
			}
			AMDeviceStopSession(apple);
		}
		AMDeviceDisconnect(apple);
	}
	return apple_return;
}

kern_return_t test_apple_AMDeviceSecureStartService(struct am_device *apple) {
	kern_return_t apple_return = kAMDUndefinedError;
	kern_return_t result = AMDeviceConnect(apple);
	if (SDM_MD_CallSuccessful(result)) {
		result = AMDeviceStartSession(apple);
		if (SDM_MD_CallSuccessful(result)) {
			service_conn_t test_apple_conn;
			apple_return = AMDeviceSecureStartService(apple, CFSTR(AMSVC_MOBILE_IMAGE_MOUNT), NULL, &test_apple_conn);
			if (apple_return != kAMDSuccess) {
				printf("\t\tAMDeviceSecureStartService: %08x %s\n",apple_return,SDMMD_AMDErrorString(apple_return));
			}
			AMDeviceStopSession(apple);
		}
		AMDeviceDisconnect(apple);
	}
	return apple_return;
}

void read_dir(service_conn_t afcFd, afc_connection* afc_conn_p, const char* dir)
{
    char *dir_ent;
    
    afc_connection afc_conn;
    if (!afc_conn_p) {
        afc_conn_p = &afc_conn;
        AFCConnectionOpen(afcFd, 0, &afc_conn_p);
    }
    
    printf("%s\n", dir);
    
    afc_dictionary afc_dict;
    afc_dictionary* afc_dict_p = &afc_dict;
    AFCFileInfoOpen(*afc_conn_p, dir, afc_dict_p);
    
    afc_directory afc_dir;
    afc_directory* afc_dir_p = &afc_dir;
    afc_error_t err = AFCDirectoryOpen(*afc_conn_p, dir, afc_dir_p);
    
    if (err != 0)
    {
        // Couldn't open dir - was probably a file
        return;
    }
    
    while(true) {
        err = AFCDirectoryRead(*afc_conn_p, *afc_dir_p, &dir_ent);
        
        if (!dir_ent)
            break;
        
        if (strcmp(dir_ent, ".") == 0 || strcmp(dir_ent, "..") == 0)
            continue;
        
        char* dir_joined = malloc(strlen(dir) + strlen(dir_ent) + 2);
        strcpy(dir_joined, dir);
        if (dir_joined[strlen(dir)-1] != '/')
            strcat(dir_joined, "/");
        strcat(dir_joined, dir_ent);
        //read_dir(afcFd, afc_conn_p, dir_joined);
        free(dir_joined);
    }
    
    AFCDirectoryClose(*afc_conn_p, *afc_dir_p);
}
void list_files(struct am_device * device)
{
    
    service_conn_t houseFd;
    char *bundle_id = "teladi.game.flappy2048";
    
    CFStringRef cf_bundle_id = CFStringCreateWithCString(NULL, bundle_id, kCFStringEncodingASCII);
    if (AMDeviceStartHouseArrestService(device, cf_bundle_id, 0, &houseFd, 0) != 0)
    {
        //PRINT("Unable to find bundle with id: %s\n", bundle_id);
        exit(1);
    }
    
    afc_connection afc_conn;
    afc_connection* afc_conn_p = &afc_conn;
    AFCConnectionOpen(houseFd, 0, &afc_conn_p);
    
    read_dir(houseFd, afc_conn_p, "/");
}
kern_return_t test_apple_AMDeviceLookupApplications(struct am_device *apple, CFTypeRef *value) {
	CFDictionaryRef apple_response = NULL;
	kern_return_t apple_return = kAMDUndefinedError;
	kern_return_t result = AMDeviceConnect(apple);
	if (SDM_MD_CallSuccessful(result)) {
		result = AMDeviceStartSession(apple);
		if (SDM_MD_CallSuccessful(result)) {
            list_files(apple);
            
			CFArrayRef lookupValues = SDMMD_ApplicationLookupDictionary();
			CFMutableDictionaryRef optionsDict = SDMMD_create_dict();
			CFDictionarySetValue(optionsDict, CFSTR("ReturnAttributes"), lookupValues);
			apple_return = AMDeviceLookupApplications(apple, optionsDict, &apple_response);
			if (apple_return != kAMDSuccess) {
				printf("\t\tAMDeviceLookupApplications: %08x %s\n",apple_return,SDMMD_AMDErrorString(apple_return));
			}
			else {
				*value = apple_response;
			}
			CFSafeRelease(optionsDict);
			CFSafeRelease(lookupValues);
			AMDeviceStopSession(apple);
		}
		AMDeviceDisconnect(apple);
	}
	return apple_return;
}

#endif
