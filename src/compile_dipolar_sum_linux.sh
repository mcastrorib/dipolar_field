echo g++ -c -m64 -fPIC -O3 -fopenmp -I"include" -I"cpp/cuda" "cpp/pvm_misc.cpp" -o "obj/pvm_misc.cpp.o"
g++ -c -m64 -fPIC -O3 -fopenmp -I"include" -I"cpp/cuda" "cpp/pvm_misc.cpp" -o "obj/pvm_misc.cpp.o"

echo g++ -c -m64 -fPIC -O3 -fopenmp -I"include" -I"cpp/cuda" "cpp/progress_bar.cpp" -o "obj/progress_bar.cpp.o"
g++ -c -m64 -fPIC -O3 -fopenmp -I"include" -I"cpp/cuda" "cpp/progress_bar.cpp" -o "obj/progress_bar.cpp.o"

echo g++ -c -m64 -fPIC -O3 -fopenmp -I"include" -I"cpp/cuda" "cpp/vec_3d.cpp" -o "obj/vec_3d.cpp.o"
g++ -c -m64 -fPIC -O3 -fopenmp -I"include" -I"cpp/cuda" "cpp/vec_3d.cpp" -o "obj/vec_3d.cpp.o"

echo g++ -c -m64 -fPIC -O3 -fopenmp -I"include" -I"cpp/cuda" "cpp/field_gradient.cpp" -o "obj/field_gradient.cpp.o"
g++ -c -m64 -fPIC -O3 -fopenmp -I"include" -I"cpp/cuda" "cpp/field_gradient.cpp" -o "obj/field_gradient.cpp.o"

echo g++ -c -m64 -fPIC -O3 -fopenmp -I"include" -I"cpp/cuda" "cpp/dipolar_sum.cpp" -o "obj/dipolar_sum.cpp.o"
g++ -c -m64 -fPIC -O3 -fopenmp -I"include" -I"cpp/cuda" "cpp/dipolar_sum.cpp" -o "obj/dipolar_sum.cpp.o"

echo g++ -c -m64 -fPIC -O3 -fopenmp -I"include" -I"cpp/cuda" "cpp/dipolar_sum_callers.cpp" -o "obj/dipolar_sum_callers.cpp.o"
g++ -c -m64 -fPIC -O3 -fopenmp -I"include" -I"cpp/cuda" "cpp/dipolar_sum_callers.cpp" -o "obj/dipolar_sum_callers.cpp.o"

echo g++ -shared -m64 -O3 -fopenmp -o "bin/dipolar_sum_core.so" "obj/pvm_misc.cpp.o" "obj/progress_bar.cpp.o" "obj/vec_3d.cpp.o" "obj/field_gradient.cpp.o" "obj/dipolar_sum.cpp.o" "obj/dipolar_sum_callers.cpp.o"
g++ -shared -m64 -O3 -fopenmp -o "bin/dipolar_sum_core.so" "obj/pvm_misc.cpp.o" "obj/progress_bar.cpp.o" "obj/vec_3d.cpp.o" "obj/field_gradient.cpp.o" "obj/dipolar_sum.cpp.o" "obj/dipolar_sum_callers.cpp.o"