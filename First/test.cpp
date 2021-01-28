#include <iostream>
#include "BST.h"
#define BOOST_TEST_MODULE MyModuleName
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(my_insertTest)
{
	BST<int, std::string> b;

	BOOST_CHECK_NO_THROW(b.insert(22, "Jane"));
	BOOST_CHECK_NO_THROW(b.insert(22, "Mary"));
	BOOST_CHECK_NO_THROW(b.insert(0, "Harold"));

}

//found lookup was decending down wrong subtree.
BOOST_AUTO_TEST_CASE(my_lookupTest)
{
	BST<int, std::string> b;
	b.insert(22, "Mary");
	b.insert(17, "Gabe");
	b.insert(5, "Jon");

	std::string *test1 = b.lookup(22);
	std::string *test2 = b.lookup(17);
	std::string *test3 = b.lookup(5);

	BOOST_CHECK_EQUAL(*test1, "Mary");
	BOOST_CHECK_EQUAL(*test2, "Gabe");
	BOOST_CHECK_EQUAL(*test3, "Jon");
}

BOOST_AUTO_TEST_CASE(my_removeTest)
{
	BST<int, std::string> b;
	
	b.insert(22, "Jane");
	b.insert(22, "Mary");
	b.insert(0, "Harold");
	b.insert(9, "Edward");
	b.insert(37, "Victoria");
	b.insert(4, "Matilda");
	b.insert(26, "Oliver");
	b.insert(42, "Elizabeth");
	b.insert(19, "Henry");
	b.insert(4, "Stephen");
	b.insert(24, "James");
	b.insert(-1, "Edward");
	b.insert(31, "Anne");
	b.insert(23, "Elizabeth");
	b.insert(1, "William");
	b.insert(26, "Charles");

	BOOST_CHECK_NO_THROW(b.remove(37));
	BOOST_CHECK_NO_THROW(b.remove(-1));
	BOOST_CHECK_NO_THROW(b.remove(31));


	b.displayEntries();
}

BOOST_AUTO_TEST_CASE(my_copy_and_assign_test)
{
	std::cout << "my_copy_and_assign_test" << std::endl;

	BST<int, std::string> b;

	b.insert(24, "James");
	b.insert(-1, "Edward");
	b.insert(31, "Anne");
	b.insert(23, "Elizabeth");
	b.insert(1, "William");
	b.insert(26, "Charles");

	BST<int, std::string> a = b;

	BOOST_CHECK_NO_THROW(b.displayEntries());
	std::cout << " -----------------" << std::endl;

	BST<int, std::string> c;
	c = b;
	BOOST_CHECK_NO_THROW(b.displayEntries());

}

BOOST_AUTO_TEST_CASE(my_moveTest)
{
	std::cout << "my_move Test" << std::endl;

	BST<int, std::string> a;

	a.insert(24, "James");
	a.insert(-1, "Edward");
	a.insert(31, "Anne");
	a.insert(23, "Elizabeth");
	a.insert(1, "William");
	a.insert(26, "Charles");

	a.displayEntries();

	std::cout << "-------------" << std::endl;

	BST<int, std::string> b;
	BOOST_CHECK_NO_THROW(b = std::move(a));

	b.displayEntries();

}