/*
 * SMI_Msg_Queue.cpp
 *
 *  Created on: 2018. 12. 22.
 *      Author: finegs
 */
#include <iostream>
#include <utility>
#include <SMI.hpp>

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/interprocess_fwd.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/managed_external_buffer.hpp>
#include <boost/interprocess/managed_heap_memory.hpp>
#include <boost/interprocess/segment_manager.hpp>
#include <boost/interprocess/containers/list.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/pair.hpp>
#include <boost/interprocess/containers/allocation_type.hpp>
#include <boost/aligned_storage.hpp>

#if 0

//This test creates a in memory data-base using Interprocess machinery and
//serializes it through a message queue. Then rebuilds the data-base in
//another buffer and checks it against the original data-base
bool test_serialize_db()
{

using namespace boost::interprocess;

   //Typedef data to create a Interprocess map
   typedef std::pair<const std::size_t, std::size_t> MyPair;
   typedef std::less<std::size_t>   MyLess;
   typedef node_allocator<MyPair, managed_external_buffer::segment_manager> node_allocator_t;
   typedef map<std::size_t,
               std::size_t,
               std::less<std::size_t>,
               node_allocator_t>
               MyMap;

   //Some constants
   const std::size_t BufferSize  = 65536;
   const std::size_t MaxMsgSize  = 100;

   //Allocate a memory buffer to hold the destiny database using vector<char>
   std::vector<char> buffer_destiny(BufferSize, 0);

   message_queue::remove(test::get_process_id_name());
   {
      //Create the message-queues
      message_queue mq1(create_only, test::get_process_id_name(), 1, MaxMsgSize);

      //Open previously created message-queue simulating other process
      message_queue mq2(open_only, test::get_process_id_name());

      //A managed heap memory to create the origin database
      managed_heap_memory db_origin(buffer_destiny.size());

      //Construct the map in the first buffer
      MyMap *map1 = db_origin.construct<MyMap>("MyMap")
                                       (MyLess(),
                                       db_origin.get_segment_manager());
      if(!map1)
         return false;

      //Fill map1 until is full
      try{
         std::size_t i = 0;
         while(1){
            (*map1)[i] = i;
            ++i;
         }
      }
      catch(boost::interprocess::bad_alloc &){}

      //Data control data sending through the message queue
      std::size_t sent = 0;
      message_queue::size_type recvd = 0;
      message_queue::size_type total_recvd = 0;
      unsigned int priority;

      //Send whole first buffer through the mq1, read it
      //through mq2 to the second buffer
      while(1){
         //Send a fragment of buffer1 through mq1
       std::size_t bytes_to_send = MaxMsgSize < (db_origin.get_size() - sent) ?
                                       MaxMsgSize : (db_origin.get_size() - sent);
         mq1.send( &static_cast<char*>(db_origin.get_address())[sent]
               , bytes_to_send
               , 0);
         sent += bytes_to_send;
         //Receive the fragment through mq2 to buffer_destiny
       mq2.receive( &buffer_destiny[total_recvd]
                , BufferSize - recvd
                  , recvd
                  , priority);
         total_recvd += recvd;

         //Check if we have received all the buffer
         if(total_recvd == BufferSize){
            break;
         }
      }

      //The buffer will contain a copy of the original database
      //so let's interpret the buffer with managed_external_buffer
      managed_external_buffer db_destiny(open_only, &buffer_destiny[0], BufferSize);

      //Let's find the map
      std::pair<MyMap *, managed_external_buffer::size_type> ret = db_destiny.find<MyMap>("MyMap");
      MyMap *map2 = ret.first;

      //Check if we have found it
      if(!map2){
         return false;
      }

      //Check if it is a single variable (not an array)
      if(ret.second != 1){
         return false;
      }

      //Now let's compare size
      if(map1->size() != map2->size()){
         return false;
      }

      //Now let's compare all db values
     MyMap::size_type num_elements = map1->size();
     for(std::size_t i = 0; i < num_elements; ++i){
         if((*map1)[i] != (*map2)[i]){
            return false;
         }
      }

      //Destroy maps from db-s
      db_origin.destroy_ptr(map1);
      db_destiny.destroy_ptr(map2);
   }
   message_queue::remove(test::get_process_id_name());
   return true;
}


#endif
