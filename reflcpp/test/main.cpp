#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <reflcpp/ClassType_tmpl.hpp>
#include <reflcpp/BoundClassType_tmpl.hpp>
#include <reflcpp/Exceptions.hpp>
#include "A.hpp"
#include "A_reflection.hpp"
#include "B.hpp"
#include "B_reflection.hpp"
#include "D.hpp"
#include "D_reflection.hpp"
#include "E.hpp"
#include "E_reflection.hpp"

namespace {

double drand() {
    return rand()/(double)RAND_MAX;
}

}



using namespace std;
using namespace reflcpp;


int
main() {

    try {

        /*
         * Basic testing.
         */

        {
            A A_obj;
            B B_obj;

            A *A_A_ptr = &A_obj;
            A *A_B_ptr = &B_obj;
            B *B_B_ptr = &B_obj;

            ClassType_tmpl<A> A_ct;
            ClassType_tmpl<B> B_ct;
            ClassType A_c = A_ct;
            ClassType B_c = B_ct;

            cout << "A::name: " << A_c.name() << endl;
            cout << "B::name: " << B_c.name() << endl;

            // Check that getDataMembers is working.
            {
                std::vector<DataMember> fields = A_c.getDataMembers();
                assert(fields.size() == 1);
                assert(fields[0].name() == "f1");
            }

            DataMember A_f1 = A_c.getDataMember("f1");
            DataMember B_f1 = B_c.getDataMember("f1");
            BoundDataMember B_b_f1 = B_c.getBoundDataMember(B_B_ptr, "f1");
            B_b_f1.ref<int>() = 4321;
            assert(B_obj.f1 == 4321);

            BoundClassType_tmpl<B> B_bc(&B_obj);
            BoundDataMember bf = B_bc.getBoundDataMember("f1");
            bf.ref<int>() = 1783; assert(B_obj.f1 == 1783);

            BoundDataMember A_bf1 = A_c.getBoundDataMember(A_A_ptr, "f1");

            DataMember B_array2 = B_c.getDataMember("array2");
            B_array2.ref<double[10]>(&B_obj)[1] = 123987;
            assert(B_obj.array2[1] == 123987);

            DataMember B_f_c = B_c.getDataMember("c");
            B_f_c.ref<C>(&B_obj).cf1 = 98777;
            assert(B_obj.c.cf1 == 98777);

            ClassType ct = dynamic_cast_type<ClassType>(B_f_c.type());
            C *cp = &B_f_c.ref<C>(&B_obj);
            DataMember f = ct.getDataMember("cf1");
            f.ref<int>(cp) = 7777;
            assert(B_obj.c.cf1 == 7777);

            
            C *c_ptr = &B_f_c.ref<C>(&B_obj);

            /*
            There are three kinds of type involved.  The type of the actual object, the
            type of the ClassType, and the type of the pointer used.  The cases I'm testing
            here are:

                Ptr     ClassType   Obj
                ---     -----   ---
                A       A       A
                A       A       B
                B       A       B
                A       B       B
                B       B       B

            This is far from complete.  Should also test failure modes, etc.
            */

            // A A A
            {
                int n = (int) (1000000 * drand());
                assert(A_A_ptr != NULL);
                // Make sure both versions work.
                A_f1.ref<int>(A_A_ptr) = n;
                assert(A_f1.ref<int>(A_A_ptr) == n);
                assert(A_obj.f1 == n);
            }
            // A A B
            {
                int n = (int) (1000000 * drand());
                A_f1.ref<int>(A_B_ptr) = n;
                assert(A_f1.ref<int>(A_B_ptr) == n);
                assert(static_cast<A &>(B_obj).f1 == n);
            }
            // B A B
            {
                int n = (int) (1000000 * drand());
                A_f1.ref<int>(B_B_ptr) = n;
                assert(A_f1.ref<int>(B_B_ptr) == n);
                assert(static_cast<A &>(B_obj).f1 == n);
            }
            // A B B
            {
                int n = (int) (1000000 * drand());
                B_f1.ref<int>(A_B_ptr) = n;
                assert(B_f1.ref<int>(A_B_ptr) == n);
                assert(static_cast<B &>(B_obj).f1 == n);
            }
            // B B B
            {
                int n = (int) (1000000 * drand());
                B_f1.ref<int>(B_B_ptr) = n;
                assert(B_f1.ref<int>(B_B_ptr) == n);
                assert(static_cast<B &>(B_obj).f1 == n);
            }
        }

        /*
         * Test that getClass() works.
         */

        {
            A A_obj;
            B B_obj;

            A *A_A_ptr = &A_obj;
            A *A_B_ptr = &B_obj;
            B *B_B_ptr = &B_obj;

            {
                cout << "name: " << typeid(B).name() << endl;
                ClassType c = ClassType::getClass(typeid(B));
                assert(c.name() == "B");
            }

            {
                ClassType c = ClassType::getClass(typeid(*A_B_ptr));
                assert(c.name() == "B");
            }

            // Get class from field.
            {
                ClassType_tmpl<A> A_ct;
                ClassType_tmpl<B> B_ct;
                ClassType A_c = A_ct;
                ClassType B_c = B_ct;

                DataMember A_f1 = A_c.getDataMember("f1");

                Type A_f1_type = A_f1.type();
            }
        }

        /*
         * Test derivesFrom().
         */

        {
            A A_obj;
            B B_obj;

            A *A_A_ptr = &A_obj;
            A *A_B_ptr = &B_obj;
            B *B_B_ptr = &B_obj;

            ClassType_tmpl<A> A_ct;
            ClassType_tmpl<B> B_ct;
            ClassType A_c = A_ct;
            ClassType B_c = B_ct;

            assert(B_c.derivesFrom(A_c));

            // Test fundamental type testing.
            // XXX - Add back
            // assert(ClassType_tmpl<int>().derivesFrom(FundamentalType));
            //assert(ClassType_tmpl<float>().derivesFrom(FundamentalType));
        }

        /*
         * Test newInstance();
         */
        {
            ClassType_tmpl<B> B_ct;
            ClassType B_c = B_ct;

            A *a_b = B_c.newInstance<A>();
            assert(typeid(*a_b) == typeid(B));
        }

        /*
         * Test Type equivalence.
         */

        {

            ClassType_tmpl<B> B_tc1;
            ClassType_tmpl<B> B_tc2;
            Type B_c1 = B_tc1;
            Type B_c2 = B_tc2;
            assert(B_c1 == B_c2);
        }

        /*
         * Test array convention for SOAP.
         */
        {
            ClassType_tmpl<B> B_tc;
            ClassType B_c = B_tc;
            B b;

            DataMember af = B_c.getDataMember("array");
            cout << "array class name: " << af.type().name() << ", " << af.type().typeInfo().name() << endl;
            assert(af.type() == typeid(float *));
            DataMember sf = B_c.getDataMember("array_size");
            assert(sf.type() == typeid(int));
            assert(sf.ref<int>(&b) == 5);
            float *a = af.ref<float *>(&b);
            assert(a != NULL);
            int i;
            for (i = 0; i < 5; i++) {
                assert(a[i] == i);
            }
        }

        /*
         * Test for methods.
         */
	    {
            int ret;
            C C_obj;
		    D D_obj;
            D *D_D_ptr = &D_obj;

            ClassType_tmpl<D> D_ct;
            ClassType D_c = D_ct;

		    cout << "D::name: " << D_c.name() << endl;

			// getMemberFunctions
			std::vector<MemberFunction> functions = D_c.getMemberFunctions();
            assert(functions.size() == 7);
            assert(functions[0].name() == "m0");

            // No arguments, return int
            MemberFunction D_m0 = D_c.getMemberFunction("m0");
            assert(D_m0.invoke<int>(D_D_ptr) == 5);

            // No arguments, void
            MemberFunction D_m0_void = D_c.getMemberFunction("m0_void");
            D_m0_void.invoke<void>(D_D_ptr);

            // 1 argument, return int
            MemberFunction D_m1 = D_c.getMemberFunction("m1");
            ret = D_m1.invoke<int, D, const int>(D_D_ptr, 5); // see Note 1
            assert(ret == 10); // see Note 2
            // getArguments
            {
                std::vector<const Type *> args = D_m1.getArguments();
                assert(args.size() == 1);
                assert(args[0]->typeInfo() == typeid(int));
            }

            // 1 argument, void
            MemberFunction D_m1_void = D_c.getMemberFunction("m1_void");
            D_m1_void.invoke<void, D, const int>(D_D_ptr, 5); // see Note 1

            // 2 arguments, return int
            MemberFunction D_m2 = D_c.getMemberFunction("m2");
            ret = D_m2.invoke<int, D, const int, const int>(D_D_ptr, 5, 10); // see Note 1
            assert(ret == 15); // see Note 2
            // getArguments
            {
                std::vector<const Type *> args = D_m2.getArguments();
                assert(args.size() == 2);
                assert(args[0]->typeInfo() == typeid(int));               
                assert(args[1]->typeInfo() == typeid(int));
            }
            // RMI type invocation
            {
                int i = 12, j = 15;
                Arguments args;
                args.addRefArgument(i);
                args.addRefArgument(j);
                assert(D_m2.rmiInvoke<int>(D_D_ptr, args) == 27);
            }
 
            // 1 non-primitive argument, non-primitive return
            C_obj.cf1 = 15;
            MemberFunction D_m1_c = D_c.getMemberFunction("m1_c");
            assert(D_m1_c.invoke<C>(D_D_ptr, C_obj).cf1 == 20);

            // Bound member function
            C_obj.cf1 = 15;
            BoundMemberFunction D_bm1_c = D_c.getBoundMemberFunction(D_D_ptr, "m1_c");
            assert(D_bm1_c.invoke<C>(C_obj).cf1 == 20);
            // getArguments
            {
                std::vector<const Type *> args = D_bm1_c.getArguments();
                assert(args.size() == 1);
                assert(args[0]->typeInfo() == typeid(C));
            }
            // RMI type invocation
            {
                C_obj.cf1 = 11;
                Arguments args;
                args.addRefArgument(C_obj);
                assert(D_bm1_c.rmiInvoke<C>(args).cf1 == 20);
            }

            // Reference arguments
            C_obj.cf1 = 15;
            BoundMemberFunction D_bm1_ref_c = D_c.getBoundMemberFunction(D_D_ptr, "m1_ref_c");
            D_bm1_ref_c.invoke<void>(C_obj);
            assert(C_obj.cf1 == 29);
            // getArguments
            {
                std::vector<const Type *> args = D_bm1_c.getArguments();
                assert(args.size() == 1);
                assert(args[0]->typeInfo() == typeid(C)); // typeid doesn't distinguish between C and C&
            }
        }

        /*
         * Test that getClass(const std::string &) works.
         */

        {
            // B
            ClassType B_ct = ClassType::getClass(string("B"));
            assert(B_ct.name() == "B");

            // E
            ClassType E_ct = ClassType::getClass(string("E"));
            assert(E_ct.name() == "E");
            PtrHolder_smptr_t obj = E_ct.createInstance();
            MemberFunction E_m0 = E_ct.getMemberFunction("m0");
            Arguments args;
            int r = E_m0.genericInvoke<int>(*obj, args); //XXX - Change this to use obj
            assert(r == 115);
        }
    }
    catch (const exception& ex) {
        cerr << "ERROR: " << ex.what() << endl;
        exit (1);
    }

    return 0;
}

/*
Note:
  1. For literals, template parameters are deduced without the 'const'ness. Therefore,
     it was required to explicitly specify the template parameters in these cases.
  2. assert macro gets confused when more than 1 template parameter is explicitly specified,
     probably because of the additional ','s. Therefore, it is not possible to perform the 
     assert in the same line as funciton invocation. Not a big deal though.
*/
