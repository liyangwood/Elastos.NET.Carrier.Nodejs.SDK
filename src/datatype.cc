// /*
//  * Copyright (c) 2018 Elastos Foundation
//  *
//  * Permission is hereby granted, free of charge, to any person obtaining a copy
//  * of this software and associated documentation files (the "Software"), to deal
//  * in the Software without restriction, including without limitation the rights
//  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  * copies of the Software, and to permit persons to whom the Software is
//  * furnished to do so, subject to the following conditions:
//  *
//  * The above copyright notice and this permission notice shall be included in all
//  * copies or substantial portions of the Software.
//  *
//  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  * SOFTWARE.
//  */

#include "elca.h"

namespace elca {

    BootstrapNode *set_BootstrapNode(napi_env env, napi_value obj, BootstrapNode* bootstraps, BootstrapBuf* bootstraps_buf) {
        napi_status status;
        napi_value val;
        napi_valuetype valuetype;
        size_t size;
        
        status = napi_typeof(env, obj, &valuetype);
        if (status != napi_ok) return nullptr;
        if (valuetype != napi_object) return nullptr;

        //ipv4
        status = napi_get_named_property(env, obj, "ipv4", &val);
        if (status != napi_ok) {
            //TODO:: print error!
            return nullptr;
        }

        status = napi_get_value_string_utf8(env, val, bootstraps_buf->ipv4, MAX_IPV4_ADDRESS_LEN, &size);
        if (status != napi_ok) {
            //TODO:: print error!
            return nullptr;
        }
        bootstraps->ipv4 = bootstraps_buf->ipv4;

        //ipv6
        status = napi_get_named_property(env, obj, "ipv6", &val);
        if (status == napi_ok) {
            status = napi_get_value_string_utf8(env, val, bootstraps_buf->ipv6, MAX_IPV6_ADDRESS_LEN, &size);
            if (status == napi_ok) {
                bootstraps->ipv6 = bootstraps_buf->ipv6;
            }     
        }

        //port
        status = napi_get_named_property(env, obj, "port", &val);
        if (status != napi_ok) {
            //TODO:: print error!
            return nullptr;
        }

        status = napi_get_value_string_utf8(env, val, bootstraps_buf->port, MAX_PORT_LEN, &size);
        if (status != napi_ok) {
            //TODO:: print error!
            return nullptr;
        }
        bootstraps->port = bootstraps_buf->port;

        //public_key
        status = napi_get_named_property(env, obj, "public_key", &val);
        if (status != napi_ok) {
            //TODO:: print error!
            return nullptr;
        }

        status = napi_get_value_string_utf8(env, val, bootstraps_buf->public_key, MAX_PUBLIC_KEY_LEN, &size);
        if (status != napi_ok) {
            //TODO:: print error!
            return nullptr;
        }
        bootstraps->public_key = bootstraps_buf->public_key;

        printf("ipv4:%s, ipv6:%s, port:%s, public_key:%s\n", 
                bootstraps->ipv4, bootstraps->ipv6, bootstraps->port, bootstraps->public_key);

        return bootstraps;
    }

    ElaOptions *set_OptionsValue(napi_env env, napi_value obj, ElaOptions *opts, BootstrapBuf** bootstraps_buf) {
        napi_status status;
        napi_value val;
        napi_valuetype valuetype;
        size_t size;
        
        status = napi_typeof(env, obj, &valuetype);
        if (status != napi_ok) return nullptr;
        if (valuetype != napi_object) return nullptr;

        //udp_enabled
        status = napi_get_named_property(env, obj, "udp_enabled", &val);
        if (status != napi_ok) {
            //TODO:: print error!
            return nullptr;
        }

        status = napi_get_value_bool(env, val, &opts->udp_enabled);
        if (status != napi_ok) {
            //TODO:: print error!
            return nullptr;
        }

        //persistent_location
        status = napi_get_named_property(env, obj, "persistent_location", &val);
        if (status != napi_ok) {
            //TODO:: print error!
            return nullptr;
        }
        
        status = napi_get_value_string_utf8(env, val, (char *)opts->persistent_location, 1024, &size);
        if (status != napi_ok) {
            //TODO:: print error!
            return nullptr;
        }

        //bootstraps
        status = napi_get_named_property(env, obj, "bootstraps", &val);
        if (status != napi_ok) {
            //TODO:: print error!
            return nullptr;
        }


        bool is_array;
        status = napi_is_array(env, val, &is_array);
        if (status != napi_ok || !is_array) {
            //TODO:: print error!
            return nullptr;
        }

        uint32_t length;
        status = napi_get_array_length(env, val, &length);
        if (status != napi_ok) {
            //TODO:: print error!
            printf("status: %d\n", status);
            return nullptr;
        }

        if (length < 1) {
            //TODO:: print error!
            return nullptr;
        }
        opts->bootstraps_size = length;
        // printf("length=%d\n", sizeof(BootstrapNode) * opts->bootstraps_size);

        opts->bootstraps = (BootstrapNode *)calloc(1, sizeof(BootstrapNode) * length);
        if (!opts->bootstraps) {
            fprintf(stderr, "bootstraps out of memory.");
            return nullptr;
        }

        *bootstraps_buf = (BootstrapBuf *)calloc(1, sizeof(BootstrapBuf) * length);
        if (!*bootstraps_buf) {
            fprintf(stderr, "bootstraps_buf out of memory.");
            return nullptr;
        }

        napi_value element;
        for (uint32_t i = 0; i < length; i++) {
            printf("index:%d-- ", i);
            status = napi_get_element(env, val, i, &element);
            if (status != napi_ok) {
                //TODO:: print error!
                return nullptr;
            }

            if (!set_BootstrapNode(env, element, &opts->bootstraps[i], &(*bootstraps_buf)[i])) {
                //TODO:: print error!
                return nullptr;
            }
        }

        return opts;
    }

