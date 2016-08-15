  1 /**                                                                                                                                                                                          
  2   \file                                                                                                                                                                                      
  3   \brief File contains structs to provide client-server interaction.                                                                                                                         
  4 */                                                                                                                                                                                           
  5                                                                                                                                                                                              
  6 #ifndef _CLIENT_SERVER_H_                                                                                                                                                                    
  7 #define _CLIENT_SERVER_H_                                                                                                                                                                    
  8                                                                                                                                                                                              
  9 #include "position.h"                                                                                                                                                                        
 10 #include "common.h"                                                                                                                                                                          
 11                                                                                                                                                                                              
 12 /**                                                                                                                                                                                          
 13   \brief Enum to represent user actions.                                                                                                                                                     
 14 */                                                                                                                                                                                           
 15 enum Doing {                                                                                                                                                                                 
 16   NOTHING = 0,                                                                                                                                                                               
 17   PLANT_BOMB = 1,                                                                                                                                                                            
 18   MOVE_LEFT,                                                                                                                                                                                 
 19   MOVE_RIGHT,                                                                                                                                                                                
 20   MOVE_TOP,                                                                                                                                                                                  
 21   MOVE_DOWN                                                                                                                                                                                  
 22 };                                                                                                                                                                                           
 23                                                                                                                                                                                              
 24 /**                                                                                                                                                                                          
 25   \brief This struct should be sent to server from client.                                                                                                                                   
 26 */                                                                                                                                                                                           
 27 struct ClientToServer                                                                                                                                                                        
 28 {                                                                                                                                                                                            
 29   uint8_t id;                                                                                                                                                                                
 30   enum Doing doing;                                                                                                                                                                          
 31 } __attribute__((packed));                                                                                                                                                                   
 32                                                                                                                                                                                              
 33 /**                                                                                                                                                                                          
 34   \brief Struct to recv message from server.                                                                                                                                                 
 35                                                                                                                                                                                              
 36   First time sending new id, and new field.                                                                                                                                                  
 37 */                                                                                                                                                                                           
 38 struct ServerToClient {                                                                                                                                                                      
 39   uint8_t id;                                                                                                                                                                                
 40   struct Field field;                                                                                                                                                                        
 41   struct Dklb stats;                                                                                                                                                                         
 42 } __attribute__((packed));                                                                                                                                                                   
 43                                                                                                                                                                                              
 44 #endif /* _CLIENT_SERVER_H_ */
