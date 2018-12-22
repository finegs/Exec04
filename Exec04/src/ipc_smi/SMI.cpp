/*
 * SMI.cpp
 *
 *  Created on: 2018. 12. 22.
 *      Author: finegs
 */

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <exception>
#include <boost/current_function.hpp>
#include <boost/exception/all.hpp>
#include <boost/interprocess/creation_tags.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>

#include <SMI.hpp>

using namespace std;
using namespace boost::interprocess;


int f01()
{
	int rc = EXIT_FAILURE;
	struct shm_remove
	{
		shm_remove()
		{
			shared_memory_object::remove("MySharedMemory");
			cout << "shm_remove" << endl;
		}
		~shm_remove()
		{
			shared_memory_object::remove("MySharedMemory");
			cout << "~shm_remove" << endl;
		}
	} remover;

	managed_shared_memory managed_shm(open_or_create, "MySharedMemory", UCHAR_MAX);

	try
	{
		void *ptr = managed_shm.allocate(100);

		managed_shm.deallocate(ptr);

		ptr = managed_shm.allocate(100, std::nothrow);

		managed_shm.deallocate(ptr);

		rc = EXIT_SUCCESS;
	}
	catch (std::exception& ex)
	{
		cerr << "Exception : " << ex.what() << endl;

		exception_ptr pp = current_exception();

		clog << (pp ? pp.__cxa_exception_type()->name() : "null") << endl;

		clog << boost::current_exception_diagnostic_information() << endl;
	}
	catch (...)
	{
		exception_ptr p = current_exception();
		clog << (p ? p.__cxa_exception_type()->name() : "null") << endl;

		clog << boost::current_exception_diagnostic_information() << endl;
	}

	return rc;
}

int f02()
{

	int rc = EXIT_FAILURE;

	//Remove shared memory on construction and destruction
	struct shm_remove
	{
		shm_remove() { shared_memory_object::remove("MySharedMemory"); }
		~shm_remove(){ shared_memory_object::remove("MySharedMemory"); }
	} remover;

	try
	{

		{
			//Create a managed shared memory
			managed_shared_memory shm(create_only, "MySharedMemory", 1000);

			//Check size
			assert(shm.get_size() == 1000);
			//Construct a named object
			MyClass *myclass = shm.construct<MyClass>("MyClass")();
			//The managed segment is unmapped here

			cout << "&myClass = " << &(*myclass) << endl;
		}
		{
			//Now that the segment is not mapped grow it adding extra 500 bytes
			managed_shared_memory::grow("MySharedMemory", 500);

			//Map it again
			managed_shared_memory shm(open_only, "MySharedMemory");
			//Check size
			assert(shm.get_size() == 1500);
			//Check "MyClass" is still there
			MyClass *myclass = shm.find<MyClass>("MyClass").first;
			assert(myclass != 0);
			//The managed segment is unmapped here
		}
		{
			//Now minimize the size of the segment
			managed_shared_memory::shrink_to_fit("MySharedMemory");

			//Map it again
			managed_shared_memory shm(open_only, "MySharedMemory");
			//Check size
			assert(shm.get_size() < 1000);
			//Check "MyClass" is still there
			MyClass *myclass = shm.find<MyClass>("MyClass").first;
			assert(myclass != 0);
			//The managed segment is unmapped here
		}

		rc = EXIT_SUCCESS;
	}
	catch (...)
	{
		clog << boost::current_exception_diagnostic_information() << endl;
	}

	return rc;
}

int f03()
{
	int rc = EXIT_FAILURE;

	//Remove shared memory on construction and destruction
	struct shm_remove
	{
		shm_remove() { shared_memory_object::remove("MySharedMemory"); }
		~shm_remove(){ shared_memory_object::remove("MySharedMemory"); }
	} remover;

	//Managed memory segment that allocates portions of a shared memory
	//segment with the default management algorithm
	managed_shared_memory managed_shm(create_only, "MySharedMemory", UCHAR_MAX);

	const std::size_t Alignment = 128;

	try {

		//Allocate 100 bytes aligned to Alignment from segment, throwing version
		void *ptr = managed_shm.allocate_aligned(100, Alignment);

		//Check alignment
		assert((static_cast<char*>(ptr)-static_cast<char*>(0)) % Alignment == 0);

		//Deallocate it
		managed_shm.deallocate(ptr);

		//Non throwing version
		ptr = managed_shm.allocate_aligned(100, Alignment, std::nothrow);

		//Check alignment
		assert((static_cast<char*>(ptr)-static_cast<char*>(0)) % Alignment == 0);

		//Deallocate it
		managed_shm.deallocate(ptr);

		//If we want to efficiently allocate aligned blocks of memory
		//use managed_shared_memory::PayloadPerAllocation value
		assert(Alignment > managed_shared_memory::PayloadPerAllocation);

		//This allocation will maximize the size of the aligned memory
		//and will increase the possibility of finding more aligned memory
		ptr = managed_shm.allocate_aligned
				(3*Alignment - managed_shared_memory::PayloadPerAllocation, Alignment);

		//Check alignment
		assert((static_cast<char*>(ptr)-static_cast<char*>(0)) % Alignment == 0);

		//Deallocate it
		managed_shm.deallocate(ptr);

		rc = EXIT_SUCCESS;

	}
	catch(...)
	{
		clog << "Exception : " << boost::current_exception_diagnostic_information() << endl;
	}


	return rc;
}


