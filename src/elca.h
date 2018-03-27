/*
 * Copyright (c) 2018 Elastos Foundation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __ELASTOS_CARRIER_ADDON_H__
#define __ELASTOS_CARRIER_ADDON_H__


#include <node_api.h>

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#include <ela_carrier.h>
#include <ela_session.h>

namespace elca {
    
    #define MAX_IPV4_ADDRESS_LEN (16)
    #define MAX_IPV6_ADDRESS_LEN (48)
    #define MAX_PORT_LEN         (16)
    #define MAX_PUBLIC_KEY_LEN   (48)

    typedef struct _BootstrapBuf {
        char ipv4[MAX_IPV4_ADDRESS_LEN];
        char ipv6[MAX_IPV6_ADDRESS_LEN];
        char port[MAX_PORT_LEN];
        char public_key[MAX_PUBLIC_KEY_LEN];
    } BootstrapBuf;

    typedef struct _CallbackContext {
        napi_env env;
        napi_value carrier;
        napi_value callbacks;
        napi_value context;

        napi_ref idle;
        napi_ref handle;
        napi_ref connection_status;
        napi_ref friends_list;
        napi_ref friend_connection;
        napi_ref friend_info;
        napi_ref friend_presence;
        napi_ref friend_request;
        napi_ref friend_added;
        napi_ref friend_removed;
        napi_ref friend_message;
        napi_ref friend_invite;


    } CallbackContext;

    extern napi_value default_value;

    typedef struct _Elca {
        CallbackContext cbcontext;
        ElaCarrier * carrier;
    } Elca;

    // BootstrapNode *set_BootstrapNode(napi_env env, napi_value obj, 
    //                 BootstrapNode* bootstraps, BootstrapBuf* bootstraps_buf);
    ElaOptions *set_OptionsValue(napi_env env, napi_value obj, ElaOptions *opts, 
                    BootstrapBuf** bootstraps_buf);

    napi_value set_CallbackFuntions(napi_env env, napi_value js_callbacks, 
                    ElaCallbacks* callbacks,  CallbackContext* cbcontext);

    napi_value create_UserInfo(napi_env env, const ElaUserInfo *info);
    napi_value create_FriendInfo(napi_env env, const ElaFriendInfo *info);

}

#endif /* __ELASTOS_CARRIER_ADDON_H__ */