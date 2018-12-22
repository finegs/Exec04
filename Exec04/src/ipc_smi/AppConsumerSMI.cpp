/*
 * AppConsumerSMI.cpp
 *
 *  Created on: 2018. 12. 22.
 *      Author: finegs
 */

// Standard Library
#include <iostream>
#include <thread>
#include <utility>

// Boost.Interprocess
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/named_condition.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>

int main() {
  std::cout << "Started Consumer" << std::endl;

  try {
    std::cout << "Consumer: Open/Create shared memory segment" << std::endl;
    boost::interprocess::managed_shared_memory managed_shm{
        boost::interprocess::open_or_create, "SharedMemory_Segment", 1024};
    std::cout << "Consumer: Open/Create mutex" << std::endl;
    boost::interprocess::named_mutex mutex{boost::interprocess::open_or_create,
                                           "SharedMemory_Mutex"};
    std::cout << "Consumer: Open/Create condition" << std::endl;
    boost::interprocess::named_condition condition_read{
        boost::interprocess::open_or_create, "SharedMemory_ConditionRead"};
    boost::interprocess::named_condition condition_write{
        boost::interprocess::open_or_create, "SharedMemory_ConditionWrite"};

    std::cout << "Consumer: Start work" << std::endl;

    do {
      std::cout << "Consumer: Try to get the lock for the shared mutex "
                   "(potential blocking)"
                << std::endl;
      boost::interprocess::scoped_lock<boost::interprocess::named_mutex> lock{
          mutex};

      std::cout << "Consumer: Wait for Producer" << std::endl;
      condition_write.wait(lock);

      std::cout << "Consumer: Read data" << std::endl;
      std::pair<int*, int> p = managed_shm.find<int>("SharedMemory_Data");

      if (p.first) {
        std::cout << "Consumer: Found data = " << *p.first << std::endl;

        std::cout << "Consumer: Destroy data" << std::endl;
        managed_shm.destroy<int>("SharedMemory_Data");
      }

      std::cout << "Consumer: Unlock the mutex" << std::endl;
      lock.unlock();

      std::cout << "Consumer: Notify Producer" << std::endl;
      condition_read.notify_one();
    } while (true);
  } catch (const boost::interprocess::interprocess_exception& ex) {
    std::cerr << "boost::interprocess::interprocess_exception: " << ex.what()
              << std::endl;
  } catch (const std::exception& ex) {
    std::cerr << "std::exception: " << ex.what() << std::endl;
  } catch (...) {
    std::cerr << "unhandled exception\n";
  }
}


