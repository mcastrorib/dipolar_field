g++ -c -m64 -fPIC -O3 -fopenmp -I"include" "src/pvm_misc.cpp" -o "obj/pvm_misc.cpp.obj"
g++ -c -m64 -fPIC -O3 -fopenmp -I"include" "src/vec_3d.cpp" -o "obj/vec_3d.cpp.obj"
g++ -c -m64 -fPIC -O3 -fopenmp -I"include" "src/dipolar_sum.cpp" -o "obj/dipolar_sum.cpp.obj"
g++ -c -m64 -fPIC -O3 -fopenmp -I"include" "src/dipolar_sum_callers.cpp" -o "obj/dipolar_sum_callers.cpp.obj"

g++ -shared -m64 -O3 -fopenmp -o "bin/dipolar_sum_core.dll" "obj/pvm_misc.cpp.obj" "obj/vec_3d.cpp.obj" "obj/dipolar_sum.cpp.obj" "obj/dipolar_sum_callers.cpp.obj"