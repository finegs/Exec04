/*
 * AppProducerSMI.cpp
 *
 *  Created on: 2018. 12. 22.
 *      Author: finegs
 */

#include <SMI.hpp>


// Standard Library
#include <iostream>
#include <thread>

// Boost.Interprocess
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/named_condition.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>

using namespace std;
using namespace boost::interprocess;

int main() {
  std::cout << "Started Producer" << std::endl;

  try {
    std::cout << "Producer: Open/Create shared memory segment" << std::endl;
    boost::interprocess::managed_shared_memory managed_shm{
        boost::interprocess::open_or_create, "SharedMemory_Segment", 1024};
    std::cout << "Producer: Open/Create mutex" << std::endl;
    boost::interprocess::named_mutex mutex{boost::interprocess::open_or_create,
                                           "SharedMemory_Mutex"};
    std::cout << "Producer: Open/Create condition" << std::endl;
    boost::interprocess::named_condition condition_read{
        boost::interprocess::open_or_create, "SharedMemory_ConditionRead"};
    boost::interprocess::named_condition condition_write{
        boost::interprocess::open_or_create, "SharedMemory_ConditionWrite"};

    std::cout << "Producer: Start work" << std::endl;
    for (int i = 0; i < 10; ++i) {
      std::cout << "Producer: Try to get the lock for the shared mutex "
                   "(potential blocking)"
                << std::endl;
      boost::interprocess::scoped_lock<boost::interprocess::named_mutex> lock{
          mutex};

      std::cout << "Producer: Wait for Consumer" << std::endl;
      condition_read.wait(lock);

      std::cout << "Producer: Construct data" << std::endl;
      managed_shm.construct<int>("SharedMemory_Data")(i);

      std::cout << "Producer: Unlock the mutex" << std::endl;
      lock.unlock();

      std::cout << "Producer: Notify Consumer" << std::endl;
      condition_write.notify_one();
    }
  } catch (const boost::interprocess::interprocess_exception& ex) {
    std::cerr << "boost::interprocess::interprocess_exception: " << ex.what()
              << std::endl;
  } catch (const std::exception& ex) {
    std::cerr << "std::exception: " << ex.what() << std::endl;
  } catch (...) {
    std::cerr << "unhandled exception\n";
  }
}

