//This  psuedo code cannot be compiled.

//We can have a big container called ElastosRT, all smaller components, like Carrier, DID, Wallet are inside properties
import {Carrier} from 'ElastosRT';

const options = {
  udp_enabled:true,
  loglevel: 4,
  logfile: 'elashell.log', // in real app, we cannot access to file system

  datadir: '.data', // in real app, we cannot access to file system. 

  bootstraps: [
    {
      ipv4: '13.58.208.50',
      port: 33445,
      public_key: '89vny8MrKdDKs7Uta9RdVmspPjnRMdwMmaiEW27pZ7gh'
    },
    {
      ipv4: '18.216.102.47',
      port: 33445,
      public_key: 'G5z8MqiNDFTadFUPfMdYsYtkUDbX5mNCMVHMZtsCnFeb'
    },
    {
      ipv4: '18.216.6.197',
      port: 33445,
      public_key: 'H8sqhRrQuJZ6iLtP2wanxt4LzdNrN2NNFnpPdq1uJ9n2'
    },
    {
      ipv4: '54.223.36.193',
      port: 33445,
      public_key: '5tuHgK1Q4CYf4K5PutsEPK5E3Z7cbtEBdx7LwmdzqXHL'
    },
    {
      ipv4: '52.80.187.125',
      port: 33445,
      public_key: '3khtxZo89SBScAMaHhTvD68pPHiKxgZT6hTCSZZVgNEm'
    }
  ]
}



const callback= {
  //idle: (args)=>{action('idle_callback', args)},
  connection_status: (args)=>{action('connection_callback', args)},
  friend_list: (args)=>{action('friends_list_callback', args)},
  friend_connection: (args)=>{action('friend_connection_callback', args)},
  friend_info: (args)=>{action('friend_info_callback', args)},
  friend_presence: (args)=>{action('friend_presence_callback', args)},
  friend_request: (args)=>{action('friend_request_callback', args)},
  friend_added: (args)=>{action('friend_added_callback', args)},
  friend_removed: (args)=>{action('friend_removed_callback', args)},
  friend_message: (args)=>{action('message_callback', args)},
  friend_invite: (args)=>{action('invite_request_callback', args)},

}
let carrierSessionId;
const dryRun = ()=>{
  Carrier.createNewCarrier(options, callback, (err, result)=>{
    if(err){
      console.log(err);
    }
    else{
      carrierSessionId = result;
    }
  })
}

const killCarrier = (sessionId)=>{
  Carrier.kill(sessionId, (err, result)=>{
    if(err){
      console.log(err);
    }
    else{
      console.log(`Carrier sessionID:${sessionId} killed`);
    }
  })
}

 const action = (actionType, args)=>{
   switch(actionType){
     case "friend_message":{
       showMessage(args);
       break;
     }
     case "connection_status":{
       console.log("Connection status changed, ", args);
       break;;
     }
     default:{
       console.log('Unhandled event,', actionType, args)
     }
   }
 }