/*
 * Copyright (C) 2016 "IoT.bzh"
 * Author Fulup Ar Foll <fulup@iot.bzh>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * references:
 *  
 */

#define _GNU_SOURCE

#include <sys/prctl.h>
#include <stdio.h>

#include "mpdc-binding.h"

// Include Binding Stub generated from Json OpenAPI
STATIC int mpdcBindingInit(void);
STATIC void mpdcDispatchEvent(const char *evtLabel, json_object *eventJ);
#include "mpdc-apidef.h"


PUBLIC  bool mpdcIfConnectFail(mpdcChannelEnumT channel, mpdcHandleT *mpdcHandle, afb_req request) {
    bool forceConnect= false; 
    mpdConnectT *mpd;
    
    // if exit try reusing current connection
    switch (channel) {
        case MPDC_CHANNEL_CMD:
            if (!mpdcHandle->mpd) forceConnect=true;
            else mpd=mpdcHandle->mpd;
            break;
        case MPDC_CHANNEL_EVT:
            if (!mpdcHandle->mpdEvt) forceConnect=true;
            else mpd=mpdcHandle->mpdEvt;
            break;
        default:
            AFB_ERROR("MDPC:ConnectFail (Hoops) invalid channel value");
            goto OnErrorExit;     
    };
    
    // if not already connected let's try to connect
    if (forceConnect) {
        // connect to MPD daemon NULL=localhost, 0=default port, 30000 timeout/ms
        mpd = mpd_connection_new(mpdcHandle->hostname, mpdcHandle->port, mpdcHandle->timeout);
        if (mpd == NULL) {
            if (afb_req_is_valid(request)) afb_req_fail (request, "MDCP:Create", "No More Memory");
            goto OnErrorExit;
        }   
        
        if (channel == MPDC_CHANNEL_CMD) mpdcHandle->mpd=mpd;
        if (channel == MPDC_CHANNEL_EVT) mpdcHandle->mpdEvt=mpd;
    }
   
    if (mpd_connection_get_error(mpd) != MPD_ERROR_SUCCESS) {
            AFB_ERROR("MDPC:Connect error=%s",  mpd_connection_get_error_message(mpd));
            mpd_connection_free(mpd);
            goto OnErrorExit;
    }
    return false;
    
 OnErrorExit:
    return true;
}


// Call when ever en event reach Mpdc Binding
STATIC void mpdcDispatchEvent(const char *evtLabel, json_object *eventJ) {
    
}

// Call at Init time (place here any runtime dependencies)
STATIC int mpdcBindingInit(void) {

    
    int rc= prctl(PR_SET_NAME, "afb-mpdc-agent",NULL,NULL,NULL);
    if (rc) AFB_ERROR("ERROR: AlsaCore fail to rename process");
    
    // create a global event to send MPDC events
    const char*binderName = GetBinderName();

    // when set wait for explicit connect request
    const char *noDefConnect= getenv("MPDC_NODEF_CONNECT");
    if (!noDefConnect) rc+=mpdcapi_init(binderName);

    return rc;
}


