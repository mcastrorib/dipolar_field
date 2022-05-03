echo g++ -c -m64 -fPIC -O3 -fopenmp -I"include" "src/pvm_misc.cpp" -o "obj/pvm_misc.cpp.o"
g++ -c -m64 -fPIC -O3 -fopenmp -I"include" "src/pvm_misc.cpp" -o "obj/pvm_misc.cpp.o"

echo g++ -c -m64 -fPIC -O3 -fopenmp -I"include" "src/dipolar_sum_callers.cpp" -o "obj/dipolar_sum_callers.cpp.o"
g++ -c -m64 -fPIC -O3 -fopenmp -I"include" "src/dipolar_sum_callers.cpp" -o "obj/dipolar_sum_callers.cpp.o"

echo g++ -shared -m64 -O3 -fopenmp -o "bin/dipolar_sum_core.so" "obj/pvm_misc.cpp.o" "obj/dipolar_sum_callers.cpp.o"
g++ -shared -m64 -O3 -fopenmp -o "bin/dipolar_sum_core.so" "obj/pvm_misc.cpp.o" "obj/dipolar_sum_callers.cpp.o"