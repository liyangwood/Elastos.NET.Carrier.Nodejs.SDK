//
// Copyright (c) 2018 Elastos Foundation
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

const carrierSdk = require('../../build/Debug/elca')

var bootstraps = [
    {
        ipv4: "13.58.208.50",
        port: "33445",
        public_key: "89vny8MrKdDKs7Uta9RdVmspPjnRMdwMmaiEW27pZ7gh",
    },
    {
        ipv4: "18.216.102.47",
        port: "33445",
        public_key: "G5z8MqiNDFTadFUPfMdYsYtkUDbX5mNCMVHMZtsCnFeb"
    },
    {
        ipv4: "18.216.6.197",
        port: "33445",
        public_key: "H8sqhRrQuJZ6iLtP2wanxt4LzdNrN2NNFnpPdq1uJ9n2"
    },
    {
        ipv4: "54.223.36.193",
        port: "33445",
        public_key: "5tuHgK1Q4CYf4K5PutsEPK5E3Z7cbtEBdx7LwmdzqXHL"
    },
    {
        ipv4: "52.80.187.125",
        port: "33445",
        public_key: "3khtxZo89SBScAMaHhTvD68pPHiKxgZT6hTCSZZVgNEm"
    }
]

var opts = {
    udp_enabled: true,
    persistent_location: ".data",
    bootstraps_size: bootstraps.length,
    bootstraps: bootstraps
}

function idle_callback(carrier, context) {
    console.log("call idle_callback.");
}

function connection_callback(carrier, context) {
    console.log("call connection_callback.");
}

function friends_list_callback(carrier, context) {
    console.log("call friends_list_callback.");
}

function friend_connection_callback(carrier, context) {
    console.log("call friend_connection_callback.");
}

function friend_info_callback(carrier, context) {
    console.log("call friend_info_callback.");
}

function friend_presence_callback(carrier, context) {
    console.log("call friend_presence_callback.");
}

function friend_request_callback(carrier, context) {
    console.log("call friend_request_callback.");
}

function friend_added_callback(carrier, context) {
    console.log("call friend_added_callback.");
}

function friend_removed_callback(carrier, context) {
    console.log("call friend_removed_callback.");
}

function message_callback(carrier, context) {
    console.log("call message_callback.");
}

function invite_request_callback(carrier, context) {
    console.log("call invite_request_callback.");
}

var callbacks = {
    idle: idle_callback,
    connection_status: connection_callback,
    friends_list: friends_list_callback,
    friend_connection: friend_connection_callback,
    friend_info: friend_info_callback,
    friend_presence: friend_presence_callback,
    friend_request: friend_request_callback,
    friend_added: friend_added_callback,
    friend_removed: friend_removed_callback,
    friend_message: message_callback,
    friend_invite: invite_request_callback
}

var context = "context";

var carrier = carrierSdk.createObject(opts, callbacks, context);
carrier.run(); //Block!!
carrier.destory();
console.log("end"); 
