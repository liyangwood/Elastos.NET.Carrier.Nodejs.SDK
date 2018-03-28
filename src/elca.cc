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

 
#include "elca.h"

namespace elca {

    napi_value default_value = nullptr;

    napi_value elca_run(napi_env env, napi_callback_info info) {
        Elca* elca = nullptr;

        size_t argc = 3;
        napi_value args[3];

        napi_get_cb_info(env, info, &argc, args, nullptr, (void**)&elca);
        if (elca && elca->carrier) {
            int rc = ela_run(elca->carrier, 10);
            if (rc != 0) {
                puts("run---error!!!-");
                ela_kill(elca->carrier);
            }
        }

        puts("run---ok-");
        return nullptr;
    }

    napi_value elca_kill(napi_env env, napi_callback_info info) {
        Elca* elca = nullptr;
        napi_get_cb_info(env, info, nullptr, nullptr, nullptr, (void**)&elca);
        if (elca && elca->carrier) {
            ela_kill(elca->carrier);
        }
        return nullptr;
    }

    napi_value elca_is_ready(napi_env env, napi_callback_info info) {
        napi_value result = nullptr;
        Elca* elca = nullptr;
        bool ret = false;

        napi_get_cb_info(env, info, nullptr, nullptr, nullptr, (void**)&elca);
        if (elca && elca->carrier) {
            ret = ela_is_ready(elca->carrier);
        }

        napi_create_int32(env, ret, &result);
        return result;
    }

//------------------------------------------------------------------------------

    napi_value elca_new(napi_env env, napi_callback_info info) {
        napi_status status;
        napi_value ret = nullptr;
        ElaOptions opts;
        ElaCallbacks callbacks;
        Elca *elca = nullptr;
        BootstrapBuf *bootstraps_buf = nullptr;
        char buf[1024];
        ElaCarrier *carrier;

        size_t argc = 3;
        napi_value args[3];

        status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
        if(status != napi_ok || argc < 1) {
            // napi_throw_type_error(env, "Wrong number of arguments");
            return nullptr;
        }

        memset(&opts, 0, sizeof(ElaOptions));
        opts.persistent_location = buf;
        if (set_OptionsValue(env, args[0], &opts, &bootstraps_buf) == nullptr) {
            goto EIXT;
        }

        elca = (Elca *)malloc(sizeof(Elca));
        if (!elca) {
            fprintf(stderr, "elca out of memory.");
            return nullptr;
        }
        memset(elca, 0, sizeof(Elca));
 
        if (set_CallbackFuntions(env, args[1], &callbacks, &elca->cbcontext) == nullptr) {
            goto EIXT;
        }

        carrier = ela_new(&opts, &callbacks, elca);
        if (carrier) {       
            status = napi_create_object(env, &ret);
            if (status != napi_ok || !ret) {
                ret = nullptr;
                goto EIXT;
            }

            elca->carrier = carrier;
            elca->cbcontext.carrier = ret;
            elca->cbcontext.context = args[2];  

            napi_value fn_run = nullptr, fn_kill = nullptr, fn_is_ready = nullptr;
            napi_create_function(env, nullptr, 0, elca_run, (void*)elca, &fn_run);
            napi_create_function(env, nullptr, 0, elca_kill, (void*)elca, &fn_kill);
            napi_create_function(env, nullptr, 0, elca_is_ready, (void*)elca, &fn_is_ready);

            // Set the properties
            napi_property_descriptor descriptors[] = {
                { "run", NULL, 0, 0, 0, fn_run, napi_default, 0 },
                { "destory", NULL, 0, 0, 0, fn_kill, napi_default, 0 },
                { "isReady", NULL, 0, 0, 0, fn_is_ready, napi_default, 0 },
            };

            status = napi_define_properties(env,
                                            ret,
                                            sizeof(descriptors) / sizeof(descriptors[0]),
                                            descriptors);
            if (status != napi_ok) {
                goto EIXT;
            }

            napi_create_object(env, &default_value);                 
        }

EIXT:
        if (!ret && elca) free(elca);
        if (opts.bootstraps) free(opts.bootstraps);
        if (bootstraps_buf) free(bootstraps_buf);

        return ret;
    }


    napi_value init(napi_env env, napi_value exports) {
        napi_status status;
        napi_value fn;

        status = napi_create_function(env, nullptr, 0, elca_new, nullptr, &fn);
        if (status != napi_ok) return nullptr;

        status = napi_set_named_property(env, exports, "createObject", fn);
        if (status != napi_ok) return nullptr;

        // ela_log_init(4, "ela_addon.log", log_print);

        return exports;
    }

    NAPI_MODULE(NODE_GYP_MODULE_NAME, init)
} // namespace elca
