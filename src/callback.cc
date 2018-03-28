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

    static napi_value RunCallBack(napi_ref handle, size_t argc, napi_value* argv, CallbackContext *cbcontext) {
        napi_value args[8];
        napi_status status;
        napi_env env = cbcontext->env;
        size_t i;

        if (!handle) return nullptr;

        args[0] = cbcontext->carrier;
        argc++;

        for(i = 1; i < argc; i++) {
            args[i] = argv[i - 1];
        }
        args[i] = cbcontext->context;
        argc++;

        // napi_handle_scope scope;
        // status = napi_open_handle_scope(env, &scope);
        // if (status != napi_ok) return nullptr;

        napi_value global;
        status = napi_get_global(env, &global);
        if (status != napi_ok) return nullptr;

        napi_value fn;
        status = napi_get_reference_value(env, handle, &fn);
        napi_value result;
        status = napi_make_callback(env, nullptr, global, fn, argc, args, &result);
        // status = napi_call_function(env, global, info->fn, info->argc, info->argv, &result);
        if (status != napi_ok) {
            napi_throw_error(env, nullptr, "Run callback function 'idle' error!");
            return nullptr;
        }

        // status = napi_close_handle_scope(env, scope);

        return result;
    }

    static void idle_callback(ElaCarrier *w, void *context)
    {    
        Elca *elca = (Elca*)context;
        CallbackContext *cbcontext = &elca->cbcontext;

        RunCallBack(cbcontext->idle, 0, nullptr, cbcontext);
    }

    static void connection_callback(ElaCarrier *w, ElaConnectionStatus status,
                                    void *context)
    {
        Elca *elca = (Elca*)context;         
        CallbackContext *cbcontext = &elca->cbcontext;
        napi_env env = cbcontext->env;

        napi_value argv[1];
        argv[0] = nullptr;
        napi_create_int32(env, status, argv);
   
        RunCallBack(cbcontext->connection_status, 1, argv, cbcontext);
    }

    static bool friends_list_callback(ElaCarrier *w, const ElaFriendInfo *friend_info,
                                 void *context)
    {
        Elca *elca = (Elca*)context;         
        CallbackContext *cbcontext = &elca->cbcontext;
        napi_env env = cbcontext->env;

        napi_value argv[1];
        argv[0] = create_FriendInfo(env, friend_info);

        bool result = true;
        napi_value ret = RunCallBack(cbcontext->friends_list, 1, argv, cbcontext);
        napi_get_value_bool(env, ret, &result);

        return result;
    }

    static void friend_added_callback(ElaCarrier *w, const ElaFriendInfo *info,
                                  void *context)
    {
        Elca *elca = (Elca*)context;         
        CallbackContext *cbcontext = &elca->cbcontext;
        napi_env env = cbcontext->env;

        napi_value argv[1];
        argv[0] = create_FriendInfo(env, info);

        RunCallBack(cbcontext->friend_added, 1, argv, cbcontext);
    }

    static void friend_removed_callback(ElaCarrier *w, const char *friendid,
                                        void *context)
    {
        Elca *elca = (Elca*)context;         
        CallbackContext *cbcontext = &elca->cbcontext;
        napi_env env = cbcontext->env;
        
        napi_value argv[1];
        argv[0] = nullptr;
        napi_create_string_utf8(env, friendid, NAPI_AUTO_LENGTH, argv);
      
        RunCallBack(cbcontext->friend_removed, 1, argv, cbcontext);
    }

    static void friend_info_callback(ElaCarrier *w, const char *friendid,
                                    const ElaFriendInfo *info, void *context)
    {
        Elca *elca = (Elca*)context;         
        CallbackContext *cbcontext = &elca->cbcontext;
        napi_env env = cbcontext->env;

        napi_value argv[2];
        argv[0] = nullptr; argv[1] = nullptr;

        napi_create_string_utf8(env, friendid, NAPI_AUTO_LENGTH, &argv[0]);
        argv[1] = create_FriendInfo(env, info);

        RunCallBack(cbcontext->friend_info, 2, argv, cbcontext);
    }

    static void friend_connection_callback(ElaCarrier *w, const char *friendid,
                                        ElaConnectionStatus status, void *context)
    {
        Elca *elca = (Elca*)context;         
        CallbackContext *cbcontext = &elca->cbcontext;
        napi_env env = cbcontext->env;

        napi_value argv[2];
        argv[0] = nullptr; argv[1] = nullptr;

        napi_create_string_utf8(env, friendid, NAPI_AUTO_LENGTH, &argv[0]);
        napi_create_int32(env, status, &argv[1]);

        RunCallBack(cbcontext->friend_connection, 2, argv, cbcontext);

    }

    static void friend_presence_callback(ElaCarrier *w, const char *friendid,
                                        ElaPresenceStatus status,
                                        void *context)
    {
        Elca *elca = (Elca*)context;         
        CallbackContext *cbcontext = &elca->cbcontext;
        napi_env env = cbcontext->env;

        napi_value argv[2];
        argv[0] = nullptr; argv[1] = nullptr;

        napi_create_string_utf8(env, friendid, NAPI_AUTO_LENGTH, &argv[0]);
        napi_create_int32(env, status, &argv[1]);

        RunCallBack(cbcontext->friend_presence, 2, argv, cbcontext);
    }

    static void friend_request_callback(ElaCarrier *w, const char *userid,
                                        const ElaUserInfo *info, const char *hello,
                                        void *context)
    {
        Elca *elca = (Elca*)context;         
        CallbackContext *cbcontext = &elca->cbcontext;
        napi_env env = cbcontext->env;

        napi_value argv[3];
        argv[0] = nullptr; argv[1] = nullptr; argv[2] = nullptr;

        napi_create_string_utf8(env, userid, NAPI_AUTO_LENGTH, &argv[0]);
        argv[1] = create_UserInfo(env, info);
        napi_create_string_utf8(env, hello, NAPI_AUTO_LENGTH, &argv[2]);

        RunCallBack(cbcontext->friend_request, 3, argv, cbcontext);
    }

    static void message_callback(ElaCarrier *w, const char *from,
                                const char *msg, size_t len, void *context)
    {
        Elca *elca = (Elca*)context;         
        CallbackContext *cbcontext = &elca->cbcontext;
        napi_env env = cbcontext->env;

        napi_value argv[3];
        argv[0] = nullptr; argv[1] = nullptr; argv[2] = nullptr;

        napi_create_string_utf8(env, from, NAPI_AUTO_LENGTH, &argv[0]);
        napi_create_string_utf8(env, msg, NAPI_AUTO_LENGTH, &argv[1]);
        napi_create_int32(env, len, &argv[2]);


        RunCallBack(cbcontext->friend_message, 3, argv, cbcontext);
    }

    static void invite_request_callback(ElaCarrier *w, const char *from,
                                        const char *data, size_t len, void *context)
    {
        Elca *elca = (Elca*)context;         
        CallbackContext *cbcontext = &elca->cbcontext;
        napi_env env = cbcontext->env;

        napi_value argv[3];
        argv[0] = nullptr; argv[1] = nullptr; argv[2] = nullptr;

        napi_create_string_utf8(env, from, NAPI_AUTO_LENGTH, &argv[0]);
        napi_create_string_utf8(env, data, NAPI_AUTO_LENGTH, &argv[1]);
        napi_create_int32(env, len, &argv[2]);

        RunCallBack(cbcontext->friend_invite, 3, argv, cbcontext);
    }

    static inline napi_ref set_CallbackFuntion(napi_env env, napi_value callbacks, const char* name, napi_ref* handle) {
        napi_status status;
        napi_value fn = nullptr;
        napi_valuetype valuetype;
        
        *handle = nullptr;

        status = napi_get_named_property(env, callbacks, name, &fn);
        if (status != napi_ok || !fn) return nullptr;

        status = napi_typeof(env, fn, &valuetype);
        if (status != napi_ok || valuetype != napi_function) {
            puts("type is not function");
            return nullptr;
        };
  
        status = napi_create_reference(env, fn, 1, handle);
        if (status != napi_ok) {
            return nullptr;
        }

        return *handle;
    }


    napi_value set_CallbackFuntions(napi_env env, napi_value js_callbacks, ElaCallbacks* callbacks,  CallbackContext *cbcontext) {
        napi_status status;
        napi_valuetype valuetype;
        
        status = napi_typeof(env, js_callbacks, &valuetype);
        if (status != napi_ok) return nullptr;
        if (valuetype != napi_object) return nullptr;
  
        cbcontext->env = env;
        cbcontext->callbacks = js_callbacks;

        set_CallbackFuntion(env, js_callbacks, "idle", &cbcontext->idle);
        set_CallbackFuntion(env, js_callbacks, "connection_status", &cbcontext->connection_status);
        set_CallbackFuntion(env, js_callbacks, "friends_list", &cbcontext->friends_list);
        set_CallbackFuntion(env, js_callbacks, "friend_connection", &cbcontext->friend_connection);
        set_CallbackFuntion(env, js_callbacks, "friend_info", &cbcontext->friend_info);
        set_CallbackFuntion(env, js_callbacks, "friend_presence", &cbcontext->friend_presence);
        set_CallbackFuntion(env, js_callbacks, "friend_request", &cbcontext->friend_request);
        set_CallbackFuntion(env, js_callbacks, "friend_added", &cbcontext->friend_added);
        set_CallbackFuntion(env, js_callbacks, "friend_removed", &cbcontext->friend_removed);
        set_CallbackFuntion(env, js_callbacks, "friend_message", &cbcontext->friend_message);
        set_CallbackFuntion(env, js_callbacks, "friend_invite", &cbcontext->friend_invite);

        memset(callbacks, 0, sizeof(ElaCallbacks));
        callbacks->idle = idle_callback;
        callbacks->connection_status = connection_callback;
        callbacks->friend_list = friends_list_callback;
        callbacks->friend_connection = friend_connection_callback;
        callbacks->friend_info = friend_info_callback;
        callbacks->friend_presence = friend_presence_callback;
        callbacks->friend_request = friend_request_callback;
        callbacks->friend_added = friend_added_callback;
        callbacks->friend_removed = friend_removed_callback;
        callbacks->friend_message = message_callback;
        callbacks->friend_invite = invite_request_callback;

        return js_callbacks;
    }

} // namespace elca
