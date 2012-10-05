/***************************************************************************                                                                                     
*   Copyright 2012 Advanced Micro Devices, Inc.                                     
*                                                                                    
*   Licensed under the Apache License, Version 2.0 (the "License");   
*   you may not use this file except in compliance with the License.                 
*   You may obtain a copy of the License at                                          
*                                                                                    
*       http://www.apache.org/licenses/LICENSE-2.0                      
*                                                                                    
*   Unless required by applicable law or agreed to in writing, software              
*   distributed under the License is distributed on an "AS IS" BASIS,              
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.         
*   See the License for the specific language governing permissions and              
*   limitations under the License.                                                   

***************************************************************************/                                                                                     

#include "stdafx.h"

#include <vector>
#include <iostream>
#include <functional>
#include <algorithm>

#include <bolt/AMP/transform.h>

template<typename Container>
static void printA2(const char * msg, const Container &a, const Container &b, int x_size) 
{
	std::wcout << msg << std::endl;
	for (int i = 0; i < x_size; i++)
		std::wcout << a[i] << "\t" << b[i] << std::endl;
};

static void printA(const char * msg, const int *a, int x_size) 
{
	std::wcout << msg << std::endl;
	for (int i = 0; i < x_size; i++)
		std::wcout << a[i] << std::endl;
};



/*
* Demonstrates:
   * Use of bolt::transform function
   * Bolt delivers same result as stl::transform
   * Bolt syntax is similar to STL transform
   * Works for both integer arrays and STL vectors
   */
void simpleTransform1()
{
	const int aSize = 16;
	int a[aSize] = {4,0,5,5,0,5,5,1,3,1,0,3,1,1,3,5};
	int b[aSize] = {1,9,0,8,6,1,7,7,1,0,1,3,5,7,9,8};
	int out[aSize];

	bolt::transform(a, a+aSize, out, bolt::negate<int>());
	printA2("Transform Neg - From Pointer", a, out, aSize);

	bolt::transform(a, a+aSize, b, out, bolt::plus<int>());
	printA("\nTransformVaddOut", out, aSize);

	static const int vSz=10;
	std::vector<int> vec(10);
	std::generate(vec.begin(), vec.end(), rand);
	std::vector<int> outVec(10);
	//std::transform(vec.begin(),vec.end(), outVec.begin(), bolt::negate<int>());
	bolt::transform(vec.begin(),vec.end(), outVec.begin(), bolt::negate<int>());
	printA2("Transform Neg - from Vector", vec, outVec, vSz);

	

#if 0
	// Same as above but using lamda rather than standard "plus" functor:
	// Doesn't compile in Dev11 Preview due to compiler bug, should be fixed in newer rev.
	// FIXME- try with new C++AMP compiler.
	bolt::transform(a, a+aSize, b, out, [&](int x, int y)
	{
		return x+y;
	});
	printA("\nTransformVaddOut-Lambda", out, aSize);
#endif
};


/* Demostrates:
* Bolt works for template arguments, ie int, float
*/
template<typename T>
void simpleTransform2(const int sz) 
{
	std::vector<T> A(sz);
	std::vector<T> S(sz);
	std::vector<T> B(sz);

	for (int i=0; i < sz; i++) {
		//A[i] = T(i);     // sequential assignment
		A[i] = T(rand())/137;  // something a little more exciting.
	};

	std::transform (A.begin(), A.end(), S.begin(), std::negate<T>());  // single-core CPU version
    bolt::transform(A.begin(), A.end(), B.begin(), bolt::negate<T>()); // bolt version on GPU or mcore CPU.
	
	// Check result:
	const int maxErrCount = 10;
	int errCount = 0;
	for (unsigned x=0; x< S.size(); x++) {
		const T s = S[x];
		const T b = B[x];
		//std::cout << s << "," << b << std::endl;
		if ((s != b) && (++errCount < maxErrCount)) {
			std::cout << "ERROR#" << errCount << " " << s << "!=" << b << std::endl;
		};
	};
};


// Show use of Saxpy Functor object.
struct SaxpyFunctor
{
	float _a;
	SaxpyFunctor(float a) : _a(a) {};

	float operator() (const float &xx, const float &yy) restrict(cpu,amp)
	{
		return _a * xx + yy;
	};
	
};


void transformSaxpy(int aSize)
{
	std::string fName = __FUNCTION__ ;
	fName += ":";

	std::vector<float> A(aSize), B(aSize), Z1(aSize), Z0(aSize);

	for (int i=0; i<aSize; i++) {
		A[i] = float(i);
		B[i] = 10000.0f + (float)i;
	}

	SaxpyFunctor sb(10.0);

	std::transform(A.begin(), A.end(), B.begin(), Z0.begin(), sb);
	bolt::transform(A.begin(), A.end(), B.begin(), Z1.begin(), sb);  

	//checkResults(fName, Z0.begin(), Z0.end(), Z1.begin());

};

void simpleTransform()
{
	simpleTransform1();
	simpleTransform2<int>(128);
	simpleTransform2<float>(1000);
	simpleTransform2<float>(100000);

	transformSaxpy(256);
};


int _tmain(int argc, _TCHAR* argv[])
{
	simpleTransform();
	return 0;
}