    napi_value create_UserInfo(napi_env env, const ElaUserInfo *info) {
        napi_value userid, name, description, has_avatar, gender, phone, email, region;
        napi_status status;

        if (!info) {
            return default_value;
        }

        puts(info->userid);
        status = napi_create_string_utf8(env, info->userid, NAPI_AUTO_LENGTH , &userid);
        if (status != napi_ok) { //TODO:: delete
            return nullptr;
        }

        status = napi_create_string_utf8(env, info->name, NAPI_AUTO_LENGTH , &name);
        if (status != napi_ok) {
            return nullptr;
        }

        status = napi_create_string_utf8(env, info->description, NAPI_AUTO_LENGTH , &description);
        if (status != napi_ok) {
            return nullptr;
        }

        status = napi_create_int32(env, info->has_avatar , &has_avatar);
        if (status != napi_ok) {
            return nullptr;
        }

        status = napi_create_string_utf8(env, info->gender, NAPI_AUTO_LENGTH , &gender);
        if (status != napi_ok) {
            return nullptr;
        }

        status = napi_create_string_utf8(env, info->phone, NAPI_AUTO_LENGTH , &phone);
        if (status != napi_ok) {
            return nullptr;
        }

        status = napi_create_string_utf8(env, info->email, NAPI_AUTO_LENGTH , &email);
        if (status != napi_ok) {
            return nullptr;
        }

        status = napi_create_string_utf8(env, info->region, NAPI_AUTO_LENGTH , &region);
        if (status != napi_ok) {
            return nullptr;
        }

        napi_value obj;
        status = napi_create_object(env, &obj);
        if (status != napi_ok) {
            return nullptr;
        }

        // Set the properties
        napi_property_descriptor descriptors[] = {
            { "userid", NULL, 0, 0, 0, userid, napi_default, 0 },
            { "name", NULL, 0, 0, 0, name, napi_default, 0 },
            { "description", NULL, 0, 0, 0, description, napi_default, 0 },
            { "has_avatar", NULL, 0, 0, 0, has_avatar, napi_default, 0 },
            { "gender", NULL, 0, 0, 0, gender, napi_default, 0 },
            { "phone", NULL, 0, 0, 0, phone, napi_default, 0 },
            { "email", NULL, 0, 0, 0, email, napi_default, 0 },
            { "region", NULL, 0, 0, 0, region, napi_default, 0 }
        };

        status = napi_define_properties(env, obj,
                                        sizeof(descriptors) / sizeof(descriptors[0]),
                                        descriptors);
        if (status != napi_ok) {
            return nullptr;
        }

        return obj;
    }

    napi_value create_FriendInfo(napi_env env, const ElaFriendInfo *info) {
        napi_value user_info, label, connection_status, presence;
        napi_status status;

        if (!info) {
            return default_value;
        }

        user_info = create_UserInfo(env, &info->user_info);

        status = napi_create_string_utf8(env, info->label, NAPI_AUTO_LENGTH , &label);
        if (status != napi_ok) {
            return nullptr;
        }

        status = napi_create_int32(env, info->status , &connection_status);
        if (status != napi_ok) {
            return nullptr;
        }

        status = napi_create_int32(env, info->presence , &presence);
        if (status != napi_ok) {
            return nullptr;
        }

        napi_value obj;
        status = napi_create_object(env, &obj);
        if (status != napi_ok) {
            return nullptr;
        }

        // Set the properties
        napi_property_descriptor descriptors[] = {
            { "user_info", NULL, 0, 0, 0, user_info, napi_default, 0 },
            { "label", NULL, 0, 0, 0, label, napi_default, 0 },
            { "status", NULL, 0, 0, 0, connection_status, napi_default, 0 },
            { "presence", NULL, 0, 0, 0, presence, napi_default, 0 }
        };

        status = napi_define_properties(env, obj,
                                        sizeof(descriptors) / sizeof(descriptors[0]),
                                        descriptors);
        if (status != napi_ok) {
            return nullptr;
        }

        return obj;
    }

} // namespace elca
