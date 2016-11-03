
#include <iostream>
using namespace std;

#include "dog.h"
#include "cat.h"
#include "person.h"

int main()
{
	person boy("xiaojie",23);
	cout<<endl;
	cat rongrong("rongrong");
	cout<<endl;
	dog hanhan("hanhan");
	cout<<endl;

	boy.play_with(rongrong);
	cout<<endl;
	boy.play_with(hanhan);

	return 0;
}
 