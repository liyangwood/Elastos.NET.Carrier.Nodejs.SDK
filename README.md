# TL;DR
Elastos.NET.Carrier.Nodejs.SDK is the [Node.js Add-on](https://nodeaddons.com/) for Elastos Carrier. It allows Javascript code to call [Carrier C++](https://github.com/elastos/Elastos.NET.Carrier.Native.SDK) functions. 

# Design concerns
## Thread safe
Carrier is multithreaded C++ module. JS is single thread. We need to make sure threads are well managed in NodeAddOn level. JS code call the addon to create new Carrier object. the addon will create new thread to handle the long running working thread(s) until it quits. JS caller can get a session ID to identify this particular carrier object. All communication between add-on and js code will have this session ID as the first arg. 

## Session management
When Carrier C++ code create a new Carrier object, it returns a memory pointer to the new gneerated carrier object. But the memory pointer cannot be sent to the js code. Instead, we will hold the object in add-on memory into a table. A unique session ID can find the pointer to this Carrier object. The session ID can be sent to JS. Everytime, when JS call any function, or any callback function will need this session ID to identify which Carrier object it is talking to.

## Streaming
<!--- To Do -->

# Demo App
We are going to make a demo web app to clone the Carrier Native SDK shell demo app. The shell demo app is a command line app. You can run two shell app as two different users. They can talk to each other, adding/removing friends, steaming etc.

We are going to make the same app but in browser using this NodeJs SDK. 

