// SuperSmartPointer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SuperSmartPointer.h"
#include <string>
#include <sstream>


//Special class used for testing
class TestObject
{
public:
	TestObject()
	{
		//Add a unique name to the object
		std::ostringstream ss;
		ss << "TestObject" << s_count;	//Build the string with the name
		m_name = ss.str();
		ss.str("");						//clear the stream buffer for future use
		s_count++;						//object created, add to the count

		std::cout<<"Creating"<<m_name<<" Total objects: "<<(s_count - 1)<<std::endl;
	}

	void printName()
	{
		std::cout<<m_name<<std::endl;
	}

	int getNumberOfTestObjects()
	{
		return s_count;
	}

	~TestObject()
	{
		std::cout<<"Deleting: "<<m_name<<std::endl;
		s_count--;		//object deleted, remove from the count
	}

private:
	std::string m_name;
	static int s_count;
};

int TestObject::s_count = 1;




//Actual test harness
int _tmain(int argc, _TCHAR* argv[])
{
	//Create a test object with one ssp
	TestObject* myTestObject1 = new TestObject();

	{
		//Create a testObject by calling the SSP default constructor
		SuperSmartPointer<TestObject> ssp4;

		{
			//Test the dereferencing operator override
			SuperSmartPointer<TestObject> ssp4(myTestObject1);
			std::cout<<"Printing ssp4 object name via * - ";
			(*ssp4).printName();

			{
				//Test the -> operator
				SuperSmartPointer<TestObject> ssp1(myTestObject1);
				std::cout<<"Printing ssp1 object name via -> - ";
				ssp1->printName();

				//Test copy constructor
				SuperSmartPointer<TestObject> ssp2(ssp4);
				std::cout<<"Printing ssp2 made via copy constructor - ";
				ssp2->printName();

				std::cout<<"ssp1 should now go out of scope but myTestObject1 won't be deleted"<<std::endl;
				std::cout<<"ssp2 should now go out of scope but myTestObject2 won't be deleted"<<std::endl;
			}

			std::cout<<"ssp3 should now go out of scope and delete myTestObject1"<<std::endl;
		}
		std::cout<<"ssp4 should now go out of scope and delete the autoCreated myTestObject2"<<std::endl;
	}

	system ("pause");

	return 0;
}


